//
// Created by Song on 2025/11/10.
//

#include "start.h"
#include "./Log/log.h"
#include "./lol/lolm.h"
#include <chrono>
#include <thread>
#include <csignal>
#include <csetjmp>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cerrno>
#include <dlfcn.h>

// 线程安全游戏数据桥接
#include "SharedGameData.h"

// Dobby — inline hook
#include "./Dobby/include/dobby.h"

// ImGui (eglSwapBuffers hook 使用)
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <EGL/egl.h>

// 触摸输入 (读取 /dev/input/event*)
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <GLES3/gl3.h>

// ═══════════════════════════════════════════════════════════════════════════════
// SIGSEGV/SIGBUS 信号安全网
// ═══════════════════════════════════════════════════════════════════════════════

static thread_local sigjmp_buf t_jumpBuf;
static thread_local volatile sig_atomic_t t_guardActive = 0;

static struct sigaction s_oldSigsegv;
static struct sigaction s_oldSigbus;

static void crashSignalHandler(int sig, siginfo_t* info, void* ctx) {
    if (t_guardActive) {
        t_guardActive = 0;
        siglongjmp(t_jumpBuf, sig);
    }
    struct sigaction* old = (sig == SIGSEGV) ? &s_oldSigsegv : &s_oldSigbus;
    if (old->sa_flags & SA_SIGINFO) {
        old->sa_sigaction(sig, info, ctx);
    } else if (old->sa_handler != SIG_DFL && old->sa_handler != SIG_IGN) {
        old->sa_handler(sig);
    } else {
        signal(sig, SIG_DFL);
        raise(sig);
    }
}

static std::once_flag s_crashGuardOnce;
static void installCrashGuard() {
    std::call_once(s_crashGuardOnce, []() {
        struct sigaction sa{};
        sa.sa_sigaction = crashSignalHandler;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGSEGV, &sa, &s_oldSigsegv);
        sigaction(SIGBUS,  &sa, &s_oldSigbus);
        LOG(LOG_LEVEL_INFO, "[CrashGuard] 信号安全网已安装 (SIGSEGV + SIGBUS)");
    });
}

// ═══════════════════════════════════════════════════════════════════════════════
// 小地图坐标映射
// ═══════════════════════════════════════════════════════════════════════════════

static constexpr float MAP_MIN_X = -50.0f;
static constexpr float MAP_MAX_X = 50.0f;
static constexpr float MAP_MIN_Z = -50.0f;
static constexpr float MAP_MAX_Z = 50.0f;

static ImVec2 WorldToMinimap(const lol::UnityVector3& worldPos,
                             ImVec2 minimapOrigin, float minimapSize) {
    float nx = (worldPos.x - MAP_MIN_X) / (MAP_MAX_X - MAP_MIN_X);
    float nz = 1.0f - (worldPos.z - MAP_MIN_Z) / (MAP_MAX_Z - MAP_MIN_Z);
    nx = std::clamp(nx, 0.0f, 1.0f);
    nz = std::clamp(nz, 0.0f, 1.0f);
    return {minimapOrigin.x + nx * minimapSize,
            minimapOrigin.y + nz * minimapSize};
}

/** @brief 将裸 (x,z) 世界坐标转成小地图像素坐标 (全图模式) */
static ImVec2 XZToMinimap(float wx, float wz,
                           ImVec2 origin, float size) {
    float nx = (wx - MAP_MIN_X) / (MAP_MAX_X - MAP_MIN_X);
    float nz = 1.0f - (wz - MAP_MIN_Z) / (MAP_MAX_Z - MAP_MIN_Z);
    return {origin.x + std::clamp(nx, 0.f, 1.f) * size,
            origin.y + std::clamp(nz, 0.f, 1.f) * size};
}

// ═══════════════════════════════════════════════════════════════════════════════
// 触摸输入 — 读取 /dev/input/event* 原始触摸事件, 喂给 ImGui IO
//
// 游戏进程无权直接读 /dev/input/event* (SELinux + DAC)。
// 需要用户通过 ADB 手动提权:
//   adb shell su -c "chmod 666 /dev/input/event*"
// init() 会每隔几秒自动重试, chmod 生效后即可使用。
// ═══════════════════════════════════════════════════════════════════════════════

namespace touch_input {

    static int   s_fd       = -2;     // -2=未初始化/重试中, -1=永久失败, >=0=有效
    static float s_absMaxX  = 1.0f;   // 数字化仪 X 轴最大值 (物理竖屏坐标系)
    static float s_absMaxY  = 1.0f;   // 数字化仪 Y 轴最大值
    static float s_rawX     = -1.0f;  // 归一化原始触摸坐标 (0..1, 数字化仪坐标系)
    static float s_rawY     = -1.0f;
    static float s_curX     = -1.0f;  // 转换后的屏幕坐标 (已应用旋转)
    static float s_curY     = -1.0f;
    static bool  s_touching = false;
    static int   s_retryCount = 0;
    static std::chrono::steady_clock::time_point s_lastRetry{};

    // ── 触摸坐标旋转模式 ──
    //
    // /dev/input/event* 原始触摸始终在物理数字化仪坐标系 (通常是竖屏)
    // 当游戏横屏运行时, 需要旋转坐标映射:
    //   -1 = 自动检测 (对比数字化仪方向 vs 屏幕方向)
    //    0 = 不旋转 (竖屏游戏 或 内核已旋转)
    //    1 = ROTATION_90  (逆时针90°, 最常见横屏)
    //    2 = ROTATION_180
    //    3 = ROTATION_270 (顺时针90°, 反向横屏)
    //
    static int s_rotation = -1;

    /** @brief 循环切换旋转模式 (供 UI 按钮调用) */
    static void cycleRotation() {
        if (s_rotation < 0) s_rotation = 0;   // 从自动 → 手动0
        else s_rotation = (s_rotation + 1) % 4;
        LOG(LOG_LEVEL_INFO, "[Touch] 旋转模式切换 → %d (0=无, 1=90°, 2=180°, 3=270°)", s_rotation);
    }

    /** @brief 获取当前旋转模式描述 */
    static const char* getRotationLabel() {
        switch (s_rotation) {
            case -1: return "Auto";
            case 0:  return "0";
            case 1:  return "90";
            case 2:  return "180";
            case 3:  return "270";
            default: return "?";
        }
    }

    /** @brief 尝试打开触摸设备, 失败则每 5 秒自动重试 */
    static void init() {
        // 已成功打开
        if (s_fd >= 0) return;

        // 限制重试频率: 每 5 秒重试一次
        auto now = std::chrono::steady_clock::now();
        if (s_fd == -2 && s_retryCount > 0) {
            if (now - s_lastRetry < std::chrono::seconds(5)) return;
        }
        s_lastRetry = now;
        s_retryCount++;

        // 首次运行时打印提示
        if (s_retryCount == 1) {
            LOG(LOG_LEVEL_INFO, "[Touch] ══════ 初始化触摸输入 ══════");
            LOG(LOG_LEVEL_INFO, "[Touch] 如果触摸不可用, 请在 PC 上执行:");
            LOG(LOG_LEVEL_INFO, "[Touch]   adb shell su -c \"chmod 666 /dev/input/event*\"");
        }

        for (int i = 0; i < 10; i++) {
            char path[64];
            snprintf(path, sizeof(path), "/dev/input/event%d", i);

            int fd = open(path, O_RDONLY | O_NONBLOCK);
            if (fd < 0) continue;

            unsigned long absBits[(ABS_MAX + 63) / 64] = {};
            if (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absBits)), absBits) < 0) {
                close(fd); continue;
            }

            bool hasMtX = (absBits[ABS_MT_POSITION_X / (sizeof(long) * 8)]
                           >> (ABS_MT_POSITION_X % (sizeof(long) * 8))) & 1;
            if (!hasMtX) { close(fd); continue; }

            struct input_absinfo absX{}, absY{};
            ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &absX);
            ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &absY);
            s_absMaxX = absX.maximum > 0 ? (float)absX.maximum : 1.0f;
            s_absMaxY = absY.maximum > 0 ? (float)absY.maximum : 1.0f;

            s_fd = fd;
            LOG(LOG_LEVEL_INFO, "[Touch] ✓ 触摸设备就绪: %s (重试 %d 次)", path, s_retryCount);
            LOG(LOG_LEVEL_INFO, "[Touch]   数字化仪范围: X=0..%.0f  Y=0..%.0f  (宽高比=%.2f)",
                s_absMaxX, s_absMaxY, s_absMaxX / s_absMaxY);
            return;
        }

        // 仍然失败, 保持 s_fd = -2 以便下次重试
        if (s_retryCount <= 3) {
            LOG(LOG_LEVEL_INFO, "[Touch] 等待权限... (第 %d 次尝试)", s_retryCount);
        }
    }

    /**
     * @brief 处理原始触摸事件, 应用旋转变换后喂给 ImGui IO
     *
     * 旋转变换公式 (rawX/rawY 是归一化的数字化仪竖屏坐标 0..1):
     *   ROTATION_0:   screenX = rawX * W,           screenY = rawY * H
     *   ROTATION_90:  screenX = (1-rawY) * W,       screenY = rawX * H     (逆时针90°)
     *   ROTATION_180: screenX = (1-rawX) * W,       screenY = (1-rawY) * H
     *   ROTATION_270: screenX = rawY * W,            screenY = (1-rawX) * H (顺时针90°)
     */
    static void processEvents(float screenW, float screenH) {
        if (s_fd < 0) return;
        struct input_event ev{};
        while (read(s_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            switch (ev.type) {
                case EV_ABS:
                    if (ev.code == ABS_MT_POSITION_X)
                        s_rawX = (float)ev.value / s_absMaxX;   // 归一化到 0..1
                    else if (ev.code == ABS_MT_POSITION_Y)
                        s_rawY = (float)ev.value / s_absMaxY;
                    else if (ev.code == ABS_MT_TRACKING_ID)
                        s_touching = (ev.value >= 0);
                    break;
                case EV_KEY:
                    if (ev.code == BTN_TOUCH) s_touching = (ev.value != 0);
                    break;
                case EV_SYN:
                    if (ev.code == SYN_REPORT) {
                        auto& io = ImGui::GetIO();
                        if (s_rawX >= 0.0f && s_rawY >= 0.0f) {

                            // ── 确定旋转模式 ──
                            int rot = s_rotation;
                            if (rot < 0) {
                                // 自动检测: 屏幕横屏 + 数字化仪竖屏 → 需要旋转
                                bool surfaceLandscape  = (screenW > screenH);
                                bool digitizerPortrait = (s_absMaxY > s_absMaxX * 1.1f);
                                if (surfaceLandscape && digitizerPortrait) {
                                    rot = 3;  // 默认 ROTATION_270 (顺时针横屏)
                                } else if (!surfaceLandscape && s_absMaxX > s_absMaxY * 1.1f) {
                                    rot = 3;  // 竖屏surface + 横屏digitizer (罕见, 尝试270°)
                                } else {
                                    rot = 0;  // 方向一致, 无需旋转
                                }
                            }

                            // ── 应用旋转变换 ──
                            float fx, fy;
                            switch (rot) {
                                default:
                                case 0:  // 不旋转
                                    fx = s_rawX * screenW;
                                    fy = s_rawY * screenH;
                                    break;
                                case 1:  // ROTATION_90 (逆时针, 最常见横屏)
                                    fx = (1.0f - s_rawY) * screenW;
                                    fy = s_rawX * screenH;
                                    break;
                                case 2:  // ROTATION_180
                                    fx = (1.0f - s_rawX) * screenW;
                                    fy = (1.0f - s_rawY) * screenH;
                                    break;
                                case 3:  // ROTATION_270 (顺时针, 反向横屏)
                                    fx = s_rawY * screenW;
                                    fy = (1.0f - s_rawX) * screenH;
                                    break;
                            }

                            s_curX = fx;
                            s_curY = fy;
                            io.MousePos = ImVec2(fx, fy);
                        }
                        io.MouseDown[0] = s_touching;
                    }
                    break;
            }
        }
    }

    static void drawDebugIndicator() {
        if (s_fd < 0 || s_curX < 0.0f) return;
        auto* dl = ImGui::GetForegroundDrawList();
        ImU32 color = s_touching ? IM_COL32(255, 0, 0, 180) : IM_COL32(255, 255, 0, 80);
        dl->AddCircleFilled(ImVec2(s_curX, s_curY), 15.0f, color);
    }

} // namespace touch_input

// ═══════════════════════════════════════════════════════════════════════════════
// DrawGameOverlay — ImGui 覆盖层绘制
// ═══════════════════════════════════════════════════════════════════════════════

static void DrawGameOverlay(const lol::MiniMapData& data, bool inBattle) {

    // ═══ 1. 状态/信息面板（左上角，按钮折叠） ═══
    static bool s_panelExpanded = true;   // 面板展开/折叠状态

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.65f);
    ImGui::Begin("Game Info", nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

    // ── 折叠/展开 按钮 (单击即可, 触摸友好) ──
    if (ImGui::Button(s_panelExpanded ? "[-] Hide" : "[+] Show", ImVec2(120, 0))) {
        s_panelExpanded = !s_panelExpanded;
    }

    // ── 触摸旋转切换按钮 (如果自动检测方向不对, 点击切换) ──
    ImGui::SameLine();
    {
        char rotBtn[32];
        snprintf(rotBtn, sizeof(rotBtn), "Rot:%s", touch_input::getRotationLabel());
        if (ImGui::Button(rotBtn, ImVec2(100, 0))) {
            touch_input::cycleRotation();
        }
    }

    if (!inBattle) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[Waiting]");
        ImGui::End();
        return;
    }

    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "IN BATTLE");

    if (s_panelExpanded) {
        ImGui::Separator();

        // ── 敌方英雄信息 (可折叠) ──
        if (ImGui::CollapsingHeader("Enemy Heroes", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Count: %d", (int)data.enemyHeroes.size());
            ImGui::Spacing();

            for (size_t i = 0; i < data.enemyHeroes.size(); i++) {
                const auto& hero = data.enemyHeroes[i];
                ImGui::PushID((int)i);

                const char* name = hero.heroName.empty() ? "Unknown" : hero.heroName.c_str();
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
                                   "[Lv.%u] %s", hero.heroLevel, name);

                if (!hero.summonerName.empty()) {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                                       "(%s)", hero.summonerName.c_str());
                }

                float hpRatio = (hero.maxHp > 0.0f) ? (hero.curHp / hero.maxHp) : 0.0f;
                hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
                ImVec4 hpColor = (hpRatio > 0.5f)  ? ImVec4(0.2f, 0.9f, 0.2f, 1.0f) :
                                 (hpRatio > 0.25f) ? ImVec4(0.9f, 0.9f, 0.2f, 1.0f) :
                                                      ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
                char hpOverlay[64];
                snprintf(hpOverlay, sizeof(hpOverlay), "%.0f / %.0f", hero.curHp, hero.maxHp);

                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, hpColor);
                ImGui::ProgressBar(hpRatio, ImVec2(280, 0), hpOverlay);
                ImGui::PopStyleColor();

                if (hero.hasWorldPos) {
                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
                                       "  Pos: (%.0f, %.0f, %.0f)",
                                       hero.worldPos.x, hero.worldPos.y, hero.worldPos.z);
                }

                ImGui::Separator();
                ImGui::PopID();
            }
        }

        // ── 眼位信息 (可折叠) ──
        if (!data.wards.empty()) {
            if (ImGui::CollapsingHeader("Wards", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Count: %d", (int)data.wards.size());
                for (size_t i = 0; i < data.wards.size(); i++) {
                    const auto& ward = data.wards[i];
                    if (ward.hasWorldPos) {
                        ImGui::Text("  Ward#%d (%.0f, %.0f) type=%d",
                                    (int)(i + 1),
                                    ward.worldPos.x, ward.worldPos.z,
                                    ward.iconType);
                    }
                }
            }
        }
    } // end s_panelExpanded

    ImGui::End();


    // ═══ 2. ESP 方框覆盖层（World-to-Screen 屏幕投影） ═══
    {
        ImGuiIO& espIo = ImGui::GetIO();
        const float screenW = espIo.DisplaySize.x;
        const float screenH = espIo.DisplaySize.y;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(screenW, screenH));
        ImGui::Begin("##ESP", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImDrawList* espDraw = ImGui::GetWindowDrawList();

        for (const auto& hero : data.enemyHeroes) {
            if (!hero.hasScreenPos) continue;

            float sx = hero.screenX;
            float sy = screenH - hero.screenY;

            constexpr float kMargin = 100.0f;
            if (sx < -kMargin || sx > screenW + kMargin ||
                sy < -kMargin || sy > screenH + kMargin)
                continue;

            const float boxW = 60.0f;
            const float boxH = 120.0f;
            ImVec2 boxMin(sx - boxW * 0.5f, sy - boxH * 0.8f);
            ImVec2 boxMax(sx + boxW * 0.5f, sy + boxH * 0.2f);

            float hpRatio = (hero.maxHp > 0.0f) ? (hero.curHp / hero.maxHp) : 0.0f;
            hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
            ImU32 boxColor = (hpRatio > 0.5f)  ? IM_COL32(0, 255, 0, 220) :
                             (hpRatio > 0.25f) ? IM_COL32(255, 255, 0, 220) :
                                                  IM_COL32(255, 0, 0, 220);

            espDraw->AddRectFilled(boxMin, boxMax, IM_COL32(0, 0, 0, 40));
            espDraw->AddRect(boxMin, boxMax, boxColor, 0.0f, 0, 2.0f);

            const float hpBarH = 4.0f;
            const float hpBarY = boxMin.y - hpBarH - 2.0f;
            ImVec2 hpBgMin(boxMin.x, hpBarY);
            ImVec2 hpBgMax(boxMax.x, hpBarY + hpBarH);
            ImVec2 hpFgMax(boxMin.x + (boxMax.x - boxMin.x) * hpRatio, hpBarY + hpBarH);
            espDraw->AddRectFilled(hpBgMin, hpBgMax, IM_COL32(0, 0, 0, 160));
            espDraw->AddRectFilled(hpBgMin, hpFgMax, boxColor);

            const char* name = hero.heroName.empty() ? nullptr : hero.heroName.c_str();
            if (name) {
                ImVec2 textSize = ImGui::CalcTextSize(name);
                float textX = sx - textSize.x * 0.5f;
                float textY = hpBarY - textSize.y - 1.0f;
                espDraw->AddText(ImVec2(textX + 1, textY + 1), IM_COL32(0, 0, 0, 200), name);
                espDraw->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 240), name);
            }

            char lvlBuf[16];
            snprintf(lvlBuf, sizeof(lvlBuf), "Lv%u", hero.heroLevel);
            espDraw->AddText(ImVec2(boxMax.x + 2, boxMin.y),
                             IM_COL32(255, 255, 100, 240), lvlBuf);

            char hpBuf[32];
            snprintf(hpBuf, sizeof(hpBuf), "%.0f/%.0f", hero.curHp, hero.maxHp);
            ImVec2 hpTextSize = ImGui::CalcTextSize(hpBuf);
            espDraw->AddText(ImVec2(sx - hpTextSize.x * 0.5f, boxMax.y + 2),
                             IM_COL32(200, 200, 200, 220), hpBuf);
        }

        ImGui::End();
    }

    // ═══ 3. 小地图雷达（全图视野, 显示所有玩家） ═══
    {
        ImGuiIO& io = ImGui::GetIO();
        // 小地图边长 = 屏幕短边 × 比例系数 (0.30 = 30%)
        // 调大此值可放大小地图, 例如 0.40f=40%, 0.50f=50%
        const float radarSize = std::min(io.DisplaySize.x, io.DisplaySize.y) * 0.35f;
        const float margin = 10.0f;   // 雷达窗口距屏幕边缘的间距 (像素)
        const float winPad = 6.0f;    // 雷达窗口内边距 (像素)

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(winPad, winPad));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        float winSize = radarSize + winPad * 2;
        ImGui::SetNextWindowPos(
            ImVec2(io.DisplaySize.x - winSize - margin,
                   io.DisplaySize.y - winSize - margin),
            ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(winSize, winSize), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("## Radar", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoScrollbar);

        ImVec2 O = ImGui::GetCursorScreenPos();
        ImVec2 E(O.x + radarSize, O.y + radarSize);
        ImDrawList* dl = ImGui::GetWindowDrawList();

        // 快捷宏: 全图世界坐标 → 雷达像素
        #define M(wx,wz) XZToMinimap(wx, wz, O, radarSize)

        // ── 裁剪区域 ──
        dl->PushClipRect(O, E, true);

        // ── 地图底色 (透明) ──
        dl->AddRectFilled(O, E, IM_COL32(0, 0, 0, 0));

        // ── 河道 (蓝色斜带) ──
        {
            float rw = radarSize * 0.04f;
            ImVec2 r1 = M(2000, 12600), r2 = M(12600, 2000);
            dl->AddLine(r1, r2, IM_COL32(30, 50, 90, 180), rw);
            dl->AddLine(r1, r2, IM_COL32(50, 80, 140, 60), rw * 0.5f);
        }

        // ── 三条路线 (土黄色路径) ──
        {
            ImU32 laneCol = IM_COL32(80, 70, 45, 150);
            float lw = radarSize * 0.02f;
            dl->AddLine(M(1200,1200), M(1200,13400), laneCol, lw);
            dl->AddLine(M(1200,13400), M(13400,13400), laneCol, lw);
            dl->AddLine(M(1200,1200), M(13400,1200), laneCol, lw);
            dl->AddLine(M(13400,1200), M(13400,13400), laneCol, lw);
            dl->AddLine(M(2000,2000), M(12600,12600), laneCol, lw);
        }

        // ── 草丛 (浅绿小块) ──
        {
            ImU32 bushCol = IM_COL32(35, 65, 30, 200);
            float bs = radarSize * 0.02f;
            auto bush = [&](float wx, float wz) {
                ImVec2 c = M(wx, wz);
                dl->AddRectFilled(ImVec2(c.x-bs, c.y-bs*0.6f),
                                  ImVec2(c.x+bs, c.y+bs*0.6f), bushCol, bs*0.3f);
            };
            bush(3500,10500); bush(4500,9500); bush(10500,5000); bush(9500,4500);
            bush(6000,8500);  bush(8500,6000); bush(3000,3000);  bush(11500,11500);
        }

        // ── 基地 (蓝/红) ──
        {
            float baseR = radarSize * 0.025f;
            ImVec2 bb = M(500, 500), rb = M(14100, 14100);
            dl->AddCircleFilled(bb, baseR, IM_COL32(30,100,220,180), 16);
            dl->AddCircle(bb, baseR, IM_COL32(60,140,255,220), 16, 1.5f);
            dl->AddCircleFilled(rb, baseR, IM_COL32(200,40,40,180), 16);
            dl->AddCircle(rb, baseR, IM_COL32(255,80,80,220), 16, 1.5f);
        }

        // ── 大龙/小龙 ──
        {
            float pitR = radarSize * 0.012f;
            ImVec2 baron = M(4200, 10200);
            dl->AddCircle(baron, pitR, IM_COL32(180,50,200,180), 12, 1.5f);
            dl->AddText(ImVec2(baron.x-3, baron.y-4), IM_COL32(200,100,220,200), "B");
            ImVec2 dragon = M(10200, 4200);
            dl->AddCircle(dragon, pitR, IM_COL32(220,160,30,180), 12, 1.5f);
            dl->AddText(ImVec2(dragon.x-3, dragon.y-4), IM_COL32(230,180,50,200), "D");
        }

        // ── 防御塔 (小方块) ──
        {
            float tw = radarSize * 0.006f;
            auto tower = [&](float wx, float wz, ImU32 col) {
                ImVec2 c = M(wx, wz);
                dl->AddRectFilled(ImVec2(c.x-tw,c.y-tw), ImVec2(c.x+tw,c.y+tw), col);
            };
            ImU32 bT = IM_COL32(60,140,255,200), rT = IM_COL32(255,80,80,200);
            tower(1200,5500,bT); tower(3500,3500,bT); tower(5500,1200,bT);
            tower(13400,9000,rT); tower(11000,11000,rT); tower(9000,13400,rT);
        }

        // ── 边框 ──
        dl->AddRect(O, E, IM_COL32(80,120,80,250), 0.0f, 0, 2.0f);

        // ── 动画脉冲 ──
        timespec ts_r{};
        clock_gettime(CLOCK_MONOTONIC, &ts_r);
        float anim = (float)ts_r.tv_sec + (float)ts_r.tv_nsec / 1e9f;
        float pulse = (std::sin(anim * 3.0f) + 1.0f) * 0.5f;

        // 英雄圆点半径 (缩小以避免重叠)
        const float heroR = std::max(radarSize * 0.018f, 3.0f);
        const float wardR = std::max(radarSize * 0.012f, 2.0f);

        // ── 绘制所有英雄 ──
        for (const auto& hero : data.enemyHeroes) {
            if (!hero.hasWorldPos) continue;
            ImVec2 p = WorldToMinimap(hero.worldPos, O, radarSize);

            const bool isMyTeam = (hero.iconType == 1);  // MiniMapIconType_MyTeamHero

            if (isMyTeam) {
                // ── 己方英雄: 绿色圆 + 脉冲 ──
                float oR = heroR * (1.4f + 0.25f * pulse);
                dl->AddCircle(p, oR, IM_COL32(80, 220, 80, (int)(40 + 30 * pulse)), 0, 1.0f);
                dl->AddCircleFilled(p, heroR, IM_COL32(30, 200, 30, 180), 16);
                dl->AddCircle(p, heroR, IM_COL32(60, 255, 60, 240), 16, 1.5f);

                // 等级 (右上角, 小字)
                char lv[8]; snprintf(lv, sizeof(lv), "%u", hero.heroLevel);
                dl->AddText(ImVec2(p.x + heroR + 1, p.y - heroR - 2),
                            IM_COL32(100, 255, 100, 230), lv);
            } else {
                // ── 敌方英雄: 根据 HP 变色的圆 ──
                float hp = (hero.maxHp > 0) ? std::clamp(hero.curHp / hero.maxHp, 0.f, 1.f) : 0.f;
                float cr = (hp > 0.5f) ? (1.f - hp) * 2.f : 1.f;
                float cg = (hp > 0.5f) ? 1.f : hp * 2.f;
                ImU32 col  = IM_COL32((int)(cr * 255), (int)(cg * 255), 40, 255);
                ImU32 fill = IM_COL32((int)(cr * 255), (int)(cg * 255), 40, 120);

                // 脉冲外圈
                float oR = heroR * (1.4f + 0.25f * pulse);
                dl->AddCircle(p, oR, IM_COL32((int)(cr * 255), (int)(cg * 255), 40,
                              (int)(30 + 25 * pulse)), 0, 1.0f);

                // 实心圆
                dl->AddCircleFilled(p, heroR, fill, 16);
                dl->AddCircle(p, heroR, col, 16, 1.5f);

                // 小 HP 条 (紧贴圆点下方, 窄)
                if (hero.maxHp > 0) {
                    float bw = heroR * 2.f;
                    float bh = std::max(1.5f, heroR * 0.2f);
                    float bx = p.x - heroR;
                    float by = p.y + heroR + 1.f;
                    dl->AddRectFilled(ImVec2(bx, by), ImVec2(bx + bw, by + bh),
                                      IM_COL32(0, 0, 0, 140));
                    dl->AddRectFilled(ImVec2(bx, by), ImVec2(bx + bw * hp, by + bh), col);
                }

                // 等级 (右上角, 小字)
                char lv[8]; snprintf(lv, sizeof(lv), "%u", hero.heroLevel);
                dl->AddText(ImVec2(p.x + heroR + 1, p.y - heroR - 2),
                            IM_COL32(255, 255, 100, 220), lv);
            }


            // ── [DEBUG] 在每个英雄旁显示真实世界坐标 ──
            {
                char coordBuf[48];
                snprintf(coordBuf, sizeof(coordBuf), "(%.0f,%.0f)",
                         hero.worldPos.x, hero.worldPos.z);
                ImU32 dbgCol = isMyTeam ? IM_COL32(150, 255, 150, 255)
                                        : IM_COL32(255, 200, 150, 255);
                dl->AddText(ImVec2(p.x - 20, p.y + heroR + 6),
                            IM_COL32(0, 0, 0, 200), coordBuf);
                dl->AddText(ImVec2(p.x - 21, p.y + heroR + 5), dbgCol, coordBuf);
            }

        }

        // ── 眼位 (蓝色小菱形) ──
        for (const auto& ward : data.wards) {
            if (!ward.hasWorldPos) continue;
            ImVec2 p = WorldToMinimap(ward.worldPos, O, radarSize);
            dl->AddCircleFilled(p, wardR, IM_COL32(50, 150, 255, 100));
            dl->AddCircle(p, wardR, IM_COL32(100, 200, 255, 220), 0, 1.0f);
            float s = wardR * 0.5f;
            dl->AddQuadFilled(ImVec2(p.x, p.y - s), ImVec2(p.x + s, p.y),
                              ImVec2(p.x, p.y + s), ImVec2(p.x - s, p.y),
                              IM_COL32(120, 210, 255, 240));
        }

        // ── 恢复裁剪 ──
        dl->PopClipRect();

        #undef M

        ImGui::Dummy(ImVec2(radarSize, radarSize));
        ImGui::End();
        ImGui::PopStyleVar(2);
    }
}


// ═══════════════════════════════════════════════════════════════════════════════
// eglSwapBuffers Hook — 直接绘制方案
//
// hook 游戏的 eglSwapBuffers，在每帧提交前绘制 ImGui 覆盖层。
// eglSwapBuffers 是 EGL 公开 API，dlsym 直接获取，无需 DobbySymbolResolver。
// ImGui OpenGL3 后端自动保存/恢复 GL 状态，不影响游戏渲染。
// ═══════════════════════════════════════════════════════════════════════════════

namespace egl_hook {
    static EGLBoolean (*g_origSwapBuffers)(EGLDisplay, EGLSurface) = nullptr;
    static std::atomic<bool> g_ready{false};
    static ImGuiContext*      g_ctx       = nullptr;
    static int                g_width     = 0;
    static int                g_height    = 0;
    static double             g_lastTime  = 0.0;

    static bool initImGui(EGLDisplay display, EGLSurface surface) {
        EGLint w = 0, h = 0;
        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);
        if (w <= 0 || h <= 0) {
            LOG(LOG_LEVEL_ERROR, "[EglHook] surface 尺寸无效: %dx%d", w, h);
            return false;
        }
        g_width  = w;
        g_height = h;

        IMGUI_CHECKVERSION();
        g_ctx = ImGui::CreateContext();
        if (!g_ctx) {
            LOG(LOG_LEVEL_ERROR, "[EglHook] ImGui::CreateContext 失败");
            return false;
        }

        auto& io       = ImGui::GetIO();
        io.IniFilename = nullptr;
        io.DisplaySize = ImVec2((float)w, (float)h);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(3.f);

        // ── 加载中文字体 (stb_truetype 内置光栅化器, 支持 TrueType) ──
        const float fontSize = 22.0f;
        io.Fonts->TexDesiredWidth = 4096;

        // 辅助: 用 open()/read() 读文件到 IM_ALLOC 内存 (绕过 fopen SELinux)
        auto tryLoadFont = [&](const char* path, int fontNo = 0) -> bool {
            struct stat st{};
            if (stat(path, &st) != 0 || !S_ISREG(st.st_mode)) return false;

            int fd = open(path, O_RDONLY);
            if (fd < 0) return false;
            int fileSize = (int)st.st_size;
            void* data = IM_ALLOC(fileSize);
            if (!data) { close(fd); return false; }
            size_t total = 0;
            while (total < (size_t)fileSize) {
                ssize_t n = read(fd, (char*)data + total, fileSize - total);
                if (n <= 0) break;
                total += n;
            }
            close(fd);
            if ((int)total != fileSize) { IM_FREE(data); return false; }

            ImFontConfig cfg;
            cfg.FontDataOwnedByAtlas = true;
            cfg.OversampleH = 1;
            cfg.OversampleV = 1;
            cfg.PixelSnapH  = true;
            cfg.FontNo      = fontNo;

            ImFont* font = io.Fonts->AddFontFromMemoryTTF(
                    data, fileSize, fontSize, &cfg,
                    io.Fonts->GetGlyphRangesChineseFull());
            if (!font) { IM_FREE(data); return false; }

            if (!io.Fonts->Build()) {
                LOG(LOG_LEVEL_INFO, "[Font] 跳过 (stb_truetype 不支持此格式): %s", path);
                io.Fonts->Clear();
                return false;
            }

            // 验证中文字形
            const ImFontGlyph* g = font->FindGlyphNoFallback((ImWchar)0x4E2D);
            if (!g) {
                LOG(LOG_LEVEL_WARN, "[Font] 无中文字形: %s [FontNo=%d]", path, fontNo);
                io.Fonts->Clear();
                return false;
            }

            LOG(LOG_LEVEL_INFO, "[Font] ✓ %s [FontNo=%d] (%d bytes, atlas %dx%d)",
                path, fontNo, fileSize, io.Fonts->TexWidth, io.Fonts->TexHeight);
            return true;
        };

        // 候选列表 (按优先级)
        // stb_truetype 仅支持 TrueType 轮廓 (.ttf)
        // CFF/CFF2 轮廓 (.otf, 部分 .ttc) 需要 FreeType, 放到最后作为兜底
        const char* fontPaths[] = {
            // ── 用户自定义 (最高优先级) ──
            "/data/local/tmp/chinese.ttf",
            // ── TrueType .ttf (stb_truetype 原生支持) ──
            "/system/fonts/DroidSansFallback.ttf",         // Android 4.x-6.x, 广泛兼容
            "/system/fonts/NotoSansSC-Regular.ttf",        // 部分 Android 用 .ttf 版本
            "/system/fonts/NotoSansCJKsc-Regular.ttf",
            "/system/fonts/MiLanProVF.ttf",                // 小米
            "/system/fonts/HarmonyOS_Sans_SC.ttf",         // 华为
            "/system/fonts/OPPOSans-Regular.ttf",          // OPPO
            "/system/fonts/VivoSans-Regular.ttf",          // vivo
            "/system/fonts/RobotoFallback-Regular.ttf",    // 部分原生 ROM
            // ── CFF/CFF2 格式 (stb_truetype 可能不支持, 放最后) ──
            "/system/fonts/NotoSansSC-Regular.otf",
            "/system/fonts/NotoSansHans-Regular.otf",
            "/system/fonts/NotoSansCJKsc-Regular.otf",
            "/system/fonts/NotoSansSC-Regular.ttc",
            "/system/fonts/NotoSansCJKsc-Regular.ttc",
            "/system/fonts/NotoSansCJK-Regular.ttc",       // CFF2, 需要 FreeType
            "/data/local/tmp/chinese.ttc",
        };

        bool fontOK = false;
        for (const char* path : fontPaths) {
            if (tryLoadFont(path)) { fontOK = true; break; }
        }

        // 扫描 /system/fonts/ 兜底
        if (!fontOK) {
            DIR* dir = opendir("/system/fonts");
            if (dir) {
                struct dirent* entry;
                while (!fontOK && (entry = readdir(dir)) != nullptr) {
                    const char* name = entry->d_name;
                    size_t len = strlen(name);
                    if (len < 5) continue;
                    const char* ext = name + len - 4;
                    if (strcasecmp(ext, ".ttf") != 0 && strcasecmp(ext, ".otf") != 0 &&
                        strcasecmp(ext, ".ttc") != 0) continue;
                    char full[256];
                    snprintf(full, sizeof(full), "/system/fonts/%s", name);
                    if (tryLoadFont(full)) fontOK = true;
                }
                closedir(dir);
            }
        }

        // 全部失败 → 默认 ASCII 字体
        if (!fontOK) {
            LOG(LOG_LEVEL_WARN, "[EglHook] ⚠ 未找到中文字体, 回退默认字体");
            ImFontConfig fallbackCfg;
            fallbackCfg.SizePixels = fontSize;
            io.Fonts->AddFontDefault(&fallbackCfg);
            io.Fonts->Build();
        }

        LOG(LOG_LEVEL_INFO, "[EglHook] 字体图集: %dx%d", io.Fonts->TexWidth, io.Fonts->TexHeight);

        if (!ImGui_ImplOpenGL3_Init("#version 300 es")) {
            LOG(LOG_LEVEL_ERROR, "[EglHook] ImGui_ImplOpenGL3_Init 失败");
            ImGui::DestroyContext(g_ctx);
            g_ctx = nullptr;
            return false;
        }

        LOG(LOG_LEVEL_INFO, "[EglHook] ✓ ImGui 初始化完成 (%dx%d)", w, h);
        return true;
    }

    static EGLBoolean hooked_eglSwapBuffers(EGLDisplay display, EGLSurface surface) {
        if (!g_ready.load(std::memory_order_relaxed)) {
            if (initImGui(display, surface))
                g_ready.store(true, std::memory_order_release);
        }

        if (g_ready.load(std::memory_order_acquire) && g_ctx) {
            ImGui::SetCurrentContext(g_ctx);

            // ── 触摸输入: 自动重试初始化 + 每帧处理事件 ──
            touch_input::init();       // 内部有频率限制, 每 5s 重试一次
            touch_input::processEvents((float)g_width, (float)g_height);

            EGLint curW = 0, curH = 0;
            eglQuerySurface(display, surface, EGL_WIDTH,  &curW);
            eglQuerySurface(display, surface, EGL_HEIGHT, &curH);
            if (curW > 0 && curH > 0 && (curW != g_width || curH != g_height)) {
                g_width  = curW;
                g_height = curH;
            }

            auto& io     = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)g_width, (float)g_height);

            timespec ts{};
            clock_gettime(CLOCK_MONOTONIC, &ts);
            double now   = (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
            io.DeltaTime = g_lastTime > 0.0 ? (float)(now - g_lastTime) : (1.0f / 60.0f);
            g_lastTime   = now;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            static lol::MiniMapData gameData;
            SharedGameData::getInstance().pullData(gameData);
            bool inBattle = SharedGameData::getInstance().isBattleActive();
            DrawGameOverlay(gameData, inBattle);

            // 触摸调试: 红色圆点跟随手指 (确认触摸是否生效)
            touch_input::drawDebugIndicator();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        return g_origSwapBuffers(display, surface);
    }
} // namespace egl_hook

// ═══════════════════════════════════════════════════════════════════════════════
// GuiNativeThread — 安装 eglSwapBuffers hook
// ═══════════════════════════════════════════════════════════════════════════════

static void GuiNativeThread() {
    LOG(LOG_LEVEL_INFO, "[GuiNative] ══════ 启动直接绘制模式 (eglSwapBuffers Hook) ══════");
    installCrashGuard();


    void* swapAddr = dlsym(RTLD_DEFAULT, "eglSwapBuffers");
    if (!swapAddr) {
        void* eglLib = dlopen("libEGL.so", RTLD_LAZY);
        if (eglLib) {
            swapAddr = dlsym(eglLib, "eglSwapBuffers");
            dlclose(eglLib);
        }
    }
    if (!swapAddr) {
        LOG(LOG_LEVEL_ERROR, "[GuiNative] ✘ 无法找到 eglSwapBuffers");
        return;
    }
    LOG(LOG_LEVEL_INFO, "[GuiNative] ✓ eglSwapBuffers → %p", swapAddr);

    {
        int crashSig = sigsetjmp(t_jumpBuf, 1);
        if (crashSig != 0) {
            LOG(LOG_LEVEL_ERROR, "[GuiNative] ✘ DobbyHook 触发信号 %d", crashSig);
            t_guardActive = 0;
            return;
        }
        t_guardActive = 1;

        int ret = DobbyHook(
            swapAddr,
            (dobby_dummy_func_t)egl_hook::hooked_eglSwapBuffers,
            (dobby_dummy_func_t*)&egl_hook::g_origSwapBuffers
        );
        t_guardActive = 0;

        if (ret != 0) {
            LOG(LOG_LEVEL_ERROR, "[GuiNative] ✘ DobbyHook 安装失败 (ret=%d)", ret);
            return;
        }
    }

    LOG(LOG_LEVEL_INFO, "[GuiNative] ✓ eglSwapBuffers hook 已安装");
}

// ═══════════════════════════════════════════════════════════════════════════════
// TestFunction — 数据采集主入口 (实时刷新)
// ═══════════════════════════════════════════════════════════════════════════════

static void TestFunction(void *pli2cppModeBase, void *pCodeRegistration,
                         void *pMetadataRegistration, void *pGlobalMetadataHeader,
                         void *pMetadataImagesTable) {

    LOG(LOG_LEVEL_INFO, "[TestFunction] ══════ 启动 ══════");
    installCrashGuard();

    lol::lol lol(pli2cppModeBase, pCodeRegistration,
                 pMetadataRegistration, pGlobalMetadataHeader, pMetadataImagesTable);
    LOG(LOG_LEVEL_INFO, "[TestFunction] 游戏数据采集初始化完成 ✓");

    constexpr int kCollectMs = 50;
    constexpr int kPrintMs   = 5000;
    auto lastCollect = std::chrono::steady_clock::now();
    auto lastPrint   = lastCollect;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        if (now - lastCollect >= std::chrono::milliseconds(kCollectMs)) {

            int crashSig = sigsetjmp(t_jumpBuf, 1);
            if (crashSig != 0) {
                LOG(LOG_LEVEL_ERROR, "[TestFunction] ⚠ 捕获信号 %d，跳过本次采集", crashSig);
                t_guardActive = 0;
                SharedGameData::getInstance().setBattleActive(false);
            } else {
                t_guardActive = 1;
                bool isBattle = lol.get_BattleStarted();
                SharedGameData::getInstance().setBattleActive(isBattle);
                if (isBattle) {
                    lol.updateMiniMapData();
                    SharedGameData::getInstance().pushData(lol.getMiniMapData());

                    if (now - lastPrint >= std::chrono::milliseconds(kPrintMs)) {
                        lol.printMiniMapData();
                        lastPrint = now;
                    }
                }
                t_guardActive = 0;
            }
            lastCollect = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// MyStartPoint — 注入入口
// ═══════════════════════════════════════════════════════════════════════════════

bool MyStartPoint(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                  void *pGlobalMetadataHeader, void *pMetadataImagesTable) {
    try {
        if (pli2cppModeBase == nullptr) {
            LOG(LOG_LEVEL_INFO, "[MyStartPoint] 独立应用模式 — 跳过");
            return false;
        }

        LOG(LOG_LEVEL_INFO, "[MyStartPoint] 注入模式 — 启动数据采集 + eglSwapBuffers Hook 绘制");
        installCrashGuard();

        std::thread(GuiNativeThread).detach();

        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            TestFunction(pli2cppModeBase, pCodeRegistration,
                         pMetadataRegistration, pGlobalMetadataHeader, pMetadataImagesTable);
        }).detach();

        return true;
    }
    catch (const std::exception& e) {
        LOG(LOG_LEVEL_ERROR, "[MyStartPoint] 异常: %s", e.what());
        return false;
    }
    catch (...) {
        LOG(LOG_LEVEL_ERROR, "[MyStartPoint] 未知异常");
        return false;
    }
}

