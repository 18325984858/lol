//
// Created by Song on 2026/2/3.
//

#ifndef DOBBY_PROJECT_IL2CPPHEADER_H
#define DOBBY_PROJECT_IL2CPPHEADER_H
#include "../Symbol/Symbol.h"
#include "../UnityApi/unityapi.h"
#include <string>
#include <vector>
#include <cstdint>
#include <memory> // 必须包含此头文件
#include <map>


#define UNITY_2018_4_16F1

#ifdef UNITY_2018_4_16F1
#include "../UnityStruct/unity2018-4-16f1/UnityStructInfo.h"
#else
#include "../UnityStruct/UnityInfo.h"
#endif

namespace li2cppHeader {

    // --- 字段信息 ---
    class UnityField {
    public:
        std::string name;
        std::string typeName;
        uint32_t offset;
        bool isStatic;
        uintptr_t staticAddr;

        // 默认构造
        UnityField() : offset(0), isStatic(false), staticAddr(0) {}

        // 常用参数化构造
        UnityField(std::string n, std::string t, uint32_t off, bool s = false, uintptr_t addr = 0)
                : name(std::move(n)), typeName(std::move(t)), offset(off), isStatic(s), staticAddr(addr) {}
    };

// --- 方法信息 ---
    class UnityMethod {
    public:
        std::string name;
        std::string returnType;
        std::vector<std::string> parameterTypes;
        uintptr_t methodPointer;
        uint32_t token;

        // 新增：用于生成的字段
        std::string signature;
        std::string typeSignature; // 例如 "viii" (void, int, int, int)

        UnityMethod() : methodPointer(0), token(0) {}

        // 参数化构造
        UnityMethod(std::string n, std::string ret, uintptr_t ptr, uint32_t tk = 0)
                : name(std::move(n)), returnType(std::move(ret)), methodPointer(ptr), token(tk) {}
    };

// --- 类信息 ---
    class UnityClass {
    public:
        // 定义一个共用体来优化存储
        union TypeFlags {
            uint32_t raw; // 方便一次性初始化或读取
            struct {
                uint32_t isStruct : 1;    // 占用 1 bit
                uint32_t isEnum   : 1;    // 占用 1 bit
                uint32_t isValueType : 1; // 占用 1 bit
                uint32_t isInterface : 1; // 占用 1 bit
                uint32_t reserved : 28;   // 剩余位保留
            } bits;

            TypeFlags() : raw(0) {}
        };

        std::string namespc;
        std::string className;
        std::string assembly;

        uintptr_t klassPtr;
        uintptr_t parentPtr;
        uint32_t instanceSize;
        uint32_t flags; // IL2CPP 原生的 flags

        TypeFlags typeAttr; // 我们自定义的分类标记

        std::vector<UnityField> fields;
        std::vector<UnityMethod> methods;
        std::vector<std::string> interfaces;

        // 默认构造
        UnityClass() : klassPtr(0), parentPtr(0), instanceSize(0), flags(0) {
            typeAttr.raw = 0;
        }

        // 参数化构造
        UnityClass(std::string ns, std::string name, uintptr_t ptr)
                : namespc(std::move(ns)), className(std::move(name)), klassPtr(ptr),
                  parentPtr(0), instanceSize(0), flags(0) {
            typeAttr.raw = 0;
        }

        std::string GetFullName() const {
            return namespc.empty() ? className : namespc + "::" + className;
        }
    };

    class li2cppHeader :public li2cppApi::cUnityApi{
    public:
        li2cppHeader(void* dqil2cppBase=nullptr,
                     void *pCodeRegistration=nullptr,
                     void *pMetadataRegistration=nullptr,
                     void *pGlobalMetadataHeader=nullptr,
                     void* pMetadataImagesTable=nullptr);
        ~li2cppHeader();


    public:
        void start();
        void Init( std::map<std::string, std::shared_ptr<UnityClass>>& classMap);
        std::string GetClassUniqueName( std::map<std::string, std::shared_ptr<UnityClass>>& classMap, std::shared_ptr<UnityClass>& targetClass,const Il2CppType* type);
        void DumpFields(Il2CppClass* klass, std::shared_ptr<UnityClass>& unityClass);
        std::string GetIdaCompatibleType(const Il2CppType* type);
        std::string GetSafeUniqueName(Il2CppClass* klass);
        uint32_t GetTypeSize(const Il2CppType* type);
        void SaveToIdaHeader(const std::string& path);
        std::string GetSafeGenericName(const Il2CppType* type);
        std::string CleanIdentifier(std::string name);
        void DumpMethods(Il2CppClass* klass, std::shared_ptr<UnityClass>& unityClass);
        std::string GetTypeAbbreviation(const std::string& type);
        void SaveToMethodJson(const std::string& path);
    private:
        uint64_t m_il2cppbase = 0;
        void* m_pGlobalMetadata =nullptr;
        Il2CppGlobalMetadataHeader *m_pGlobalMetadataHeader = nullptr;
        Il2CppMetadataRegistration *m_pil2CppMetadataRegistration = nullptr;
        Il2CppCodeRegistration *m_pIl2CppCodeRegistration = nullptr;
        Il2CppImageGlobalMetadata* m_pMetadataImagesTable = nullptr;

        // 2. 为了方便快速查找，可以再定义一个以全名为 Key 的索引
        std::map<std::string, std::shared_ptr<UnityClass>> m_classMap;
        std::string m_packname = "com.tencent.lolm";

    };
}


#endif //DOBBY_PROJECT_IL2CPPHEADER_H
