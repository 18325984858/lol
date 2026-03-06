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
#include "../interface/interface.h"

struct Il2CppGlobalMetadataHeader;

namespace lol {

    /**
     * @brief 全局解密密钥/元数据数据表
     * @details 来源于 .rodata:0B9BBF1B，用于字符串解密运算
     */
    const uint8_t byte_B9BBF1B[277] = {
            0xF8, 0x99, 0x13, 0x9D, 0xF5, 0xE1, 0x05, 0x93, 0x96, 0x43, 0x14, 0x15, 0xE7, 0x70,
            0xC6, 0xDD,
            0x38, 0xB3, 0xEF, 0xF8, 0xBA, 0xF5, 0x66, 0x27, 0x47, 0x8E, 0xC7, 0x6A, 0x70, 0x4E,
            0x9B, 0x52,
            0xEC, 0x89, 0x56, 0x63, 0x7A, 0x99, 0x78, 0x7B, 0xD1, 0x97, 0xEA, 0xCD, 0x77, 0xAC,
            0xCE, 0x5E,
            0x69, 0x74, 0xCE, 0x5A, 0xC6, 0x60, 0x61, 0x0B, 0x44, 0xD9, 0xB9, 0xFE, 0xD0, 0xFF,
            0x95, 0x48,
            0xC9, 0xE1, 0x07, 0x4F, 0x5B, 0x3F, 0x9F, 0xC8, 0xEA, 0x15, 0xD1, 0x52, 0xAD, 0xD0,
            0x72, 0x94,
            0x65, 0xB9, 0xEE, 0xA6, 0xE1, 0xCC, 0x6B, 0xB9, 0xF0, 0xCD, 0x2A, 0x47, 0x75, 0x1A,
            0x18, 0x6F,
            0xF0, 0x93, 0x5E, 0x4C, 0xD5, 0x92, 0x0A, 0xA6, 0xC7, 0xC9, 0x96, 0xA5, 0xEE, 0x53,
            0xA7, 0x0F,
            0xA9, 0x7D, 0xA6, 0x29, 0xB0, 0x98, 0xB7, 0x5C, 0x29, 0x4D, 0xFF, 0xDC, 0x3E, 0x46,
            0x39, 0x04,
            0xA3, 0xC6, 0x5C, 0x29, 0x74, 0x27, 0x0F, 0xD0, 0x93, 0xEE, 0x8A, 0x9B, 0xF8, 0xAE,
            0x7D, 0x0B,
            0x27, 0x23, 0xD0, 0x92, 0xB6, 0x38, 0x85, 0xE0, 0xD7, 0xC2, 0x60, 0xCC, 0x00, 0x7E,
            0x8B, 0x9D,
            0x5F, 0x93, 0xF9, 0x83, 0x52, 0x4D, 0xEF, 0x3D, 0xCA, 0x46, 0x44, 0x69, 0xD2, 0xCF,
            0x9F, 0x3E,
            0x69, 0x8D, 0x51, 0xA1, 0x9D, 0x8A, 0x12, 0x1C, 0xE5, 0x81, 0x49, 0x9D, 0x7B, 0x70,
            0x16, 0x68,
            0x7F, 0x6F, 0xFA, 0xA6, 0xBB, 0x0B, 0x40, 0x80, 0x17, 0xB6, 0x22, 0x54, 0x21, 0x16,
            0x91, 0xB5,
            0x73, 0x27, 0x8A, 0x4A, 0x86, 0x96, 0x0E, 0xEB, 0x57, 0x6A, 0x8F, 0xD4, 0xC9, 0xEC,
            0x69, 0x97,
            0x5F, 0xD0, 0xB3, 0x7C, 0xD7, 0xDB, 0xBB, 0x00, 0xF9, 0x7B, 0xA6, 0xCE, 0x92, 0xBF,
            0x5A, 0xDD,
            0x2B, 0x44, 0x92, 0x8A, 0xE1, 0x1F, 0xB9, 0x38, 0x4C, 0x4C, 0xF3, 0x87, 0x08, 0x67,
            0x7C, 0x48,
            0xC4, 0x51, 0x47, 0xDE, 0xE7, 0x29, 0x31, 0x1E, 0xF5, 0xB5, 0xC3, 0x00, 0x39, 0x46,
            0xC4, 0x8F,
            0x00, 0x00, 0x00, 0x00, 0x00
    };

    struct Il2CppClass_1
    {
        void* image;
        void* gc_desc;
        const char* name;
        const char* namespaze;
        Il2CppType byval_arg;
        Il2CppType this_arg;
        Il2CppClass* element_class;
        Il2CppClass* castClass;
        Il2CppClass* declaringType;
        Il2CppClass* parent;
        void *generic_class;
        void* typeMetadataHandle;
        void* interopData;
        Il2CppClass* klass;
        void* fields;
        void* events;
        void* properties;
        void* methods;
        Il2CppClass** nestedTypes;
        Il2CppClass** implementedInterfaces;
        Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets;
    };

    struct Il2CppClass_2
    {
        Il2CppClass** typeHierarchy;
        void *unity_user_data;
        uint32_t initializationExceptionGCHandle;
        uint32_t cctor_started;
        uint32_t cctor_finished;
        size_t cctor_thread;
        void* genericContainerHandle;
        uint32_t instance_size;
        uint32_t actualSize;
        uint32_t element_size;
        int32_t native_size;
        uint32_t static_fields_size;
        uint32_t thread_static_fields_size;
        int32_t thread_static_fields_offset;
        uint32_t flags;
        uint32_t token;
        uint16_t method_count;
        uint16_t property_count;
        uint16_t field_count;
        uint16_t event_count;
        uint16_t nested_type_count;
        uint16_t vtable_count;
        uint16_t interfaces_count;
        uint16_t interface_offsets_count;
        uint8_t typeHierarchyDepth;
        uint8_t genericRecursionDepth;
        uint8_t rank;
        uint8_t minimumAlignment;
        uint8_t naturalAligment;
        uint8_t packingSize;
        uint8_t bitflags1;
        uint8_t bitflags2;
    };

    struct Il2CppClass
    {
        Il2CppClass_1 _1;
        void* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        VirtualInvokeData vtable[255];
    };
    struct FrameEngine_Visual_FEVisi_Fields {
    };
    struct FrameEngine_Visual_FEVisi_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct FrameEngine_Visual_FEVisi_c {
        Il2CppClass_1 _1;
        struct FrameEngine_Visual_FEVisi_StaticFields* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        FrameEngine_Visual_FEVisi_VTable vtable;
    };
    struct FrameEngine_Visual_FEVisi_o {
        FrameEngine_Visual_FEVisi_c *klass;
        void *monitor;
        FrameEngine_Visual_FEVisi_Fields fields;
    };
    struct FrameEngine_Visual_FEVisi_StaticFields {
        uint64_t myPlayerGid;
        struct FrameEngine_Logic_BattleTeam_o* judgeTeam;
        struct FrameEngine_Logic_BattleTeam_o* myTeam;
        struct FrameEngine_Logic_BattleTeam_o* neutralTeam;
        struct FrameEngine_Logic_BattleTeam_o* enemyTeam;
        struct FrameEngine_Logic_BattlePlayer_o* myPlayer;
        struct FrameEngine_Visual_BattleActorVisi_o* myHero;
        struct FrameEngine_Visual_BattleActorVisi_o* myOriginHero;
        struct FrameEngine_Logic_BattleMonsterSpawner_o* monsterSpawner;
        struct System_Collections_Generic_Dictionary_System_Int32_UnityEngine_Vector2_o* trigonometricCache;
        bool CameraOptimization;
        bool disableLogForLowest;
        bool alwaysShowForRecorder;
        bool ignoreCameraRotate;
        bool keepShowSoldierHeadbar;
        struct FrameEngine_Logic_CherryTeam_o* myCherryTeam;
        struct FrameEngine_Logic_Battle_o* battle_k_BackingField;
        struct FrameEngine_Logic_ActorMgr_o* ActorMgr_k_BackingField;
        struct FrameEngine_Logic_BulletMgr_o* BulletMgr_k_BackingField;
        struct FrameEngine_Interface_TargetUtilsBase_o* targetUtilsBase_k_BackingField;
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
        /** @brief 判断当前是否处于对局（战斗）中 */
        bool get_BattleStarted();

        /** @brief 获取所有玩家的队伍管理器对象指针 */
        void* get_battleTeamMgr();

    public:
        void* test();
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
