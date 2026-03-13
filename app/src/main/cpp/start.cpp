//
// Created by Song on 2025/11/10.
//

#include "start.h"
#include "./li2cppDumper/li2cppdumper.h"
#include "./il2cppHeader/il2cppHeader.h"
#include "./File/file.h"
#include "./Log/log.h"
#include "./lol/lolm.h"
#include <chrono>
#include <thread>
#include <csignal>
#include <csetjmp>
#include <atomic>

// AndroidSurfaceImgui
#include "ANativeWindowCreator.h"
#include "AImGui.h"

// 线程安全游戏数据桥接
#include "SharedGameData.h"

// Dobby — 用于绕过 Android linker namespace 限制
#include "./Dobby/include/dobby.h"

#include <mutex>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <dlfcn.h>
#include <sys/system_properties.h>
#include <unordered_map>
#include <unordered_set>

#define _SOURCE_SO_PATH_ "libdobbyproject.so"
#define _Det_PACK_PATH_ "com.DefaultCompany.Demo1"

// ═══════════════════════════════════════════════════════════════════════════════
// SIGSEGV/SIGBUS 信号安全网 —— 变量声明 (提前声明, 供 customResolve 使用)
// ═══════════════════════════════════════════════════════════════════════════════

/// 每线程独立的跳转缓冲区（thread_local 确保多线程安全）
static thread_local sigjmp_buf t_jumpBuf;
/// 标记当前线程是否在保护区内（只在保护区内才跳转恢复）
static thread_local volatile sig_atomic_t t_guardActive = 0;

// ═══════════════════════════════════════════════════════════════════════════════
// Dobby-backed API Resolver — 绕过 Android Linker Namespace 限制
//
// 问题:
//   Android 7+ 的 linker namespace 策略阻止 app 进程调用
//   dlopen("/system/lib64/libgui.so")，导致 ANativeWindowCreator 初始化失败。
//
// 解决方案:
//   1. 优先尝试标准 dlopen（适用于 root 环境或放宽的 namespace）
//   2. 若 dlopen 失败，生成一个"虚拟句柄"并记录库名
//   3. 后续 dlsym 调用改用 DobbySymbolResolver，它通过扫描
//      /proc/self/maps + ELF 解析来查找符号，完全绕过 namespace
// ═══════════════════════════════════════════════════════════════════════════════

namespace {
    /// 所有句柄 → 库名 映射（真实句柄和虚拟句柄都记录）
    std::unordered_map<void*, std::string> g_handleToLibName;
    /// 仅虚拟句柄集合（Close 时不调用 dlclose）
    std::unordered_set<void*> g_fakeHandles;
    uintptr_t g_nextFakeHandle = 0xDB000000;

    /// 从完整路径提取文件名: "/system/lib64/libgui.so" → "libgui.so"
    std::string extractLibName(const char* path) {
        std::string p(path);
        auto pos = p.rfind('/');
        return (pos != std::string::npos) ? p.substr(pos + 1) : p;
    }

    /// 自定义 Open: 无论 dlopen 成功与否都记录库名
    void* customOpen(const char* filename, int flag) {
        std::string libName = extractLibName(filename);

        // 1) 尝试标准 dlopen
        void* handle = dlopen(filename, flag);
        if (handle) {
            LOG(LOG_LEVEL_ERROR, "[DobbyResolver] dlopen(\"%s\") 成功 → %p", filename, handle);
            g_handleToLibName[handle] = libName;
            return handle;
        }

        // 2) dlopen 失败 — 生成虚拟句柄，后续全部走 Dobby
        LOG(LOG_LEVEL_ERROR, "[DobbyResolver] dlopen(\"%s\") 失败(errno=%d), 创建虚拟句柄 (lib=%s)",
            filename, errno, libName.c_str());
        void* fakeHandle = reinterpret_cast<void*>(g_nextFakeHandle++);
        g_handleToLibName[fakeHandle] = libName;
        g_fakeHandles.insert(fakeHandle);
        return fakeHandle;
    }

    /// 内部辅助: 用信号保护调用 DobbySymbolResolver (防止 ELF 解析 SIGSEGV)
    void* safeDobbyResolve(const char* imageName, const char* symbol) {
        // 保存外层跳转缓冲区
        sigjmp_buf savedBuf;
        memcpy(&savedBuf, &t_jumpBuf, sizeof(sigjmp_buf));
        volatile sig_atomic_t savedGuard = t_guardActive;

        int crashSig = sigsetjmp(t_jumpBuf, 1);
        if (crashSig != 0) {
            // DobbySymbolResolver 内部发生 SIGSEGV — 恢复外层缓冲区并返回 null
            LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✘ DobbySymbolResolver(\"%s\", \"%.60s\") 触发信号 %d",
                imageName ? imageName : "null", symbol, crashSig);
            t_guardActive = savedGuard;
            memcpy(&t_jumpBuf, &savedBuf, sizeof(sigjmp_buf));
            return nullptr;
        }

        t_guardActive = 1;
        void* addr = DobbySymbolResolver(imageName, symbol);
        t_guardActive = savedGuard;
        memcpy(&t_jumpBuf, &savedBuf, sizeof(sigjmp_buf));
        return addr;
    }

    /// 自定义 Resolve: dlsym → Dobby(具体库) → Dobby(全路径) 三级回退
    void* customResolve(void* handle, const char* symbol) {
        bool isFake = g_fakeHandles.count(handle) > 0;
        auto it = g_handleToLibName.find(handle);
        const char* libName = (it != g_handleToLibName.end()) ? it->second.c_str() : "???";

        // ── 第1级：真实句柄尝试 dlsym ──
        if (!isFake) {
            void* addr = dlsym(handle, symbol);
            if (addr) {
                return addr;
            }
        }

        // ── 第2级：DobbySymbolResolver（短名称，如 "libgui.so"）——带信号保护 ──
        if (it != g_handleToLibName.end()) {
            void* addr = safeDobbyResolve(it->second.c_str(), symbol);
            if (addr) {
                LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✓ Dobby(\"%s\", \"%.60s\") → %p",
                    libName, symbol, addr);
                return addr;
            }
        }

        // ── 第3级：DobbySymbolResolver（完整系统路径）——带信号保护 ──
        {
            const char* fullPaths[] = {
#ifdef __LP64__
                "/system/lib64/libgui.so",
                "/system/lib64/libutils.so",
#else
                "/system/lib/libgui.so",
                "/system/lib/libutils.so",
#endif
            };
            for (const char* fp : fullPaths) {
                void* addr = safeDobbyResolve(fp, symbol);
                if (addr) {
                    LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✓ Dobby(\"%s\", \"%.60s\") → %p", fp, symbol, addr);
                    return addr;
                }
            }
        }

        LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✘ 三级回退全部失败: lib=\"%s\" sym=\"%.120s\"",
            libName, symbol);
        LOG(LOG_LEVEL_ERROR, "[DobbyResolver]   提示: 请确认 /proc/self/maps 中是否加载了该库");
        return nullptr;
    }

    /// 自定义 Close: 虚拟句柄仅清理映射，真实句柄走 dlclose
    int customClose(void* handle) {
        g_handleToLibName.erase(handle);
        if (g_fakeHandles.count(handle)) {
            g_fakeHandles.erase(handle);
            return 0;
        }
        return dlclose(handle);
    }

    /// 安装自定义 Resolver（只需调用一次）
    void installDobbyApiResolver() {
        // 输出 Android 版本信息（诊断用）
        char verBuf[128] = {};
        __system_property_get("ro.build.version.release", verBuf);
        char sdkBuf[128] = {};
        __system_property_get("ro.build.version.sdk", sdkBuf);
        LOG(LOG_LEVEL_ERROR, "[DobbyResolver] 安装 Dobby API Resolver — Android %s (SDK %s)", verBuf, sdkBuf);

        // ── 诊断: 检查 libgui.so / libutils.so 是否在 /proc/self/maps 中 ──
        {
            FILE* fp = fopen("/proc/self/maps", "r");
            if (fp) {
                char line[512];
                bool foundGui = false, foundUtils = false;
                while (fgets(line, sizeof(line), fp)) {
                    if (!foundGui && strstr(line, "libgui.so")) {
                        foundGui = true;
                        LOG(LOG_LEVEL_INFO, "[DobbyResolver] ✓ maps 中找到 libgui.so : %.100s", line);
                    }
                    if (!foundUtils && strstr(line, "libutils.so")) {
                        foundUtils = true;
                        LOG(LOG_LEVEL_INFO, "[DobbyResolver] ✓ maps 中找到 libutils.so : %.100s", line);
                    }
                    if (foundGui && foundUtils) break;
                }
                fclose(fp);
                if (!foundGui)
                    LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✘ /proc/self/maps 中未找到 libgui.so — DobbySymbolResolver 将失败!");
                if (!foundUtils)
                    LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✘ /proc/self/maps 中未找到 libutils.so — DobbySymbolResolver 将失败!");
            } else {
                LOG(LOG_LEVEL_ERROR, "[DobbyResolver] ✘ 无法打开 /proc/self/maps (errno=%d)", errno);
            }
        }

        android::ANativeWindowCreator::SetupCustomApiResolver({customOpen, customResolve, customClose});
    }
} // anonymous namespace
// 第3层防御: SIGSEGV/SIGBUS 信号安全网
//
// 为什么 catch(...) 无法防御空指针崩溃？
//   - 空指针解引用在 Linux/Android 上产生 SIGSEGV 信号
//   - SIGSEGV 是 POSIX 信号，不是 C++ 异常
//   - catch(...) 只捕获 throw 抛出的 C++ 异常，对信号完全无效
//
// 解决方案: sigaction + siglongjmp
//   - sigsetjmp()  在安全点设置"存档"
//   - 发生 SIGSEGV 时 → 信号处理器被调用 → siglongjmp() 跳回"存档"点
//   - 线程不死，从崩溃中恢复
// ═══════════════════════════════════════════════════════════════════════════════

// (t_jumpBuf 和 t_guardActive 已在文件顶部声明)

/// 保存原始信号处理器，保护区外交还给系统
static struct sigaction s_oldSigsegv;
static struct sigaction s_oldSigbus;

/**
 * @brief 信号处理函数 —— 在保护区内拦截 SIGSEGV/SIGBUS，跳回安全点
 */
static void crashSignalHandler(int sig, siginfo_t* info, void* ctx) {
    if (t_guardActive) {
        // 在保护区内 → 跳回 sigsetjmp 设置的安全点
        t_guardActive = 0;
        siglongjmp(t_jumpBuf, sig);  // 返回值 = sig（非零）
    }

    // 不在保护区内 → 交还给原始处理器
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

/**
 * @brief 安装信号安全网（进程级，只需调用一次，线程安全）
 */
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
// RunMonitoring — 监控主循环（三层防御版）
//
// 第1层: 状态机 —— 每次循环开头检查 get_BattleStarted()
//        游戏结束 → 立即停止调用 updateMiniMapData
//
// 第2层: IsReadableMemory —— 在 updateMiniMapData 内部遍历每个对象前
//        用 pipe write 验证内存可读性（见 lolm.cpp）
//
// 第3层: siglongjmp —— 若前两层都漏过，实际 SIGSEGV 发生时
//        信号处理器跳回安全点，线程存活
// ═══════════════════════════════════════════════════════════════════════════════

/// 对局生命周期状态
enum class MonitorState {
    WaitForBattle,   ///< 等待进入对局
    InBattle,        ///< 对局进行中
    BattleEnded      ///< 对局已结束，执行清理后回到 WaitForBattle
};

void RunMonitoring(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                   void *pGlobalMetadataHeader, void *pMetadataImagesTable) {

    const auto kIdleInterval = std::chrono::milliseconds(2000);    // 等待对局时的轮询间隔
    const auto kDataInterval = std::chrono::milliseconds(4000);    // 数据采集间隔
    const auto kCooldownInterval = std::chrono::milliseconds(3000); // 崩溃恢复后的冷却

    lol::lol lol(pli2cppModeBase, pCodeRegistration,
                 pMetadataRegistration, pGlobalMetadataHeader, pMetadataImagesTable);

    // 安装信号安全网
    installCrashGuard();

    MonitorState state = MonitorState::WaitForBattle;

    while (true) {
        switch (state) {

            // ═══════ 等待对局开始 ═══════
            case MonitorState::WaitForBattle: {
                if (lol.get_BattleStarted()) {
                    LOG(LOG_LEVEL_INFO, "[Monitor] ══════ 检测到对局开始 ══════");
                    state = MonitorState::InBattle;
                    continue;  // 立即进入 InBattle，不等待
                }
                std::this_thread::sleep_for(kIdleInterval);
                break;
            }

                // ═══════ 对局进行中 ═══════
            case MonitorState::InBattle: {
                // ── 第1层: 每次循环都重新检查 ──
                if (!lol.get_BattleStarted()) {
                    LOG(LOG_LEVEL_INFO, "[Monitor] ══════ 对局正常结束 ══════");
                    state = MonitorState::BattleEnded;
                    continue;
                }

                // ── 第3层: sigsetjmp 设置安全跳转点 ──
                // sigsetjmp 返回 0 = 正常执行
                // sigsetjmp 返回 非0 = 从 siglongjmp 恢复（值=信号编号）
                int crashSig = sigsetjmp(t_jumpBuf, 1);
                if (crashSig != 0) {
                    // ★ 从 SIGSEGV/SIGBUS 崩溃中恢复到此处
                    LOG(LOG_LEVEL_ERROR,
                        "[Monitor] ⚠ 数据采集中捕获信号 %d，游戏对象可能已被回收，转入结束状态",
                        crashSig);
                    t_guardActive = 0;
                    state = MonitorState::BattleEnded;
                    std::this_thread::sleep_for(kCooldownInterval);
                    continue;
                }

                // 进入保护区 —— 从这里到 t_guardActive=0 之间的 SIGSEGV 都会被拦截
                t_guardActive = 1;
                {
                    lol.updateMiniMapData();
                    lol.printMiniMapData();
                }
                t_guardActive = 0;  // 离开保护区

                std::this_thread::sleep_for(kDataInterval);
                break;
            }

                // ═══════ 对局结束 ═══════
            case MonitorState::BattleEnded: {
                LOG(LOG_LEVEL_INFO, "[Monitor] 清理完毕，回到等待状态");
                // TODO: 此处可执行结算逻辑、上报数据、清空缓存等
                state = MonitorState::WaitForBattle;
                std::this_thread::sleep_for(kIdleInterval);
                break;
            }
        }
    }
}

void Dumper(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
            void *pGlobalMetadataHeader,void*pMetadataImagesTable){

    LOG(LOG_LEVEL_INFO,"[DUMPER] Start Dumper!");

    // 1. 创建对象并初始化
    // 这一步可能会因为构造函数内部的内存分配失败等原因抛出异常
    li2cpp::li2cppDumper li2CppDumperInfo(
            pli2cppModeBase,
            pCodeRegistration,
            pMetadataRegistration,
            pGlobalMetadataHeader,
            pMetadataImagesTable
    );

    // 2. 初始化信息
    // 这一步是您的主要操作，如果发生错误（如空指针解引用），
    // 并且该错误被设计为抛出 C++ 异常，就会被下面的 catch 捕获
    li2CppDumperInfo.initInfo();
    LOG(LOG_LEVEL_INFO,"[DUMPER] Dumper End!");
}

void DumperHeader(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                  void *pGlobalMetadataHeader,void*pMetadataImagesTable) {
    LOG(LOG_LEVEL_INFO,"[DUMPER] Start DumperHeader!");

    li2cppHeader::li2cppHeader li2cppHeaderInfo(
            pli2cppModeBase,
            pCodeRegistration,
            pMetadataRegistration,
            pGlobalMetadataHeader,
            pMetadataImagesTable
    );

    li2cppHeaderInfo.start();

    LOG(LOG_LEVEL_INFO,"[DUMPER] DumperHeader End!");
}

// ═══════════════════════════════════════════════════════════════════════════════
// 远程绘制 —— ImGui 覆盖层绘制、Server/Client 线程
// ═══════════════════════════════════════════════════════════════════════════════

// 召唤师峡谷地图近似边界（Unity 世界坐标）
static constexpr float MAP_MIN_X = -200.0f;
static constexpr float MAP_MAX_X = 14800.0f;
static constexpr float MAP_MIN_Z = -200.0f;
static constexpr float MAP_MAX_Z = 14800.0f;

/**
 * @brief  将世界坐标映射到小地图 ImGui 绘制坐标
 */
static ImVec2 WorldToMinimap(const lol::UnityVector3& worldPos,
                             ImVec2 minimapOrigin, float minimapSize) {
    float nx = (worldPos.x - MAP_MIN_X) / (MAP_MAX_X - MAP_MIN_X);
    float nz = 1.0f - (worldPos.z - MAP_MIN_Z) / (MAP_MAX_Z - MAP_MIN_Z);
    nx = std::clamp(nx, 0.0f, 1.0f);
    nz = std::clamp(nz, 0.0f, 1.0f);
    return ImVec2(minimapOrigin.x + nx * minimapSize,
                  minimapOrigin.y + nz * minimapSize);
}

/**
 * @brief  使用 ImGui 绘制游戏覆盖层（敌方英雄面板 + 小地图雷达 + 眼位标记）
 */
static void DrawGameOverlay(const lol::MiniMapData& data, bool inBattle) {

    // ═══ 1. 状态/信息面板（左上角） ═══
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.65f);
    ImGui::Begin("## StatusPanel", nullptr,
                 ImGuiWindowFlags_AlwaysAutoResize |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoMove);

    if (!inBattle) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[Waiting] No active battle");
        ImGui::End();
        return;
    }

    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "=== IN BATTLE ===");
    ImGui::Separator();

    // ── 敌方英雄信息 ──
    ImGui::Text("Enemy Heroes: %d", (int)data.enemyHeroes.size());
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

        // HP 血条
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

    // ── 眼位信息 ──
    if (!data.wards.empty()) {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f),
                           "Wards: %d", (int)data.wards.size());
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

    ImGui::End();

    // ═══ 2. ESP 方框覆盖层（World-to-Screen 屏幕投影） ═══
    {
        ImGuiIO& espIo = ImGui::GetIO();
        const float screenW = espIo.DisplaySize.x;
        const float screenH = espIo.DisplaySize.y;

        // 使用全屏透明窗口作为 ESP 画布
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

            // Unity 屏幕空间 (原点左下) → ImGui 屏幕空间 (原点左上)
            float sx = hero.screenX;
            float sy = screenH - hero.screenY;

            // 剔除屏幕外的目标 (留一定余量)
            constexpr float kMargin = 100.0f;
            if (sx < -kMargin || sx > screenW + kMargin ||
                sy < -kMargin || sy > screenH + kMargin) {
                continue;
            }

            // ── 方框大小 (基于距离/血量可动态调整，这里使用固定大小) ──
            const float boxW = 60.0f;   // 方框宽度
            const float boxH = 120.0f;  // 方框高度
            ImVec2 boxMin(sx - boxW * 0.5f, sy - boxH * 0.8f);   // 角色脚底偏上
            ImVec2 boxMax(sx + boxW * 0.5f, sy + boxH * 0.2f);   // 脚底偏下

            // ── HP 颜色 ──
            float hpRatio = (hero.maxHp > 0.0f) ? (hero.curHp / hero.maxHp) : 0.0f;
            hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
            ImU32 boxColor = (hpRatio > 0.5f)  ? IM_COL32(0, 255, 0, 220) :
                             (hpRatio > 0.25f) ? IM_COL32(255, 255, 0, 220) :
                                                  IM_COL32(255, 0, 0, 220);

            // ── 方框 (外边框 + 半透明填充) ──
            espDraw->AddRectFilled(boxMin, boxMax, IM_COL32(0, 0, 0, 40));
            espDraw->AddRect(boxMin, boxMax, boxColor, 0.0f, 0, 2.0f);

            // ── HP 血条 (方框顶部上方) ──
            const float hpBarH = 4.0f;
            const float hpBarY = boxMin.y - hpBarH - 2.0f;
            ImVec2 hpBgMin(boxMin.x, hpBarY);
            ImVec2 hpBgMax(boxMax.x, hpBarY + hpBarH);
            ImVec2 hpFgMax(boxMin.x + (boxMax.x - boxMin.x) * hpRatio, hpBarY + hpBarH);
            espDraw->AddRectFilled(hpBgMin, hpBgMax, IM_COL32(0, 0, 0, 160));
            espDraw->AddRectFilled(hpBgMin, hpFgMax, boxColor);

            // ── 英雄名称 (血条上方) ──
            const char* name = hero.heroName.empty() ? nullptr : hero.heroName.c_str();
            if (name) {
                ImVec2 textSize = ImGui::CalcTextSize(name);
                float textX = sx - textSize.x * 0.5f;
                float textY = hpBarY - textSize.y - 1.0f;
                // 文字阴影
                espDraw->AddText(ImVec2(textX + 1, textY + 1), IM_COL32(0, 0, 0, 200), name);
                espDraw->AddText(ImVec2(textX, textY), IM_COL32(255, 255, 255, 240), name);
            }

            // ── 等级标签 (方框右上角) ──
            char lvlBuf[16];
            snprintf(lvlBuf, sizeof(lvlBuf), "Lv%u", hero.heroLevel);
            espDraw->AddText(ImVec2(boxMax.x + 2, boxMin.y),
                             IM_COL32(255, 255, 100, 240), lvlBuf);

            // ── HP 数值 (方框下方) ──
            char hpBuf[32];
            snprintf(hpBuf, sizeof(hpBuf), "%.0f/%.0f", hero.curHp, hero.maxHp);
            ImVec2 hpTextSize = ImGui::CalcTextSize(hpBuf);
            espDraw->AddText(ImVec2(sx - hpTextSize.x * 0.5f, boxMax.y + 2),
                             IM_COL32(200, 200, 200, 220), hpBuf);
        }

        ImGui::End();
    }

    // ═══ 3. 小地图雷达（右下角） ═══
    const float radarSize = 300.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 radarPos(io.DisplaySize.x - radarSize - 20,
                    io.DisplaySize.y - radarSize - 20);

    ImGui::SetNextWindowPos(radarPos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(radarSize + 16, radarSize + 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("## Radar", nullptr,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("Radar");
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // 雷达背景
    drawList->AddRectFilled(canvasPos,
                            ImVec2(canvasPos.x + radarSize, canvasPos.y + radarSize),
                            IM_COL32(20, 30, 20, 180));
    drawList->AddRect(canvasPos,
                      ImVec2(canvasPos.x + radarSize, canvasPos.y + radarSize),
                      IM_COL32(80, 80, 80, 200));

    // 敌方英雄标记（红色方框 + 圆点 + 等级标签 + 英雄名）
    for (const auto& hero : data.enemyHeroes) {
        if (!hero.hasWorldPos) continue;
        ImVec2 pos = WorldToMinimap(hero.worldPos, canvasPos, radarSize);

        // ── 方框 (外框 + 内填充) ──
        const float boxHalf = 12.0f;  // 方框半边长
        ImVec2 boxMin(pos.x - boxHalf, pos.y - boxHalf);
        ImVec2 boxMax(pos.x + boxHalf, pos.y + boxHalf);
        // 半透明红色填充
        drawList->AddRectFilled(boxMin, boxMax, IM_COL32(255, 40, 40, 80));
        // 亮红色边框 (2px 粗)
        drawList->AddRect(boxMin, boxMax, IM_COL32(255, 60, 60, 240), 0.0f, 0, 2.0f);

        // ── 中心圆点 ──
        drawList->AddCircleFilled(pos, 4.0f, IM_COL32(255, 255, 255, 230));

        // ── 等级标签 (方框右上角) ──
        char lvlBuf[8];
        snprintf(lvlBuf, sizeof(lvlBuf), "%u", hero.heroLevel);
        drawList->AddText(ImVec2(boxMax.x + 2, boxMin.y - 2),
                          IM_COL32(255, 255, 100, 240), lvlBuf);

        // ── 英雄名 (方框下方) ──
        const char* name = hero.heroName.empty() ? nullptr : hero.heroName.c_str();
        if (name) {
            ImVec2 textSize = ImGui::CalcTextSize(name);
            drawList->AddText(ImVec2(pos.x - textSize.x * 0.5f, boxMax.y + 2),
                              IM_COL32(255, 200, 200, 220), name);
        }
    }

    // 眼位标记（蓝色方框 + 菱形）
    for (const auto& ward : data.wards) {
        if (!ward.hasWorldPos) continue;
        ImVec2 pos = WorldToMinimap(ward.worldPos, canvasPos, radarSize);

        // ── 方框 ──
        const float wboxHalf = 8.0f;
        ImVec2 wboxMin(pos.x - wboxHalf, pos.y - wboxHalf);
        ImVec2 wboxMax(pos.x + wboxHalf, pos.y + wboxHalf);
        drawList->AddRectFilled(wboxMin, wboxMax, IM_COL32(50, 150, 255, 60));
        drawList->AddRect(wboxMin, wboxMax, IM_COL32(80, 180, 255, 220), 0.0f, 0, 1.5f);

        // ── 中心菱形 ──
        const float s = 4.0f;
        drawList->AddQuadFilled(
                ImVec2(pos.x, pos.y - s), ImVec2(pos.x + s, pos.y),
                ImVec2(pos.x, pos.y + s), ImVec2(pos.x - s, pos.y),
                IM_COL32(100, 200, 255, 240));
    }

    ImGui::Dummy(ImVec2(radarSize, radarSize));
    ImGui::End();
}

// ═══════════════════════════════════════════════════════════════════════════════
// GuiServerThread — 独立应用 RenderServer 模式
//
// 在 ANativeWindow 透明覆盖层上运行，通过 TCP 接收 ImGui DrawData 并渲染。
// ═══════════════════════════════════════════════════════════════════════════════

static void GuiServerThread() {
    LOG(LOG_LEVEL_INFO, "[GuiServer] ══════ 启动 RenderServer ══════");

    // ── 步骤 1: 安装 CrashGuard (信号安全网) ──
    installCrashGuard();

    // ── 步骤 2: 预检 — 测试 dlopen 能否打开系统库 ──
    bool apiResolved = false;
    {
#ifdef __LP64__
        const char* libguiPath  = "/system/lib64/libgui.so";
        const char* libutilsPath = "/system/lib64/libutils.so";
#else
        const char* libguiPath  = "/system/lib/libgui.so";
        const char* libutilsPath = "/system/lib/libutils.so";
#endif
        void* hGui   = dlopen(libguiPath, RTLD_LAZY);
        const char* errGui = hGui ? nullptr : dlerror();
        void* hUtils = dlopen(libutilsPath, RTLD_LAZY);
        const char* errUtils = hUtils ? nullptr : dlerror();

        LOG(LOG_LEVEL_INFO, "[GuiServer] dlopen(\"%s\") → %p %s",
            libguiPath, hGui, hGui ? "✓" : (errGui ? errGui : "unknown error"));
        LOG(LOG_LEVEL_INFO, "[GuiServer] dlopen(\"%s\") → %p %s",
            libutilsPath, hUtils, hUtils ? "✓" : (errUtils ? errUtils : "unknown error"));

        if (hGui && hUtils) {
            // dlopen 可用 — AImGui 内部的默认解析器 (dlopen/dlsym) 可以正常工作
            LOG(LOG_LEVEL_INFO, "[GuiServer] ✓ 系统库可直接 dlopen — 使用默认解析器");
            dlclose(hGui);
            dlclose(hUtils);
            apiResolved = true;
        } else {
            if (hGui)   dlclose(hGui);
            if (hUtils) dlclose(hUtils);

            // dlopen 被 linker namespace 拦截 — 回退到 Dobby Resolver
            LOG(LOG_LEVEL_INFO, "[GuiServer] dlopen 失败, 尝试 Dobby Resolver (带信号保护)...");

            int crashSig = sigsetjmp(t_jumpBuf, 1);
            if (crashSig != 0) {
                LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ Dobby Resolver 触发信号 %d (SIGSEGV/SIGBUS)", crashSig);
                LOG(LOG_LEVEL_ERROR, "[GuiServer] 独立应用无法解析 libgui.so 符号, 无法创建覆盖层");
                t_guardActive = 0;
                return;
            }
            t_guardActive = 1;
            try {
                installDobbyApiResolver();
                LOG(LOG_LEVEL_INFO, "[GuiServer] ✓ Dobby Resolver 安装成功");
                apiResolved = true;
            } catch (const std::exception& e) {
                LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ Dobby Resolver 异常: %s", e.what());
            } catch (...) {
                LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ Dobby Resolver 未知异常");
            }
            t_guardActive = 0;
        }
    }

    if (!apiResolved) {
        LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ 无法解析系统库符号 (dlopen 和 Dobby 均失败)");
        LOG(LOG_LEVEL_ERROR, "[GuiServer] 解决方案:");
        LOG(LOG_LEVEL_ERROR, "[GuiServer]   1. 确保设备已 root (Magisk/KernelSU)");
        LOG(LOG_LEVEL_ERROR, "[GuiServer]   2. 检查 SELinux 是否为 permissive: getenforce");
        LOG(LOG_LEVEL_ERROR, "[GuiServer]   3. 确认 /proc/self/maps 中包含 libgui.so");
        return;
    }

    // ── 步骤 3: 获取屏幕信息 (诊断) ──
    try {
        auto dispInfo = android::ANativeWindowCreator::GetDisplayInfo();
        LOG(LOG_LEVEL_INFO, "[GuiServer] 屏幕信息: %dx%d, 旋转=%d°",
            dispInfo.width, dispInfo.height, dispInfo.theta);
        if (dispInfo.width == 0 || dispInfo.height == 0) {
            LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ 屏幕尺寸为 0 — GetDisplayInfo 失败");
            LOG(LOG_LEVEL_ERROR, "[GuiServer] 可能原因: SurfaceComposerClient 无法连接 SurfaceFlinger");
            return;
        }
    } catch (const std::exception& e) {
        LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ GetDisplayInfo 异常: %s", e.what());
        return;
    }

    // ── 步骤 4: 初始化 AImGui (RenderServer) ──
    try {
        android::AImGui::Options opts;
        opts.renderType           = android::AImGui::RenderType::RenderServer;
        opts.compressionFrameData = true;
        opts.serverListenAddress  = "0.0.0.0";  // 监听所有网卡

        LOG(LOG_LEVEL_INFO, "[GuiServer] 正在初始化 AImGui (RenderServer), 端口 16888...");
        android::AImGui imgui(opts);

        if (!imgui) {
            LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ AImGui 初始化失败!");
            LOG(LOG_LEVEL_ERROR, "[GuiServer] 请检查 logcat tag=\"AImGui\" 获取详细错误:");
            LOG(LOG_LEVEL_ERROR, "[GuiServer]   adb logcat -s AImGui:*");
            LOG(LOG_LEVEL_ERROR, "[GuiServer] 常见原因:");
            LOG(LOG_LEVEL_ERROR, "[GuiServer]   - ANativeWindow 创建失败 (SurfaceFlinger 拒绝)");
            LOG(LOG_LEVEL_ERROR, "[GuiServer]   - EGL 初始化失败");
            LOG(LOG_LEVEL_ERROR, "[GuiServer]   - TCP 端口 16888 被占用");
            return;
        }

        LOG(LOG_LEVEL_INFO, "[GuiServer] ✓ 初始化成功，等待 Client 连接...");

        // 触摸事件处理线程
        std::thread inputThread([&]() {
            while (imgui) {
                imgui.ProcessInputEvent();
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        });

        // 主渲染循环 —— Server 端 EndFrame 负责接收并渲染 DrawData
        while (imgui) {
            imgui.BeginFrame();
            imgui.EndFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        LOG(LOG_LEVEL_INFO, "[GuiServer] 服务器退出");
        if (inputThread.joinable())
            inputThread.join();
    } catch (const std::exception& e) {
        LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ AImGui 运行异常: %s", e.what());
    } catch (...) {
        LOG(LOG_LEVEL_ERROR, "[GuiServer] ✘ AImGui 运行时发生未知异常");
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// eglSwapBuffers Hook — 直接绘制方案
//
// 通过 hook 游戏进程的 eglSwapBuffers 函数，在每帧画面提交前
// 直接在游戏的 GL 上下文中绘制 ImGui 覆盖层。
//
// 优点:
//   - 不需要创建独立的 ANativeWindow 覆盖层
//   - 不需要解析 libgui.so / libutils.so 的私有 C++ 符号
//   - 不需要 DobbySymbolResolver (不会触发 ELF 解析 SIGSEGV)
//   - eglSwapBuffers 是 EGL 公开 API, dlsym 即可获取地址
//   - 渲染直接发生在游戏渲染线程, 无需额外渲染线程
//   - ImGui OpenGL3 后端会自动保存/恢复全部 GL 状态, 不影响游戏渲染
// ═══════════════════════════════════════════════════════════════════════════════

namespace egl_hook {
    /// 原始 eglSwapBuffers 函数指针 (由 DobbyHook 填充)
    static EGLBoolean (*g_origSwapBuffers)(EGLDisplay, EGLSurface) = nullptr;

    /// ImGui 上下文与状态
    static std::atomic<bool> g_ready{false};
    static ImGuiContext*      g_ctx       = nullptr;
    static int                g_width     = 0;
    static int                g_height    = 0;
    static double             g_lastTime  = 0.0;

    /**
     * @brief  在游戏 GL 上下文中初始化 ImGui (仅首次调用)
     */
    static bool initImGui(EGLDisplay display, EGLSurface surface) {
        EGLint w = 0, h = 0;
        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);
        if (w <= 0 || h <= 0) {
            LOG(LOG_LEVEL_ERROR, "[EglHook] surface 尺寸无效: %dx%d, 跳过初始化", w, h);
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

        auto& io         = ImGui::GetIO();
        io.IniFilename   = nullptr;
        io.DisplaySize   = ImVec2((float)w, (float)h);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(3.f);

        ImFontConfig fontCfg;
        fontCfg.SizePixels = 22.f;
        io.Fonts->AddFontDefault(&fontCfg);

        if (!ImGui_ImplOpenGL3_Init("#version 300 es")) {
            LOG(LOG_LEVEL_ERROR, "[EglHook] ImGui_ImplOpenGL3_Init 失败");
            ImGui::DestroyContext(g_ctx);
            g_ctx = nullptr;
            return false;
        }

        LOG(LOG_LEVEL_INFO, "[EglHook] ✓ ImGui 初始化完成 (surface %dx%d)", w, h);
        return true;
    }

    /**
     * @brief  hooked eglSwapBuffers — 在游戏每帧提交前绘制 ImGui 覆盖层
     *
     *  调用流程:  游戏渲染 → (我们的 ImGui 绘制) → 原始 eglSwapBuffers
     *  ImGui OpenGL3 后端在 RenderDrawData 中会保存并恢复所有 GL 状态,
     *  所以不会影响游戏的渲染状态机。
     */
    static EGLBoolean hooked_eglSwapBuffers(EGLDisplay display, EGLSurface surface) {

        // ── 首次调用: 初始化 ImGui ──
        if (!g_ready.load(std::memory_order_relaxed)) {
            if (initImGui(display, surface)) {
                g_ready.store(true, std::memory_order_release);
            }
        }

        // ── 绘制 ImGui 覆盖层 ──
        if (g_ready.load(std::memory_order_acquire) && g_ctx) {
            ImGui::SetCurrentContext(g_ctx);

            // 检测 surface 尺寸变化 (屏幕旋转等)
            EGLint curW = 0, curH = 0;
            eglQuerySurface(display, surface, EGL_WIDTH,  &curW);
            eglQuerySurface(display, surface, EGL_HEIGHT, &curH);
            if (curW > 0 && curH > 0 && (curW != g_width || curH != g_height)) {
                g_width  = curW;
                g_height = curH;
                LOG(LOG_LEVEL_INFO, "[EglHook] surface 尺寸变化 → %dx%d", curW, curH);
            }

            auto& io     = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)g_width, (float)g_height);

            // 计算 DeltaTime
            timespec ts{};
            clock_gettime(CLOCK_MONOTONIC, &ts);
            double now  = (double)ts.tv_sec + ts.tv_nsec / 1e9;
            io.DeltaTime = g_lastTime > 0.0 ? (float)(now - g_lastTime) : (1.0f / 60.0f);
            g_lastTime   = now;

            // ── ImGui 帧 ──
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            // 使用 static 保留上一帧的数据 (pullData 仅在有新数据时更新)
            static lol::MiniMapData gameData;
            SharedGameData::getInstance().pullData(gameData);
            bool inBattle = SharedGameData::getInstance().isBattleActive();
            DrawGameOverlay(gameData, inBattle);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        // ── 调用原始 eglSwapBuffers ──
        return g_origSwapBuffers(display, surface);
    }
} // namespace egl_hook

// ═══════════════════════════════════════════════════════════════════════════════
// GuiNativeThread — 注入 SO 内的直接绘制模式
//
// 通过 hook eglSwapBuffers 在游戏渲染线程中直接绘制 ImGui 覆盖层。
// 不再需要:
//   - ANativeWindowCreator (不需要创建独立覆盖窗口)
//   - DobbySymbolResolver  (不需要解析 libgui.so 私有符号)
//   - 独立渲染线程        (渲染在游戏线程中自动进行)
// ═══════════════════════════════════════════════════════════════════════════════

static void GuiNativeThread() {
    LOG(LOG_LEVEL_INFO, "[GuiNative] ══════ 启动直接绘制模式 (eglSwapBuffers Hook) ══════");

    // ── 步骤 0: 安装 CrashGuard ──
    installCrashGuard();
    LOG(LOG_LEVEL_INFO, "[GuiNative] [步骤0] CrashGuard 就绪");

    // ── 步骤 1: 查找 eglSwapBuffers 地址 (公开 API, dlsym 直接获取) ──
    void* swapAddr = dlsym(RTLD_DEFAULT, "eglSwapBuffers");
    if (!swapAddr) {
        // 尝试从 libEGL.so 获取
        void* eglLib = dlopen("libEGL.so", RTLD_LAZY);
        if (eglLib) {
            swapAddr = dlsym(eglLib, "eglSwapBuffers");
            dlclose(eglLib);
        }
    }
    if (!swapAddr) {
        LOG(LOG_LEVEL_ERROR, "[GuiNative] ✘ 无法找到 eglSwapBuffers — dlsym 失败");
        LOG(LOG_LEVEL_ERROR, "[GuiNative]   如果游戏使用 Vulkan 渲染, 需要改用 vkQueuePresentKHR hook");
        return;
    }
    LOG(LOG_LEVEL_INFO, "[GuiNative] [步骤1] ✓ eglSwapBuffers → %p", swapAddr);

    // ── 步骤 2: 安装 DobbyHook (inline hook, 不涉及 ELF 符号解析) ──
    {
        int crashSig = sigsetjmp(t_jumpBuf, 1);
        if (crashSig != 0) {
            LOG(LOG_LEVEL_ERROR, "[GuiNative] ✘ DobbyHook 安装触发信号 %d", crashSig);
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

    LOG(LOG_LEVEL_INFO, "[GuiNative] [步骤2] ✓ eglSwapBuffers hook 已安装");
    LOG(LOG_LEVEL_INFO, "[GuiNative] 覆盖层将在游戏首次渲染时自动初始化, 无需额外线程");
    // hook 安装完成后此线程即可退出, 绘制在游戏渲染线程中自动进行
}

// ══════════════════════════════════════════════════════════════════════════════
// GuiClientThread — 注入 SO 内的 RenderClient 模式 (远程绘制, 已弃用)
//
// 从 SharedGameData 读取游戏数据 → 构建 ImGui 帧 → 序列化 DrawData → TCP 发送
// ═══════════════════════════════════════════════════════════════════════════════

static void GuiClientThread() {
    LOG(LOG_LEVEL_INFO, "[GuiClient] ══════ 启动 RenderClient ══════");

    // 1. 安装 Dobby API Resolver（GetDisplayInfo 需要）
    try {
        installDobbyApiResolver();
        LOG(LOG_LEVEL_INFO, "[GuiClient] Dobby API Resolver 安装成功 ✓");
    } catch (const std::exception& e) {
        LOG(LOG_LEVEL_ERROR, "[GuiClient] ✘ installDobbyApiResolver 抛出异常: %s", e.what());
        return;
    } catch (...) {
        LOG(LOG_LEVEL_ERROR, "[GuiClient] ✘ installDobbyApiResolver 抛出未知异常");
        return;
    }

    // 2. 等待 Server 端就绪
    LOG(LOG_LEVEL_INFO, "[GuiClient] 等待 Server 就绪 (5s)...");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    try {
        // 3. 初始化 AImGui (RenderClient)
        android::AImGui::Options opts;
        opts.renderType           = android::AImGui::RenderType::RenderClient;
        opts.compressionFrameData = true;
        opts.clientConnectAddress = "127.0.0.1";  // 默认连接本机 Server

        LOG(LOG_LEVEL_INFO, "[GuiClient] 正在连接 Server %s:16888...",
            opts.clientConnectAddress.c_str());
        android::AImGui imgui(opts);

        if (!imgui) {
            LOG(LOG_LEVEL_ERROR, "[GuiClient] 连接 Server 失败!");
            return;
        }

        LOG(LOG_LEVEL_INFO, "[GuiClient] 连接成功，开始渲染循环");

        // 4. 渲染主循环
        lol::MiniMapData gameData;

        while (imgui) {
            imgui.BeginFrame();

            // 从 SharedGameData 拉取最新数据
            SharedGameData::getInstance().pullData(gameData);
            bool inBattle = SharedGameData::getInstance().isBattleActive();

            // 绘制覆盖层
            DrawGameOverlay(gameData, inBattle);

            imgui.EndFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
        }

        LOG(LOG_LEVEL_INFO, "[GuiClient] 客户端退出");
    } catch (const std::exception& e) {
        LOG(LOG_LEVEL_ERROR, "[GuiClient] ✘ AImGui 运行异常: %s", e.what());
    } catch (...) {
        LOG(LOG_LEVEL_ERROR, "[GuiClient] ✘ AImGui 运行时发生未知异常");
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// TestFunction — 数据采集主入口
// ═══════════════════════════════════════════════════════════════════════════════

void TestFunction(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                   void *pGlobalMetadataHeader, void *pMetadataImagesTable) {

    LOG(LOG_LEVEL_INFO, "[TestFunction] ══════ TestFunction 启动 ══════");

    // ── 0. 安装信号安全网 ──
    LOG(LOG_LEVEL_INFO, "[TestFunction] [步骤0] 安装 CrashGuard...");
    installCrashGuard();
    LOG(LOG_LEVEL_INFO, "[TestFunction] [步骤0] CrashGuard 已安装 ✓");

    // ── 1. 初始化游戏数据采集 ──
    LOG(LOG_LEVEL_INFO, "[TestFunction] 正在初始化游戏数据采集...");
    lol::lol lol(pli2cppModeBase, pCodeRegistration,
                 pMetadataRegistration, pGlobalMetadataHeader, pMetadataImagesTable);
    LOG(LOG_LEVEL_INFO, "[TestFunction] 游戏数据采集初始化完成 ✓");

    // ── 实时坐标刷新: 位置/HP 每 50ms 更新一次, 日志每 5s 打印一次 ──
    int collectIntervalMs = 50;        // 坐标采集间隔 (ms) — 实时刷新
    int printIntervalMs   = 5000;      // 日志打印间隔 (ms) — 降低 I/O 压力
    auto lastCollectTime = std::chrono::steady_clock::now();
    auto lastPrintTime   = lastCollectTime;

    // ── 2. 数据采集主循环 ──
    while (true) {

        auto now = std::chrono::steady_clock::now();
        if (now - lastCollectTime >= std::chrono::milliseconds(collectIntervalMs)) {

            // 第3层: sigsetjmp 安全跳转点
            int crashSig = sigsetjmp(t_jumpBuf, 1);
            if (crashSig != 0) {
                LOG(LOG_LEVEL_ERROR,
                    "[TestFunction] ⚠ 数据采集捕获信号 %d，跳过本次采集", crashSig);
                t_guardActive = 0;
                SharedGameData::getInstance().setBattleActive(false);
            } else {
                t_guardActive = 1;  // 进入保护区
                bool isBattle = lol.get_BattleStarted();
                SharedGameData::getInstance().setBattleActive(isBattle);
                if (isBattle) {
                    lol.updateMiniMapData();
                    // ★ 推送数据到 SharedGameData，供渲染线程消费
                    SharedGameData::getInstance().pushData(lol.getMiniMapData());

                    // 日志仅按低频打印, 避免高频采集时 I/O 刷屏
                    if (now - lastPrintTime >= std::chrono::milliseconds(printIntervalMs)) {
                        lol.printMiniMapData();
                        lastPrintTime = now;
                    }
                }
                t_guardActive = 0;  // 离开保护区
            }

            lastCollectTime = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));   // ~60Hz 采集节奏
    }

    LOG(LOG_LEVEL_INFO, "[TestFunction] 已退出");
}




bool MyStartPoint(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                  void *pGlobalMetadataHeader,void*pMetadataImagesTable){
    try {

        if (pli2cppModeBase == nullptr) {
            return false;
        }
        else
        {
            LOG(LOG_LEVEL_INFO, "[MyStartPoint] 注入模式 — 启动数据采集 + 直接绘制 (eglSwapBuffers Hook)");

            // 0. 先安装 CrashGuard (进程级, 所有线程共享信号处理器)
            installCrashGuard();

            // 1. 安装 eglSwapBuffers hook (绘制在游戏渲染线程中自动进行)
            LOG(LOG_LEVEL_INFO, "[MyStartPoint] 启动 GuiNativeThread (安装 eglSwapBuffers hook)...");
            std::thread renderThread(GuiNativeThread);
            renderThread.detach();

            // 2. 延迟 2 秒后启动数据采集线程
            LOG(LOG_LEVEL_INFO, "[MyStartPoint] 2 秒后启动 TestFunction...");
            std::thread dataThread([=]() {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                TestFunction(pli2cppModeBase, pCodeRegistration,
                             pMetadataRegistration, pGlobalMetadataHeader, pMetadataImagesTable);
            });
            dataThread.detach();
        }

        //Dumper(pli2cppModeBase,pCodeRegistration,pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

        // 成功执行到这里
        return true;
    }
        // 【开始 catch 块】
        // 捕获所有继承自 std::exception 的标准异常
    catch (const std::exception& e) {
        // 打印异常信息
        std::cerr << "【错误】li2CppDumping 过程中发生标准异常: " << e.what() << std::endl;
        return false; // 操作失败
    }
        // 捕获其他任何未被标准异常类捕获的异常（例如抛出的基本类型或其他自定义类型）
    catch (...) {
        // 打印一个通用的错误信息
        std::cerr << "【错误】li2CppDumping 过程中发生未知异常。" << std::endl;
        return false; // 操作失败
    }
}