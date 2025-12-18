//
// Created by Song on 2025/11/19.
//

#ifndef DOBBY_PROJECT_LI2CPPDUMPER_H
#define DOBBY_PROJECT_LI2CPPDUMPER_H

#endif //DOBBY_PROJECT_LI2CPPDUMPER_H

#include "../UnityApi/unityapi.h"
#include "../File/file.h"
#include <map>
#include <list>

namespace li2cpp {

    typedef struct cMethodDefinitionAndMethodSpec{
        const Il2CppMethodDefinition* Method;
        const Il2CppMethodSpec* Spec;
        uint64_t offset;
    }cMethodDefinitionAndMethodSpec,*cPMethodDefinitionAndMethodSpec;

    class li2cppDumper:public li2cppApi::cUnityApi {
    public:
        li2cppDumper(void*dqil2cppBase=nullptr,
                     void *pCodeRegistration=nullptr,
                     void *pMetadataRegistration=nullptr,
                     void *pGlobalMetadataHeader=nullptr,
                     void*pMetadataImagesTable=nullptr);
        virtual~li2cppDumper();
    public:
        bool initInfo();
    public:
        //此函数功能是Dumpclass类型
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
        std::string get_method_space_class_name(StringIndex stringIndex,GenericInstIndex genericInstIndex);
        std::string get_method_generic_name(GenericInstIndex genericInstIndex);
    public:
        std::shared_ptr<std::list<std::shared_ptr<li2cpp::cMethodDefinitionAndMethodSpec>>> GetMethodToList(uint32_t Token);

    public:
        //存储保存的泛型数据
        std::shared_ptr<std::list<std::shared_ptr<cMethodDefinitionAndMethodSpec>>> m_methodList = nullptr;
    private:
        bool writeLog(std::string str);
    private:
        const std::string m_pathlog = "/data/data/com.DefaultCompany.Demo1/cache/log.cs";
        std::shared_ptr<cMyfile> m_outlog = nullptr;
    private:
        const std::string m_pathDumpCs = "/data/data/com.DefaultCompany.Demo1/cache/dumpcs.cs";
        //定义一个写文件的对象
        std::shared_ptr<cMyfile> m_outDumpCs = nullptr;
        //存储要写的数据
        std::shared_ptr<std::vector<std::string>> m_outPuts = nullptr;
    private:
        const std::string m_pathDumpstr = "/data/data/com.DefaultCompany.Demo1/cache/dumpstr.cs";
        //定义一个dump str的文件对象
        std::shared_ptr<cMyfile> m_outDumpstr = nullptr;
        //存储kIl2CppMetadataUsageStringLiteral类型的字符串
        std::shared_ptr<std::map<int,std::string>> m_kIl2CppMetadataUsageStringLiteral = nullptr;
    };
};


