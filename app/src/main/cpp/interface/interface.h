
//
// Created by Song on 2026/1/10.
//

#ifndef DOBBY_PROJECT_INTERFACE_H
#define DOBBY_PROJECT_INTERFACE_H



#include "../UnityApi/unityapi.h"
#include "../File/file.h"
#include <map>
#include <list>
#include <unordered_map> // 必须添加，用于性能优化
#include <vector>
#include <memory>
#include <string>

namespace fun {
    typedef struct _Data{
        std::shared_ptr<std::string> m_pName;
        uint64_t m_offset;
    }CData,*PCData;

    typedef struct _fieldData{
        CData m_Data;
        FieldInfo *fieldInfo;
    }CFieldData,*PCFieldData;

    typedef struct _TemplateData{
        std::shared_ptr<std::string> m_pName;
        std::shared_ptr<std::string> m_pGenericsName;
        uint64_t m_offset;
    }CTemplateData,*PTemplateData;

    typedef struct _ClassData{
        std::shared_ptr<CTemplateData> m_pClassData;                                        //存储当前类名
        std::shared_ptr<std::vector<std::shared_ptr<CFieldData>>> m_pVectorStaticData;      //存储当前类名
        std::shared_ptr<std::vector<std::shared_ptr<CTemplateData>>> m_pVectorFunctionData; //存储方法名
    }CClassData,*PClassData;

    typedef struct _ClassInfo{
        std::shared_ptr<CData> m_ModuleData;                                                //存储函数模块信息
        std::shared_ptr<std::vector<CClassData>> m_pClassStruct;                            //存储类结构
    }CClassInfo,*PCClassInfo;



    class function : public li2cppApi::cUnityApi {
    public:
        function(void* dqil2cppBase=nullptr,
                 void *pCodeRegistration=nullptr,
                 void *pMetadataRegistration=nullptr,
                 void *pGlobalMetadataHeader=nullptr,
                 void* pMetadataImagesTable=nullptr);
        ~function();
    public:
        void fillingClassInfo();
        void DeepClearClassInfo();
        bool writeLog(std::string str);
        void initPackPath(std::string strPackName);
    public:
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 参数一：模块名例如 libil2cpp.so
        // 参数二：类所在模块名例如 Assembly-CSharp.dll
        // 参数三：类名 例如 <>c__DisplayClass6_0
        // 参数四：泛型名 例如 JoystickSkillHandler.<>c__DisplayClass6_0
        // 参数五：要查找的静态成员名称
        // 返回值：成功返回找到的值，失败返回0
        void *GetStaticMember(std::string pMainModuleName, std::string pModuleName,std::string pClassName,std::string ptemplateName, std::string pStaticName);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 参数一：模块名例如 libil2cpp.so
        // 参数二：类所在模块名例如 Assembly-CSharp.dll
        // 参数三：类名 例如 <>c__DisplayClass6_0
        // 参数四：泛型名 例如 JoystickSkillHandler.<>c__DisplayClass6_0
        // 参数五：要查找成员方法名称
        // 参数六：要查找成员方法的泛型名
        // 返回值：成功返回找到的值，失败返回0
        void* GetMethodFun(std::string pMainModuleName, std::string pModuleName,std::string pClassName, std::string ptemplateName,std::string pMethodName,std::string pMethodtemplateName = "");
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::shared_ptr<fun::CClassInfo>AddModuleInfo(std::shared_ptr<fun::CClassInfo> pData, std::string pModuleName,uint64_t offset = -1);
        std::shared_ptr<fun::CClassData>AddClassStructInfo(std::shared_ptr<fun::CClassData> pData, std::string pClassName,std::string pGenericsName, uint64_t offset = -1);
        std::shared_ptr<std::vector<std::shared_ptr<fun::CFieldData>>>AddVectorStaticData(std::shared_ptr<std::vector<std::shared_ptr<fun::CFieldData>>> pData,std::string pStaticName,FieldInfo *fieldInfo, uint64_t offset = -1);
        std::shared_ptr<std::vector<std::shared_ptr<fun::CTemplateData>>>AddVectorFunctionData(std::shared_ptr<std::vector<std::shared_ptr<fun::CTemplateData>>> pData,std::string pClassName, std::string pGenericsName,uint64_t offset = -1);
        std::shared_ptr<std::list<std::shared_ptr<fun::CClassInfo>>>AddClassStruct(std::shared_ptr<std::list<std::shared_ptr<fun::CClassInfo>>> pData,std::shared_ptr<fun::CData> pModuleData,std::shared_ptr<std::vector<fun::CClassData>>pClassStruct);
    private:
        std::string m_pathlog = "";
        std::shared_ptr<cMyfile> m_outlog = nullptr;
        std::shared_ptr<std::list<std::shared_ptr<CClassInfo>>>m_pClassInfo = nullptr;
    };

}

#endif //DOBBY_PROJECT_INTERFACE_H
