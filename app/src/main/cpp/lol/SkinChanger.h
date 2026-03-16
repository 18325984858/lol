#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include <cstdint>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
#include "../Dobby/include/dobby.h"
#include "../Log/log.h"

/**
 * @class SkinChanger
 * @brief 战斗内实时换肤 —— Hook GetHeroSkinPrefab 替换 skinId
 *
 * @details
 *   Hook 目标: BattleActorVisi::GetHeroSkinPrefab(BattleActor actor, int skinId)
 *   RVA: 0x4F6E684
 *
 *   原理: 当游戏加载英雄模型时调用此函数获取 prefab 路径，
 *         我们拦截并将 skinId 替换为目标皮肤ID。
 *
 *   仅替换自己英雄（通过 IsMyHero 判断），仅客户端可见。
 */

struct SkinInfo {
    int    skinId;      // SkinAssetObject.resID
    std::string name;   // SkinResObject.szName
};

class SkinChanger {
public:
    static SkinChanger& getInstance() {
        static SkinChanger instance;
        return instance;
    }

    SkinChanger(const SkinChanger&) = delete;
    SkinChanger& operator=(const SkinChanger&) = delete;

    /**
     * @brief 安装 DobbyHook
     * @param il2cppBase  libil2cpp.so 基地址
     * @return true=安装成功
     */
    bool install(void* il2cppBase);

    /** @brief 设置目标皮肤ID（-1 表示不修改/使用原始皮肤） */
    void setTargetSkinId(int skinId) {
        LOG(LOG_LEVEL_INFO, "[SkinChanger] setTargetSkinId: %d → %d", m_targetSkinId.load(), skinId);
        m_targetSkinId.store(skinId, std::memory_order_release);
    }

    /** @brief 获取当前目标皮肤ID */
    int getTargetSkinId() const { return m_targetSkinId.load(std::memory_order_acquire); }

    /** @brief 是否已安装 hook */
    bool isInstalled() const { return m_installed.load(std::memory_order_acquire); }

    /** @brief 开启/关闭换肤 */
    void setEnabled(bool enabled) {
        LOG(LOG_LEVEL_INFO, "[SkinChanger] setEnabled: %d → %d", m_enabled.load(), enabled);
        m_enabled.store(enabled, std::memory_order_release);
    }

    /** @brief 是否开启换肤 */
    bool isEnabled() const { return m_enabled.load(std::memory_order_acquire); }

    /** @brief 获取当前英雄的皮肤列表 (线程安全) */
    std::vector<SkinInfo> getSkinList() const {
        std::lock_guard<std::mutex> lock(m_skinListMutex);
        return m_skinList;
    }

    /** @brief 皮肤列表是否已采集 */
    bool hasSkinList() const { return m_skinListReady.load(std::memory_order_acquire); }

    /**
     * @brief 应用换肤: 写入 _skinID 字段并调用 ReloadModelPrefab 触发模型重载
     * @param skinId  目标皮肤ID, -1 恢复原始皮肤
     */
    void applySkin(int skinId);

private:
    SkinChanger() = default;
    ~SkinChanger() = default;

    // Hook 函数签名: void* GetHeroSkinPrefab(void* this, void* actor, int skinId)
    using GetHeroSkinPrefab_t = void* (*)(void*, void*, int);

    static void* hooked_GetHeroSkinPrefab(void* thisPtr, void* actor, int skinId);

    /** @brief 从 BattleActorVisi 的 visiResHero->skins[] 遍历皮肤列表 */
    static void collectSkinsFromHero(void* actorVisiPtr);

    /** @brief 检查内存是否可读 (pipe write 方式) */
    static bool isReadableMemory(const void* ptr, size_t size = sizeof(void*));

    /** @brief 读取 Il2CppString 为 UTF-8 std::string */
    static std::string readIl2CppString(void* pStr);

    static GetHeroSkinPrefab_t s_origGetHeroSkinPrefab;
    static void* s_il2cppBase;

    std::atomic<int>  m_targetSkinId{-1};
    std::atomic<bool> m_installed{false};
    std::atomic<bool> m_enabled{false};

    // 皮肤列表
    mutable std::mutex          m_skinListMutex;
    std::vector<SkinInfo>       m_skinList;
    std::atomic<bool>           m_skinListReady{false};

    // 缓存自己英雄的 BattleActorVisi* 和原始 skinID
    std::atomic<uintptr_t> m_myHeroPtr{0};
    int                    m_originalSkinId{-1};

    // RVAs
    static constexpr uintptr_t RVA_GetHeroSkinPrefab   = 0x4F6E684;
    static constexpr uintptr_t RVA_IsMyHero            = 0x4F6D908;
    static constexpr uintptr_t RVA_ReloadModelPrefab   = 0x4F794D8;
    static constexpr uintptr_t RVA_LoadMonoAgentAsset   = 0x4F775BC;

    // BattleActorVisi field offsets
    static constexpr uintptr_t OFF_visiResHero   = 0xC0;   // ChampionResObject*
    static constexpr uintptr_t OFF_skinID        = 0x154;   // int32_t _skinID

    // ChampionResObject (ISeria_ConfigVisi) field offsets
    static constexpr uintptr_t OFF_CR_skins      = 0x88;   // SkinAssetObject[] (managed array)

    // SkinAssetObject field offsets
    static constexpr uintptr_t OFF_SA_resID      = 0x10;   // int resID
    static constexpr uintptr_t OFF_SA_res        = 0x18;   // SkinResObject*

    // SkinResObject field offsets
    static constexpr uintptr_t OFF_SR_szName     = 0x38;   // string szName

    // Il2CppObject layout: klass(8) + monitor(8) = 16 bytes header
    // Managed array: Il2CppObject(16) + bounds(8) + max_length(8) + elements...
    static constexpr size_t    ARRAY_HEADER_SIZE = 32;      // 16 + 8 + 8
};

#endif // SKIN_CHANGER_H
