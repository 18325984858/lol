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

    if(m_pClassInfo && !m_pClassInfo->empty()){
        for(auto&ite:*m_pClassInfo){
            //判断模块名是否一致，如果一致说明在此链表中
            if(*ite->m_ModuleData->m_pName == pModuleName){
                //指向当前模块存储的所有数据结构开头,遍历当前所有结构
                for (auto&pClassStruct:*ite->m_pClassStruct) {
                    //判断类的名字，并判断泛型是否一致
                    if(*pClassStruct.m_pClassData->m_pName == pClassName &&
                       (ptemplateName.empty() ? 1 : *pClassStruct.m_pClassData->m_pGenericsName == ptemplateName)){
                        //如果找到类就遍历类下面的静态成员名，
                        for(auto&pStaticStruct:*pClassStruct.m_pVectorStaticData){
                            if(*pStaticStruct->m_Data.m_pName == pStaticName){
                                //如果都相等返回当前找到的指针
                                FieldInfo *fieldInfo = (FieldInfo*)pStaticStruct->fieldInfo;
                                uint64_t Value = 0;
                                if(fieldInfo != nullptr) {
                                    il2cpp_field_static_get_value(fieldInfo, &Value);
                                }
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

                // 打印信息
                //LOG(LOG_LEVEL_INFO, "    [Field] Name: %s, Offset: 0x%X", fieldName, offset);
                Buf = "[Field] Name:";
                Buf.append(fieldName);
                writeLog(Buf);
                AddVectorStaticData(pClassDataObj->m_pVectorStaticData, fieldName,field, (uint64_t)offset);
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