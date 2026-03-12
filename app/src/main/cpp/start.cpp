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

#define _SOURCE_SO_PATH_ "libdobbyproject.so"
#define _Det_PACK_PATH_ "com.DefaultCompany.Demo1"

// ═══════════════════════════════════════════════════════════════════════════════
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

/// 每线程独立的跳转缓冲区（thread_local 确保多线程安全）
static thread_local sigjmp_buf t_jumpBuf;
/// 标记当前线程是否在保护区内（只在保护区内才跳转恢复）
static thread_local volatile sig_atomic_t t_guardActive = 0;

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
 * @brief 安装信号安全网（进程级，只需调用一次）
 */
static void installCrashGuard() {
    struct sigaction sa{};
    sa.sa_sigaction = crashSignalHandler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGSEGV, &sa, &s_oldSigsegv);
    sigaction(SIGBUS,  &sa, &s_oldSigbus);
    LOG(LOG_LEVEL_INFO, "[CrashGuard] 信号安全网已安装 (SIGSEGV + SIGBUS)");
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

/// 假设这是你的监控逻辑函数
// 我们将对象指针传入，以便线程能访问其成员
void TestFunction(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                   void *pGlobalMetadataHeader,void*pMetadataImagesTable) {
    // 设置 2 秒的检查间隔
    const std::chrono::milliseconds interval(4000);

    lol::lol lol(pli2cppModeBase,pCodeRegistration,
                 pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

    void * p = lol.updateMiniMapData();
    while (true) {

            p =lol.updateMiniMapData();


        // 线程休眠 4 秒后再进行下一次循环
        // 这种方式比手动计算 elapsedTime 更简洁且 CPU 占用极低
        std::this_thread::sleep_for(interval);
    }
}




bool MyStartPoint(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                  void *pGlobalMetadataHeader,void*pMetadataImagesTable){
    try {

        if (pli2cppModeBase == nullptr) {
            return false;
        }

        {
        LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]MyStartPoint");

            // 3. 创建并启动线程
            // 将 pDumper 作为参数传递给线程
            std::thread monitorThreadTest(TestFunction,pli2cppModeBase,pCodeRegistration,
                                      pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

            // 4. 分离线程
            monitorThreadTest.detach();

            // 3. 创建并启动线程
            // 将 pDumper 作为参数传递给线程
            //std::thread monitorThread(RunMonitoring,pli2cppModeBase,pCodeRegistration,
            //                          pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

            // 4. 分离线程
            //monitorThread.detach();


            // 3. 创建并启动线程
            // 将 pDumper 作为参数传递给线程
            //std::thread monitorThread0(Dumper,pli2cppModeBase,pCodeRegistration,
            //                          pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

            // 4. 分离线程
            //monitorThread0.detach();

            // 3. 创建并启动线程
            // 将 pDumper 作为参数传递给线程
            //std::thread monitorThread1(DumperHeader,pli2cppModeBase,pCodeRegistration,
            //                          pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

            // 4. 分离线程
            //monitorThread1.detach();

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