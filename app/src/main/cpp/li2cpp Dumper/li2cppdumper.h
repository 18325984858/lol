//
// Created by Song on 2025/11/19.
//

#ifndef DOBBY_PROJECT_LI2CPPDUMPER_H
#define DOBBY_PROJECT_LI2CPPDUMPER_H

#include "../UnityApi/unityapi.h"
#include "../File/file.h"
#include <map>
#include <list>
#include <unordered_map> // 必须添加，用于性能优化
#include <vector>
#include <memory>
#include <string>

namespace li2cpp {

    typedef struct cMethodDefinitionAndMethodSpec{
        const Il2CppMethodDefinition* Method;
        const Il2CppMethodSpec* Spec;
        uint64_t offset;
    }cMethodDefinitionAndMethodSpec,*cPMethodDefinitionAndMethodSpec;

    class li2cppDumper : public li2cppApi::cUnityApi {
    public:
        li2cppDumper(void* dqil2cppBase=nullptr,
                     void *pCodeRegistration=nullptr,
                     void *pMetadataRegistration=nullptr,
                     void *pGlobalMetadataHeader=nullptr,
                     void* pMetadataImagesTable=nullptr);
        virtual ~li2cppDumper();

    public:
        bool initInfo();
        std::string dumpcs();
        std::string dumpType(const Il2CppType *type);
        std::string dump_field(Il2CppClass *klass);
        std::string dump_property(Il2CppClass *klass);
        std::string get_method_modifier(uint32_t flags);
        std::string dump_method(Il2CppClass *klass);
        std::string dumpStr();
        std::string dump_RuntimeMetadata(uintptr_t* metadataPointer);
        std::string dumpGenericsMethod();
        std::string get_type_name(const Il2CppType* type);
        std::string get_method_space_name(const Il2CppMethodSpec* spec);
        std::string get_method_generic_name(GenericInstIndex genericInstIndex);

    public:
        // 保留你原有的接口
        std::shared_ptr<std::list<std::shared_ptr<li2cpp::cMethodDefinitionAndMethodSpec>>> GetMethodToList(uint32_t Token);

    public:
        // 存储保存的泛型数据
        std::shared_ptr<std::list<std::shared_ptr<cMethodDefinitionAndMethodSpec>>> m_methodList = nullptr;

    private:
        // 性能优化辅助：用于快速根据 Token 查找 Spec 实例
        std::shared_ptr<std::unordered_multimap<uint32_t, std::shared_ptr<cMethodDefinitionAndMethodSpec>>> m_methodMap = nullptr;
        bool writeLog(std::string str);

    private:
        const std::string m_pathlog = "/data/data/com.DefaultCompany.Demo1/cache/log.cs";
        std::shared_ptr<cMyfile> m_outlog = nullptr;

    private:
        const std::string m_pathDumpCs = "/data/data/com.DefaultCompany.Demo1/cache/dumpcs.cs";
        std::shared_ptr<cMyfile> m_outDumpCs = nullptr;
        std::shared_ptr<std::vector<std::string>> m_outPuts = nullptr;

    private:
        const std::string m_pathDumpstr = "/data/data/com.DefaultCompany.Demo1/cache/dumpstr.cs";
        std::shared_ptr<cMyfile> m_outDumpstr = nullptr;
        std::shared_ptr<std::map<int,std::string>> m_kIl2CppMetadataUsageStringLiteral = nullptr;
    };
};

#endif //DOBBY_PROJECT_LI2CPPDUMPER_H