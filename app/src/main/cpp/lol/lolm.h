/**
 * @file    lolm.h
 * @brief   LOL 手游（英雄联盟手游）专用业务逻辑层 —— 字符串解密、对局状态查询等
 * @author  Song
 * @date    2026/01/04
 * @update  2026/03/05
 *
 * @details 包含 LOL 手游特定的字符串解密表（byte_B9BBF1B）、加密字符串结构（LolStrStruct）、
 *          对局状态查询类（FEVisi）以及综合业务类（lol）。lol 类提供字符串解密、
 *          元数据字符串索引表初始化等功能。
 */

#ifndef DOBBY_PROJECT_LOLM_H
#define DOBBY_PROJECT_LOLM_H

#include <iostream>
#include <stdint.h>
#include <map>
#include <vector>
#include <cstring>
#include <sstream>
#include <utility>
#include <type_traits>
#include <typeinfo>
#include "LolHeader.h"
#include "../interface/interface.h"
#include "../Log/log.h"

struct Il2CppGlobalMetadataHeader;

namespace lol {

    /** @brief Unity 世界坐标 (float x, y, z) */
    struct UnityVector3 {
        float x;
        float y;
        float z;
    };

    /** @brief 屏幕坐标点（Unity 屏幕空间，原点左下） */
    struct ScreenPoint {
        float x;
        float y;
    };

    /** @brief 敌方英雄信息 */
    struct MiniMapEnemyHeroInfo {
        int32_t     iconType;       ///< 图标类型 (MiniMapIconType)
        uint32_t    heroLevel;      ///< 英雄等级
        float       curHp;          ///< 当前血量
        float       maxHp;          ///< 最大血量
        UnityVector3 worldPos;      ///< 世界坐标
        bool        hasWorldPos;    ///< 坐标是否有效
        int32_t     heroResId;      ///< 英雄配置 ID（_resId）
        uint32_t    objId;          ///< 当前英雄实例对象 ID
        std::string heroName;       ///< 英雄名称（从 BattleActor_DC.name 读取）
        std::string summonerName;   ///< 召唤师名（BattlePlayer.get_name）
        float       screenX;        ///< 屏幕坐标 X（Unity 屏幕空间，原点左下）
        float       screenY;        ///< 屏幕坐标 Y（Unity 屏幕空间，原点左下）
        bool        hasScreenPos;   ///< 屏幕坐标是否有效
        float       atkRange;       ///< 普攻最大范围（Fix64 转 float），-1 表示无效
    };

    /** @brief 眼/守卫信息 */
    struct MiniMapWardInfo {
        UnityVector3 worldPos;      ///< 世界坐标
        bool         hasWorldPos;   ///< 坐标是否有效
        int32_t      iconType;      ///< 图标类型
        float        screenX;       ///< 屏幕坐标 X
        float        screenY;       ///< 屏幕坐标 Y
        bool         hasScreenPos;  ///< 屏幕坐标是否有效
    };

    /** @brief 小兵信息 */
    struct MiniMapMinionInfo {
        UnityVector3 worldPos;      ///< 世界坐标
        bool         hasWorldPos;   ///< 坐标是否有效
        float        curHp;         ///< 当前血量
        float        maxHp;         ///< 最大血量
        float        screenX;       ///< 屏幕坐标 X
        float        screenY;       ///< 屏幕坐标 Y
        bool         hasScreenPos;  ///< 屏幕坐标是否有效
        uint32_t     objId;         ///< 目标对象 ID
        int32_t      camp;          ///< 所属阵营
        bool         isEnemy;       ///< 是否为敌方小兵
    };

    /** @brief 小地图一次遍历的全部数据快照 */
    struct MiniMapData {
        std::vector<MiniMapEnemyHeroInfo> enemyHeroes;  ///< 敌方英雄列表
        std::vector<MiniMapWardInfo>      wards;        ///< 眼/守卫列表
        std::vector<MiniMapMinionInfo>    minions;      ///< 小兵列表

        // 己方英雄当前技能有效范围
        float       mySkillRange;    ///< 当前技能有效范围(float)，-1 无效
        UnityVector3 myWorldPos;     ///< 己方英雄世界坐标
        float       myScreenX;       ///< 己方屏幕 X
        float       myScreenY;       ///< 己方屏幕 Y
        bool        hasMyScreenPos;  ///< 己方屏幕坐标是否有效
        int32_t     myCamp;          ///< 己方阵营，-1 无效
        std::vector<ScreenPoint> mySkillRangeScreenPoints; ///< 己方攻击范围投影轮廓

        void clear() {
            enemyHeroes.clear();
            wards.clear();
            minions.clear();
            mySkillRange = -1.0f;
            myWorldPos = {0,0,0};
            myScreenX = 0; myScreenY = 0;
            hasMyScreenPos = false;
            myCamp = -1;
            mySkillRangeScreenPoints.clear();
        }
    };

    /** @brief DataShellList 遍历后的一项结果数据 */
    struct DataShellListEntry {
        uint32_t index = 0;            ///< 元素下标
        std::string listTypeName;      ///< DataShellList 运行时类型名
        std::string itemTypeName;      ///< 元素类型名
        std::string valueText;         ///< 元素的字符串表示
        std::vector<uint8_t> rawBytes; ///< 元素原始字节
        void* objectPtr = nullptr;     ///< 元素对象指针（仅引用类型有效）
    };

    /** @brief FixVector2_Fix64 原始数据 */
    struct FixVector2Raw {
        int64_t x = 0;
        int64_t y = 0;
    };

    /** @brief 技能弹道数据快照 */
    struct BulletInfo {
        uint32_t objId = 0;
        int32_t resId = 0;
        int32_t trajectoryType = -1;
        int32_t bulletTargetType = -1;
        bool isFinish = false;
        float currentSpeed = -1.0f;
        float distanceLimit = -1.0f;
        float speedScale = -1.0f;
        UnityVector3 initialPoint{};
        UnityVector3 startMovePosition{};
        UnityVector3 targetPosition{};
        UnityVector3 lastTickPosition{};
        UnityVector3 velocity{};
        void* caster = nullptr;
        void* targetActor = nullptr;
        int32_t targetActorOffset = -1;
        std::string bulletTypeName;
    };

    /** @brief 碰撞多边形数据 */
    struct TerrainPolygon {
        std::string polygonType;
        std::vector<UnityVector3> points;
    };

    /** @brief 地形碰撞快照 */
    struct TerrainData {
        std::vector<TerrainPolygon> obstaclePolygons;
        std::vector<TerrainPolygon> grassPolygons;
        TerrainPolygon boundPolygon;

        void clear() {
            obstaclePolygons.clear();
            grassPolygons.clear();
            boundPolygon.polygonType.clear();
            boundPolygon.points.clear();
        }
    };

    /**
     * @struct  LolStrStruct
     * @brief   加密字符串缓存结构 —— 存储解密状态和原始字符串
     */
    typedef struct LolStrStruct{
        uint32_t isDecryption;  ///< 解密标志（0=未解密, 非0=已解密）
        std::string srcStr;     ///< 原始/解密后的字符串内容
    }LolStrStruct,*PLolStrStruct;

    /**
     * @class FEVisi
     * @brief 对局状态查询类 —— 提供战斗开始状态和玩家信息的查询接口
     *
     * @details 内部持有 fun::function 实例，通过 IL2CPP 运行时 API 查询游戏对局状态。
     */
    class FEVisi{


    public:
        /**
         * @brief 构造函数 —— 初始化 il2cpp 基地址和核心元数据指针
         * @param dqil2cppBase              il2cpp 模块基地址
         * @param pCodeRegistration         代码注册表指针
         * @param pMetadataRegistration     元数据注册表指针
         * @param pGlobalMetadataHeader     全局元数据头指针
         * @param pMetadataImagesTable      元数据镜像表指针
         */
        FEVisi(void* dqil2cppBase=nullptr,
               void *pCodeRegistration=nullptr,
               void *pMetadataRegistration=nullptr,
               void *pGlobalMetadataHeader=nullptr,
               void* pMetadataImagesTable=nullptr);

        /** @brief 析构函数 */
        ~FEVisi();

    public:
        /** @brief 遍历小地图图标，收集敌方英雄/眼位数据到 m_miniMapData */
        void* updateMiniMapData();

        /** @brief 遍历 miniIcons 字典，收集小兵数据到 m_miniMapData.minions */
        void updateMinionData();

        /** @brief 遍历当前战斗中的技能弹道，收集到 m_bulletData */
        void updateBulletData();

        /** @brief 遍历当前战斗地图的地形碰撞多边形，收集到 m_terrainData */
        bool updateTerrainData();

            /**
             * @brief  遍历 DataShellList，并将每个元素的数据与类型信息保存到动态数组中
             * @tparam  T  DataShellList 元素类型；必须与目标 list 的真实元素类型一致
             * @param   pDataShellList  DataShellList 实例指针
             * @return  遍历结果动态数组，失败返回空 vector
             */
            template<typename T>
            std::vector<DataShellListEntry> collectDataShellList(void* pDataShellList) {
                std::vector<DataShellListEntry> result;
                if (pDataShellList == nullptr || m_pfunctionInfo == nullptr) {
                    LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: input=%p functionInfo=%p", pDataShellList, m_pfunctionInfo.get());
                    return result;
                }

                auto* listKlass = GetObjectKlass(pDataShellList);
                if (listKlass == nullptr) {
                    LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: GetObjectKlass failed, list=%p", pDataShellList);
                    return result;
                }

                LOG(LOG_LEVEL_INFO, "[Bullet] collectDataShellList: list=%p klass=%p", pDataShellList, listKlass);

                const uint64_t getCountAddr = m_pfunctionInfo->GetMethodFunByClass(listKlass, "get_Count", 0);
                const uint64_t getItemAddr = m_pfunctionInfo->GetMethodFunByClass(listKlass, "get_Item", 1);
                if (getCountAddr == 0 || getItemAddr == 0) {
                    LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: method resolve failed count=%p item=%p",
                        reinterpret_cast<void*>(getCountAddr), reinterpret_cast<void*>(getItemAddr));
                    return result;
                }

                LOG(LOG_LEVEL_INFO, "[Bullet] collectDataShellList: get_Count=%p get_Item=%p",
                    reinterpret_cast<void*>(getCountAddr), reinterpret_cast<void*>(getItemAddr));

                const auto shellFields = ReadMemberValue<FrameEngine_DataShellList_T1_T2_T3_T4_Fields>(
                        pDataShellList, static_cast<uint32_t>(sizeof(void*) * 2));
                void* backingList = reinterpret_cast<void*>(shellFields.list);
                LOG(LOG_LEVEL_INFO, "[Bullet] collectDataShellList: backing list=%p", backingList);

                const ::MethodInfo* countMethod = nullptr;
                const ::MethodInfo* itemMethod = nullptr;
                void* iter = nullptr;
                while (auto* method = m_pfunctionInfo->il2cpp_class_get_methods(listKlass, &iter)) {
                    const char* methodName = m_pfunctionInfo->il2cpp_method_get_name(method);
                    if (!methodName) {
                        continue;
                    }

                    if (!countMethod && strcmp(methodName, "get_Count") == 0 &&
                        m_pfunctionInfo->il2cpp_method_get_param_count(method) == 0) {
                        countMethod = method;
                    } else if (!itemMethod && strcmp(methodName, "get_Item") == 0 &&
                               m_pfunctionInfo->il2cpp_method_get_param_count(method) == 1) {
                        itemMethod = method;
                    }

                    if (countMethod && itemMethod) {
                        break;
                    }
                }

                if (!countMethod || !itemMethod) {
                    LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: MethodInfo resolve failed count=%p item=%p",
                        (void*)countMethod, (void*)itemMethod);
                    return result;
                }

                LOG(LOG_LEVEL_INFO, "[Bullet] collectDataShellList: countMethod=%p itemMethod=%p",
                    (void*)countMethod, (void*)itemMethod);

                ::Il2CppException* invokeException = nullptr;
                ::Il2CppObject* countObject = m_pfunctionInfo->il2cpp_runtime_invoke(countMethod, pDataShellList, nullptr, &invokeException);
                if (invokeException != nullptr || countObject == nullptr) {
                    LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: count invoke failed object=%p exc=%p", countObject, invokeException);
                    return result;
                }

                int32_t count = 0;
                std::memcpy(&count,
                            reinterpret_cast<uint8_t*>(countObject) + sizeof(Il2CppObject),
                            sizeof(int32_t));
                if (count <= 0) {
                    LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: count=%d backingList=%p", count, backingList);
                    return result;
                }

                LOG(LOG_LEVEL_INFO, "[Bullet] collectDataShellList: count=%d", count);

                result.reserve(static_cast<size_t>(count));
                const std::string listTypeName = getManagedTypeName(pDataShellList);

                for (int32_t i = 0; i < count; ++i) {
                    invokeException = nullptr;
                    void* itemArgs[1] = { &i };
                    ::Il2CppObject* itemObject = m_pfunctionInfo->il2cpp_runtime_invoke(itemMethod, pDataShellList, itemArgs, &invokeException);
                    if (invokeException != nullptr || itemObject == nullptr) {
                        LOG(LOG_LEVEL_WARN, "[Bullet] collectDataShellList: item invoke failed index=%d object=%p exc=%p",
                            i, itemObject, invokeException);
                        continue;
                    }

                    T item{};
                    if constexpr (std::is_pointer_v<T>) {
                        item = reinterpret_cast<T>(itemObject);
                    } else {
                        std::memcpy(&item,
                                    reinterpret_cast<uint8_t*>(itemObject) + sizeof(Il2CppObject),
                                    sizeof(T));
                    }

                    DataShellListEntry entry;
                    entry.index = static_cast<uint32_t>(i);
                    entry.listTypeName = listTypeName;
                    entry.rawBytes = packDataShellValue(item);
                    entry.valueText = formatDataShellValue(item);

                    if constexpr (std::is_pointer_v<T>) {
                        entry.objectPtr = reinterpret_cast<void*>(item);
                        if (entry.objectPtr != nullptr) {
                            entry.itemTypeName = getManagedTypeName(entry.objectPtr);
                        } else {
                            entry.itemTypeName = "<null>";
                        }
                    } else {
                        entry.itemTypeName = getCppTypeName<T>();
                    }

                    result.push_back(std::move(entry));
                }

                return result;
            }

        /**
         * @brief  遍历 LGameVector / 兼容 get_Count/get_Item 的容器，并返回元素类型化数组
         * @tparam  T  元素类型
         * @param   pVector  容器实例指针
         * @param   logTag   日志前缀标签，默认使用 [Bullet]
         * @return  采集到的元素数组，失败返回空 vector
         */
        template<typename T>
        std::vector<T> collectLGameVector(void* pVector, const char* logTag = "[Bullet]") {
            std::vector<T> result;
            if (pVector == nullptr || m_pfunctionInfo == nullptr) {
                LOG(LOG_LEVEL_WARN, "%s collectLGameVector: input=%p functionInfo=%p", logTag, pVector, m_pfunctionInfo.get());
                return result;
            }

            auto* vectorKlass = GetObjectKlass(pVector);
            if (vectorKlass == nullptr) {
                LOG(LOG_LEVEL_WARN, "%s collectLGameVector: GetObjectKlass failed, vector=%p", logTag, pVector);
                return result;
            }

            LOG(LOG_LEVEL_INFO, "%s collectLGameVector: vector=%p klass=%p", logTag, pVector, vectorKlass);

            m_pfunctionInfo->il2cpp_runtime_class_init(vectorKlass);
            m_pfunctionInfo->il2cpp_class_init_all_method(vectorKlass);

            struct CountCandidate {
                const ::MethodInfo* method = nullptr;
                const char* name = nullptr;
                int priority = 100;
            };

            const ::MethodInfo* countMethod = nullptr;
            const ::MethodInfo* itemMethod = nullptr;
            const char* countMethodName = nullptr;
            const char* itemMethodName = nullptr;
            std::vector<CountCandidate> countCandidates;

            auto addCountCandidate = [&](const ::MethodInfo* method, const char* methodName, int priority) {
                if (method == nullptr || methodName == nullptr || priority >= 100) {
                    return;
                }
                for (const auto& candidate : countCandidates) {
                    if (candidate.method == method) {
                        return;
                    }
                }
                countCandidates.push_back(CountCandidate{ method, methodName, priority });
            };

            for (auto* current = vectorKlass; current != nullptr; current = m_pfunctionInfo->il2cpp_class_get_parent(current)) {
                m_pfunctionInfo->il2cpp_runtime_class_init(current);
                m_pfunctionInfo->il2cpp_class_init_all_method(current);

                const ::MethodInfo* localCountMethod = nullptr;
                const char* localCountMethodName = nullptr;
                int localCountPriority = 100;
                const ::MethodInfo* localItemMethod = nullptr;
                const char* localItemMethodName = nullptr;
                const ::MethodInfo* localPtrItemMethod = nullptr;
                const char* localPtrItemMethodName = nullptr;

                void* iter = nullptr;
                while (auto* method = m_pfunctionInfo->il2cpp_class_get_methods(current, &iter)) {
                    const char* methodName = m_pfunctionInfo->il2cpp_method_get_name(method);
                    if (methodName == nullptr) {
                        continue;
                    }

                    const uint32_t paramCount = m_pfunctionInfo->il2cpp_method_get_param_count(method);
                    if (paramCount == 0) {
                        int candidatePriority = 100;
                        if (strcmp(methodName, "get_Count") == 0) {
                            candidatePriority = 0;
                        } else if (strcmp(methodName, "GetCount") == 0) {
                            candidatePriority = 1;
                        } else if (strcmp(methodName, "get_Length") == 0) {
                            candidatePriority = 2;
                        }

                        if (candidatePriority < localCountPriority) {
                            localCountMethod = method;
                            localCountMethodName = methodName;
                            localCountPriority = candidatePriority;
                        }

                        addCountCandidate(method, methodName, candidatePriority);
                    } else if (paramCount == 1) {
                        if (strcmp(methodName, "get_Item") == 0 || strcmp(methodName, "GetElementByIndex") == 0) {
                            if (localItemMethod == nullptr || strcmp(methodName, "get_Item") == 0) {
                                localItemMethod = method;
                                localItemMethodName = methodName;
                            }
                        } else if (strcmp(methodName, "GetPtrByIndex") == 0 && localPtrItemMethod == nullptr) {
                            localPtrItemMethod = method;
                            localPtrItemMethodName = methodName;
                        }
                    }
                }

                if (countMethod == nullptr && localCountMethod != nullptr) {
                    countMethod = localCountMethod;
                    countMethodName = localCountMethodName;
                }

                if (itemMethod == nullptr) {
                    if (localItemMethod != nullptr) {
                        itemMethod = localItemMethod;
                        itemMethodName = localItemMethodName;
                    } else if (localPtrItemMethod != nullptr) {
                        itemMethod = localPtrItemMethod;
                        itemMethodName = localPtrItemMethodName;
                    }
                }

            }

            std::sort(countCandidates.begin(), countCandidates.end(), [](const CountCandidate& lhs, const CountCandidate& rhs) {
                return lhs.priority < rhs.priority;
            });

            if (countMethod == nullptr && !countCandidates.empty()) {
                countMethod = countCandidates.front().method;
                countMethodName = countCandidates.front().name;
            }

            LOG(LOG_LEVEL_INFO, "%s collectLGameVector: countMethod=%p(%s) itemMethod=%p(%s)",
                logTag,
                (void*)countMethod,
                countMethodName ? countMethodName : "<null>",
                (void*)itemMethod,
                itemMethodName ? itemMethodName : "<null>");

            if (countMethod == nullptr || itemMethod == nullptr) {
                LOG(LOG_LEVEL_WARN, "%s collectLGameVector: method resolve failed", logTag);
                return result;
            }

            const ::Il2CppType* itemReturnType = m_pfunctionInfo->il2cpp_method_get_return_type(itemMethod);
            const char* itemReturnTypeName = itemReturnType ? m_pfunctionInfo->il2cpp_type_get_name(itemReturnType) : nullptr;
            const std::string itemReturnTypeText = itemReturnTypeName ? itemReturnTypeName : "";
            const bool itemReturnsBoxedValue = !itemReturnTypeText.empty() &&
                (itemReturnTypeText == "System.IntPtr" ||
                 itemReturnTypeText == "System.UIntPtr" ||
                 itemReturnTypeText == "intptr_t" ||
                 itemReturnTypeText == "uintptr_t" ||
                 itemReturnTypeText == "void*" ||
                 itemReturnTypeText == "native int" ||
                 itemReturnTypeText == "native uint" ||
                 itemReturnTypeText.find("IntPtr") != std::string::npos ||
                 itemReturnTypeText.find("UIntPtr") != std::string::npos ||
                 itemReturnTypeText.find("Void*") != std::string::npos ||
                 itemReturnTypeText.find("void*") != std::string::npos);
            LOG(LOG_LEVEL_INFO, "%s collectLGameVector: itemReturnType=%s boxedValue=%d",
                logTag,
                itemReturnTypeName ? itemReturnTypeName : "<null>",
                itemReturnsBoxedValue ? 1 : 0);

            int32_t count = -1;
            for (const auto& candidate : countCandidates) {
                ::Il2CppException* countException = nullptr;
                ::Il2CppObject* countObject = m_pfunctionInfo->il2cpp_runtime_invoke(candidate.method, pVector, nullptr, &countException);
                if (countException != nullptr || countObject == nullptr) {
                    LOG(LOG_LEVEL_WARN, "%s collectLGameVector: count invoke failed method=%s object=%p exc=%p",
                        logTag, candidate.name ? candidate.name : "<null>", countObject, countException);
                    continue;
                }

                int32_t candidateCount = -1;
                std::memcpy(&candidateCount,
                            reinterpret_cast<uint8_t*>(countObject) + sizeof(Il2CppObject),
                            sizeof(int32_t));
                LOG(LOG_LEVEL_INFO, "%s collectLGameVector: count candidate %s=%d",
                    logTag, candidate.name ? candidate.name : "<null>", candidateCount);

                if (candidateCount > 0) {
                    count = candidateCount;
                    countMethod = candidate.method;
                    countMethodName = candidate.name;
                    break;
                }

                if (candidateCount == 0 && count < 0) {
                    count = 0;
                    countMethod = candidate.method;
                    countMethodName = candidate.name;
                }
            }

            if (count <= 0) {
                int32_t probedCount = 0;
                constexpr int32_t kMaxProbeCount = 4096;
                for (int32_t probeIndex = 0; probeIndex < kMaxProbeCount; ++probeIndex) {
                    ::Il2CppException* probeException = nullptr;
                    void* probeArgs[1] = { &probeIndex };
                    ::Il2CppObject* probeObject = m_pfunctionInfo->il2cpp_runtime_invoke(itemMethod, pVector, probeArgs, &probeException);
                    if (probeException != nullptr || probeObject == nullptr) {
                        break;
                    }
                    ++probedCount;
                }

                if (probedCount > 0) {
                    count = probedCount;
                    LOG(LOG_LEVEL_WARN, "%s collectLGameVector: count fallback via %s probe=%d",
                        logTag, itemMethodName ? itemMethodName : "<null>", count);
                    if (probedCount == kMaxProbeCount) {
                        LOG(LOG_LEVEL_WARN, "%s collectLGameVector: probe hit hard limit=%d, result may be truncated",
                            logTag, kMaxProbeCount);
                    }
                } else {
                    LOG(LOG_LEVEL_WARN, "%s collectLGameVector: count=%d", logTag, count);
                    return result;
                }
            }

            LOG(LOG_LEVEL_INFO, "%s collectLGameVector: count=%d", logTag, count);

            result.reserve(static_cast<size_t>(count));
            for (int32_t i = 0; i < count; ++i) {
                ::Il2CppException* itemException = nullptr;
                ::Il2CppObject* itemObject = nullptr;

                if constexpr (std::is_pointer_v<T>) {
                    if (!itemReturnsBoxedValue && itemMethod != nullptr && itemMethod->methodPointer != nullptr) {
                        using DirectItemMethod = ::Il2CppObject* (*)(void*, int32_t, const ::MethodInfo*);
                        auto directItemMethod = reinterpret_cast<DirectItemMethod>(itemMethod->methodPointer);
                        if (i < 4) {
                            LOG(LOG_LEVEL_INFO, "%s collectLGameVector: direct item call index=%d ptr=%p",
                                logTag, i, reinterpret_cast<void*>(itemMethod->methodPointer));
                        }
                        itemObject = directItemMethod ? directItemMethod(pVector, i, itemMethod) : nullptr;
                    }
                }

                if (itemObject == nullptr) {
                    void* itemArgs[1] = { &i };
                    itemObject = m_pfunctionInfo->il2cpp_runtime_invoke(itemMethod, pVector, itemArgs, &itemException);
                }

                if (itemException != nullptr || itemObject == nullptr) {
                    LOG(LOG_LEVEL_WARN, "%s collectLGameVector: item invoke failed index=%d object=%p exc=%p",
                        logTag, i, itemObject, itemException);
                    continue;
                }

                if constexpr (std::is_pointer_v<T>) {
                    if (itemReturnsBoxedValue || (itemMethodName != nullptr && strcmp(itemMethodName, "GetPtrByIndex") == 0)) {
                        T item{};
                        std::memcpy(&item,
                                    reinterpret_cast<uint8_t*>(itemObject) + sizeof(Il2CppObject),
                                    sizeof(T));
                        result.push_back(item);
                    } else {
                        result.push_back(reinterpret_cast<T>(itemObject));
                    }
                } else {
                    T item{};
                    std::memcpy(&item,
                                reinterpret_cast<uint8_t*>(itemObject) + sizeof(Il2CppObject),
                                sizeof(T));
                    result.push_back(item);
                }

                if constexpr (std::is_pointer_v<T>) {
                    if (i < 4) {
                        LOG(LOG_LEVEL_INFO, "%s collectLGameVector: item[%d]=%p", logTag, i, reinterpret_cast<void*>(result.back()));
                    }
                }
            }
            return result;
        }

        /** @brief 打印 m_miniMapData 中存储的所有数据（调试用） */
        void printMiniMapData() const;

        /** @brief 获取最近一次 updateBulletData() 遍历的弹道数据快照 */
        const std::vector<BulletInfo>& getBulletData() const { return m_bulletData; }

        /** @brief 获取最近一次 updateTerrainData() 遍历的地形数据快照 */
        const TerrainData& getTerrainData() const { return m_terrainData; }

        /** @brief 获取最近一次 updateMiniMapData() 遍历的小地图数据快照 */
        const MiniMapData& getMiniMapData() const { return m_miniMapData; }

        /** @brief 判断当前是否处于对局（战斗）中 */
        bool get_BattleStarted();

        /** @brief 获取 il2cpp 基地址 */
        void* getIl2cppBase() const { return m_il2cppBase; }

        /** @brief 获取所有玩家的队伍管理器对象指针 */
        void* get_battleTeamMgr();

        /** @brief 获取当前战斗对象 */
        void* get_battle();

        /** @brief 获取当前战斗的弹道管理器对象 */
        void* get_bulletMgr();

        /** @brief 获取当前战斗的地图管理器对象 */
        void* get_visiMapMgr();

        /**
         * @brief  验证指针指向的内存页是否可读（不触发 SIGSEGV）
         * @param  ptr   要验证的指针
         * @param  size  要读取的字节数
         * @return true=可读, false=不可读/无效
         * @details 使用 pipe write 系统调用验证，内核返回 EFAULT 而非 SIGSEGV
         */
        static bool IsReadableMemory(const void* ptr, size_t size = sizeof(void*));

    private:
        void* m_il2cppBase; // Store base address

        int32_t get_MiniIconBaseCtrlType(void* pData);

        float DecoderFix64(uint64_t value);

        // ========================= MiniMap 拆分函数 =========================//

        /**
         * @brief  获取 IL2CPP 托管对象的完整类型名（Namespace.ClassName）
         * @param  pObject  托管对象指针
         * @return 类型名字符串，对象为空返回 "<null>"
         */
        std::string getManagedTypeName(void* pObject);

        /**
         * @brief  获取小地图图标字典 (miniIcons Dictionary)
         * @return Dictionary 对象指针，失败返回 nullptr
         *
         * @details 链路: UIMainBattleMiniMapCtrl._entityCtrl
         *          → MiniMapEntityCtrl.miniMapIconCtrl
         *          → UIMiniMapIconCtrl.miniIcons
         */
        void* getMiniIconsDictionary();

        /**
         * @brief  尝试通过 FixGameObjectVisi::GetPosition 获取世界坐标
         * @param  pObject  FixGameObjectVisi 或其子类的对象指针
         * @param  outPos   [传出] 获取到的世界坐标
         * @return 成功返回 true
         */
        bool tryGetWorldPosition(void* pObject, UnityVector3& outPos);

        /**
         * @brief  将世界坐标转换为屏幕坐标（通过 Unity Camera.WorldToScreenPoint）
         * @param  worldPos     世界坐标
         * @param  outScreenX   [传出] 屏幕 X 坐标（Unity 屏幕空间，原点左下角）
         * @param  outScreenY   [传出] 屏幕 Y 坐标（Unity 屏幕空间，原点左下角）
         * @return 成功返回 true，失败（相机不存在/在相机背后）返回 false
         *
         * @details 链路: Camera.get_main() → Camera.WorldToScreenPoint(worldPos)
         *          返回的屏幕坐标 z < 0 表示目标在相机后方，此时返回 false
         */
        bool worldToScreen(const UnityVector3& worldPos, float& outScreenX, float& outScreenY);

        /**
         * @brief  获取小地图图标的世界坐标
         * @param  baseCtrl  UIMiniIconBaseCtrl 对象指针
         * @param  actor     BattleActorVisi 对象指针（可为 nullptr）
         * @param  outWorldPos [传出] 获取到的世界坐标（可为 nullptr，为空时仅返回是否成功）
         * @return 成功获取到坐标返回 true，否则返回 false
         *
         * @details 优先从 actor 获取坐标，若失败则从 baseCtrl 的 followObj 获取。
         *          链路: baseCtrl → followObj → tryGetWorldPosition
         */
        bool getIconWorldPos(void* baseCtrl, void* actor, UnityVector3* outWorldPos = nullptr);

        /**
         * @brief  判断小地图图标是否为眼/守卫类
         * @param  iconType          图标类型枚举值
         * @param  baseCtrl          UIMiniIconBaseCtrl 对象
         * @param  actor             BattleActorVisi 对象（可为 nullptr）
         * @param  baseCtrlTypeName  baseCtrl 的类型名
         * @param  actorTypeName     actor 的类型名
         * @return true 表示为眼/守卫类图标
         */
        bool isWardLikeIcon(int32_t iconType, void* actor,
                            const std::string& baseCtrlTypeName,
                            const std::string& actorTypeName);

        /** @brief 根据小地图图标信息追加一条小兵数据，成功追加返回 true */
        bool appendMinionData(int32_t iconType, void* actor,
                      const UnityVector3& worldPos, bool hasWorldPos);

        /** @brief 读取可视对象 objId，失败返回 0 */
        uint32_t readActorObjId(void* actorVisi);

        /** @brief 读取对象 camp，失败返回 -1 */
        int32_t readActorCamp(void* actorVisi);

        /** @brief 基于 WorldToScreenPoint 生成己方攻击范围的屏幕投影轮廓 */
        bool buildProjectedRangeRing(const UnityVector3& centerWorld, float range,
                         std::vector<ScreenPoint>& outPoints);


        /**
         * @brief  读取英雄等级
         * @param  ActorComponentAttribute  属性组件指针
         * @return 英雄等级，失败返回 0
         *
         * @details 读取 nativeData(+0x18) → heroLevel(+0x78) — RVA 0x8495058
         */
        uint32_t readEnemyHeroLeve(void* ActorComponentAttribute);

        /**
         * @brief  从 BattleActorVisi 获取 ActorComponentAttribute
         * @param  actorVisi  BattleActorVisi 对象指针
         * @return ActorComponentAttribute 指针，失败返回 nullptr
         *
         * @details 链路: BattleActorVisi → get_actor() → BattleActor
         *          → get_attribute() → ActorComponentAttribute
         */
        void* getActorAttribute(void* actorVisi);

        /**
         * @brief  获取己方英雄当前技能的有效目标范围
         * @param  actorVisi  BattleActorVisi 对象指针（己方英雄）
         * @return 有效范围(float)，失败返回 -1.0f
         *
         * @details 链路: BattleActorVisi → get_actor() → BattleActor
         *          → get_auxComponent() → ActorComponentAuxiliary
         *          → get_targetSystem() → TargetSystem
         *          → GetSkillValidTargetRange(currentSkill) → Fix64 → float
         */
        float getMySkillValidTargetRange(void* actorVisi, bool isSelf = false);

        void* getSkillUILogicInstance();
        void* getSkillUILogic(int skillID);
        std::vector<void*> getAllSkillUILogics();

    public:
        /**
         * @brief  模拟普通攻击 —— 通过 PlayerControl.OnTriggerSkillButtonDown 发起普攻
         * @return true=成功触发, false=失败（未找到英雄/技能/控制器等）
         *
         * @details 调用链:
         *   1. FEVisi.get_playerControl() → PlayerControl
         *   2. BattleActorVisi → BattleActor → ActorComponentSkillMgr
         *   3. ActorComponentSkillMgr.GetNormalAttackSkill() → ActorSkill
         *   4. PlayerControl.OnTriggerSkillButtonDown(valid, skillID, operID, ...)
         *   5. PlayerControl.OnTriggerSkillButtonUp(...)  完成攻击释放
         */
        bool simulateNormalAttack();

        /**
         * @brief  每 tick 调用 —— 处理延迟的 ButtonUp（普攻第二阶段）
         * @details 必须在数据采集循环中每 tick 调用此函数。
         *          simulateNormalAttack() 仅发送 ButtonDown，
         *          ButtonUp 会在下一个 tick 由此函数执行，
         *          确保游戏 Update 有时间处理 Down 状态。
         */
        void tickPendingAttack();

        /**
         * @brief  模拟英雄移动 —— 通过 PlayerControl.OnTriggerMoveButtonDown/Aim/Up 驱动
         * @param  dirX  X方向 (-1.0 ~ 1.0), 正值=右
         * @param  dirY  Y方向 (-1.0 ~ 1.0), 正值=前(屏幕上方)
         * @return true=成功触发, false=失败
         *
         * @details 必须每 tick 持续调用以维持移动。传入 (0, 0) 停止移动。
         */
        bool simulateMovement(float dirX, float dirY);

        bool simulateMoveUp();
        bool simulateMoveDown();
        bool simulateMoveLeft();
        bool simulateMoveRight();

    private:

        /**
         * @brief  读取敌方英雄的 HP 信息并输出日志
         * @param  actor  BattleActorVisi 对象指针
         *
         * @details 链路: BattleActorVisi → get_actor() → BattleActor
         *          → get_attribute() → ActorComponentAttribute
         *          → nativeData+0x138 (curHP), GetFixAttrValue(2) (maxHP)
         */
        void readEnemyHeroHP(void* actor,float* curHp=nullptr, float* maxHp=nullptr);

        /**
         * @brief  读取英雄配置 ID（_resId）
         * @param  actorVisi  BattleActorVisi 对象指针
         * @return 英雄配置 ID，失败返回 0
         *
         * @details 链路: BattleActorVisi → _resId(0x150)
         */
        int32_t readHeroResId(void* actorVisi);

        /**
         * @brief  读取英雄名称
         * @param  actorVisi  BattleActorVisi 对象指针
         * @return 英雄名称字符串，失败返回空字符串
         *
         * @details 链路: BattleActorVisi → dc(0x90) → BattleActor_DC
         *          → name → Il2CppString → std::string
         *          优先从 BattleActor_DC.name 读取；若失败则尝试 resId 映射
         */
        std::string readHeroName(void* actorVisi);

        /**
         * @brief  将 IL2CPP 托管 System.String 对象转为 C++ std::string (UTF-8)
         * @param  pIl2CppString  Il2CppString 对象指针
         * @return UTF-8 字符串，失败返回空字符串
         */
        std::string readIl2CppString(void* pIl2CppString);

        /**
         * @brief  读取召唤师名（玩家名）
         * @param  actorVisi  BattleActorVisi 对象指针
         * @return 召唤师名字符串，失败返回空字符串
         *
         * @details 链路: BattleActorVisi → get_player() → BattlePlayer
         *          → get_name() → Il2CppString → std::string
         *          get_name 内部: 缓存(this+0xB0), 未命中时通过
         *          roleId(native+0x148) → BattleContext.GetPlayerNameInBattle
         */
        std::string readSummonerName(void* actorVisi);

        //========================= IL2CPP 动态字段查找 API =========================//

        /**
         * @brief  获取托管对象的 Il2CppClass 指针（从对象头部 offset 0x00 读取）
         * @param  pObject  托管对象指针
         * @return Il2CppClass* 或 nullptr
         */
        inline ::Il2CppClass* GetObjectKlass(void* pObject) {
            if (!pObject) return nullptr;
            return reinterpret_cast<::Il2CppClass*>(*reinterpret_cast<uint64_t*>(pObject));
        }

        /**
         * @brief  从运行时 Il2CppClass（含父类链）动态查找指定字段的偏移量
         * @param  klass      目标类（运行时 Il2CppClass*）
         * @param  fieldName  字段名称
         * @return 字段偏移，查找失败返回 INVALID_OFFSET
         */
        uint32_t GetFieldOffsetFromKlass(::Il2CppClass* klass, const char* fieldName);

        /**
         * @brief  从运行时对象（含类层次链）动态查找指定字段的偏移量
         * @param  pObject    对象指针
         * @param  fieldName  字段名称
         * @return 字段偏移，查找失败返回 INVALID_OFFSET
         */
        inline uint32_t GetFieldOffsetFromObject(void* pObject, const char* fieldName) {
            return GetFieldOffsetFromKlass(GetObjectKlass(pObject), fieldName);
        }

        /**
         * @brief  从运行时 Il2CppClass 的静态字段中按名称读取 int32_t 值
         * @param  klass      目标类
         * @param  fieldName  静态字段名称
         * @param  outValue   [传出] 读取到的值
         * @return 成功返回 true，失败返回 false
         */
        bool ReadStaticFieldInt32(::Il2CppClass* klass, const char* fieldName, int32_t& outValue);

        /**
         * @brief   通过对象基地址 + 偏移读取成员值（模板版本，支持任意类型）
         * @tparam  T       要读取的目标类型（如 int32_t, float, bool, void* 等）
         * @param   pObject 对象基地址（Il2CppObject* 或任意运行时对象指针）
         * @param   offset  成员在对象中的偏移（通过 GetStaticMember 的 pOutOffset 获取）
         * @return  读取到的值，若 pObject 为空则返回 T 的默认值
         */
        template<typename T>
        T ReadMemberValue(void* pObject, uint32_t offset) {
            if (pObject == nullptr) return T{};
            return *reinterpret_cast<T*>((uint64_t)pObject + offset);
        }

        /**
         * @brief   查询指定类中任意成员（静态/实例）的偏移量
         * @param   pModuleName     类所在模块名（如 "Assembly-CSharp.dll"）
         * @param   pClassName      类名
         * @param   pTemplateName   泛型名（可为空）
         * @param   pFieldName      字段名
         * @return  字段偏移，查找失败返回 INVALID_OFFSET (0xFFFFFFFF)
         */
        static constexpr uint32_t INVALID_OFFSET = 0xFFFFFFFF;
        inline uint32_t GetFieldOffset(const std::string& pModuleName,
                                       const std::string& pClassName,
                                       const std::string& pTemplateName,
                                       const std::string& pFieldName) {
            if (m_pfunctionInfo == nullptr) return INVALID_OFFSET;
            uint32_t offset = INVALID_OFFSET;
            m_pfunctionInfo->GetMember("ilbil2cpp.so",pModuleName, pClassName, pTemplateName, pFieldName, &offset);
            return offset;
        }

        /**
         * @brief   通过对象基地址 + 偏移读取指针类型成员（便捷版本）
         * @param   pObject 对象基地址
         * @param   offset  成员在对象中的偏移
         * @return  读取到的指针值，若 pObject 为空则返回 nullptr
         */
        inline void* ReadMemberPtr(void* pObject, uint32_t offset) {
            if (pObject == nullptr) return nullptr;
            return *reinterpret_cast<void**>((uint64_t)pObject + offset);
        }

        /**
         * @brief  读取 FixVector3_Fix64 原始坐标并转换为 UnityVector3
         * @param  pObject  对象指针
         * @param  offset   字段偏移
         * @param  outValue [传出] 转换后的坐标
         * @return 成功返回 true
         */
        bool readFixVector3(void* pObject, uint32_t offset, UnityVector3& outValue);

        /**
         * @brief  读取 FixVector2_Fix64 原始坐标
         * @param  pObject  对象指针
         * @param  offset   字段偏移
         * @param  outValue [传出] 原始坐标
         * @return 成功返回 true
         */
        bool readFixVector2Raw(void* pObject, uint32_t offset, FixVector2Raw& outValue);

        /**
         * @brief   通过类名和字段名自动查找偏移并读取成员值
         * @tparam  T               要读取的目标类型
         * @param   pObject         对象基地址
         * @param   pModuleName     类所在模块名（如 "Assembly-CSharp.dll"）
         * @param   pClassName      类名
         * @param   pTemplateName   泛型名（可为空）
         * @param   pFieldName      字段名
         * @return  读取到的值，查找失败或 pObject 为空则返回 T 的默认值
         */
        template<typename T>
        T ReadMemberByName(void* pObject, const std::string& pModuleName,
                           const std::string& pClassName, const std::string& pTemplateName,
                           const std::string& pFieldName) {
            if (pObject == nullptr || m_pfunctionInfo == nullptr) return T{};
            uint32_t offset = 0xFFFFFFFF;
            m_pfunctionInfo->GetStaticMember("libil2cpp.so", pModuleName, pClassName, pTemplateName, pFieldName, &offset);
            if (offset == 0xFFFFFFFF) return T{};
            return *reinterpret_cast<T*>((uint64_t)pObject + offset);
        }

        template<typename T>
        static std::string getCppTypeName() {
            if constexpr (std::is_same_v<T, FixVector2Raw>) {
                return "FixVector2Raw";
            } else if constexpr (std::is_same_v<T, FrameEngine_Common_FixVector3_Fix64_Fields>) {
                return "FixVector3_Fix64";
            } else if constexpr (std::is_same_v<T, void*>) {
                return "void*";
            } else if constexpr (std::is_same_v<T, bool>) {
                return "bool";
            } else if constexpr (std::is_same_v<T, int8_t>) {
                return "int8_t";
            } else if constexpr (std::is_same_v<T, uint8_t>) {
                return "uint8_t";
            } else if constexpr (std::is_same_v<T, int16_t>) {
                return "int16_t";
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                return "uint16_t";
            } else if constexpr (std::is_same_v<T, int32_t>) {
                return "int32_t";
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return "uint32_t";
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return "int64_t";
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                return "uint64_t";
            } else if constexpr (std::is_same_v<T, float>) {
                return "float";
            } else if constexpr (std::is_same_v<T, double>) {
                return "double";
            } else if constexpr (std::is_pointer_v<T>) {
                return "pointer";
            } else {
                return typeid(T).name();
            }
        }

        template<typename T>
        static std::string formatDataShellValue(const T& value) {
            if constexpr (std::is_same_v<T, bool>) {
                return value ? "true" : "false";
            } else if constexpr (std::is_pointer_v<T>) {
                std::ostringstream oss;
                oss << reinterpret_cast<const void*>(value);
                return oss.str();
            } else if constexpr (std::is_floating_point_v<T>) {
                std::ostringstream oss;
                oss << value;
                return oss.str();
            } else if constexpr (std::is_integral_v<T>) {
                if constexpr (std::is_signed_v<T>) {
                    return std::to_string(static_cast<long long>(value));
                } else {
                    return std::to_string(static_cast<unsigned long long>(value));
                }
            } else {
                return "<unsupported>";
            }
        }

        template<typename T>
        static std::vector<uint8_t> packDataShellValue(const T& value) {
            std::vector<uint8_t> bytes(sizeof(T));
            if (!bytes.empty()) {
                std::memcpy(bytes.data(), &value, sizeof(T));
            }
            return bytes;
        }
    private:
        std::shared_ptr<fun::function> m_pfunctionInfo; ///< 类信息查询接口实例
        std::vector<BulletInfo> m_bulletData;           ///< 技能弹道快照
        TerrainData m_terrainData;                      ///< 地形碰撞快照
        MiniMapData m_miniMapData;                      ///< 小地图遍历数据快照
    };

    /**
     * @class lol
     * @brief LOL 手游综合业务类 —— 继承 FEVisi，提供字符串解密和元数据索引表管理
     *
     * @details 负责初始化字符串索引表、判断字符串加密状态、执行字符串解密等核心功能。
     */
    class lol: public FEVisi {
    public:
        /**
         * @brief 构造函数
         * @param dqil2cppBase              il2cpp 模块基地址
         * @param pCodeRegistration         代码注册表指针
         * @param pMetadataRegistration     元数据注册表指针
         * @param pGlobalMetadataHeader     全局元数据头指针
         * @param pMetadataImagesTable      元数据镜像表指针
         */
        lol(void* dqil2cppBase=nullptr,
            void *pCodeRegistration=nullptr,
            void *pMetadataRegistration=nullptr,
            void *pGlobalMetadataHeader=nullptr,
            void* pMetadataImagesTable=nullptr);

        /** @brief 析构函数 */
        ~lol();

    private:
        /**
         * @brief   解密加密字符串
         * @param   Srcstr      加密的原始字符串指针
         * @param   nameIndex   字符串在索引表中的索引
         * @return  解密后的字符串
         */
        std::string decryPtthestring(char*Srcstr, uint32_t nameIndex);

        /**
         * @brief   初始化字符串索引表
         * @param   pGlobalMetadataHeader 全局元数据头指针
         * @return  初始化后的索引表指针
         */
        void* inItStringindexTable(const Il2CppGlobalMetadataHeader* pGlobalMetadataHeader);

        /**
         * @brief   判断指定索引的字符串是否已加密/已解密
         * @param   nameIndex 字符串索引
         * @return  true 表示已加密需要解密，false 表示无需处理
         */
        bool IsEncryptionOrIsDecryption(uint32_t nameIndex);



    private:
        void*parrayStringIndex = NULL;                                                          ///< 字符串索引数组原始指针
        std::shared_ptr<std::map<uint32_t,std::shared_ptr<LolStrStruct>>> m_pMapStringIndex = nullptr; ///< 字符串索引 -> 解密缓存 的映射表
    };
}
#endif //DOBBY_PROJECT_LOLM_H
