//
// Created by Song on 2025/11/10.
//

#include "start.h"
#include "./li2cppDumper/li2cppdumper.h"
#include "./File/file.h"
#include "./Log/log.h"
#include "./interface/interface.h"
#include <chrono>
#include <thread>

#define _SOURCE_SO_PATH_ "libdobbyproject.so"
#define _Det_PACK_PATH_ "com.DefaultCompany.Demo1"

bool MyStartPoint(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                  void *pGlobalMetadataHeader,void*pMetadataImagesTable){
    try {

        if (pli2cppModeBase == nullptr) {
            return false;
        }


        {
        LOG(LOG_LEVEL_INFO, "[Test MyStartPoint]MyStartPoint");
        fun::function functionInfo(pli2cppModeBase,
                                   pCodeRegistration,
                                   pMetadataRegistration,
                                   pGlobalMetadataHeader);
        functionInfo.fillingClassInfo();

            // 获取当前时间点
            auto lastUpdateTime = std::chrono::steady_clock::now();
            const std::chrono::milliseconds interval(2000); // 设定间隔为 2000 毫秒 (2秒)

            while (true) {
                auto currentTime = std::chrono::steady_clock::now();

                // 计算当前时间与上次执行时间的偏差
                auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime);

                if (elapsedTime >= interval) {
                    // 更新最后一次执行的时间点
                    lastUpdateTime = currentTime;

                    int *state = (int *) functionInfo.GetStaticMember(
                            "ilbil2cpp.so",
                            "Assembly-CSharp.dll",
                            "BattleReadyState",
                            "BattleReadyState",
                            "state"
                    );

                    if (state != nullptr) {
                        // 解引用获取实际值，注意内存安全
                        LOG(LOG_LEVEL_INFO, "[Test Game] state Value: %p", state);
                    } else {
                        LOG(LOG_LEVEL_ERROR, "[Test Game] Failed to get state pointer");
                    }


                    // --- 执行你的逻辑 ---
                    // 注意：isGameInitialized 在你提供的 dump 中是成员变量 (0x10)，
                    // 如果它是静态的，请确保 GetStaticMember 能够正确获取地址。
                    int *nHeroNum = (int *) functionInfo.GetStaticMember(
                            "ilbil2cpp.so",
                            "Assembly-CSharp.dll",
                            "ExtMSHeroSc",
                            "star_def.ExtMSHeroSc",
                            "nHeroNum"
                    );

                    if (nHeroNum != nullptr) {
                        // 解引用获取实际值，注意内存安全
                        LOG(LOG_LEVEL_INFO, "[Test Game] nHeroNum Value: %p", nHeroNum);
                    } else {
                        LOG(LOG_LEVEL_ERROR, "[Test Game] Failed to get nHeroNum pointer");
                    }
                }
                // 短暂休眠以降低 CPU 占用，防止死循环跑满单核
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

        }
   /*
        //li2cpp::li2cppDumper li2CppDumperInfo(0,0,0,0,0);
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
*/
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