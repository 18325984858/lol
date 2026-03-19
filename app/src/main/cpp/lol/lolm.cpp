//
// Created by user on 2026/1/4.
//

#include "lolm.h"
#include "../UnityApi/unityapi.h"
#include "../Log/log.h"
#include "LolOffset.h"
#include <cstring>
#include <unistd.h>   // pipe, write, close
#include <errno.h>
#include <chrono>
#include <unordered_map>
#include <sys/mman.h>  // mprotect



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


     LOG(LOG_LEVEL_INFO,"[DumpStr] pGlobalMetadataHeader : %p stringCount : %llu tableSize : %u",
         pGlobalMetadataHeader, (unsigned long long)stringCount, tableSize);

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

uint32_t lol::FEVisi::GetFieldOffsetFromKlass(::Il2CppClass* klass, const char* fieldName) {
    if (!klass || !fieldName || !m_pfunctionInfo) return INVALID_OFFSET;
    auto* current = klass;
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

bool lol::FEVisi::ReadStaticFieldInt32(::Il2CppClass* klass, const char* fieldName, int32_t& outValue) {
    if (!klass || !fieldName || !m_pfunctionInfo) return false;
    auto* current = klass;
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


    //此处补丁是为了将游戏模式设置成
    // ── Patch: dqil2cppBase + 0xB69B494 ──
    // 原始指令: F3 03 01 2A (mov w19, w1)
    // 目标指令: 73 00 80 52 (mov w19, #3)
    {
        uint8_t* patchAddr = reinterpret_cast<uint8_t*>(dqil2cppBase) + 0xB69B494;
        const uint8_t original[] = {0xF3, 0x03, 0x01, 0x2A};
        const uint8_t patched[]  = {0x93, 0x00, 0x80, 0x52};

        if (memcmp(patchAddr, original, sizeof(original)) == 0) {
            size_t pageSize = sysconf(_SC_PAGESIZE);
            uintptr_t pageStart = reinterpret_cast<uintptr_t>(patchAddr) & ~(pageSize - 1);
            if (mprotect(reinterpret_cast<void*>(pageStart), pageSize * 2, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
                memcpy(patchAddr, patched, sizeof(patched));
                __builtin___clear_cache(reinterpret_cast<char*>(patchAddr),
                                        reinterpret_cast<char*>(patchAddr + sizeof(patched)));
                LOG(LOG_LEVEL_INFO, "[Patch] 0xB69B494 patch 成功: F3 03 01 2A (MOV W19, W1) → 93 00 80 52 (MOV W19, #4)");
            } else {
                LOG(LOG_LEVEL_ERROR, "[Patch] mprotect 失败: %s", strerror(errno));
            }
        } else {
            LOG(LOG_LEVEL_WARN, "[Patch] 0xB69B494 原始字节不匹配, 跳过 (当前: %02X %02X %02X %02X)",
                patchAddr[0], patchAddr[1], patchAddr[2], patchAddr[3]);
        }
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
// 英雄名称读取
// ═══════════════════════════════════════════════════════════════════════════════
int32_t lol::FEVisi::readHeroResId(void* actorVisi) {
    if (!actorVisi) return 0;

    static uint32_t s_resIdOffset = INVALID_OFFSET;
    if (s_resIdOffset == INVALID_OFFSET) {
        s_resIdOffset = GetFieldOffset(
                "Assembly-CSharp.dll", "BattleActorVisi",
                "FrameEngine.Visual.BattleActorVisi", "_resId");
        LOG(LOG_LEVEL_INFO, "[HeroName] _resId offset = 0x%X", s_resIdOffset);
    }
    if (s_resIdOffset == INVALID_OFFSET) return 0;
    return ReadMemberValue<int32_t>(actorVisi, s_resIdOffset);
}

std::string lol::FEVisi::readHeroName(void* actorVisi) {
    if (!actorVisi) return "";

    // ═══════════════════════════════════════════════════════════════
    // 英雄名称读取策略（安全版）:
    //
    // ⚠ 不直接调用 BattleActorVisi.get_name()，因其内部会触发
    //   IsHero() → BattleContext.GetPlayerNameInBattle(get_roleId())
    //   等复杂链路，在注���上下文中调用会导致崩溃。
    //
    // 方式1: 直接读取 <name>k__BackingField 缓存字段（安全的内存读取）
    //        IDA: sub_45704A8 → cached = ActorVisi[45] → offset +0x168
    // 方式2: 通过 UIHeroUtils.GetHeroName(resId) 回退查询配置表
    // ═══════════════════════════════════════════════════════════════

    // ── 方式1: 读取 <name>k__BackingField 缓存 ──
    static uint32_t s_cachedNameOffset = INVALID_OFFSET;
    static bool s_offsetResolved = false;
    if (!s_offsetResolved) {
        s_offsetResolved = true;
        s_cachedNameOffset = GetFieldOffset(
                "Assembly-CSharp.dll", "BattleActorVisi",
                "FrameEngine.Visual.BattleActorVisi", "<name>k__BackingField");
        if (s_cachedNameOffset == INVALID_OFFSET) {
            s_cachedNameOffset = 0x168; // IDA 硬编码 fallbackFrameEngine_Visual_BattleActorVisi__get_name
            LOG(LOG_LEVEL_INFO, "[HeroName] 使用 IDA 硬编���缓存偏移 0x168");
        } else {
            LOG(LOG_LEVEL_INFO, "[HeroName] <name>k__BackingField offset = 0x%X", s_cachedNameOffset);
        }
    }

    void* pCachedStr = ReadMemberPtr(actorVisi, s_cachedNameOffset);
    if (pCachedStr) {
        std::string heroName = m_pfunctionInfo->il2cpp_Il2CppString_toCString(
                static_cast<const Il2CppString*>(pCachedStr));
        LOG(LOG_LEVEL_INFO, "[HeroName] 缓存(+0x%X) → \"%s\"", s_cachedNameOffset, heroName.c_str());
        if (!heroName.empty()) return heroName;
    }

    // ── 方式2: 回退 UIHeroUtils.GetHeroName(resId) ──
    int32_t resId = readHeroResId(actorVisi);
    if (resId <= 0) {
        LOG(LOG_LEVEL_WARN, "[HeroName] 缓存为空且 resId=%d 无效，无法获取英雄名", resId);
        return "";
    }

    typedef void* (*pGetHeroName)(int32_t, void*);
    static pGetHeroName s_getHeroName = nullptr;
    static bool s_heroNameResolved = false;
    if (!s_heroNameResolved) {
        s_heroNameResolved = true;
        s_getHeroName = (pGetHeroName)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "UIHeroUtils", "UIHeroUtils",
                "GetHeroName");
        LOG(LOG_LEVEL_INFO, "[HeroName] UIHeroUtils.GetHeroName resolved: %p", (void*)s_getHeroName);
    }
    if (!s_getHeroName) return "";

    void* pFallbackStr = s_getHeroName(resId, nullptr);
    if (!pFallbackStr) return "";

    std::string heroName = m_pfunctionInfo->il2cpp_Il2CppString_toCString(
            static_cast<const Il2CppString*>(pFallbackStr));
    LOG(LOG_LEVEL_INFO, "[HeroName] UIHeroUtils: resId=%d → \"%s\"", resId, heroName.c_str());
    return heroName;
}

std::string lol::FEVisi::readSummonerName(void* actorVisi) {
    if (!actorVisi) return "";

    // ── 1. BattleActorVisi → get_player() → BattlePlayer ──
    typedef void* (*pGetPlayer)(void*);
    static pGetPlayer s_getPlayer = nullptr;
    if (!s_getPlayer) {
        s_getPlayer = (pGetPlayer)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActorVisi", "FrameEngine.Visual.BattleActorVisi",
                "get_player");
        LOG(LOG_LEVEL_INFO, "[SummonerName] get_player resolved: %p", (void*)s_getPlayer);
    }
    if (!s_getPlayer) return "";

    void* pBattlePlayer = s_getPlayer(actorVisi);
    if (!pBattlePlayer) {
        LOG(LOG_LEVEL_WARN, "[SummonerName] BattlePlayer 为空");
        return "";
    }

    // ── 2. 验证 BattlePlayer 是有效的托管对象 ──
    auto* klass = GetObjectKlass(pBattlePlayer);
    if (!klass) {
        LOG(LOG_LEVEL_WARN, "[SummonerName] BattlePlayer klass 无效");
        return "";
    }

    // ── 3. 直接读取已缓存的召唤师名，不调用 get_name() ──
    //    IDA 反汇编 get_name (0x91E7160):
    //      LDR X0, [this, #0xB0]       ; 读取缓存
    //      BL  String::IsNullOrEmpty    ; 检查缓存
    //      若非空 → 直接返回
    //      若为空 → 调用 BattleContext.GetPlayerNameInBattle(roleId)（复杂链路，注入上下文不安全）
    //
    //    因此我们直接读 this+0xB0, 跳过复杂的 BattleContext 查找链

    // 优先通过运行时反射查找 name 字段的精确偏移
    static uint32_t s_nameOffset = INVALID_OFFSET;
    if (s_nameOffset==INVALID_OFFSET) {

        m_pfunctionInfo->GetStaticMember(
                "ilbil2cpp.so",
                "Assembly-CSharp.dll",
                "BattlePlayer",
                "FrameEngine.Logic.BattlePlayer",
                "_name",
                reinterpret_cast<uint32_t *>(&s_nameOffset));
    }

    void* pNameStr = ReadMemberPtr(pBattlePlayer, s_nameOffset);
    if (!pNameStr) {
        LOG(LOG_LEVEL_INFO, "[SummonerName] 名称缓存未填充 (offset=0x%X)", s_nameOffset);

        return "";
    }

    // ── 4. Il2CppString → std::string (UTF-8) ──
    return m_pfunctionInfo->il2cpp_Il2CppString_toCString(
            static_cast<const Il2CppString *>(pNameStr));
}

// ═══════════════════════════════════════════════════════════════════════════════
// worldToScreen() — 世界坐标 → 屏幕坐标（通过 Unity Camera.WorldToScreenPoint）
//
// 使用 FindClassByName + GetMethodFunByClass 精确解析 Camera 类方法：
//   Camera.get_main()              — 0 参数，静态属性 getter
//   Camera.WorldToScreenPoint(V3)  — 1 参数（仅 Vector3），避免匹配 2 参数重载
//
// 返回的屏幕坐标为 Unity 空间（原点左下角，Y 从下到上）。
// 绘制层（ImGui）需自行翻转 Y。
// ═══════════════════════════════════════════════════════════════════════════════

bool lol::FEVisi::worldToScreen(const UnityVector3& worldPos, float& outScreenX, float& outScreenY) {

    // ── 缓存: Camera 类和方法指针 ──
    using GetMainCameraFn     = void*(*)();
    using WorldToScreenPointFn = UnityVector3(*)(void*, UnityVector3);

    static bool                  s_resolved       = false;
    static GetMainCameraFn       s_getMainCamera  = nullptr;
    static WorldToScreenPointFn  s_w2s            = nullptr;

    if (!s_resolved) {
        s_resolved = true;

        // 查找 UnityEngine.Camera 类
        auto* cameraClass = m_pfunctionInfo->FindClassByName(
                "ilbil2cpp.so", "UnityEngine.CoreModule.dll",
                "Camera", "UnityEngine.Camera");

        if (!cameraClass) {
            LOG(LOG_LEVEL_ERROR, "[W2S] 无法找到 UnityEngine.Camera 类");
            return false;
        }

        // 确保类的方法指针已初始化
        m_pfunctionInfo->il2cpp_runtime_class_init(cameraClass);
        m_pfunctionInfo->il2cpp_class_init_all_method(cameraClass);

        // 解析 Camera.get_main (0 参数, 静态 getter)
        s_getMainCamera = reinterpret_cast<GetMainCameraFn>(
                m_pfunctionInfo->GetMethodFunByClass(cameraClass, "get_main", 0));

        // 解析 Camera.WorldToScreenPoint (1 参数: Vector3)
        s_w2s = reinterpret_cast<WorldToScreenPointFn>(
                m_pfunctionInfo->GetMethodFunByClass(cameraClass, "WorldToScreenPoint", 1));

        LOG(LOG_LEVEL_INFO, "[W2S] Camera class=%p  get_main=%p  WorldToScreenPoint=%p",
            cameraClass, (void*)s_getMainCamera, (void*)s_w2s);
    }

    if (!s_getMainCamera || !s_w2s) return false;

    // ── 获取主摄像机 (带帧级缓存, 避免每个英雄都调用 get_main) ──
    static void* s_cachedCamera = nullptr;
    static std::chrono::steady_clock::time_point s_cameraTime{};
    auto now = std::chrono::steady_clock::now();
    // 每 100ms 刷新一次摄像机指针
    if (!s_cachedCamera ||
        std::chrono::duration_cast<std::chrono::milliseconds>(now - s_cameraTime).count() > 100) {
        s_cachedCamera = s_getMainCamera();
        s_cameraTime = now;
    }
    void* mainCamera = s_cachedCamera;
    if (!mainCamera || !IsReadableMemory(mainCamera, sizeof(void*))) {
        s_cachedCamera = nullptr;  // 失效时清空缓存
        return false;
    }

    // ── 调用 WorldToScreenPoint ──
    UnityVector3 screenPos = s_w2s(mainCamera, worldPos);

    // z < 0 表示目标在相机后方，不应绘制
    if (screenPos.z < 0.0f) return false;

    outScreenX = screenPos.x;
    outScreenY = screenPos.y;
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════════
// updateMiniMapData() — 小地图主循环（遍历所有图标，收集敌方英雄HP和眼位坐标）
// ═══════════════════════════════════════════════════════════════════════════════

void *lol::FEVisi::updateMiniMapData() {

    // ── 清空上一帧数据 ──
    m_miniMapData.clear();

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
        auto* arrRKlass = GetObjectKlass(entriesArray);
        auto* arrType = m_pfunctionInfo->il2cpp_class_get_type(arrRKlass);
        auto* elemRKlass = m_pfunctionInfo->il2cpp_type_get_class_or_element_class(arrType);

        uint32_t rawHashCodeOff = GetFieldOffsetFromKlass(elemRKlass, "hashCode");
        uint32_t rawValueOff    = GetFieldOffsetFromKlass(elemRKlass, "value");

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

    // ── 4. 遍历字典条目，收集数据到 m_miniMapData ──
    constexpr size_t kArrayHeaderSize = 0x20;

    for (int i = 0; i < dictCount; i++) {
        uint8_t* entryBase = reinterpret_cast<uint8_t*>(entriesArray) + kArrayHeaderSize + i * s_entryStride;

        // ── 第2层防御: 验证 entry 内存可读 ──
        if (!IsReadableMemory(entryBase, s_entryStride)) {
            LOG(LOG_LEVEL_WARN, "[MiniMap] entry[%d] 内存不可读 (%p), 跳过", i, entryBase);
            continue;
        }

        int32_t hashCode = *reinterpret_cast<int32_t*>(entryBase + s_entryHashCodeOffset);
        if (hashCode < 0) continue;

        void* baseCtrl = *reinterpret_cast<void**>(entryBase + s_entryValueOffset);
        if (!baseCtrl) continue;

        // ── 第2层防御: 验证 baseCtrl 对象可读 ──
        if (!IsReadableMemory(baseCtrl, sizeof(void*))) {
            LOG(LOG_LEVEL_WARN, "[MiniMap] baseCtrl[%d]=%p 内存不可读, 跳过", i, baseCtrl);
            continue;
        }

        const int32_t iconType = get_MiniIconBaseCtrlType(baseCtrl);
        void* actor = s_getActor ? s_getActor(baseCtrl) : nullptr;

        // ── 获取世界坐标 ──
        UnityVector3 worldPos{};
        bool hasWorldPos = getIconWorldPos(baseCtrl, actor, &worldPos);

        // ── 后备: 从 cacheFollowPos 读取缓存坐标 (自身英雄 actor/followObj 可能为空) ──
        if (!hasWorldPos && baseCtrl && s_cacheFollowPosOffset != INVALID_OFFSET) {
            auto* posPtr = reinterpret_cast<UnityVector3*>(
                    reinterpret_cast<uint8_t*>(baseCtrl) + s_cacheFollowPosOffset);
            if (IsReadableMemory(posPtr, sizeof(UnityVector3))) {
                UnityVector3 cached = *posPtr;
                if (cached.x != 0.0f || cached.z != 0.0f) {
                    worldPos = cached;
                    hasWorldPos = true;
                }
            }
        }

        // ── 敌方英雄：收集 HP / 等级 / 坐标 / 名称 ──
        if ((MiniMapIconType)iconType == MiniMapIconType_EnemyTeamHero ||
        (MiniMapIconType)iconType ==MiniMapIconType_MyTeamHero) {
            MiniMapEnemyHeroInfo info{};
            info.iconType    = iconType;

            // ── 名称缓存 (按 resId) —— 名称在对局中不变, 只查询一次 ──
            static std::unordered_map<int32_t, std::string> s_heroNameCache;
            static std::unordered_map<int32_t, std::string> s_summonerNameCache;

            // ── 第2层防御: 验证 actor 可读后再深层访问 ──
            if (actor && IsReadableMemory(actor, sizeof(void*))) {
                auto pAttribute = getActorAttribute(actor);
                if (pAttribute && IsReadableMemory(pAttribute, 0x20)) {
                    info.heroLevel = readEnemyHeroLeve(pAttribute);
                    readEnemyHeroHP(pAttribute, &info.curHp, &info.maxHp);
                }
                info.heroResId = readHeroResId(actor);

                // 英雄名: 优先从缓存获取, 缓存未命中才调用 IL2CPP
                if (info.heroResId > 0) {
                    auto itName = s_heroNameCache.find(info.heroResId);
                    if (itName != s_heroNameCache.end()) {
                        info.heroName = itName->second;
                    } else {
                        info.heroName = readHeroName(actor);
                        if (!info.heroName.empty())
                            s_heroNameCache[info.heroResId] = info.heroName;
                    }

                    // 召唤师名: 同理
                    auto itSummoner = s_summonerNameCache.find(info.heroResId);
                    if (itSummoner != s_summonerNameCache.end()) {
                        info.summonerName = itSummoner->second;
                    } else {
                        info.summonerName = readSummonerName(actor);
                        if (!info.summonerName.empty())
                            s_summonerNameCache[info.heroResId] = info.summonerName;
                    }
                }
            }
            // ── 普攻最大范围 ──
            info.atkRange = -1.0f;
            if (actor && IsReadableMemory(actor, sizeof(void*))) {
                info.atkRange = getNormalAttackMaxRange(actor);
            }

            info.worldPos    = worldPos;
            info.hasWorldPos = hasWorldPos;

            // ── 世界坐标 → 屏幕坐标 (W2S) ──
            info.hasScreenPos = false;
            if (hasWorldPos) {
                float sx = 0.0f, sy = 0.0f;
                if (worldToScreen(worldPos, sx, sy)) {
                    info.screenX      = sx;
                    info.screenY      = sy;
                    info.hasScreenPos = true;
                }
            }

            // (高频采集: 日志由 printMiniMapData 定期输出, 此处不逐帧打印)

            m_miniMapData.enemyHeroes.push_back(info);
            continue;
        }

        // ── 眼/守卫类：收集坐标 ──
        const std::string baseCtrlTypeName = getManagedTypeName(baseCtrl);
        const std::string actorTypeName    = getManagedTypeName(actor);

        if (isWardLikeIcon(iconType, actor, baseCtrlTypeName, actorTypeName)) {
            MiniMapWardInfo ward{};
            ward.worldPos    = worldPos;
            ward.hasWorldPos = hasWorldPos;
            ward.iconType    = iconType;
            m_miniMapData.wards.push_back(ward);
        }
    }

    return pMiniIconsDictionary;
}

// ═══════════════════════════════════════════════════════════════════════════════
// printMiniMapData() — 打印 m_miniMapData 中存储的数据快照（调试用）
// ═══════════════════════════════════════════════════════════════════════════════

void lol::FEVisi::printMiniMapData() const {
    LOG(LOG_LEVEL_INFO, "[ALLRADAR]╔══════════════ MiniMap Data Snapshot ══════════════╗");
    LOG(LOG_LEVEL_INFO, "[ALLRADAR]║               英雄: %zu  眼/守卫: %zu",
        m_miniMapData.enemyHeroes.size(), m_miniMapData.wards.size());
    LOG(LOG_LEVEL_INFO, "[ALLRADAR]╠══════════════ Enemy Heroes ═══════════════════════╣");

    for (size_t i = 0; i < m_miniMapData.enemyHeroes.size(); ++i) {
        const auto& h = m_miniMapData.enemyHeroes[i];
        LOG(LOG_LEVEL_INFO,
            "[ALLRADAR]║ [%zu] type=%d Lv%u  HP=%.1f/%.1f  pos=(%.3f,%.3f,%.3f) valid=%d",
            i, h.iconType, h.heroLevel, h.curHp, h.maxHp,
            h.worldPos.x, h.worldPos.y, h.worldPos.z, h.hasWorldPos);
        LOG(LOG_LEVEL_INFO,
            "[ALLRADAR]║       resId=%d  hero=%s  summoner=%s  atkRange=%.2f",
            h.heroResId, h.heroName.c_str(), h.summonerName.c_str(), h.atkRange);
        LOG(LOG_LEVEL_INFO,
            "[ALLRADAR]║       screen=(%.1f,%.1f) screenValid=%d",
            h.screenX, h.screenY, h.hasScreenPos);
    }

    LOG(LOG_LEVEL_INFO, "[ALLRADAR]╠══════════════ Wards ══════════════════════════════╣");

    for (size_t i = 0; i < m_miniMapData.wards.size(); ++i) {
        const auto& w = m_miniMapData.wards[i];
        LOG(LOG_LEVEL_INFO,
            "[ALLRADAR]║ [%zu] type=%d  pos=(%.3f,%.3f,%.3f) valid=%d",
            i, w.iconType, w.worldPos.x, w.worldPos.y, w.worldPos.z, w.hasWorldPos);
    }

    LOG(LOG_LEVEL_INFO, "[ALLRADAR]╚══════════════════════════════���═══════════════════╝");
}

// ═══════════════════════════════════════════════════════════════════════════════
// IsReadableMemory — 第2层防御: 使用 pipe write 验证内存可读性
//
// 原理: write() 是系统调用，内核在拷贝用户空间数据时会检查页表：
//   - 可读页 → write 成功（返回 size）
//   - 不可读 → write 失败（返回 -1, errno = EFAULT）
//   - 不会触发 SIGSEGV
// ═══════════════════════════════════════════════════════════════════════════════

bool lol::FEVisi::IsReadableMemory(const void* ptr, size_t size) {
    if (ptr == nullptr || size == 0) return false;

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // pipe 创建失败，保守返回 false
        return false;
    }

    // 尝试将 ptr 指向的数据写入 pipe
    // 内核会在 copy_from_user 时验证内存可读性
    ssize_t ret = write(pipefd[1], ptr, size);

    // 立即关闭 pipe（无论成功与否）
    close(pipefd[0]);
    close(pipefd[1]);

    // ret == size → 内存可读; ret == -1 && errno == EFAULT → 内存不可读
    return (ret == (ssize_t)size);
}

// ═══════════════════════════════════════════════════════════════════════════════
// getNormalAttackMaxRange — 获取普通攻击最大施法距离
//
// IDA FE_TN75 (ActorSkill_GetCurRangeSync) 分析结论:
//   native 实例 +0x58 / +0xB8 存储当前范围 (Fix64)
//   托管层对应字段: _maxRange / _configMaxRange
//   优先读取 _maxRange，fallback 读取 _configMaxRange
// ═══════════════════════════════════════════════════════════════════════════════

float lol::FEVisi::getNormalAttackMaxRange(void* actorVisi) {
    if (!actorVisi) return -1.0f;

    auto isReasonableRange = [](float value) -> bool {
        return value >= 0.5f && value <= 30.0f;
    };

    auto tryDecodeFix64Field = [&](void* object, uint32_t offset,
                                   const char* tag, float* outValue) -> bool {
        if (!object || offset == INVALID_OFFSET || outValue == nullptr) return false;
        auto* fieldPtr = reinterpret_cast<uint8_t*>(object) + offset;
        if (!IsReadableMemory(fieldPtr, sizeof(uint64_t))) return false;

        uint64_t raw = *reinterpret_cast<uint64_t*>(fieldPtr);
        if (raw == 0) return false;

        float value = DecoderFix64(raw);
        if (!isReasonableRange(value)) {
            LOG(LOG_LEVEL_WARN,
                "[AtkRange] %s 解码异常: off=0x%X raw=0x%llX -> %.4f",
                tag, offset, (unsigned long long)raw, value);
            return false;
        }

        *outValue = value;
        LOG(LOG_LEVEL_INFO,
            "[AtkRange] %s 命中: off=0x%X raw=0x%llX -> %.4f",
            tag, offset, (unsigned long long)raw, value);
        return true;
    };

    // ── 1. BattleActorVisi → get_actor() → BattleActor ──
    typedef void* (*FnGetActor)(void*);
    static FnGetActor s_getLogicActor = nullptr;
    if (!s_getLogicActor) {
        s_getLogicActor = (FnGetActor)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActorVisi", "FrameEngine.Visual.BattleActorVisi",
                "get_actor");
    }
    if (!s_getLogicActor) return -1.0f;

    void* pBattleActor = s_getLogicActor(actorVisi);
    if (!pBattleActor || !IsReadableMemory(pBattleActor, 0x20)) return -1.0f;

    // ── 2. BattleActor → get_skillMgr() → ActorComponentSkillMgr ──
    typedef void* (*FnGetSkillMgr)(void*);
    static FnGetSkillMgr s_getSkillMgr = nullptr;
    if (!s_getSkillMgr) {
        s_getSkillMgr = (FnGetSkillMgr)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "BattleActor", "FrameEngine.Logic.BattleActor",
                "get_skillMgr");
    }
    if (!s_getSkillMgr) return -1.0f;

    void* pSkillMgr = s_getSkillMgr(pBattleActor);
    if (!pSkillMgr || !IsReadableMemory(pSkillMgr, 0x20)) return -1.0f;

    // ── 3. SkillMgr → GetNormalAttackSkill() / get_attackSkill() ──
    typedef void* (*FnGetActorSkill)(void*);
    static FnGetActorSkill s_getNormalAttackSkill = nullptr;
    static FnGetActorSkill s_getAttackSkill = nullptr;
    static bool s_attackSkillMethodInit = false;
    if (!s_attackSkillMethodInit) {
        s_attackSkillMethodInit = true;
        s_getNormalAttackSkill = (FnGetActorSkill)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "ActorComponentSkillMgr", "FrameEngine.Logic.ActorComponentSkillMgr",
                "GetNormalAttackSkill");
        s_getAttackSkill = (FnGetActorSkill)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "ActorComponentSkillMgr", "FrameEngine.Logic.ActorComponentSkillMgr",
                "get_attackSkill");
        LOG(LOG_LEVEL_INFO,
            "[AtkRange] GetNormalAttackSkill=%p get_attackSkill=%p",
            (void*)s_getNormalAttackSkill, (void*)s_getAttackSkill);
    }

    void* pActorSkill = nullptr;
    if (s_getNormalAttackSkill) {
        pActorSkill = s_getNormalAttackSkill(pSkillMgr);
    }
    if (!pActorSkill && s_getAttackSkill) {
        pActorSkill = s_getAttackSkill(pSkillMgr);
    }
    if (!pActorSkill || !IsReadableMemory(pActorSkill, 0xC0)) return -1.0f;

    // ── 4. 读取 ActorSkill._maxRange / _configMaxRange ──
    static bool s_fieldInit = false;
    static uint32_t s_maxRangeOffset = INVALID_OFFSET;
    static uint32_t s_configMaxRangeOffset = INVALID_OFFSET;
    if (!s_fieldInit) {
        s_fieldInit = true;
        s_maxRangeOffset = GetFieldOffsetFromObject(pActorSkill, "_maxRange");
        if (s_maxRangeOffset == INVALID_OFFSET)
            s_maxRangeOffset = GetFieldOffsetFromObject(pActorSkill, "maxRange");
        s_configMaxRangeOffset = GetFieldOffsetFromObject(pActorSkill, "_configMaxRange");
        if (s_configMaxRangeOffset == INVALID_OFFSET)
            s_configMaxRangeOffset = GetFieldOffsetFromObject(pActorSkill, "configMaxRange");

        LOG(LOG_LEVEL_INFO,
            "[AtkRange] ActorSkill field offsets: _maxRange=0x%X _configMaxRange=0x%X",
            s_maxRangeOffset, s_configMaxRangeOffset);
    }

    // 优先 _maxRange
    float range = -1.0f;
    if (tryDecodeFix64Field(pActorSkill, s_maxRangeOffset, "ActorSkill._maxRange", &range)) {
        return range;
    }

    // fallback _configMaxRange
    if (tryDecodeFix64Field(pActorSkill, s_configMaxRangeOffset, "ActorSkill._configMaxRange", &range)) {
        return range;
    }

    LOG(LOG_LEVEL_WARN, "[AtkRange] _maxRange 和 _configMaxRange 均无法读取");
    return -1.0f;
}

// ═══════════════════════════════════════════════════════════════════════════════
// computeSkillMaxRange — 模拟 IDA sub_55EB554 的技能最大施法距离计算
//
// 原始逻辑:
//   ActorSkill → GetSkillOperateResObject → GetFPTemplate
//   根据 FPTemplate 的 iTPMaxDistanceType(case 1/2/3) 分支计算:
//     case 1: payload=1 → baseRange(invoke_FE_TN75)
//             payload=2 → max(baseRange, globalRange)
//     case 2: MaxDist(sub_88FAE5C) → int / 10000.0
//     case 3: baseRange + MaxDist
//     default: 0
// ═══════════════════════════════════════════════════════════════════════════════

float lol::FEVisi::computeSkillMaxRange(void* pActorSkill) {
    if (!pActorSkill || !IsReadableMemory(pActorSkill, 0xC0)) return -1.0f;

    // ── 1. ActorSkill → GetSkillOperateResObject ──
    typedef void* (*FnGetObj)(void*);
    static FnGetObj s_getSkillOperate = nullptr;
    static bool s_methodInit = false;
    if (!s_methodInit) {
        s_methodInit = true;
        s_getSkillOperate = (FnGetObj)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "ActorSkill", "FrameEngine.Logic.ActorSkill",
                "GetSkillOperateResObject");
    }
    void* pSkillOperate = s_getSkillOperate ? s_getSkillOperate(pActorSkill) : nullptr;
    if (!pSkillOperate || !IsReadableMemory(pSkillOperate, 0x40)) return -1.0f;

    // ── 2. SkillOperateResObject → GetFPTemplate ──
    static FnGetObj s_getFPTemplate = nullptr;
    static bool s_fpInit = false;
    if (!s_fpInit) {
        s_fpInit = true;
        s_getFPTemplate = (FnGetObj)m_pfunctionInfo->GetMethodFun(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "SkillOperateResObject", "FrameEngine.Common.SkillOperateResObject",
                "GetFPTemplate");
    }
    void* pFPTemplate = s_getFPTemplate ? s_getFPTemplate(pSkillOperate) : nullptr;
    if (!pFPTemplate || !IsReadableMemory(pFPTemplate, 0x30)) return -1.0f;

    // ── 3. 获取 FPTemplate 的 native 数据指针 ──
    static uint32_t s_instanceOffset = INVALID_OFFSET;
    static bool s_instanceInit = false;
    if (!s_instanceInit) {
        s_instanceInit = true;
        s_instanceOffset = GetFieldOffsetFromObject(pFPTemplate, "InstanceLongValue");
        if (s_instanceOffset == INVALID_OFFSET)
            s_instanceOffset = GetFieldOffsetFromObject(pFPTemplate, "__InstanceLongValue");
        if (s_instanceOffset == INVALID_OFFSET)
            s_instanceOffset = GetFieldOffsetFromObject(pFPTemplate, "Instance");
        if (s_instanceOffset == INVALID_OFFSET)
            s_instanceOffset = GetFieldOffsetFromObject(pFPTemplate, "__Instance");
        LOG(LOG_LEVEL_INFO, "[SkillRange] FPTemplate Instance offset=0x%X", s_instanceOffset);
    }
    if (s_instanceOffset == INVALID_OFFSET) return -1.0f;

    auto* nativePtrAddr = reinterpret_cast<uint8_t*>(pFPTemplate) + s_instanceOffset;
    if (!IsReadableMemory(nativePtrAddr, sizeof(int64_t))) return -1.0f;
    int64_t fpNative = *reinterpret_cast<int64_t*>(nativePtrAddr);
    if (!fpNative || !IsReadableMemory((void*)fpNative, 0x28)) return -1.0f;

    // ── 4. 从 LayoutDump 获取 native 字段偏移 (或使用硬编码 fallback) ──
    static bool s_nativeOffsetInit = false;
    static uint32_t s_maxDistTypeOff = INVALID_OFFSET;  // iTPMaxDistanceType
    static uint32_t s_maxDistOff     = INVALID_OFFSET;  // iTPMaxDistance

    if (!s_nativeOffsetInit) {
        s_nativeOffsetInit = true;
        auto* layoutDumpClass = m_pfunctionInfo->FindClassByName(
                "ilbil2cpp.so", "Assembly-CSharp.dll",
                "LayoutDump", "FrameEngine.Common.LayoutDump");
        if (layoutDumpClass) {
            m_pfunctionInfo->il2cpp_runtime_class_init(layoutDumpClass);
            int32_t typeOff = -1, distOff = -1;
            if (ReadStaticFieldInt32(layoutDumpClass,
                    "FrameEngine_Common_FPTemplateResObject_iTPMaxDistanceType_offset",
                    typeOff) && typeOff >= 0)
                s_maxDistTypeOff = (uint32_t)typeOff;
            if (ReadStaticFieldInt32(layoutDumpClass,
                    "FrameEngine_Common_FPTemplateResObject_iTPMaxDistance_offset",
                    distOff) && distOff >= 0)
                s_maxDistOff = (uint32_t)distOff;
        }
        LOG(LOG_LEVEL_INFO, "[SkillRange] LayoutDump offsets: type=0x%X dist=0x%X",
            s_maxDistTypeOff, s_maxDistOff);
    }

    // sub_88FB1CC: FPTemplate → native+0x20 → iTPMaxDistanceType
    uint32_t typeOff = s_maxDistTypeOff != INVALID_OFFSET ? s_maxDistTypeOff : 0x20;
    // sub_88FAF04/sub_88FAE5C: FPTemplate → native+0x24 → iTPMaxDistance
    uint32_t distOff = s_maxDistOff != INVALID_OFFSET ? s_maxDistOff : 0x24;

    int32_t maxDistType = *reinterpret_cast<int32_t*>(fpNative + typeOff);
    int32_t maxDistRaw  = *reinterpret_cast<int32_t*>(fpNative + distOff);
    float maxDist = (float)maxDistRaw / 10000.0f;

    // ── 5. 获取 baseRange (invoke_FE_TN75 → ActorSkill_GetCurRangeSync) ──
    //    读取 ActorSkill._maxRange 或 _configMaxRange (Fix64)
    static uint32_t s_maxRangeOff = INVALID_OFFSET;
    static uint32_t s_configMaxRangeOff = INVALID_OFFSET;
    static bool s_rangeFieldInit = false;
    if (!s_rangeFieldInit) {
        s_rangeFieldInit = true;
        s_maxRangeOff = GetFieldOffsetFromObject(pActorSkill, "_maxRange");
        if (s_maxRangeOff == INVALID_OFFSET)
            s_maxRangeOff = GetFieldOffsetFromObject(pActorSkill, "maxRange");
        s_configMaxRangeOff = GetFieldOffsetFromObject(pActorSkill, "_configMaxRange");
        if (s_configMaxRangeOff == INVALID_OFFSET)
            s_configMaxRangeOff = GetFieldOffsetFromObject(pActorSkill, "configMaxRange");
    }

    auto readFix64AsFloat = [&](void* obj, uint32_t offset) -> float {
        if (!obj || offset == INVALID_OFFSET) return -1.0f;
        auto* ptr = reinterpret_cast<uint8_t*>(obj) + offset;
        if (!IsReadableMemory(ptr, sizeof(uint64_t))) return -1.0f;
        uint64_t raw = *reinterpret_cast<uint64_t*>(ptr);
        if (raw == 0) return -1.0f;
        return DecoderFix64(raw);
    };

    float baseRange = readFix64AsFloat(pActorSkill, s_maxRangeOff);
    if (baseRange < 0.0f)
        baseRange = readFix64AsFloat(pActorSkill, s_configMaxRangeOff);

    // ── 6. 按 iTPMaxDistanceType 分支计算 (模拟 sub_55EB554 switch) ──
    float result = -1.0f;

    switch (maxDistType) {
        case 1: {
            // payload (iTPMaxDistance) 决定模式
            if (maxDistRaw == 2) {
                // case 1, payload=2: max(baseRange, globalRange)
                // globalRange 来自 sub_A49CDE8 (Fix64 静态值)
                float globalRange = -1.0f;
                typedef int64_t (*FnGetGlobalRange)();
                static FnGetGlobalRange s_getGlobalRange = nullptr;
                if (!s_getGlobalRange)
                    s_getGlobalRange = (FnGetGlobalRange)m_pfunctionInfo->GetMethodFun(
                            "ilbil2cpp.so", "Assembly-CSharp.dll",
                            "ActorSkill", "FrameEngine.Logic.ActorSkill",
                            "get_globalNormalAtkRange");
                if (s_getGlobalRange) {
                    int64_t globalRaw = s_getGlobalRange();
                    if (globalRaw != 0) globalRange = DecoderFix64((uint64_t)globalRaw);
                }
                if (baseRange >= 0.0f && globalRange >= 0.0f)
                    result = std::max(baseRange, globalRange);
                else if (baseRange >= 0.0f)
                    result = baseRange;
                else if (globalRange >= 0.0f)
                    result = globalRange;
            } else if (maxDistRaw == 1) {
                // case 1, payload=1: 直接使用 baseRange
                result = baseRange;
            }
            break;
        }
        case 2:
            // case 2: MaxDist / 10000.0
            result = maxDist;
            break;
        case 3:
            // case 3: baseRange + MaxDist / 10000.0
            if (baseRange >= 0.0f)
                result = baseRange + maxDist;
            break;
        default:
            break;
    }

    LOG(LOG_LEVEL_INFO,
        "[SkillRange] type=%d payload=%d maxDist=%.4f baseRange=%.4f → result=%.4f",
        maxDistType, maxDistRaw, maxDist, baseRange, result);

    return result;
}

// ═══════════════════════════════════════════════════════════════════════════════
// readIl2CppString — 将 IL2CPP 托管 System.String 对象转为 C++ std::string (UTF-8)
// ═══════════════════════════════════════════════════════════════════════════════

std::string lol::FEVisi::readIl2CppString(void* pIl2CppString) {
    if (pIl2CppString == nullptr) return "";
    if (!IsReadableMemory(pIl2CppString, sizeof(void*))) {
        LOG(LOG_LEVEL_WARN, "[readIl2CppString] 对象指针 %p 内存不可读", pIl2CppString);
        return "";
    }
    return m_pfunctionInfo->il2cpp_Il2CppString_toCString(
            static_cast<const Il2CppString*>(pIl2CppString));
}