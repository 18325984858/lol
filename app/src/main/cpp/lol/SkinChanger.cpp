#include "SkinChanger.h"
#include <unistd.h>
#include <codecvt>
#include <locale>

// 静态成员初始化
SkinChanger::GetHeroSkinPrefab_t SkinChanger::s_origGetHeroSkinPrefab = nullptr;
void* SkinChanger::s_il2cppBase = nullptr;

// ═══════════════════════════════════════════════════════════════════════════════
// 内存安全检查 (pipe write 方式，不触发 SIGSEGV)
// ═══════════════════════════════════════════════════════════════════════════════

bool SkinChanger::isReadableMemory(const void* ptr, size_t size) {
    if (!ptr || size == 0) return false;
    int pipefd[2];
    if (pipe(pipefd) != 0) return false;
    ssize_t ret = write(pipefd[1], ptr, size);
    close(pipefd[0]);
    close(pipefd[1]);
    return (ret == (ssize_t)size);
}

// ═══════════════════════════════════════════════════════════════════════════════
// 读取 Il2CppString → UTF-8 std::string
// Il2CppString 布局: Il2CppObject(16) + int32_t length(4) + Il2CppChar chars[]
// Il2CppChar = uint16_t (UTF-16LE)
// ═══════════════════════════════════════════════════════════════════════════════

std::string SkinChanger::readIl2CppString(void* pStr) {
    if (!pStr || !isReadableMemory(pStr, 24)) return "";
    int32_t len = *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(pStr) + 16);
    if (len <= 0 || len > 512) return "";
    const uint16_t* chars = reinterpret_cast<const uint16_t*>(
        reinterpret_cast<uintptr_t>(pStr) + 20);
    if (!isReadableMemory(chars, len * sizeof(uint16_t))) return "";
    // UTF-16LE → UTF-8
    std::string result;
    result.reserve(len * 3);
    for (int32_t i = 0; i < len; i++) {
        uint32_t cp = chars[i];
        // Handle surrogate pairs
        if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < len) {
            uint32_t lo = chars[i + 1];
            if (lo >= 0xDC00 && lo <= 0xDFFF) {
                cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                i++;
            }
        }
        if (cp < 0x80) {
            result += (char)cp;
        } else if (cp < 0x800) {
            result += (char)(0xC0 | (cp >> 6));
            result += (char)(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            result += (char)(0xE0 | (cp >> 12));
            result += (char)(0x80 | ((cp >> 6) & 0x3F));
            result += (char)(0x80 | (cp & 0x3F));
        } else {
            result += (char)(0xF0 | (cp >> 18));
            result += (char)(0x80 | ((cp >> 12) & 0x3F));
            result += (char)(0x80 | ((cp >> 6) & 0x3F));
            result += (char)(0x80 | (cp & 0x3F));
        }
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════════
// collectSkinsFromHero — 从 BattleActorVisi 遍历英雄皮肤列表
//
// 内存链: BattleActorVisi
//   +0xC0 → ChampionResObject* visiResHero
//     +0x88 → SkinAssetObject[] skins  (C# managed array)
//       array header: Il2CppObject(16) + bounds*(8) + max_length(8) = 32 bytes
//       elements: SkinAssetObject* pointers
//         +0x10 → int resID (皮肤ID)
//         +0x18 → SkinResObject* _res
//           +0x38 → string szName (Il2CppString*)
// ═══════════════════════════════════════════════════════════════════════════════

void SkinChanger::collectSkinsFromHero(void* actorVisiPtr) {
    auto& inst = getInstance();
    if (inst.m_skinListReady.load(std::memory_order_acquire)) return;

    if (!actorVisiPtr || !isReadableMemory(actorVisiPtr, OFF_visiResHero + 8)) return;

    // 读 visiResHero (ChampionResObject*)
    void* champRes = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(actorVisiPtr) + OFF_visiResHero);
    if (!champRes || !isReadableMemory(champRes, OFF_CR_skins + 8)) {
        LOG(LOG_LEVEL_WARN, "[SkinChanger] visiResHero 不可读 (%p)", champRes);
        return;
    }

    // 读 skins (SkinAssetObject[] — C# managed array pointer)
    void* skinsArray = *reinterpret_cast<void**>(
        reinterpret_cast<uintptr_t>(champRes) + OFF_CR_skins);
    if (!skinsArray || !isReadableMemory(skinsArray, ARRAY_HEADER_SIZE)) {
        LOG(LOG_LEVEL_WARN, "[SkinChanger] skins 数组不可读 (%p)", skinsArray);
        return;
    }

    // 读 max_length (偏移 16+8=24, 即 Il2CppObject + bounds* 之后)
    uintptr_t arrayBase = reinterpret_cast<uintptr_t>(skinsArray);
    uintptr_t maxLen = *reinterpret_cast<uintptr_t*>(arrayBase + 24);
    if (maxLen == 0 || maxLen > 200) {
        LOG(LOG_LEVEL_WARN, "[SkinChanger] skins 数量异常: %zu", (size_t)maxLen);
        return;
    }

    // 元素起始位置: ARRAY_HEADER_SIZE (32) 之后
    void** elements = reinterpret_cast<void**>(arrayBase + ARRAY_HEADER_SIZE);
    if (!isReadableMemory(elements, maxLen * sizeof(void*))) {
        LOG(LOG_LEVEL_WARN, "[SkinChanger] skins 元素数组不可读");
        return;
    }

    std::vector<SkinInfo> skinList;
    skinList.reserve(maxLen);

    for (uintptr_t i = 0; i < maxLen; i++) {
        void* skinAsset = elements[i];
        if (!skinAsset || !isReadableMemory(skinAsset, OFF_SA_res + 8)) continue;

        // 读 resID
        int32_t resID = *reinterpret_cast<int32_t*>(
            reinterpret_cast<uintptr_t>(skinAsset) + OFF_SA_resID);
        if (resID <= 0) continue;

        // 读皮肤名
        std::string skinName;
        void* skinRes = *reinterpret_cast<void**>(
            reinterpret_cast<uintptr_t>(skinAsset) + OFF_SA_res);
        if (skinRes && isReadableMemory(skinRes, OFF_SR_szName + 8)) {
            void* pNameStr = *reinterpret_cast<void**>(
                reinterpret_cast<uintptr_t>(skinRes) + OFF_SR_szName);
            skinName = readIl2CppString(pNameStr);
        }

        if (skinName.empty()) {
            char buf[32];
            snprintf(buf, sizeof(buf), "Skin %d", resID);
            skinName = buf;
        }

        skinList.push_back({resID, skinName});
        LOG(LOG_LEVEL_INFO, "[SkinChanger] 发现皮肤: ID=%d  名称=%s", resID, skinName.c_str());
    }

    if (!skinList.empty()) {
        std::lock_guard<std::mutex> lock(inst.m_skinListMutex);
        inst.m_skinList = std::move(skinList);
        inst.m_skinListReady.store(true, std::memory_order_release);
        LOG(LOG_LEVEL_INFO, "[SkinChanger] 皮肤列表采集完成，共 %d 个皮肤",
            (int)inst.m_skinList.size());
    }
}

bool SkinChanger::install(void* il2cppBase) {
    if (m_installed.load()) return true;
    if (!il2cppBase) {
        LOG(LOG_LEVEL_ERROR, "[SkinChanger] il2cppBase is null");
        return false;
    }

    s_il2cppBase = il2cppBase;

    void* targetAddr = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(il2cppBase) + RVA_GetHeroSkinPrefab
    );

    int ret = DobbyHook(
        targetAddr,
        reinterpret_cast<dobby_dummy_func_t>(hooked_GetHeroSkinPrefab),
        reinterpret_cast<dobby_dummy_func_t*>(&s_origGetHeroSkinPrefab)
    );

    if (ret != 0) {
        LOG(LOG_LEVEL_ERROR, "[SkinChanger] DobbyHook failed (ret=%d)", ret);
        return false;
    }

    m_installed.store(true, std::memory_order_release);
    LOG(LOG_LEVEL_INFO, "[SkinChanger] Hook installed at %p", targetAddr);
    return true;
}

void* SkinChanger::hooked_GetHeroSkinPrefab(void* thisPtr, void* actor, int skinId) {
    auto& inst = getInstance();

    // 每次调用都记录
    static int s_callCount = 0;
    s_callCount++;
    LOG(LOG_LEVEL_INFO, "[SkinChanger] Hook #%d thisPtr=%p skinId=%d enabled=%d target=%d",
        s_callCount, thisPtr, skinId, inst.isEnabled(), inst.getTargetSkinId());

    // 判断是否为自己的英雄
    using IsMyHero_t = bool (*)(void*);
    auto IsMyHero = reinterpret_cast<IsMyHero_t>(
        reinterpret_cast<uintptr_t>(s_il2cppBase) + RVA_IsMyHero
    );

    if (IsMyHero(thisPtr)) {
        // 保存英雄指针和原始skinID
        if (inst.m_myHeroPtr.load(std::memory_order_acquire) == 0) {
            inst.m_myHeroPtr.store(reinterpret_cast<uintptr_t>(thisPtr), std::memory_order_release);
            inst.m_originalSkinId = skinId;
            LOG(LOG_LEVEL_INFO, "[SkinChanger] 缓存我方英雄: ptr=%p origSkinId=%d", thisPtr, skinId);
        }

        // 首次遇到自己英雄时采集皮肤列表
        if (!inst.m_skinListReady.load(std::memory_order_acquire)) {
            collectSkinsFromHero(thisPtr);
        }

        // 应用换肤
        if (inst.isEnabled() && inst.getTargetSkinId() > 0) {
            int newSkinId = inst.getTargetSkinId();
            LOG(LOG_LEVEL_INFO, "[SkinChanger] ★ 替换皮肤: %d → %d", skinId, newSkinId);
            skinId = newSkinId;
        }
    }

    return s_origGetHeroSkinPrefab(thisPtr, actor, skinId);
}

// ═══════════════════════════════════════════════════════════════════════════════
// applySkin — 主动触发换肤 (UI 调用)
//
// 原理: GetHeroSkinPrefab 只在模型加载时调用一次。
//       要中途换肤，需要:
//       1) 修改 _skinID 字段 (offset 0x154)
//       2) 调用 ReloadModelPrefab() 触发模型重新加载
//       ReloadModelPrefab → LoadMonoAgentAsset → GetHeroSkinPrefab(skinId)
//       此时 hook 会拦截并替换 skinId
// ═══════════════════════════════════════════════════════════════════════════════

void SkinChanger::applySkin(int skinId) {
    LOG(LOG_LEVEL_INFO, "[SkinChanger] applySkin(%d) 开始", skinId);

    if (!m_installed.load(std::memory_order_acquire)) {
        LOG(LOG_LEVEL_ERROR, "[SkinChanger] applySkin: Hook 未安装");
        return;
    }

    uintptr_t heroPtr = m_myHeroPtr.load(std::memory_order_acquire);
    if (heroPtr == 0) {
        LOG(LOG_LEVEL_ERROR, "[SkinChanger] applySkin: 英雄指针未缓存，需要先进入战斗");
        return;
    }

    if (!isReadableMemory(reinterpret_cast<void*>(heroPtr), OFF_skinID + 4)) {
        LOG(LOG_LEVEL_ERROR, "[SkinChanger] applySkin: 英雄指针 %p 不可读", (void*)heroPtr);
        m_myHeroPtr.store(0, std::memory_order_release);
        return;
    }

    // 设置目标
    if (skinId > 0) {
        m_targetSkinId.store(skinId, std::memory_order_release);
        m_enabled.store(true, std::memory_order_release);
    } else {
        // 恢复原始皮肤
        skinId = m_originalSkinId;
        m_targetSkinId.store(-1, std::memory_order_release);
        m_enabled.store(false, std::memory_order_release);
    }

    // 写入 _skinID 字段
    int32_t oldSkinId = *reinterpret_cast<int32_t*>(heroPtr + OFF_skinID);
    *reinterpret_cast<int32_t*>(heroPtr + OFF_skinID) = skinId;
    LOG(LOG_LEVEL_INFO, "[SkinChanger] 写入 _skinID: %d → %d (ptr=%p+0x154)",
        oldSkinId, skinId, (void*)heroPtr);

    // 调用 LoadMonoAgentAsset 直接触发模型重新加载
    // (不用 ReloadModelPrefab，因为它有 CanShowHeadIconWhenModelEmpty 守卫条件，
    //  当模型已存在时返回 false 导致不会重载)
    using LoadMonoAgentAsset_t = void (*)(void*);
    auto LoadMonoAgentAsset = reinterpret_cast<LoadMonoAgentAsset_t>(
        reinterpret_cast<uintptr_t>(s_il2cppBase) + RVA_LoadMonoAgentAsset
    );
    LOG(LOG_LEVEL_INFO, "[SkinChanger] 调用 LoadMonoAgentAsset(%p)...", (void*)heroPtr);
    LoadMonoAgentAsset(reinterpret_cast<void*>(heroPtr));
    LOG(LOG_LEVEL_INFO, "[SkinChanger] applySkin(%d) 完成", skinId);
}
