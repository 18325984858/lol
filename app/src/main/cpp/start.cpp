//
// Created by Song on 2025/11/10.
//

#include "start.h"
#include "./li2cppDumper/li2cppdumper.h"
#include "./File/file.h"
#include "./Log/log.h"
#include "./interface/interface.h"

#define _SOURCE_SO_PATH_ "libdobbyproject.so"
#define _Det_PACK_PATH_ "com.DefaultCompany.Demo1"

bool MyStartPoint(void *pli2cppModeBase, void *pCodeRegistration, void *pMetadataRegistration,
                  void *pGlobalMetadataHeader,void*pMetadataImagesTable){
    try {

        if(pli2cppModeBase == nullptr){
            return false;
        }

        fun::function functionInfo(pli2cppModeBase,
                                   pCodeRegistration,
                                   pMetadataRegistration,
                                   pGlobalMetadataHeader);

        functionInfo.fillingClassInfo();


        int* curRegisterID = (int*)functionInfo.GetStaticMember("ilbil2cpp.so",
                                     "Assembly-CSharp.dll",
                                     "<>c__DisplayClass6_0",
                                     "JoystickSkillHandler.<>c__DisplayClass6_0",
                                     "curRegisterID");
        LOG(LOG_LEVEL_INFO,"curRegisterID : %d", curRegisterID);

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