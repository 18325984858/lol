//
// Created by Song on 2025/11/10.
//

#include "start.h"
#include "./Log/log.h"
#include "./lol/lolm.h"
#include "./il2cppHeader/il2cppHeader.h"
#include "./li2cppDumper/li2cppdumper.h"
#include "Draw/Draw.h"
#include <chrono>
#include <thread>
#include <csignal>
#include <csetjmp>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <cmath>
#include <cctype>
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
// 触摸输入 — 读取 /dev/input/event* 原始触摸事件, 喂给 ImGui IO
//
// 游戏进程无权直接读 /dev/input/event* (SELinux + DAC)。
// 需要用户通过 ADB 手动提权:
//   adb shell su -c "chmod 666 /dev/input/event*"
// init() 会每隔几秒自动重试, chmod 生效后即可使用。
// ═══════════════════════════════════════════════════════════════════════════════

namespace touch_input {

    static int   s_fd       = -2;     // -2=未初始化/重试中, -1=永久失败, >=0=有效
    static char  s_devPath[64] = {0};
    static char  s_devName[128] = {0};
    static float s_absMaxX  = 1.0f;   // 数字化仪 X 轴最大值 (物理竖屏坐标系)
    static float s_absMaxY  = 1.0f;   // 数字化仪 Y 轴最大值
    static float s_rawX     = -1.0f;  // 归一化原始触摸坐标 (0..1, 数字化仪坐标系)
    static float s_rawY     = -1.0f;
    static float s_curX     = -1.0f;  // 转换后的屏幕坐标 (已应用旋转)
    static float s_curY     = -1.0f;
    static bool  s_touching = false;
    static int   s_retryCount = 0;
    static std::chrono::steady_clock::time_point s_lastRetry{};
    static bool  s_loggedOpenPermissionError = false;

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
    static int s_rotation = 3;

    /** @brief 循环切换旋转模式 (供 UI 按钮调用) */
    void cycleRotation() {
        if (s_rotation < 0) s_rotation = 0;   // 从自动 → 手动0
        else s_rotation = (s_rotation + 1) % 4;
        LOG(LOG_LEVEL_INFO, "[Touch] 旋转模式切换 → %d (0=无, 1=90°, 2=180°, 3=270°)", s_rotation);
    }

    /** @brief 获取当前旋转值 */
    int getRotation() { return s_rotation; }

    /** @brief 获取当前旋转模式描述 */
    const char* getRotationLabel() {
        switch (s_rotation) {
            case -1: return "Auto";
            case 0:  return "0";
            case 1:  return "90";
            case 2:  return "180";
            case 3:  return "270";
            default: return "?";
        }
    }

    int getFd() { return s_fd; }
    const char* getDevicePath() { return s_devPath[0] ? s_devPath : "<none>"; }
    const char* getDeviceName() { return s_devName[0] ? s_devName : "<unknown>"; }
    float getRawX() { return s_rawX; }
    float getRawY() { return s_rawY; }
    float getCurX() { return s_curX; }
    float getCurY() { return s_curY; }
    bool isTouching() { return s_touching; }

    struct TouchDeviceCandidate {
        int fd = -1;
        int score = -1;
        bool hasMt = false;
        bool hasSingleTouch = false;
        bool hasBtnTouch = false;
        bool hasDirect = false;
        float absMaxX = 1.0f;
        float absMaxY = 1.0f;
        char path[64] = {0};
        char name[128] = {0};
    };

    static void toLowerCopy(const char* src, char* dst, size_t dstSize) {
        if (!dst || dstSize == 0) return;
        dst[0] = '\0';
        if (!src) return;
        size_t i = 0;
        for (; src[i] != '\0' && i + 1 < dstSize; ++i) {
            dst[i] = (char)std::tolower((unsigned char)src[i]);
        }
        dst[i] = '\0';
    }

    static bool containsKeyword(const char* text, const char* keyword) {
        return text && keyword && std::strstr(text, keyword) != nullptr;
    }

    static bool buildTouchDeviceCandidate(const char* path, TouchDeviceCandidate& out) {
        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd < 0) {
            if (!s_loggedOpenPermissionError && (errno == EACCES || errno == EPERM)) {
                s_loggedOpenPermissionError = true;
                LOG(LOG_LEVEL_WARN,
                    "[Touch] 无法打开 %s: errno=%d (%s). 通常是 /dev/input/event* 权限不足, 执行: adb shell su -c \"chmod 666 /dev/input/event*\"",
                    path, errno, strerror(errno));
            }
            return false;
        }

        constexpr size_t kBitsPerLong = sizeof(unsigned long) * 8;
        constexpr auto bitArrayWords = [](size_t maxBit) constexpr {
            return (maxBit / kBitsPerLong) + 1;
        };
        auto hasBit = [&](const unsigned long* bits, size_t bit) -> bool {
            return ((bits[bit / kBitsPerLong] >> (bit % kBitsPerLong)) & 1UL) != 0;
        };

        unsigned long evBits[bitArrayWords(EV_MAX)] = {};
        unsigned long absBits[bitArrayWords(ABS_MAX)] = {};
        unsigned long keyBits[bitArrayWords(KEY_MAX)] = {};
        unsigned long propBits[bitArrayWords(INPUT_PROP_MAX)] = {};
        if (ioctl(fd, EVIOCGBIT(0, sizeof(evBits)), evBits) < 0 ||
            ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absBits)), absBits) < 0) {
            close(fd);
            return false;
        }

        ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits);
        ioctl(fd, EVIOCGPROP(sizeof(propBits)), propBits);

        char devName[128] = {0};
        ioctl(fd, EVIOCGNAME(sizeof(devName)), devName);

        bool hasMtX = hasBit(absBits, ABS_MT_POSITION_X);
        bool hasMtY = hasBit(absBits, ABS_MT_POSITION_Y);
        bool hasAbsX = hasBit(absBits, ABS_X);
        bool hasAbsY = hasBit(absBits, ABS_Y);
        bool hasBtnTouch = hasBit(keyBits, BTN_TOUCH);
        bool hasDirect = hasBit(propBits, INPUT_PROP_DIRECT);

        if ((!hasMtX || !hasMtY) && (!hasAbsX || !hasAbsY)) {
            close(fd);
            return false;
        }

        struct input_absinfo absX{}, absY{};
        int absCodeX = (hasMtX && hasMtY) ? ABS_MT_POSITION_X : ABS_X;
        int absCodeY = (hasMtX && hasMtY) ? ABS_MT_POSITION_Y : ABS_Y;
        if (ioctl(fd, EVIOCGABS(absCodeX), &absX) < 0 ||
            ioctl(fd, EVIOCGABS(absCodeY), &absY) < 0) {
            close(fd);
            return false;
        }

        char nameLower[128] = {0};
        toLowerCopy(devName, nameLower, sizeof(nameLower));

        int score = 0;
        if (hasMtX && hasMtY) score += 40;
        if (hasAbsX && hasAbsY) score += 10;
        if (hasBtnTouch) score += 20;
        if (hasDirect) score += 25;
        if (containsKeyword(nameLower, "touchscreen")) score += 80;
        if (containsKeyword(nameLower, "touch screen")) score += 80;
        if (containsKeyword(nameLower, "touch panel")) score += 70;
        if (containsKeyword(nameLower, "touchpanel")) score += 70;
        if (containsKeyword(nameLower, "_ts") || containsKeyword(nameLower, " ts")) score += 45;
        if (containsKeyword(nameLower, "sec_touchscreen")) score += 80;
        if (containsKeyword(nameLower, "fts") || containsKeyword(nameLower, "goodix") ||
            containsKeyword(nameLower, "synaptics") || containsKeyword(nameLower, "focaltech") ||
            containsKeyword(nameLower, "elan") || containsKeyword(nameLower, "novatek")) {
            score += 35;
        }
        if (containsKeyword(nameLower, "key") || containsKeyword(nameLower, "keyboard") ||
            containsKeyword(nameLower, "mouse") || containsKeyword(nameLower, "stylus") ||
            containsKeyword(nameLower, "fingerprint") || containsKeyword(nameLower, "sensor")) {
            score -= 40;
        }

        out.fd = fd;
        out.score = score;
        out.hasMt = hasMtX && hasMtY;
        out.hasSingleTouch = hasAbsX && hasAbsY;
        out.hasBtnTouch = hasBtnTouch;
        out.hasDirect = hasDirect;
        out.absMaxX = absX.maximum > 0 ? (float)absX.maximum : 1.0f;
        out.absMaxY = absY.maximum > 0 ? (float)absY.maximum : 1.0f;
        snprintf(out.path, sizeof(out.path), "%s", path);
        snprintf(out.name, sizeof(out.name), "%s", devName[0] ? devName : "<unnamed>");
        return true;
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

            // 尝试自动提权 (需要设备已 root)
            system("su -c 'setenforce 0' 2>/dev/null");
            int ret = system("su -c 'chmod 666 /dev/input/event*' 2>/dev/null");
            if (ret == 0) {
                LOG(LOG_LEVEL_INFO, "[Touch] 自动 chmod 成功");
            } else {
                LOG(LOG_LEVEL_WARN, "[Touch] 自动 chmod 失败(ret=%d), 需手动执行", ret);
            }
        }

        TouchDeviceCandidate best{};
        auto considerPath = [&](const char* path) {
            TouchDeviceCandidate candidate{};
            if (!buildTouchDeviceCandidate(path, candidate)) return;

            LOG(LOG_LEVEL_INFO,
                "[Touch] 候选设备: %s name=%s score=%d mt=%d abs=%d btnTouch=%d direct=%d range=(%.0f,%.0f)",
                candidate.path, candidate.name, candidate.score,
                candidate.hasMt ? 1 : 0,
                candidate.hasSingleTouch ? 1 : 0,
                candidate.hasBtnTouch ? 1 : 0,
                candidate.hasDirect ? 1 : 0,
                candidate.absMaxX, candidate.absMaxY);

            if (best.fd < 0 || candidate.score > best.score) {
                if (best.fd >= 0) close(best.fd);
                best = candidate;
            } else {
                close(candidate.fd);
            }
        };

        for (int i = 0; i < 32; i++) {
            char path[64];
            snprintf(path, sizeof(path), "/dev/input/event%d", i);
            considerPath(path);
        }

        DIR* dir = opendir("/dev/input");
        if (dir) {
            struct dirent* entry = nullptr;
            while ((entry = readdir(dir)) != nullptr) {
                if (strncmp(entry->d_name, "event", 5) != 0) continue;
                char path[128];
                snprintf(path, sizeof(path), "/dev/input/%s", entry->d_name);
                considerPath(path);
            }
            closedir(dir);
        }

        if (best.fd >= 0) {
            s_fd = best.fd;
            s_absMaxX = best.absMaxX;
            s_absMaxY = best.absMaxY;
            snprintf(s_devPath, sizeof(s_devPath), "%s", best.path);
            snprintf(s_devName, sizeof(s_devName), "%s", best.name);
            LOG(LOG_LEVEL_INFO, "[Touch] ✓ 选中触摸设备: %s (%s) score=%d (重试 %d 次)",
                s_devPath, s_devName, best.score, s_retryCount);
            LOG(LOG_LEVEL_INFO, "[Touch]   数字化仪范围: X=0..%.0f  Y=0..%.0f  (宽高比=%.2f)",
                s_absMaxX, s_absMaxY, s_absMaxX / s_absMaxY);
            return;
        }

        // 仍然失败, 保持 s_fd = -2 以便下次重试
        if (s_retryCount <= 3) {
            LOG(LOG_LEVEL_INFO, "[Touch] 等待权限或设备匹配... (第 %d 次尝试, 扫描 /dev/input/event0~31)", s_retryCount);
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
        bool posUpdated = false;  // 本轮 SYN_REPORT 是否收到位置更新
        while (read(s_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            switch (ev.type) {
                case EV_ABS:
                    // 多点触控协议 B (优先)
                    if (ev.code == ABS_MT_POSITION_X) {
                        s_rawX = (float)ev.value / s_absMaxX;
                        posUpdated = true;
                    } else if (ev.code == ABS_MT_POSITION_Y) {
                        s_rawY = (float)ev.value / s_absMaxY;
                        posUpdated = true;
                    }
                    // 单点触控协议 (fallback)
                    else if (ev.code == ABS_X) {
                        s_rawX = (float)ev.value / s_absMaxX;
                        posUpdated = true;
                    } else if (ev.code == ABS_Y) {
                        s_rawY = (float)ev.value / s_absMaxY;
                        posUpdated = true;
                    }
                    // 触摸状态
                    else if (ev.code == ABS_MT_TRACKING_ID) {
                        s_touching = (ev.value >= 0);
                    }
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
                                    rot = 1;  // 默认 ROTATION_90 (逆时针横屏, 最常见)
                                } else if (!surfaceLandscape && s_absMaxX > s_absMaxY * 1.1f) {
                                    rot = 1;  // 竖屏surface + 横屏digitizer (罕见, 尝试90°)
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
                        // 如果设备不发送 BTN_TOUCH/TRACKING_ID，通过位置更新推断触摸
                        if (posUpdated && !s_touching) {
                            s_touching = true;
                        }
                        io.MouseDown[0] = s_touching;
                        posUpdated = false;
                    }
                    break;
            }
        }
    }

    static void drawDebugIndicator() {
        if (s_fd < 0 || s_curX < 0.0f) return;
        if (!s_touching) return;  // 仅在触摸时显示
        auto* dl = ImGui::GetForegroundDrawList();
        ImVec2 pos(s_curX, s_curY);
        // 外圈白色环 + 内圈红色实心，更醒目
        dl->AddCircleFilled(pos, 22.0f, IM_COL32(255, 255, 255, 120));
        dl->AddCircleFilled(pos, 14.0f, IM_COL32(255, 50, 50, 220));
        dl->AddCircle(pos, 22.0f, IM_COL32(255, 255, 255, 200), 0, 2.0f);
    }

} // namespace touch_input


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
            static draw::GameOverlay overlay;
            SharedGameData::getInstance().pullData(gameData);
            bool inBattle = SharedGameData::getInstance().isBattleActive();
            overlay.drawOverlay(gameData, inBattle);

            // 触摸调试: 红色圆点跟随手指 (确认触摸是否生效)
            touch_input::drawDebugIndicator();

            // 触摸诊断日志 (每 3 秒打印一次)
            {
                static double s_lastDiag = 0.0;
                double nowD = (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
                if (nowD - s_lastDiag > 3.0) {
                    s_lastDiag = nowD;
                    LOG(LOG_LEVEL_INFO, "[TouchDiag] MousePos=(%.0f,%.0f) MouseDown=%d WantCaptureMouse=%d DisplaySize=(%.0f,%.0f) fd=%d dev=%s name=%s raw=(%.4f,%.4f) cur=(%.0f,%.0f) touching=%d rot=%s",
                        io.MousePos.x, io.MousePos.y,
                        io.MouseDown[0] ? 1 : 0,
                        io.WantCaptureMouse ? 1 : 0,
                        io.DisplaySize.x, io.DisplaySize.y,
                        touch_input::getFd(),
                        touch_input::getDevicePath(),
                        touch_input::getDeviceName(),
                        touch_input::getRawX(), touch_input::getRawY(),
                        touch_input::getCurX(), touch_input::getCurY(),
                        touch_input::isTouching() ? 1 : 0,
                        touch_input::getRotationLabel());
                }
            }

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
                // 崩溃时不改变状态，保留上一帧数据，避免闪烁
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

