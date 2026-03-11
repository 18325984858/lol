//
// Created by user on 2026/1/4.
//

#include "lolm.h"
#include "../UnityApi/unityapi.h"
#include "../Log/log.h"
#include "LolOffset.h"
#include <cstring>



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

    //保存解密后的字符串
    auto pnewNode = std::make_shared<LolStrStruct>();
    pnewNode->isDecryption = 1;
    pnewNode->srcStr = Srcstr;
    this->m_pMapStringIndex->insert({nameIndex, pnewNode});
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

uint32_t lol::FEVisi::GetFieldOffsetFromKlass(Il2CppClass* klass, const char* fieldName) {
    if (!klass || !fieldName || !m_pfunctionInfo) return INVALID_OFFSET;
    auto* current = reinterpret_cast<::Il2CppClass*>(klass);
    while (current) {
        void* iter = nullptr;
        while (auto* field = m_pfunctionInfo->il2cpp_class_get_fields(current, &iter)) {
            const char* name = m_pfunctionInfo->il2cpp_field_get_name(field);
            if (name && strcmp(name, fieldName) == 0) {
                return (uint32_t)m_pfunctionInfo->il2cpp_field_get_offset(field);
            }
        }
        current = m_pfunctionInfo->il2cpp_class_get_parent(current);
    }
    return INVALID_OFFSET;
}

bool lol::FEVisi::ReadStaticFieldInt32(Il2CppClass* klass, const char* fieldName, int32_t& outValue) {
    if (!klass || !fieldName || !m_pfunctionInfo) return false;
    auto* current = reinterpret_cast<::Il2CppClass*>(klass);
    while (current) {
        void* iter = nullptr;
        while (auto* field = m_pfunctionInfo->il2cpp_class_get_fields(current, &iter)) {
            const char* name = m_pfunctionInfo->il2cpp_field_get_name(field);
            if (name && strcmp(name, fieldName) == 0) {
                m_pfunctionInfo->il2cpp_field_static_get_value(field, &outValue);
                return true;
            }
        }
        current = m_pfunctionInfo->il2cpp_class_get_parent(current);
    }
    return false;
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
    this->m_pMapStringIndex = nullptr;
}

lol::FEVisi::FEVisi(void* dqil2cppBase,void *pCodeRegistration,void *pMetadataRegistration,
                    void *pGlobalMetadataHeader,void* pMetadataImagesTable)
    : m_il2cppBase(dqil2cppBase)
    , m_pfunctionInfo(std::make_shared<fun::function>(
            dqil2cppBase, pCodeRegistration, pMetadataRegistration,
            pGlobalMetadataHeader, pMetadataImagesTable))
{
    m_pfunctionInfo->fillingClassInfo();
}

lol::FEVisi::~FEVisi() = default;


bool lol::FEVisi::get_BattleStarted() {
    typedef bool (*get_BattleStartedpfn)();
    auto pget_BattleStarted = (get_BattleStartedpfn)m_pfunctionInfo->GetMethodFun(
            "ilbil2cpp.so",
            "Assembly-CSharp.dll",
            "FEVisi",
            "FrameEngine.Visual.FEVisi",
            "get_BattleStarted");
    return pget_BattleStarted ? pget_BattleStarted() : false;
}

void *lol::FEVisi::get_battleTeamMgr() {
    typedef void* (*get_battleTeamMgrpfn)();
    auto fn = (get_battleTeamMgrpfn)m_pfunctionInfo->GetMethodFun(
            "ilbil2cpp.so",
            "Assembly-CSharp.dll",
            "FEVisi",
            "FrameEngine.Visual.FEVisi",
            "get_battleTeamMgr");
    return fn ? fn() : nullptr;
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

    static float divideOfOne = 0.0f;
    static bool divideOfOneInit = false;

    if(!divideOfOneInit) {
        divideOfOneInit = true;
        uint64_t rawBits = m_pfunctionInfo->GetStaticMember(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "Fix64", "FrameEngine.Common.Fix64",
                "divideOfOne");
        // GetStaticMember 返回原始 IEEE 754 位模式，需要按位重新解释为 float
        memcpy(&divideOfOne, &rawBits, sizeof(float));
        LOG(LOG_LEVEL_INFO,"[MiniMap][DecoderFix64] rawBits=0x%llX divideOfOne=%f",
            (unsigned long long)rawBits, divideOfOne);
    }
    if (divideOfOne == 0.0f) return -1;

    LOG(LOG_LEVEL_INFO,"[MiniMap][DecoderFix64] value : %llu divideOfOne : %f",
        (unsigned long long)value, divideOfOne);
    return (float)value * divideOfOne;
}

// ═══════════════════════════════════════════════════════════════════════════════
// MiniMap 拆分函数实现
// ═══════════════════════════════════════════════════════════════════════════════

std::string lol::FEVisi::getManagedTypeName(void* pObject) {
    if (pObject == nullptr) return "<null>";

    auto* klass = GetObjectKlass(pObject);
    if (klass == nullptr) return "<no-klass>";

    auto* rKlass = reinterpret_cast<::Il2CppClass*>(klass);
    const char* pNamespace = m_pfunctionInfo->il2cpp_class_get_namespace(rKlass);
    const char* pName = m_pfunctionInfo->il2cpp_class_get_name(rKlass);
    if (pNamespace && pNamespace[0] != '\0') {
        return std::string(pNamespace) + "." + (pName ? pName : "<unnamed>");
    }
    return pName ? std::string(pName) : std::string("<unnamed>");
}

void* lol::FEVisi::getMiniIconsDictionary() {
    auto* _entityCtrl = (void*)m_pfunctionInfo->GetStaticMember(
            "ilbil2cpp.so", "Assembly-CSharp.dll",
            "UIMainBattleMiniMapCtrl", "UIMainBattleMiniMapCtrl",
            "_entityCtrl");
    if (_entityCtrl == nullptr) {
        LOG(LOG_LEVEL_ERROR, "[MiniMap] _entityCtrl 为空");
        return nullptr;
    }

    uint32_t miniMapIconCtrlOffset = GetFieldOffset(
            "Assembly-CSharp.dll", "MiniMapEntityCtrl", "MiniMapEntityCtrl", "miniMapIconCtrl");
    if (miniMapIconCtrlOffset == INVALID_OFFSET) return nullptr;

    void* miniMapIconCtrl = ReadMemberPtr(_entityCtrl, miniMapIconCtrlOffset);
    if (miniMapIconCtrl == nullptr) return nullptr;

    uint32_t miniIconsOffset = GetFieldOffset(
            "Assembly-CSharp.dll", "UIMiniMapIconCtrl", "UIMiniMapIconCtrl", "miniIcons");
    if (miniIconsOffset == INVALID_OFFSET) return nullptr;

    auto* pDict = ReadMemberPtr(miniMapIconCtrl, miniIconsOffset);
    if (pDict == nullptr) {
        LOG(LOG_LEVEL_ERROR, "[MiniMap] miniIcons Dictionary 为空");
    }
    return pDict;
}

bool lol::FEVisi::tryGetWorldPosition(void* pObject, UnityVector3& outPos) {
    using GetPositionFn = UnityVector3(*)(void*);
    static GetPositionFn s_getPosition = nullptr;

    if (!s_getPosition) {
        s_getPosition = reinterpret_cast<GetPositionFn>(m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "FixGameObjectVisi", "FrameEngine.Visual.FixGameObjectVisi",
                "GetPosition"));
    }
    if (pObject == nullptr || s_getPosition == nullptr) return false;
    outPos = s_getPosition(pObject);
    return true;
}

bool lol::FEVisi::getIconWorldPos(void* baseCtrl, void* actor, UnityVector3* outWorldPos) {

    // ── 缓存 followObj 字段偏移 ──
    static uint32_t s_followObjOffset = INVALID_OFFSET;
    if (s_followObjOffset == INVALID_OFFSET) {
        s_followObjOffset = GetFieldOffset(
                "Assembly-CSharp.dll", "UIMiniIconBaseCtrl",
                "UIMiniIconBaseCtrl", "followObj");
    }

    // ── 从 baseCtrl 读取 followObj ──
    void* followObj = nullptr;
    if (baseCtrl && s_followObjOffset != INVALID_OFFSET) {
        followObj = ReadMemberPtr(baseCtrl, s_followObjOffset);
    }

    // ── 优先从 actor 获取世界坐标，失败则从 followObj 获取 ──
    UnityVector3 pos{};
    bool hasWorldPos = tryGetWorldPosition(actor, pos)
                       || tryGetWorldPosition(followObj, pos);

    if (hasWorldPos && outWorldPos) {
        *outWorldPos = pos;
    }
    return hasWorldPos;
}

bool lol::FEVisi::isWardLikeIcon(int32_t iconType, void* actor,
                                  const std::string& baseCtrlTypeName,
                                  const std::string& actorTypeName) {
    using BoolFn = bool(*)(void*);
    static BoolFn s_isNeutralEye = nullptr;
    static BoolFn s_isSummonedWards = nullptr;

    if (!s_isNeutralEye) {
        s_isNeutralEye = reinterpret_cast<BoolFn>(m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActorVisi", "FrameEngine.Visual.BattleActorVisi",
                "IsNeutralEye"));
    }
    if (!s_isSummonedWards) {
        s_isSummonedWards = reinterpret_cast<BoolFn>(m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActorVisi", "FrameEngine.Visual.BattleActorVisi",
                "IsSummonedWards"));
    }

    // 1) 图标类型直接匹配
    if ((MiniMapIconType)iconType == MiniMapIconType_MyTeamWard) return true;

    // 2) 运行时方法检测
    if (actor != nullptr) {
        if (s_isNeutralEye && s_isNeutralEye(actor)) return true;
        if (s_isSummonedWards && s_isSummonedWards(actor)) return true;
    }

    // 3) 类型名关键字匹配
    if (baseCtrlTypeName.find("Ward") != std::string::npos ||
        baseCtrlTypeName.find("Eye")  != std::string::npos ||
        actorTypeName.find("Ward")    != std::string::npos ||
        actorTypeName.find("Eye")     != std::string::npos) {
        return true;
    }

    return false;
}


void* lol::FEVisi::getActorAttribute(void* actorVisi) {
    if (!actorVisi) return nullptr;

    // ── BattleActorVisi → get_actor() → BattleActor ──
    typedef void* (*pGetLogicActor)(void*);
    static pGetLogicActor s_getLogicActor = nullptr;
    if (!s_getLogicActor) {
        s_getLogicActor = (pGetLogicActor)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActorVisi", "FrameEngine.Visual.BattleActorVisi",
                "get_actor");
    }
    if (!s_getLogicActor) return nullptr;

    void* pBattleActor = s_getLogicActor(actorVisi);
    if (!pBattleActor) return nullptr;

    // ── BattleActor → get_attribute() → ActorComponentAttribute ──
    typedef void* (*pGetAttribute)(void*);
    static pGetAttribute s_getAttribute = nullptr;
    if (!s_getAttribute) {
        s_getAttribute = (pGetAttribute)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActor", "FrameEngine.Logic.BattleActor",
                "get_attribute");
    }
    if (!s_getAttribute) return nullptr;

    return s_getAttribute(pBattleActor);
}

uint32_t lol::FEVisi::readEnemyHeroLeve(void* pAttribute) {
    if (!pAttribute) return 0;

    // nativeData = *(pAttribute + 0x18)
    int64_t nativeData = *(int64_t*)((uint8_t*)pAttribute + 0x18);
    if (!nativeData) return 0;

    // heroLevel = *(nativeData + 0x78)  — RVA 0x8495058
    return *(uint32_t*)(nativeData + 0x78);
}

void lol::FEVisi::readEnemyHeroHP(void* pAttribute,float* curHp, float* maxHp) {
    if (!pAttribute) return;

    // ═══════════════════════════════════════════════════════════════
    // IDA 逆向确认的 HP 读取方式 (已通过血条UI函数交叉验证):
    //   nativeData = *(pAttribute + 0x18)
    //   curHP(Fix64) = *(nativeData + 0x138)  — RVA 0x8495B98
    //   maxHP(Fix64) = GetFixAttrValue(attr, 2) — RVA 0x84959E0
    //   maxHP(备用)  = RVA 0x8495BC8(this)
    //   heroLevel    = *(nativeData + 0x78)   — RVA 0x8495058
    //   shield       = *(nativeData + 0F0)   — RVA 0x849510C
    // ═══════════════════════════════════════════════════════════════

    int64_t nativeData = *(int64_t*)((uint8_t*)pAttribute + 0x18);
    if (!nativeData) {
        LOG(LOG_LEVEL_ERROR, "[MiniMap][HP] nativeData(this+0x18) 为空!");
        return;
    }

    //当前生命值的偏移位置
    int64_t curHP_raw  = *(int64_t*)(nativeData + 0x138);

    // maxHP: GetFixAttrValue(attr, 2)
    typedef int64_t (*pGetMaxHP)(void*);
    static pGetMaxHP s_getMaxHP = nullptr;
    if (!s_getMaxHP)
        s_getMaxHP = (pGetMaxHP)((uint64_t)m_il2cppBase + 0x84959E0);

    int64_t maxHP_raw = s_getMaxHP ? s_getMaxHP(pAttribute) : 0;

    // 备用: sub_8495BC8 (nativeData+0xB8 缓存 → GetFixAttrValue(0x17)*GetFixAttrValue(0x18))
    if (maxHP_raw == 0) {
        typedef int64_t (*pGetMaxHP_Fallback)(void*);
        static pGetMaxHP_Fallback s_getMaxHP_Fallback = nullptr;
        if (!s_getMaxHP_Fallback)
            s_getMaxHP_Fallback = (pGetMaxHP_Fallback)((uint64_t)m_il2cppBase + 0x8495BC8);
        if (s_getMaxHP_Fallback)
            maxHP_raw = s_getMaxHP_Fallback(pAttribute);
    }

    if(curHp){
        *curHp = DecoderFix64(curHP_raw); // Fix64 转 float
    }
    if(maxHp){
        *maxHp = DecoderFix64(maxHP_raw); // Fix64 转 float
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// test() — 小地图主循环（遍历所有图标，处理敌方英雄HP和眼位坐标）
// ═══════════════════════════════════════════════════════════════════════════════

void *lol::FEVisi::test() {
    auto logFixVector = [](const char* prefix, const FrameEngine_Common_FixVector3_Fix64_Fields& pos) {
        LOG(LOG_LEVEL_INFO,
            "%s raw=(%lld,%lld,%lld) approx=(%.3f,%.3f,%.3f)",
            prefix,
            static_cast<long long>(pos.x.rawValue),
            static_cast<long long>(pos.y.rawValue),
            static_cast<long long>(pos.z.rawValue),
            static_cast<double>(pos.x.rawValue) / 65536.0,
            static_cast<double>(pos.y.rawValue) / 65536.0,
            static_cast<double>(pos.z.rawValue) / 65536.0);
    };
    auto logUnityVector = [](const char* prefix, const UnityVector3& pos) {
        LOG(LOG_LEVEL_INFO, "%s xyz=(%.3f,%.3f,%.3f)", prefix, pos.x, pos.y, pos.z);
    };

    // ── 1. 获取小地图图标字典 ──
    auto* pMiniIconsDictionary = getMiniIconsDictionary();
    if (!pMiniIconsDictionary) return nullptr;

    // ── 2. 初始化 Dictionary 迭代元数据（缓存） ──
    static uint32_t s_dictCountOffset = INVALID_OFFSET;
    static uint32_t s_dictEntriesOffset = INVALID_OFFSET;
    static uint32_t s_entryHashCodeOffset = INVALID_OFFSET;
    static uint32_t s_entryValueOffset = INVALID_OFFSET;
    static size_t   s_entryStride = 0;

    if (s_dictCountOffset == INVALID_OFFSET) {
        auto* dictKlass = GetObjectKlass(pMiniIconsDictionary);
        s_dictCountOffset = GetFieldOffsetFromKlass(dictKlass, "count");
        s_dictEntriesOffset = GetFieldOffsetFromKlass(dictKlass, "entries");
        LOG(LOG_LEVEL_INFO, "[MiniMap] Dict offsets: count=%u entries=%u",
            s_dictCountOffset, s_dictEntriesOffset);
    }
    if (s_dictCountOffset == INVALID_OFFSET || s_dictEntriesOffset == INVALID_OFFSET) {
        LOG(LOG_LEVEL_ERROR, "[MiniMap] 无法获取 Dictionary 字段偏移");
        return nullptr;
    }

    int32_t dictCount = ReadMemberValue<int32_t>(pMiniIconsDictionary, s_dictCountOffset);
    void* entriesArray = ReadMemberPtr(pMiniIconsDictionary, s_dictEntriesOffset);
    if (!entriesArray) return nullptr;

    if (s_entryStride == 0) {
        auto* arrKlass = GetObjectKlass(entriesArray);
        auto* arrRKlass = reinterpret_cast<::Il2CppClass*>(arrKlass);
        auto* arrType = m_pfunctionInfo->il2cpp_class_get_type(arrRKlass);
        auto* elemRKlass = m_pfunctionInfo->il2cpp_type_get_class_or_element_class(arrType);
        auto* elemKlass = reinterpret_cast<Il2CppClass*>(elemRKlass);

        uint32_t rawHashCodeOff = GetFieldOffsetFromKlass(elemKlass, "hashCode");
        uint32_t rawValueOff    = GetFieldOffsetFromKlass(elemKlass, "value");

        int32_t instSize    = m_pfunctionInfo->il2cpp_class_instance_size(elemRKlass);
        bool    isValueType = m_pfunctionInfo->il2cpp_class_is_valuetype(elemRKlass);
        s_entryStride = isValueType ? (size_t)(instSize - 0x10) : (size_t)instSize;

        if (isValueType) {
            s_entryHashCodeOffset = rawHashCodeOff - 0x10;
            s_entryValueOffset    = rawValueOff - 0x10;
        } else {
            s_entryHashCodeOffset = rawHashCodeOff;
            s_entryValueOffset    = rawValueOff;
        }

        LOG(LOG_LEVEL_INFO, "[MiniMap] Entry stride=%zu hashCodeOff=%u valueOff=%u (raw: %u, %u)",
            s_entryStride, s_entryHashCodeOffset, s_entryValueOffset,
            rawHashCodeOff, rawValueOff);
    }

    // ── 3. 缓存 UIMiniIconBaseCtrl 字段偏移 ──
    static uint32_t s_cacheFollowPosOffset = INVALID_OFFSET;
    if (s_cacheFollowPosOffset == INVALID_OFFSET) {
        s_cacheFollowPosOffset = GetFieldOffset(
                "Assembly-CSharp.dll", "UIMiniIconBaseCtrl", "UIMiniIconBaseCtrl", "cacheFollowPos");
    }

    typedef void* (*pGetActor)(void*);
    static pGetActor s_getActor = nullptr;
    if (!s_getActor) {
        s_getActor = (pGetActor)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "UIMiniIconBaseCtrl", "UIMiniIconBaseCtrl", "get_actor");
    }

    // ── 4. 遍历字典条目 ──
    constexpr size_t kArrayHeaderSize = 0x20;

    for (int i = 0; i < dictCount; i++) {
        uint8_t* entryBase = reinterpret_cast<uint8_t*>(entriesArray) + kArrayHeaderSize + i * s_entryStride;

        int32_t hashCode = *reinterpret_cast<int32_t*>(entryBase + s_entryHashCodeOffset);
        if (hashCode < 0) continue;

        void* baseCtrl = *reinterpret_cast<void**>(entryBase + s_entryValueOffset);
        if (!baseCtrl) continue;

        const int32_t iconType = get_MiniIconBaseCtrlType(baseCtrl);
        void* actor = s_getActor ? s_getActor(baseCtrl) : nullptr;

        FrameEngine_Common_FixVector3_Fix64_Fields cacheFollowPos{};
        if (s_cacheFollowPosOffset != INVALID_OFFSET) {
            cacheFollowPos = ReadMemberValue<FrameEngine_Common_FixVector3_Fix64_Fields>(baseCtrl, s_cacheFollowPosOffset);
        }

        // ── 获取世界坐标 ──
        UnityVector3 worldPos{};
        const bool hasWorldPos = getIconWorldPos(baseCtrl, actor, &worldPos);

        // ── 敌方英雄：输出坐标 + 读取 HP ──
        if ((MiniMapIconType)iconType == MiniMapIconType_EnemyTeamHero) {
            auto pAttribute = getActorAttribute(actor);
            uint32_t heroLevel = readEnemyHeroLeve(pAttribute);

            float curHp = 0.0f, maxHp = 0.0f;
            readEnemyHeroHP(pAttribute, &curHp, &maxHp);

            LOG(LOG_LEVEL_INFO, "[MiniMap][EnemyHero] 英雄等级：%u HP=%.1f/%.1f xyz=(%.3f,%.3f,%.3f)",heroLevel, curHp, maxHp, worldPos.x, worldPos.y, worldPos.z);


            continue;
        }

        // ── 眼/守卫类：输出坐标 ──
        const std::string baseCtrlTypeName = getManagedTypeName(baseCtrl);
        const std::string actorTypeName    = getManagedTypeName(actor);

        if (isWardLikeIcon(iconType, actor, baseCtrlTypeName, actorTypeName)) {
            logFixVector("[MiniMap][WardLike] cacheFollowPos", cacheFollowPos);
            if (hasWorldPos) {
                logUnityVector("[MiniMap][WardLike] worldPos", worldPos);
            }
        }
    }

    return pMiniIconsDictionary;
}
