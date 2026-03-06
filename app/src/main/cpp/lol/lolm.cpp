//
// Created by user on 2026/1/4.
//

#include "lolm.h"
#include "../UnityApi/unityapi.h"
#include "../Log/log.h"

std::string lol::lol::decryPtthestring(char*Srcstr, uint32_t nameIndex) {

    LOG(LOG_LEVEL_INFO,"start nameIndex : %d Srcstr : %p Srcstrvalue : %s",nameIndex,Srcstr,Srcstr);

    auto pNode = this->m_pMapStringIndex->find(nameIndex);
    if (pNode != this->m_pMapStringIndex->end()) {
        return pNode->second->srcStr;
    }


    bool ret =IsEncryptionOrIsDecryption(nameIndex);
    LOG(LOG_LEVEL_INFO,"Ret = %d",ret);
    //判断是否加密了字符串
    if(ret){
        uint32_t newNameIndex = (int) nameIndex >= 0 ? nameIndex : nameIndex + 15;
        uint32_t arrayindex = 16LL * (int) (nameIndex - (newNameIndex & 0xFFFFFFF0));

        //判断是否需要第二次解密
        bool IsTwoEncryption = (uint8_t)(*Srcstr ^ byte_B9BBF1B[arrayindex]) == 0;

        //第一次解密
        *Srcstr ^= byte_B9BBF1B[arrayindex];

        if(!IsTwoEncryption){
            int keyIndex = 0;
            int count = 1;
            do{
                int tmpkey = keyIndex + 1;
                int tmpvalue = tmpkey >= 0 ? keyIndex + 1 : keyIndex + 16;
                keyIndex = tmpkey-(tmpvalue&0xFFFFFFF0);

                char srcchar = Srcstr[count];
                IsTwoEncryption = (uint8_t)(srcchar ^ byte_B9BBF1B[keyIndex + (int)arrayindex]) == 0;
                Srcstr[count++] = srcchar ^ byte_B9BBF1B[keyIndex + (int)arrayindex];
            }while(!IsTwoEncryption);
        }
        //设置表的状态
        *(uint32_t *)((uint64_t)this->parrayStringIndex + 4 * ((int)nameIndex >> 5)) |= 1 << (nameIndex & 0x1F);
    }

    //新曾节点保存解密后的字符串
    auto pnewNode = std::make_shared<LolStrStruct>();
    if (pnewNode) {
        pnewNode->isDecryption = 1;
        pnewNode->srcStr = Srcstr;
        this->m_pMapStringIndex->insert({nameIndex, pnewNode});
    }
    LOG(LOG_LEVEL_INFO,"end nameIndex : %d Srcstr : %p Srcstrvalue : %s",nameIndex,Srcstr,Srcstr);
    return Srcstr;
}

void* lol::lol::inItStringindexTable(const Il2CppGlobalMetadataHeader* pGlobalMetadataHeader) {
    uint64_t stringCount = pGlobalMetadataHeader->stringCount;
    uint32_t tableSize = (((stringCount >> 3) & 0x3FFFFFFFCLL) + 4) & 0x3FFFFFFFCLL;


    LOG(LOG_LEVEL_INFO,"[DumpStr] pGlobalMetadataHeader : %p stringCount ：%d tableSize : %d",
       pGlobalMetadataHeader,stringCount,tableSize);

    //初始化表用与记录那些字符串已经解密了
    if(this->parrayStringIndex == nullptr){
        this->parrayStringIndex = malloc(tableSize);
        if(parrayStringIndex){
            memset(parrayStringIndex,0,tableSize);
        }
    }

    if(this->m_pMapStringIndex == nullptr){
        m_pMapStringIndex = std::make_shared<std::map<uint32_t,std::shared_ptr<LolStrStruct>>>();
    }

    return (void*)this->parrayStringIndex;
}

bool lol::lol::IsEncryptionOrIsDecryption(uint32_t nameIndex) {
    uint32_t IsEncryption = 1 << (nameIndex & 0x1F);                                                                    // 判断是否加密
    uint32_t IsDecryption  = *(uint32_t*)((uint64_t)this->parrayStringIndex+ (unsigned int)((int)nameIndex >> 5));

    LOG(LOG_LEVEL_INFO,"IsEncryption : %d IsDecryption : %d",IsEncryption,IsDecryption);
    // 将nameindex除以32拿到在array中记录的索引// 判断是否重复解密
    return (IsEncryption & IsDecryption) == 0;
}

lol::lol::lol(void* dqil2cppBase,void *pCodeRegistration,
              void *pMetadataRegistration,void *pGlobalMetadataHeader,
              void* pMetadataImagesTable):
              FEVisi(dqil2cppBase,pCodeRegistration,pMetadataRegistration,
                     pGlobalMetadataHeader,pMetadataImagesTable){

    LOG(LOG_LEVEL_INFO,"[DumpStr] lol Init pGlobalMetadataHeader : %p",pGlobalMetadataHeader);

    if(pGlobalMetadataHeader) {
        this->inItStringindexTable(static_cast<const Il2CppGlobalMetadataHeader *>(pGlobalMetadataHeader));
    }

    LOG(LOG_LEVEL_INFO,"[DumpStr] lol Init Success! ");
}

lol::lol::~lol() {
    if(this->parrayStringIndex){
        free(this->parrayStringIndex);
    }

    if(this->m_pMapStringIndex){
        // 使用 auto& 以便直接修改 Map 内部存储的原始值
        for (auto& it : *this->m_pMapStringIndex) {
            it.second.reset();
        }
        this->m_pMapStringIndex->clear();
        this->m_pMapStringIndex= nullptr;
    }
}

lol::FEVisi::FEVisi(void* dqil2cppBase,void *pCodeRegistration,void *pMetadataRegistration,
                    void *pGlobalMetadataHeader,void* pMetadataImagesTable) {

    // 放弃使用 make_shared
    // 直接使用 shared_ptr 的构造函数
    m_pfunctionInfo = std::shared_ptr<fun::function>(new fun::function(
            (void*)dqil2cppBase,
            (void*)pCodeRegistration,
            (void*)pMetadataRegistration,
            (void*)pGlobalMetadataHeader,
            (void*)pMetadataImagesTable
    ));
    if(m_pfunctionInfo == nullptr){
        LOG(LOG_LEVEL_ERROR,"[Test Game] FEVisi 构造函数失败");
    }

    //初始化填充数据
    m_pfunctionInfo->fillingClassInfo();
}

lol::FEVisi::~FEVisi() {
    if(m_pfunctionInfo){
        m_pfunctionInfo = NULL;
    }
}

bool lol::FEVisi::get_BattleStarted() {
    bool isOpening = false;
    typedef bool (*get_BattleStartedpfn)();
    get_BattleStartedpfn pget_BattleStarted = (get_BattleStartedpfn)m_pfunctionInfo->GetMethodFun(
            "ilbil2cpp.so",
            "Assembly-CSharp.dll",
            "FEVisi",
            "FrameEngine.Visual.FEVisi",
            "get_BattleStarted");
    if(pget_BattleStarted){
         isOpening = pget_BattleStarted();
    }
    return isOpening;
}

void *lol::FEVisi::get_battleTeamMgr() {
    typedef void* (*get_battleTeamMgrpfn)();
    void*pData = nullptr;

    get_battleTeamMgrpfn get_battleTeamMgrfun = (get_battleTeamMgrpfn)m_pfunctionInfo->GetMethodFun(
            "ilbil2cpp.so",
            "Assembly-CSharp.dll",
            "FEVisi",
            "FrameEngine.Visual.FEVisi",
            "get_battleTeamMgr");
    if(get_battleTeamMgrfun){
        pData = get_battleTeamMgrfun();
    }
    return pData;
}

void *lol::FEVisi::test() {
    static const MethodInfo* s_findObjectsOfType = nullptr;
    static Il2CppReflectionType* s_feVisiTypeObject = nullptr;

    LOG(LOG_LEVEL_INFO, "[Test] FEVisi::test enter, cachedType=%p cachedMethod=%p", s_feVisiTypeObject, s_findObjectsOfType);

    // 1) Resolve FEVisi class
    ::Il2CppClass* pFEVisiClass = reinterpret_cast<::Il2CppClass*>(m_pfunctionInfo->FindClassByName(
            "ilbil2cpp.so",
            "Assembly-CSharp.dll",
            "FEVisi",
            "FrameEngine.Visual.FEVisi"));

    if (!pFEVisiClass) {
        LOG(LOG_LEVEL_ERROR, "[Test] FindClassByName failed: FEVisi not found");
        return nullptr;
    }

    // 2) Equivalent to GetType(): Il2CppClass -> Il2CppType -> System.Type
    if (!s_feVisiTypeObject) {
        const Il2CppType* pType = m_pfunctionInfo->il2cpp_class_get_type(pFEVisiClass);
        if (!pType) {
            LOG(LOG_LEVEL_ERROR, "[Test] il2cpp_class_get_type failed");
            return nullptr;
        }

        s_feVisiTypeObject = m_pfunctionInfo->il2cpp_type_get_object(pType);
        if (!s_feVisiTypeObject) {
            LOG(LOG_LEVEL_ERROR, "[Test] il2cpp_type_get_object failed");
            return nullptr;
        }
        LOG(LOG_LEVEL_INFO, "[Test] FEVisi System.Type object=%p", s_feVisiTypeObject);
    } else {
        LOG(LOG_LEVEL_INFO, "[Test] Reuse cached FEVisi System.Type object=%p", s_feVisiTypeObject);
    }

    // 3) Cache UnityEngine.Object.FindObjectsOfType(System.Type)
    if (!s_findObjectsOfType) {
        ::Il2CppClass* pObjectClass = reinterpret_cast<::Il2CppClass*>(m_pfunctionInfo->FindClassByName(
                "ilbil2cpp.so",
                "UnityEngine.CoreModule.dll",
                "Object",
                "UnityEngine.Object"));

        if (!pObjectClass) {
            LOG(LOG_LEVEL_ERROR, "[Test] FindClassByName failed: UnityEngine.Object not found");
            return nullptr;
        }

        void* iter = nullptr;
        while (const MethodInfo* pMethod = m_pfunctionInfo->il2cpp_class_get_methods(pObjectClass, &iter)) {
            const char* methodName = m_pfunctionInfo->il2cpp_method_get_name(pMethod);
            if (!methodName || std::string(methodName) != "FindObjectsOfType") {
                continue;
            }

            uint32_t paramCount = m_pfunctionInfo->il2cpp_method_get_param_count(pMethod);
            if (paramCount != 1) {
                continue;
            }

            const Il2CppType* pArgType = m_pfunctionInfo->il2cpp_method_get_param(pMethod, 0);
            const char* pArgTypeName = pArgType ? m_pfunctionInfo->il2cpp_type_get_name(pArgType) : nullptr;
            if (!pArgTypeName || std::string(pArgTypeName).find("System.Type") == std::string::npos) {
                continue;
            }

            s_findObjectsOfType = pMethod;
            break;
        }

        if (!s_findObjectsOfType) {
            LOG(LOG_LEVEL_ERROR, "[Test] FindObjectsOfType(System.Type) not found");
            return nullptr;
        }
        LOG(LOG_LEVEL_INFO, "[Test] Cached FindObjectsOfType(System.Type) method=%p", s_findObjectsOfType);
    } else {
        LOG(LOG_LEVEL_INFO, "[Test] Reuse cached FindObjectsOfType method=%p", s_findObjectsOfType);
    }

    // 4) Invoke FindObjectsOfType(type)
    void* params[1] = { s_feVisiTypeObject };
    Il2CppException* pExc = nullptr;

    LOG(LOG_LEVEL_INFO, "[Test] Invoke FindObjectsOfType method=%p typeArg=%p", s_findObjectsOfType, params[0]);
    Il2CppObject* pResult = m_pfunctionInfo->il2cpp_runtime_invoke(s_findObjectsOfType, nullptr, params, &pExc);

    if (pExc) {
        LOG(LOG_LEVEL_ERROR, "[Test] il2cpp_runtime_invoke exception=%p", pExc);
        return nullptr;
    }

    LOG(LOG_LEVEL_INFO, "[Test] FindObjectsOfType result array: %p", pResult);
    return pResult;
}
