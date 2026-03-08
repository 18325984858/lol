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

int32_t lol::FEVisi::get_MiniIconBaseCtrlType(void* pData) {

    uint32_t iconTypeOffset = GetFieldOffset(
            "Assembly-CSharp.dll",
            "UIMiniIconBaseCtrl",
            "UIMiniIconBaseCtrl",
            "iconType");
    if (iconTypeOffset == INVALID_OFFSET) return -1;

    return ReadMemberValue<int32_t>(pData, iconTypeOffset);
}

void *lol::FEVisi::test() {
    LOG(LOG_LEVEL_INFO, "[MiniMap] ========== test() 开始 ==========");

    // -------- Step 1: 获取 _entityCtrl 静态成员 --------
    auto* _entityCtrl = m_pfunctionInfo->GetStaticMember(
            "ilbil2cpp.so", "Assembly-CSharp.dll",
            "UIMainBattleMiniMapCtrl", "UIMainBattleMiniMapCtrl",
            "_entityCtrl");
    if (_entityCtrl == nullptr) {
        LOG(LOG_LEVEL_WARN, "[MiniMap] _entityCtrl 为空，跳过");
        return nullptr;
    }
    LOG(LOG_LEVEL_INFO, "[MiniMap] Step1 _entityCtrl: %p", _entityCtrl);

    // -------- Step 2: 通过偏移读取 miniMapIconCtrl --------
    uint32_t miniMapIconCtrlOffset = GetFieldOffset(
            "Assembly-CSharp.dll", "MiniMapEntityCtrl", "MiniMapEntityCtrl", "miniMapIconCtrl");
    if (miniMapIconCtrlOffset == INVALID_OFFSET) {
        LOG(LOG_LEVEL_WARN, "[MiniMap] miniMapIconCtrl 偏移查找失败");
        return nullptr;
    }
    void* miniMapIconCtrl = ReadMemberPtr(_entityCtrl, miniMapIconCtrlOffset);
    if (miniMapIconCtrl == nullptr) {
        LOG(LOG_LEVEL_WARN, "[MiniMap] miniMapIconCtrl 为空 (offset: 0x%X)", miniMapIconCtrlOffset);
        return nullptr;
    }
    LOG(LOG_LEVEL_INFO, "[MiniMap] Step2 miniMapIconCtrl: %p (offset: 0x%X)", miniMapIconCtrl, miniMapIconCtrlOffset);

    // -------- Step 3: 通过偏移读取 miniIcons 字典 --------
    uint32_t miniIconsOffset = GetFieldOffset(
            "Assembly-CSharp.dll", "UIMiniMapIconCtrl", "UIMiniMapIconCtrl", "miniIcons");
    if (miniIconsOffset == INVALID_OFFSET) {
        LOG(LOG_LEVEL_WARN, "[MiniMap] miniIcons 偏移查找失败");
        return nullptr;
    }
    System_Collections_Generic_Dictionary_TKey_TValue_o* pMiniIconsDictionary = static_cast<System_Collections_Generic_Dictionary_TKey_TValue_o *>(ReadMemberPtr(
            miniMapIconCtrl, miniIconsOffset));
    if (pMiniIconsDictionary == nullptr) {
        LOG(LOG_LEVEL_WARN, "[MiniMap] miniIcons 字典为空 (offset: 0x%X)", miniIconsOffset);
        return nullptr;
    }
    LOG(LOG_LEVEL_INFO, "[MiniMap] Step3 miniIcons: %p (offset: 0x%X) (count: 0x%X)", pMiniIconsDictionary, miniIconsOffset, pMiniIconsDictionary->fields.count);

    for(int i = 0; i < pMiniIconsDictionary->fields.count; i++) {
        System_Collections_Generic_Dictionary_Entry_TKey_TValue_array * pArray = pMiniIconsDictionary->fields.entries;
        System_Collections_Generic_Dictionary_Entry_TKey_TValue_o & pData = pArray->m_Items[i];

        // Dictionary 内部实现：hashCode < 0 表示空闲槽位，跳过
        if (pData.fields.hashCode < 0) continue;

        auto* baseCtrl = pData.fields.value;
        if (baseCtrl == nullptr) continue;

        int32_t iconType = get_MiniIconBaseCtrlType(baseCtrl);
        LOG(LOG_LEVEL_INFO, "[MiniMap] Entry[%d] iconType=%d, baseCtrl=%p", i, iconType, baseCtrl);

        if ((MiniMapIconType)iconType == MiniMapIconType_EnemyTeamHero) {
            // 读取敌方英雄关联的 actor
            UIMiniIconBaseCtrl_o* pIconCtrl = static_cast<UIMiniIconBaseCtrl_o*>(baseCtrl);
            auto* actor = pIconCtrl->fields.actor;
            bool isShowing = pIconCtrl->fields.isShowing;

            LOG(LOG_LEVEL_INFO, "[MiniMap] 发现敌方英雄! Entry[%d] actor=%p isShowing=%d",
                i, actor, isShowing);
        }
        else if((MiniMapIconType)iconType == MiniMapIconType_MyTeamWard){



        }
    }

    LOG(LOG_LEVEL_INFO, "[MiniMap] ========== test() 完成 ==========");
    return pMiniIconsDictionary;
}
