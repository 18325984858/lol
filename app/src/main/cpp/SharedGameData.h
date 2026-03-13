/**
 * @file    SharedGameData.h
 * @brief   线程安全的游戏数据桥接器 —— 在数据采集线程和 ImGui 渲染线程之间共享数据
 * @author  Song
 * @date    2026/03/12
 *
 * @details 单例模式。数据���集线程调用 pushData() 写入最新快照，
 *          ImGui 渲染线程调用 pullData() 获取最新快照并绘制覆盖层。
 */

#ifndef SHARED_GAME_DATA_H
#define SHARED_GAME_DATA_H

#include <mutex>
#include <atomic>
#include "lol/lolm.h"

class SharedGameData {
public:
    static SharedGameData& getInstance() {
        static SharedGameData instance;
        return instance;
    }

    SharedGameData(const SharedGameData&) = delete;
    SharedGameData& operator=(const SharedGameData&) = delete;

    /**
     * @brief 由数据采集线程调用 —— 写入最新的小地图数据快照
     * @param data 最新的 MiniMapData 快照
     */
    void pushData(const lol::MiniMapData& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
        m_hasNewData.store(true, std::memory_order_release);
    }

    /**
     * @brief 由 ImGui 渲染线程调用 —— 拉取最新数据（消费型：拉取后标记为已读）
     * @param outData [传出] 最新的 MiniMapData
     * @return true=有新数据, false=��更新
     */
    bool pullData(lol::MiniMapData& outData) {
        if (!m_hasNewData.load(std::memory_order_acquire))
            return false;
        std::lock_guard<std::mutex> lock(m_mutex);
        outData = m_data;
        m_hasNewData.store(false, std::memory_order_release);
        return true;
    }

    /** @brief 获取最新数据（非消费型：不修改 hasNewData 标志） */
    lol::MiniMapData getLatestData() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

    /** @brief 设置对局是否进行中 */
    void setBattleActive(bool active) {
        m_battleActive.store(active, std::memory_order_release);
    }

    /** @brief 查询对局是否进行中 */
    bool isBattleActive() const {
        return m_battleActive.load(std::memory_order_acquire);
    }

private:
    SharedGameData() = default;
    ~SharedGameData() = default;

    std::mutex m_mutex;
    lol::MiniMapData m_data;
    std::atomic<bool> m_hasNewData{false};
    std::atomic<bool> m_battleActive{false};
};

#endif // SHARED_GAME_DATA_H



