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
#include "LolHeader.h"
#include "../interface/interface.h"

struct Il2CppGlobalMetadataHeader;

namespace lol {
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

    private:
        void* m_il2cppBase; // Store base address

    public:
        /** @brief 判断当前是否处于对局（战斗）中 */
        bool get_BattleStarted();

        /** @brief 获取所有玩家的队伍管理器对象指针 */
        void* get_battleTeamMgr();

        int32_t get_MiniIconBaseCtrlType(void* pData);

    public:
        float DecoderFix64(uint64_t value);

    public:

        void* test();
        void* test1();
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
    private:
        std::shared_ptr<fun::function> m_pfunctionInfo; ///< 类信息查询接口实例
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

    public:
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
