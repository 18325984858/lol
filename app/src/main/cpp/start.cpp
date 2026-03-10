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

#define _SOURCE_SO_PATH_ "libdobbyproject.so"
#define _Det_PACK_PATH_ "com.DefaultCompany.Demo1"

/// 假设这是你的监控逻辑函数
// 我们将对象指针传入，以便线程能访问其成员
void RunMonitoring(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                   void *pGlobalMetadataHeader,void*pMetadataImagesTable) {
    // 设置 2 秒的检查间隔
    const std::chrono::milliseconds interval(4000);

    lol::lol lol(pli2cppModeBase,pCodeRegistration,
                 pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);
    int nFlags = lol.get_BattleStarted();
    while (true) {
        // --- 业务逻辑执行区 ---
        if(nFlags){
        LOG(LOG_LEVEL_INFO,"[TEST GAME] 成功进入对局 %p",lol.get_battleTeamMgr());

            lol =lol.test1();


        }else{
            nFlags = lol.get_BattleStarted();
        }
        // -----------------------

        // 线程休眠 4 秒后再进行下一次循环
        // 这种方式比手动计算 elapsedTime 更简洁且 CPU 占用极低
        std::this_thread::sleep_for(interval);
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

    void * p = lol.test();
    while (true) {

            p =lol.test();


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
            std::thread monitorThread(RunMonitoring,pli2cppModeBase,pCodeRegistration,
                                      pMetadataRegistration,pGlobalMetadataHeader,pMetadataImagesTable);

            // 4. 分离线程
            monitorThread.detach();


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