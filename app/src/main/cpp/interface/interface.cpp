//
// Created by Song on 2026/1/10.
//
#include "interface.h"
#include "../Log/log.h"
#include <sstream>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <android/log.h>

fun::function::function(void* dqil2cppBase,
                        void *pCodeRegistration,
                        void *pMetadataRegistration,
                        void *pGlobalMetadataHeader,
                        void* pMetadataImagesTable) :
        li2cppApi::cUnityApi(dqil2cppBase,
                             static_cast<Il2CppCodeRegistration *>(pCodeRegistration),
                             static_cast<Il2CppMetadataRegistration *>(pMetadataRegistration),
                             static_cast<Il2CppGlobalMetadataHeader *>(pGlobalMetadataHeader),
                             static_cast<Il2CppImageGlobalMetadata *>(pMetadataImagesTable)){
    initPackPath("com.tencent.lolm");

    // 初始化文件输出流
    auto initFile = [&](std::shared_ptr<cMyfile>& file, const std::string& path) {
        if (file == nullptr) {
            file = std::make_shared<cMyfile>(path);
            if (file) file->openFile(cMyfile::FileMode::ReadWrite);
        }
    };
    initFile(m_outlog, m_pathlog);

    if(m_pClassInfo){
        m_pClassInfo = std::make_shared<std::list<std::shared_ptr<CClassInfo>>>();
    }




}


/**
 * 日志记录函数
 * 负责将调试信息写入到 /data/data/com.DefaultCompany.Demo1/cache/log.cs
 */
bool fun::function::writeLog(std::string str) {
    if (m_outlog == nullptr) {
        return false;
    }

    // 格式化输出，增加标识前缀
    //std::string out = "[SFK] : " + str;


    //LOG(LOG_LEVEL_INFO,"%s",str.c_str());
    // 调用你文件类 cMyfile 的接口写入一行
    m_outlog->writeLine(str.c_str());

    // 实时冲刷缓冲区，防止日志丢失
    m_outlog->flush();

    return true;
}

void fun::function::initPackPath(std::string strPackName) {
    m_pathlog = "/data/data/"+strPackName+"/files/testlog.cs";
}

void*fun::function::GetStaticMember(std::string pMainModuleName, std::string pModuleName,
                                    std::string pClassName, std::string ptemplateName,
                                    std::string pStaticName) {
    LOG(LOG_LEVEL_INFO,"GetStaticMember call");
    if(m_pClassInfo && !m_pClassInfo->empty()){
        for(auto&ite:*m_pClassInfo){
            //判断模块名是否一致，如果一致说明在此链表中
            if(*ite->m_ModuleData->m_pName == pModuleName){
                LOG(LOG_LEVEL_INFO,"pModuleName : %s",pModuleName.c_str());

                //指向当前模块存储的所有数据结构开头,遍历当前所有结构
                for (auto&pClassStruct:*ite->m_pClassStruct) {
                    //判断类的名字，并判断泛型是否一致
                    if(*pClassStruct.m_pClassData->m_pName == pClassName &&
                       (ptemplateName.empty() ? 1 : *pClassStruct.m_pClassData->m_pGenericsName == ptemplateName)){

                        LOG(LOG_LEVEL_INFO,"pClassName : %s ptemplateName : %s",pClassName.c_str(),ptemplateName.c_str());

                        //如果找到类就遍历类下面的静态成员名，
                        for (auto& pStaticStruct : *pClassStruct.m_pVectorStaticData) {
                            // 假设 m_pName 是 std::string 或 const char*，直接比较字符串内容
                            if (*pStaticStruct->m_Data.m_pName == pStaticName) {

                                // 1. 获取 FieldInfo 指针 (保留你的位运算逻辑)
                                FieldInfo *fieldInfo = (FieldInfo*)((uint64_t)pStaticStruct->fieldInfo & 0x000000FFFFFFFFFF);

                                // 2. 基础指针检查
                                if (fieldInfo == nullptr) {
                                    writeLog( "fieldInfo is nullptr for : "+ pStaticName);
                                    return nullptr;
                                }

                                LOG(LOG_LEVEL_INFO, "Found Field: %s | fieldInfo: %p", pStaticName.c_str(), fieldInfo);
                                writeLog( "Found Field: "+ pStaticName);
                                // ================= 防御性检查核心 =================

                                // 3. 获取该字段所属的类
                                Il2CppClass* parentClass = il2cpp_field_get_parent(fieldInfo);
                                if (parentClass == nullptr) {
                                    writeLog( "Failed to get parent class for field: "+ pStaticName);
                                    return nullptr;
                                }

                                // 4. 关键：检查类是否初始化 (static_fields 是否为空)
                                // 如果类没有初始化，去读静态值 100% 导致崩溃
                                if (parentClass->static_fields == nullptr) {
                                    writeLog( "Class not initialized. Attempting to initialize: "+ pStaticName);


                                    // 尝试手动初始化类
                                    il2cpp_runtime_class_init(parentClass);

                                    // 再次检查是否成功
                                    if (parentClass->static_fields == nullptr) {
                                        writeLog("Initialization failed. Cannot retrieve static value.");
                                        return nullptr;
                                    }
                                }

                                // 5. 再次检查该字段是否真的是静态字段 (双重保险)
                                // 虽然你在 pVectorStaticData 里，但为了防止数据错乱，最好校验一下
                                uint32_t attrs = il2cpp_field_get_flags(fieldInfo);
                                if (!(attrs & FIELD_ATTRIBUTE_STATIC)) {
                                    LOG(LOG_LEVEL_ERROR, "Field is not static!");
                                    return nullptr;
                                }

                                // ================= 安全调用 =================

                                uint64_t Value = 0;

                                // 注意：这里仍然不需要 try-catch，因为上面的检查通过后，这里基本不会崩
                                // 除非 fieldInfo 指针本身是野指针（那是上层数据源的问题）
                                this->il2cpp_field_static_get_value(fieldInfo, &Value);

                                LOG(LOG_LEVEL_INFO, "Get Value Success -> fieldInfo: %p Value: 0x%llX", fieldInfo, Value);
                                return (void*)Value;
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

std::shared_ptr<fun::CClassInfo>
fun::function::AddModuleInfo(std::shared_ptr<fun::CClassInfo> pData,
                             std::string pModuleName, uint64_t offset) {
    if(pModuleName.empty() && pData==nullptr){
        return nullptr;
    }

    auto p = std::make_shared<CData>();
    if(p){
        p->m_pName = std::make_shared<std::string>(pModuleName);
        p->m_offset = offset;

        //将模块信息添加到指定的CClassInfo结构中
        pData->m_ModuleData = p;
    }
    return pData;
}

std::shared_ptr<fun::CClassData>
fun::function::AddClassStructInfo(std::shared_ptr<fun::CClassData> pData,std::string pClassName,
                                  std::string pGenericsName,uint64_t offset) {
    if(pData == nullptr && pClassName.empty()){
        return nullptr;
    }

    auto p = std::make_shared<fun::CTemplateData>();
    if(p){
        p->m_pName = std::make_shared<std::string>(pClassName);
        if(!pGenericsName.empty()){
            p->m_pGenericsName = std::make_shared<std::string>(pGenericsName);
        }
        p->m_offset = offset;

        pData->m_pClassData = p;
    }
    return pData;
}

std::shared_ptr<std::vector<std::shared_ptr<fun::CFieldData>>>
fun::function::AddVectorStaticData(std::shared_ptr<std::vector<std::shared_ptr<fun::CFieldData>>> pData,
                                   std::string pStaticName,FieldInfo *fieldInfo, uint64_t offset) {
    if (pData == nullptr && pStaticName.empty()) {
        return nullptr;
    }

    auto p = std::make_shared<fun::CFieldData>();
    if (p) {
        p->m_Data.m_pName = std::make_shared<std::string>(pStaticName);
        p->m_Data.m_offset = offset;
        p->fieldInfo = fieldInfo;
        pData->push_back(p);
    }
    return pData;
}

std::shared_ptr<std::vector<std::shared_ptr<fun::CTemplateData>>>
fun::function::AddVectorFunctionData(std::shared_ptr<std::vector<std::shared_ptr<fun::CTemplateData>>> pData,
                                     std::string pClassName, std::string pGenericsName,uint64_t offset) {
    if (pData == nullptr && pClassName.empty()) {
        return nullptr;
    }
    auto p = std::make_shared<fun::CTemplateData>();
    if (p) {
        p->m_pName = std::make_shared<std::string>(pClassName);
        if (!pGenericsName.empty()) {
            p->m_pGenericsName = std::make_shared<std::string>(pGenericsName);
        }
        p->m_offset = offset;
        pData->push_back(p);
    }
    return pData;
}

std::shared_ptr<std::list<std::shared_ptr<fun::CClassInfo>>>
fun::function::AddClassStruct(std::shared_ptr<std::list<std::shared_ptr<fun::CClassInfo>>> pData,
                              std::shared_ptr<fun::CData> pModuleData,std::shared_ptr<std::vector<fun::CClassData>> pClassStruct) {
    if (pData == nullptr) {
        return nullptr;
    }

    auto p = std::make_shared<fun::CClassInfo>();
    if (p) {
        p->m_ModuleData = pModuleData;
        p->m_pClassStruct = pClassStruct;

        pData->push_back(p);
    }
    return pData;
}

fun::function::~function() {
    DeepClearClassInfo();       //释放 m_pClassInfo 成员

    if (m_outlog) { m_outlog->flush(); m_outlog->closeFile(); }
}

void fun::function::DeepClearClassInfo() {
    if (m_pClassInfo == nullptr) return;

    // 1. 遍历 list 中的每一个 CClassInfo
    for (auto it = m_pClassInfo->begin(); it != m_pClassInfo->end(); ++it) {
        auto pClassInfo = *it;
        if (!pClassInfo) continue;

        // --- 释放 CClassInfo 内部成员 ---

        // 释放模块信息
        if (pClassInfo->m_ModuleData) {
            pClassInfo->m_ModuleData.reset();
        }

        // 遍历并释放类结构 vector (m_pClassStruct)
        if (pClassInfo->m_pClassStruct) {
            for (auto& classData : *(pClassInfo->m_pClassStruct)) {

                // 释放 CClassData 内部的嵌套容器
                if (classData.m_pClassData) {
                    classData.m_pClassData.reset();
                }

                // 显式清空静态数据 vector
                if (classData.m_pVectorStaticData) {
                    classData.m_pVectorStaticData->clear();
                    classData.m_pVectorStaticData.reset();
                }

                // 显式清空函数数据 vector
                if (classData.m_pVectorFunctionData) {
                    classData.m_pVectorFunctionData->clear();
                    classData.m_pVectorFunctionData.reset();
                }
            }
            pClassInfo->m_pClassStruct->clear();
            pClassInfo->m_pClassStruct.reset();
        }
    }

    // 2. 最后清空主链表并重置顶层指针
    m_pClassInfo->clear();
    m_pClassInfo.reset();
    m_pClassInfo = nullptr;
}

void fun::function::fillingClassInfo() {
    // 初始化顶层全局链表
    if (m_pClassInfo == nullptr) {
        m_pClassInfo = std::make_shared<std::list<std::shared_ptr<CClassInfo>>>();
    }

    char szBuf[0x1000] = {0};
    std::string Buf = "";
    auto domain = il2cpp_domain_get();
    size_t size;
    auto assemblies = il2cpp_domain_get_assemblies(domain, &size);

    LOG(LOG_LEVEL_INFO, "======== Start Filling Class Info (Assemblies: %zu) ========", size);

    for (size_t i = 0; i < size; ++i) {
        auto image = il2cpp_assembly_get_image(assemblies[i]);
        const char* imageName = il2cpp_image_get_name(image);

        //LOG(LOG_LEVEL_INFO, "[Module] Processing: %s", imageName);
        Buf = "[Module] Processing ";
        writeLog(Buf+imageName);

        // 1. 准备模块数据
        auto pModuleData = std::make_shared<CData>();
        pModuleData->m_pName = std::make_shared<std::string>(imageName);
        pModuleData->m_offset = (uint64_t)image;

        auto pClassStructVec = std::make_shared<std::vector<CClassData>>();

        auto classCount = il2cpp_image_get_class_count(image);
        for (int j = 0; j < classCount; ++j) {
            auto klass = il2cpp_image_get_class(image, j);
            if (!klass) continue;

            // 2. 准备类的数据结构
            auto pClassDataObj = std::make_shared<CClassData>();
            pClassDataObj->m_pVectorStaticData = std::make_shared<std::vector<std::shared_ptr<CFieldData>>>();
            pClassDataObj->m_pVectorFunctionData = std::make_shared<std::vector<std::shared_ptr<CTemplateData>>>();

            // 3. 获取类名
            const char* ns = il2cpp_class_get_namespace(const_cast<Il2CppClass *>(klass));
            const char* name = il2cpp_class_get_name(const_cast<Il2CppClass *>(klass));
            std::string className = (ns && *ns) ? (std::string(ns) + "." + name) : name;
            Buf = "[className] name ";
            Buf.append(name);
            writeLog(Buf);

            std::string genericsName = "";
            auto type = il2cpp_class_get_type(const_cast<Il2CppClass *>(klass));
            const char* pTypeName = il2cpp_type_get_name(type); // 注意：某些版本返回 char*
            if (pTypeName) {
                genericsName = pTypeName;
                // LOG(LOG_LEVEL_DEBUG, "  [Class] Full Name: %s", pTypeName);
                // il2cpp_free(pTypeName); // 如果你的 SDK 需要释放，请取消注释

                Buf = "[generics] name:";
                Buf.append(pTypeName);
                writeLog(Buf);
            }

            // 填充类基础信息
            AddClassStructInfo(pClassDataObj, className, genericsName, (uint64_t)klass);

            // 5. 遍历字段 (Fields)
            void* iter_field = nullptr;
            while (auto field = il2cpp_class_get_fields(const_cast<Il2CppClass *>(klass), &iter_field)) {
                const char* fieldName = il2cpp_field_get_name(field);
                uint32_t offset = il2cpp_field_get_offset(field);

                // 1. 获取字段属性标记
                uint32_t attrs = il2cpp_field_get_flags(field);

                // 2. 关键修复：只处理静态字段 (STATIC)
                //if (!(attrs & FIELD_ATTRIBUTE_STATIC)) {
                    // 如果是普通成员变量，直接跳过获取静态值的步骤，或者记录 offset 即可
                    // AddVectorStaticData(...); // 如果你需要记录成员变量偏移，保留这行，但别取值
                //    continue;
                //}

                // 3. 关键修复：确保类已经初始化，否则静态内存可能为空
                // 注意：il2cpp_runtime_class_init 可能需要 include 相关头文件或通过 dlsym 获取
                //il2cpp_runtime_class_init(const_cast<Il2CppClass *>(klass));

                Buf = "[Static Field] Name:";
                writeLog(Buf+fieldName);

                // 4. 关键修复：分配内存缓冲区
                // 静态字段的值可能是 int, float, struct, 或者 指针。
                // 为了安全，我们通常准备一个足够大的缓冲区，或者只针对 Object 类型取指针。

                // 获取字段类型
                const Il2CppType* type = il2cpp_field_get_type(field);
                uint32_t typeType = type->type; // 或 il2cpp_type_get_type(type)

                // 准备一个足够容纳大多数基础类型的缓冲区 (64位下指针是8字节)
                uint64_t buffer = 0;

                // 调用 API，传入 buffer 的地址 (&buffer) 而不是 buffer 本身
                //this->il2cpp_field_static_get_value(field, &buffer);

                // 格式化输出
                // 注意：如果该字段是结构体(struct)，这里只能打印出前8个字节，可能会乱码或不准确
                // 但对于指针(String, List, Object)或基础类型(Int, Bool)，这是有效的
                //sprintf(szBuf, " Value (ptr/int): 0x%llX", buffer);

                //writeLog( szBuf);

                // 记录数据
                AddVectorStaticData(pClassDataObj->m_pVectorStaticData, fieldName, field, (uint64_t)offset);
            }

            // 6. 遍历方法 (Methods)
            void* iter_method = nullptr;
            while (auto method = il2cpp_class_get_methods(const_cast<Il2CppClass *>(klass), &iter_method)) {
                const char* methodName = il2cpp_method_get_name(method);
                // 获取方法的虚地址（在内存中的地址）
                uint64_t methodAddr = (uint64_t)method->methodPointer;
                sprintf(szBuf,"%p",methodAddr);
                //LOG(LOG_LEVEL_INFO, "    [Method] Name: %s, Addr: 0x%llX", methodName, methodAddr);
                Buf = "[Method] Name: ";
                Buf.append(methodName);
                writeLog(Buf+" "+szBuf);
                AddVectorFunctionData(pClassDataObj->m_pVectorFunctionData, methodName, "", methodAddr);
            }

            // 7. 存入模块向量
            pClassStructVec->push_back(*pClassDataObj);
        }

        // 8. 存入全局链表
        AddClassStruct(m_pClassInfo, pModuleData, pClassStructVec);
    }
    LOG(LOG_LEVEL_INFO, "======== Class Info Filling Completed ========");
}