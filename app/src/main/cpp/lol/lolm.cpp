//
// Created by user on 2026/1/4.
//

#include "lolm.h"
#include "../UnityApi/unityapi.h"
#include "../Log/log.h"
#include "LolOffset.h"
#include <cmath>



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

    m_il2cppBase = dqil2cppBase; // Initialize base address

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

    static uint32_t iconTypeOffset = INVALID_OFFSET;
    if(iconTypeOffset == INVALID_OFFSET) {
            iconTypeOffset = GetFieldOffset(
                "Assembly-CSharp.dll",
                "UIMiniIconBaseCtrl",
                "UIMiniIconBaseCtrl",
                "iconType");
    }
    if (iconTypeOffset == INVALID_OFFSET) return -1;
    return ReadMemberValue<int32_t>(pData, iconTypeOffset);
}

float lol::FEVisi::DecoderFix64(uint64_t value) {

    static float divideOfOne = INVALID_OFFSET;

    if(divideOfOne == INVALID_OFFSET) {
        divideOfOne = m_pfunctionInfo->GetStaticMember(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "Fix64", "FrameEngine.Common.Fix64",
                "divideOfOne");
    }
    if (divideOfOne == INVALID_OFFSET) return -1;

    LOG(LOG_LEVEL_INFO,"[MiniMap][DecoderFix64] value : %d divideOfOne : %f",value,divideOfOne);
    return (float)value * divideOfOne;
}

void *lol::FEVisi::test1(){
    LOG(LOG_LEVEL_INFO, "[Test1] Start Trace using hardcoded offsets");

    // 1. FEVisi.get_battle()
    typedef void* (*pBattle_get_battle)();
    static pBattle_get_battle Battle_get_battle = nullptr;
    if(Battle_get_battle == nullptr) {
        Battle_get_battle = (pBattle_get_battle) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll", "FEVisi", "FrameEngine.Visual.FEVisi", "get_battle");
    }

    if(!Battle_get_battle) { LOG(LOG_LEVEL_ERROR, "[Test1]Battle_get_battle not found"); return nullptr; }
    void* pBattle = Battle_get_battle();
    if (!pBattle) return nullptr;

    // 2. Battle.get_gamePlayComponentMgr()
    typedef void* (*pBattle_get_gamePlayComponentMgr)(void*);
    static pBattle_get_gamePlayComponentMgr Battle_get_gamePlayComponentMgr = nullptr;
    if(Battle_get_gamePlayComponentMgr == nullptr) {
        Battle_get_gamePlayComponentMgr = (pBattle_get_gamePlayComponentMgr) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll", "Battle", "FrameEngine.Logic.Battle", "get_gamePlayComponentMgr");
    }
    if (!Battle_get_gamePlayComponentMgr) { LOG(LOG_LEVEL_ERROR, "[Test1]Battle_get_gamePlayComponentMgr not found"); return nullptr; }
    void* pGameplayComponentMgr = Battle_get_gamePlayComponentMgr(pBattle);
    LOG(LOG_LEVEL_INFO, "[Test1]pGameplayComponentMgr: %p", pGameplayComponentMgr);
    if (!pGameplayComponentMgr) return nullptr;

    // 3. get_cherryDataMgr()
    typedef void* (*pGameplayComponentMgr_get_cherryDataMgr)(void*);
    static pGameplayComponentMgr_get_cherryDataMgr GameplayComponentMgr_get_cherryDataMgr = nullptr;
    if(GameplayComponentMgr_get_cherryDataMgr == nullptr) {
        GameplayComponentMgr_get_cherryDataMgr = (pGameplayComponentMgr_get_cherryDataMgr) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll", "GamePlayComponentMgr", "FrameEngine.Logic.GamePlayComponentMgr", "get_cherryDataMgr");
    }
     if (!GameplayComponentMgr_get_cherryDataMgr) { LOG(LOG_LEVEL_ERROR, "[Test1]GamePlayComponentMgr_get_cherryDataMgr not found"); return nullptr; }
    void* pCherryDataMgr = GameplayComponentMgr_get_cherryDataMgr(pGameplayComponentMgr);
    LOG(LOG_LEVEL_INFO, "[Test1]pCherryDataMgr: %p", pCherryDataMgr);
    if (!pCherryDataMgr) return nullptr;

    // 4. get_teamList()
    typedef void* (*pCherryDataMgr_get_teamList)(void*);
    static pCherryDataMgr_get_teamList CherryDataMgr_get_teamList = nullptr;
    if(CherryDataMgr_get_teamList == nullptr) {
        CherryDataMgr_get_teamList = (pCherryDataMgr_get_teamList) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll", "CherryDataMgr", "FrameEngine.Logic.CherryDataMgr", "get_teamList");
    }
    if (!CherryDataMgr_get_teamList) { LOG(LOG_LEVEL_ERROR, "[Test1]CherryDataMgr_get_teamList not found"); return nullptr; }
    void* pTeamList = CherryDataMgr_get_teamList(pCherryDataMgr);
    LOG(LOG_LEVEL_INFO, "[Test1]pTeamList: %p", pTeamList);
    if (!pTeamList) return nullptr;

    // 5. Iterate List using hardcoded offsets
    // RVA: 0x08E479E4 Spec: DataShellList<CherryTeam...>.get_Count
    // RVA: 0x08E478EC Spec: DataShellList<CherryTeam...>.get_Item



    typedef int32_t (*pDataShellList_get_Count)(void*);
    static pDataShellList_get_Count DataShellList_get_Count = nullptr;
    if(DataShellList_get_Count == nullptr) {
        DataShellList_get_Count = (pDataShellList_get_Count) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll", "DataShellList`4", "FrameEngine.DataShellList<T1,T2,T3,T4>", "get_Count");
    }
    LOG(LOG_LEVEL_INFO, "[Test1]DataShellList_get_Count: %p", DataShellList_get_Count);

    int32_t size = DataShellList_get_Count(pTeamList);
    LOG(LOG_LEVEL_INFO, "[Test1]Team List Size: %d", size);


    typedef void* (*pDataShellList_get_Item)(void*,int);
    static pDataShellList_get_Item DataShellList_get_Item = nullptr;
    if(DataShellList_get_Item == nullptr) {
        DataShellList_get_Item = (pDataShellList_get_Item) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll", "DataShellList`4", "FrameEngine.DataShellList<T1,T2,T3,T4>", "get_Item");
    }
    LOG(LOG_LEVEL_INFO, "[Test1]DataShellList_get_Item: %p", DataShellList_get_Item);


    for (int i = 0; i < size; i++) {
        void* pCherryTeam = DataShellList_get_Item(pTeamList, i);
        if (!pCherryTeam) continue;

        // 6. Match Camp & HP using hardcoded offsets
        // get_camp RVA: 0x7ddfd5c
        // get_curHp RVA: 0x7ddfd48

        typedef int32_t (*pCherryTeam_get_camp)(void*);
        static pCherryTeam_get_camp CherryTeam_get_camp = nullptr;

        typedef int32_t (*pCherryTeam_get_curHp)(void*);
        static pCherryTeam_get_curHp CherryTeam_get_curHp = nullptr;

        if (!CherryTeam_get_camp) {
            CherryTeam_get_camp = (pCherryTeam_get_camp)((uint64_t)m_il2cppBase + 0x7ddfd5c);
        }
        if (!CherryTeam_get_curHp) {
            CherryTeam_get_curHp = (pCherryTeam_get_curHp)((uint64_t)m_il2cppBase + 0x7ddfd48);
        }

        int32_t camp = (CherryTeam_get_camp) ? CherryTeam_get_camp(pCherryTeam) : -1;
        int32_t hp = (CherryTeam_get_curHp) ? CherryTeam_get_curHp(pCherryTeam) : 0;

        LOG(LOG_LEVEL_INFO, "[Test1][Team %d] Camp: %d HP: %d", i, camp, hp);
    }

    return nullptr;
}

void *lol::FEVisi::test() {

    typedef FrameEngine_Logic_Battle_o* (*pBattle_get_battle)();

    static pBattle_get_battle Battle_get_battle = nullptr;

    if(Battle_get_battle == nullptr) {
        Battle_get_battle = (pBattle_get_battle) m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so",
                "Assembly-CSharp.dll",
                "FEVisi",
                "FrameEngine.Visual.FEVisi",
                "get_battle");
    }
    LOG(LOG_LEVEL_INFO,"Battle_get_battle : %p",Battle_get_battle);
    auto pBattle = Battle_get_battle();
    LOG(LOG_LEVEL_INFO,"pBattle : %p",pBattle);

    return nullptr;


    constexpr double kFix64Scale = 65536.0;
    struct UnityVector3 {
        float x;
        float y;
        float z;
    };

    auto fix64ToDouble = [](int64_t rawValue) -> double {
        return static_cast<double>(rawValue) / kFix64Scale;
    };
    auto getManagedTypeName = [](void* pObject) -> std::string {
        if (pObject == nullptr) return "<null>";

        auto* pManagedObject = reinterpret_cast<Il2CppObject*>(pObject);
        if (pManagedObject->klass == nullptr) return "<no-klass>";

        const char* pNamespace = pManagedObject->klass->_1.namespaze;
        const char* pName = pManagedObject->klass->_1.name;
        if (pNamespace && pNamespace[0] != '\0') {
            return std::string(pNamespace) + "." + (pName ? pName : "<unnamed>");
        }
        return pName ? std::string(pName) : std::string("<unnamed>");
    };
    auto logFixVector = [&](const char* prefix, const FrameEngine_Common_FixVector3_Fix64_Fields& pos) {
        LOG(LOG_LEVEL_INFO,
            "%s raw=(%lld,%lld,%lld) approx=(%.3f,%.3f,%.3f)",
            prefix,
            static_cast<long long>(pos.x.rawValue),
            static_cast<long long>(pos.y.rawValue),
            static_cast<long long>(pos.z.rawValue),
            fix64ToDouble(pos.x.rawValue),
            fix64ToDouble(pos.y.rawValue),
            fix64ToDouble(pos.z.rawValue));
    };
    auto logUnityVector = [&](const char* prefix, const UnityVector3& pos) {
        LOG(LOG_LEVEL_INFO,
            "%s xyz=(%.3f,%.3f,%.3f)",
            prefix,
            pos.x,
            pos.y,
            pos.z);
    };

    using FixGameObjectVisiGetPositionFn = UnityVector3(*)(void*);
    using BattleActorVisiBoolFn = bool(*)(void*);

    static FixGameObjectVisiGetPositionFn s_fixGameObjectVisiGetPosition = nullptr;
    static BattleActorVisiBoolFn s_battleActorVisiIsNeutralEye = nullptr;
    static BattleActorVisiBoolFn s_battleActorVisiIsSummonedWards = nullptr;

    if (!s_fixGameObjectVisiGetPosition) {
        s_fixGameObjectVisiGetPosition = reinterpret_cast<FixGameObjectVisiGetPositionFn>(m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so",
                "Assembly-CSharp.dll",
                "FixGameObjectVisi",
                "FrameEngine.Visual.FixGameObjectVisi",
                "GetPosition"));
    }
    if (!s_battleActorVisiIsNeutralEye) {
        s_battleActorVisiIsNeutralEye = reinterpret_cast<BattleActorVisiBoolFn>(m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so",
                "Assembly-CSharp.dll",
                "BattleActorVisi",
                "FrameEngine.Visual.BattleActorVisi",
                "IsNeutralEye"));
    }
    if (!s_battleActorVisiIsSummonedWards) {
        s_battleActorVisiIsSummonedWards = reinterpret_cast<BattleActorVisiBoolFn>(m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so",
                "Assembly-CSharp.dll",
                "BattleActorVisi",
                "FrameEngine.Visual.BattleActorVisi",
                "IsSummonedWards"));
    }

    auto tryInvokeGetPosition = [&](void* pObject,
                                    UnityVector3& outPos) -> bool {
        if (pObject == nullptr || s_fixGameObjectVisiGetPosition == nullptr) return false;
        outPos = s_fixGameObjectVisiGetPosition(pObject);
        return true;
    };

    auto* _entityCtrl = (void*)m_pfunctionInfo->GetStaticMember(
            "ilbil2cpp.so", "Assembly-CSharp.dll",
            "UIMainBattleMiniMapCtrl", "UIMainBattleMiniMapCtrl",
            "_entityCtrl");
    if (_entityCtrl == nullptr) {
        return nullptr;
    }

    uint32_t miniMapIconCtrlOffset = GetFieldOffset(
            "Assembly-CSharp.dll", "MiniMapEntityCtrl", "MiniMapEntityCtrl", "miniMapIconCtrl");
    if (miniMapIconCtrlOffset == INVALID_OFFSET) {
        return nullptr;
    }
    void* miniMapIconCtrl = ReadMemberPtr(_entityCtrl, miniMapIconCtrlOffset);
    if (miniMapIconCtrl == nullptr) {
        return nullptr;
    }

    uint32_t miniIconsOffset = GetFieldOffset(
            "Assembly-CSharp.dll", "UIMiniMapIconCtrl", "UIMiniMapIconCtrl", "miniIcons");
    if (miniIconsOffset == INVALID_OFFSET) {
        return nullptr;
    }
    auto* pMiniIconsDictionary = static_cast<System_Collections_Generic_Dictionary_TKey_TValue_o*>(ReadMemberPtr(
            miniMapIconCtrl, miniIconsOffset));
    if (pMiniIconsDictionary == nullptr || pMiniIconsDictionary->fields.entries == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < pMiniIconsDictionary->fields.count; i++) {
        auto* pArray = pMiniIconsDictionary->fields.entries;
        auto& pData = pArray->m_Items[i];

        if (pData.fields.hashCode < 0) continue;

        auto* baseCtrl = pData.fields.value;
        if (baseCtrl == nullptr) continue;

        const int32_t iconType = get_MiniIconBaseCtrlType(baseCtrl);
        auto* pIconCtrl = static_cast<UIMiniIconBaseCtrl_o*>(baseCtrl);

        typedef FrameEngine_Visual_BattleActorVisi_o* (*pBattleActorVisi_get_actor)(UIMiniIconBaseCtrl_o*_pthis);
        pBattleActorVisi_get_actor BattleActorVisi_get_actor = nullptr;
        if(BattleActorVisi_get_actor == nullptr) {
            BattleActorVisi_get_actor = (pBattleActorVisi_get_actor) m_pfunctionInfo->GetMethodFun(
                    "ilbil2cpp.so",
                    "Assembly-CSharp.dll",
                    "UIMiniIconBaseCtrl",
                    "UIMiniIconBaseCtrl",
                    "get_actor");
        }

        auto* actor = BattleActorVisi_get_actor(pIconCtrl);
        auto* followObj = pIconCtrl->fields.followObj;
        const auto& cacheFollowPos = pIconCtrl->fields.cacheFollowPos;
        const std::string baseCtrlTypeName = getManagedTypeName(baseCtrl);
        const std::string actorTypeName = getManagedTypeName(actor);

        UnityVector3 worldPos{};
        const bool hasWorldPos = tryInvokeGetPosition(actor, worldPos)
                                 || tryInvokeGetPosition(followObj, worldPos);

        bool isWardLike = (MiniMapIconType)iconType == MiniMapIconType_MyTeamWard;
        if (actor != nullptr) {
            const bool isNeutralEye = s_battleActorVisiIsNeutralEye && s_battleActorVisiIsNeutralEye(actor);
            const bool isSummonedWards = s_battleActorVisiIsSummonedWards && s_battleActorVisiIsSummonedWards(actor);
            isWardLike = isWardLike || isNeutralEye || isSummonedWards;
        }
        if (baseCtrlTypeName.find("Ward") != std::string::npos ||
            baseCtrlTypeName.find("Eye") != std::string::npos ||
            actorTypeName.find("Ward") != std::string::npos ||
            actorTypeName.find("Eye") != std::string::npos) {
            isWardLike = true;
        }

        if ((MiniMapIconType)iconType == MiniMapIconType_EnemyTeamHero) {
            logFixVector("[MiniMap][EnemyHero] cacheFollowPos", cacheFollowPos);
            if (hasWorldPos) {
                logUnityVector("[MiniMap][EnemyHero] worldPos", worldPos);
            }


            typedef FrameEngine_Logic_BattleActor_o* (*pBattleActor_get_actor)(FrameEngine_Visual_BattleActorVisi_o*_pthis);
            static pBattleActor_get_actor BattleActor_get_actor = nullptr;
            if(BattleActor_get_actor == nullptr) {
                BattleActor_get_actor = (pBattleActor_get_actor) m_pfunctionInfo->GetMethodFun(
                        "ilbil2cpp.so",
                        "Assembly-CSharp.dll",
                        "BattleActorVisi",
                        "FrameEngine.Visual.BattleActorVisi",
                        "get_actor");
            }
            LOG(LOG_LEVEL_INFO,"[MiniMap][HP]BattleActor_get_actor: %p ",BattleActor_get_actor);
            auto pBattleActoractor = BattleActor_get_actor(actor);

            typedef FrameEngine_Logic_ActorComponentAttribute_o* (*pActorComponentAttribute_get_attribute)(FrameEngine_Logic_BattleActor_o*_pthis);
            static pActorComponentAttribute_get_attribute ActorComponentAttribute_get_attribute = nullptr;
            if(ActorComponentAttribute_get_attribute == nullptr) {
                ActorComponentAttribute_get_attribute = (pActorComponentAttribute_get_attribute) m_pfunctionInfo->GetMethodFun(
                        "ilbil2cpp.so",
                        "Assembly-CSharp.dll",
                        "BattleActor",
                        "FrameEngine.Logic.BattleActor",
                        "get_attribute");
            }
            LOG(LOG_LEVEL_INFO,"[MiniMap][HP]ActorComponentAttribute_get_attribute: %p ",ActorComponentAttribute_get_attribute);
            auto pActorComponentAttribute = ActorComponentAttribute_get_attribute(pBattleActoractor);

            LOG(LOG_LEVEL_INFO,"[MiniMap][HP]pBattleActoractor: %p pActorComponentAttribute:%p",pBattleActoractor,pActorComponentAttribute);

                if (pActorComponentAttribute) {
                    uint64_t value = *(uint64_t *) (
                            pActorComponentAttribute->fields.__InstanceLongValue + 0x138);
                    float d = DecoderFix64(value);
                    LOG(LOG_LEVEL_INFO, "[MiniMap][HP]敌方血量信息: %f", d);
                }

            continue;
        }

        if (isWardLike) {
            logFixVector("[MiniMap][WardLike] cacheFollowPos", cacheFollowPos);
            if (hasWorldPos) {
                logUnityVector("[MiniMap][WardLike] worldPos", worldPos);
            }
        }
    }

    return pMiniIconsDictionary;
}
