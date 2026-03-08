//
// Created by Song on 3/8/2026.
//

#ifndef DOBBY_PROJECT_LOLHEADER_H
#define DOBBY_PROJECT_LOLHEADER_H
#include "../interface/interface.h"
namespace lol{
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

    // Image: Assembly-CSharp.dll
    // Namespace:
    enum MiniMapIconType // TypeDefIndex : 6063
    {
        MiniMapIconType_None = 0,
        MiniMapIconType_MyTeamHero = 1,
        MiniMapIconType_EnemyTeamHero = 2,
        MiniMapIconType_HomeBase = 3,
        MiniMapIconType_Monster = 4,
        MiniMapIconType_MonsterAnctor = 5,
        MiniMapIconType_Solider = 6,
        MiniMapIconType_Tower = 7,
        MiniMapIconType_SummonerMonster = 8,
        MiniMapIconType_Bullet = 9,
        MiniMapIconType_MyTeamWard = 10,
        MiniMapIconType_BornCountDown = 11,
        MiniMapIconType_HexGateLine = 12,
        MiniMapIconType_MAX = 13
    };

    typedef void(*Il2CppMethodPointer)();

    struct MethodInfo;

    struct VirtualInvokeData
    {
        Il2CppMethodPointer methodPtr;
        const MethodInfo* method;
    };

    struct Il2CppType
    {
        void* data;
        unsigned int bits;
    };

    struct Il2CppClass;

    struct Il2CppObject
    {
        Il2CppClass *klass;
        void *monitor;
    };

    union Il2CppRGCTXData
    {
        void* rgctxDataDummy;
        const MethodInfo* method;
        const Il2CppType* type;
        Il2CppClass* klass;
    };

    struct Il2CppRuntimeInterfaceOffsetPair
    {
        Il2CppClass* interfaceType;
        int32_t offset;
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

    typedef uintptr_t il2cpp_array_size_t;
    typedef int32_t il2cpp_array_lower_bound_t;
    struct Il2CppArrayBounds
    {
        il2cpp_array_size_t length;
        il2cpp_array_lower_bound_t lower_bound;
    };

    typedef void (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo*, void*, void**, void*);
    struct MethodInfo
    {
        Il2CppMethodPointer methodPointer;
        Il2CppMethodPointer virtualMethodPointer;
        InvokerMethod invoker_method;
        const char* name;
        Il2CppClass *klass;
        const Il2CppType *return_type;
        const Il2CppType** parameters;
        union
        {
            const Il2CppRGCTXData* rgctx_data;
            const void* methodMetadataHandle;
        };
        union
        {
            const void* genericMethod;
            const void* genericContainerHandle;
        };
        uint32_t token;
        uint16_t flags;
        uint16_t iflags;
        uint16_t slot;
        uint8_t parameters_count;
        uint8_t bitflags;
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

    struct UnityEngine_Object_Fields {
        void* m_CachedPtr; // 0x10
    };

    struct UnityEngine_Component_Fields : UnityEngine_Object_Fields {
    };

    struct UnityEngine_Behaviour_Fields : UnityEngine_Component_Fields {
    };

    struct UnityEngine_MonoBehaviour_Fields : UnityEngine_Behaviour_Fields {
    };

    struct BaseUI_Fields : UnityEngine_MonoBehaviour_Fields {
        bool isPauseByFullScreenDialog; // 0x18
        bool destroyOnExit; // 0x19
        struct System_Action_o* onDepthChanged; // 0x20
        int32_t playEnterAnimCount; // 0x28
        int32_t playExitAnimCount; // 0x2C
        struct System_String_o* uiName; // 0x30
        struct System_String_o* prefabName; // 0x38
        struct System_Collections_Generic_Dictionary_System_String_BaseUI_o* subStackedUIDic; // 0x40
        struct System_String_o* curSubStackedUIName; // 0x48
        struct System_Collections_Generic_List_BaseUI_o* tempList; // 0x50
        int32_t mutuallyExclusiveLayer; // 0x58
        int32_t uiType; // 0x5C
        int32_t backHandleType; // 0x60
        struct UIPlayDoTween_o* playDoTween; // 0x68
        struct System_Collections_Generic_List_UIPlayDoTween_o* allPlayDoTweenList; // 0x70
        struct UnityEngine_Transform_o* cacheTrans; // 0x78
        struct UnityEngine_Canvas_o* panelCanvas; // 0x80
        struct UnityEngine_UI_GraphicRaycaster_o* graphicRaycaster; // 0x88
        struct System_Collections_Generic_List_BaseUI_o* subBaseUIList; // 0x90
        struct System_Collections_Generic_List_BaseUI_o* nonStackedUIListAttached; // 0x98
        struct BaseUI_o* stackedUIAttachTo; // 0xA0
        struct System_Collections_Generic_List_BaseUI_o* tempUIList; // 0xA8
        struct NonDeactiveComponent_o* nonDeactiveComponent; // 0xB0
        struct UICommonTopbarCtrl_o* topBarCtrl; // 0xB8
        struct System_Collections_IEnumerator_o* playShowAnimCorutine; // 0xC0
        struct UnityEngine_Events_UnityAction_o* playShowCallback; // 0xC8
        struct System_Action_o* showAction; // 0xD0
        struct System_Action_o* reShowAction; // 0xD8
        struct System_Action_o* hideAction; // 0xE0
        struct System_Action_o* startHideAction; // 0xE8
        struct System_Action_o* recycleAction; // 0xF0
        uint32_t pageSessionIdForReport_k_BackingField; // 0xF8
        struct System_String_array* emptySubBaseUINameList; // 0x100
        bool isPlayingEnterAnim_k_BackingField; // 0x108
        int64_t startShowTime; // 0x110
        int64_t firstStartShowTime; // 0x118
        struct UnityEngine_WaitForEndOfFrame_o* callEndofFrame; // 0x120
    };

    struct BattleBaseUI_Fields : BaseUI_Fields {
        struct System_Collections_Generic_List_BattleUINode_o* m_Behaviours; // 0x128
        struct System_Collections_Generic_List_BattleUINode_o* realTimeUpdateMonos; // 0x130
        struct System_Collections_Generic_List_BattleUINode_o* logicTimeUpdateMonos; // 0x138
        struct System_Collections_Generic_List_BattleUINode_o* lateUpdateMonos; // 0x140
        bool ignoreGraphicRaycaster; // 0x148
        bool m_IsBattleStarted; // 0x149
        bool isOnShowing; // 0x14A
        struct System_Collections_Generic_List_RectMask2DEx_o* rectMask2DExs; // 0x150
        bool isAwake; // 0x158
        int32_t currentRealUpdateIndex; // 0x15C
        int32_t currentLogicUpdateIndex; // 0x160
        int32_t currentLateUpdateIndex; // 0x164
    };
    struct BattleBaseUI_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct BattleBaseUI_c {
        Il2CppClass_1 _1;
        void* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        BattleBaseUI_VTable vtable;
    };
    struct BattleBaseUI_o {
        BattleBaseUI_c *klass;
        void *monitor;
        BattleBaseUI_Fields fields;
    };
    struct UnityEngine_Vector2_Fields {
        float x;
        float y;
    };
    struct MiniMapEntityCtrl_Fields : BattleBaseUI_Fields {
        struct UIMiniMapIconCtrl_o* miniMapIconCtrl;
        struct UIMiniMapIndicatorMgr_o* indicatorMgr;
        struct UIMiniMapSkillEffectMgr_o* skillEffectMgr;
        struct UnityEngine_RectTransform_o* contentRoot;
        struct UnityEngine_Camera_o* minimapCamera;
        struct UnityEngine_UI_RawImage_o* minimapMaskClear;
        struct UnityEngine_UI_RawImage_o* minimapSprite;
        struct UnityEngine_UI_RawImage_o* minimapSpriteMask;
        struct Img_o* mapCornerBorder;
        bool debugOutputTouchPoint;
        struct UnityEngine_CanvasGroup_o* canvasGroup;
        struct UnityEngine_RenderTexture_o* miniMapRt;
        struct UnityEngine_Vector2_Fields useUICameraOffsetMin;
        struct UnityEngine_Vector2_Fields useUICameraOffsetMax;
        struct UnityEngine_Vector2_Fields useMinimapCameraOffsetMin;
        struct UnityEngine_Vector2_Fields useMinimapCameraOffsetMax;
        struct UnityEngine_RectTransform_o* rectTransform;
        struct DVisiEventListenerNoArg_o* deleCacheOf_OnEventScreenSizeChange;
        struct DVisiEventListener_o* deleCacheOf_OnMinimapSetting;
        struct DVisiEventListener_o* deleCacheOf_OnMapBlockAnchorSwitch;
        struct DVisiEventListener_o* deleCacheOf_OnMapBorderState;
    };
    struct MiniMapEntityCtrl_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct MiniMapEntityCtrl_c {
        Il2CppClass_1 _1;
        void* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        MiniMapEntityCtrl_VTable vtable;
    };
    struct MiniMapEntityCtrl_o {
        MiniMapEntityCtrl_c *klass;
        void *monitor;
        MiniMapEntityCtrl_Fields fields;
    };

    struct System_Collections_Generic_Dictionary_Entry_TKey_TValue_Fields {
        int32_t hashCode; // 0x0
        int32_t next; // 0x0
        void* key; // 0x0
        void* value; // 0x0
    };
    struct System_Collections_Generic_Dictionary_Entry_TKey_TValue_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct System_Collections_Generic_Dictionary_Entry_TKey_TValue_c {
        Il2CppClass_1 _1;
        void* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        System_Collections_Generic_Dictionary_Entry_TKey_TValue_VTable vtable;
    };
    struct System_Collections_Generic_Dictionary_Entry_TKey_TValue_o {
        System_Collections_Generic_Dictionary_Entry_TKey_TValue_Fields fields;
    };

    struct System_Collections_Generic_Dictionary_Entry_TKey_TValue_array{
        Il2CppObject obj;
        Il2CppArrayBounds * bounds;
        il2cpp_array_size_t max_length;
        System_Collections_Generic_Dictionary_Entry_TKey_TValue_o m_Items[65535];
    };

    struct System_Collections_Generic_Dictionary_TKey_TValue_Fields {
        struct System_Int32_array* buckets; // 0x0
        struct System_Collections_Generic_Dictionary_Entry_TKey_TValue_array* entries; // 0x0
        int32_t count; // 0x0
        int32_t version; // 0x0
        int32_t freeList; // 0x0
        int32_t freeCount; // 0x0
        struct System_Collections_Generic_IEqualityComparer_TKey_o* comparer; // 0x0
        struct System_Collections_Generic_Dictionary_KeyCollection_TKey_TValue_o* keys; // 0x0
        struct System_Collections_Generic_Dictionary_ValueCollection_TKey_TValue_o* values; // 0x0
        Il2CppObject* syncRoot; // 0x0
    };
    struct System_Collections_Generic_Dictionary_TKey_TValue_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct System_Collections_Generic_Dictionary_TKey_TValue_c {
        Il2CppClass_1 _1;
        struct System_Collections_Generic_Dictionary_TKey_TValue_StaticFields* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        System_Collections_Generic_Dictionary_TKey_TValue_VTable vtable;
    };
    struct System_Collections_Generic_Dictionary_TKey_TValue_o {
        System_Collections_Generic_Dictionary_TKey_TValue_c *klass;
        void *monitor;
        System_Collections_Generic_Dictionary_TKey_TValue_Fields fields;
    };
    struct System_Collections_Generic_Dictionary_TKey_TValue_StaticFields {
        struct System_String_o* VersionName; // 0x0
        struct System_String_o* HashSizeName; // 0x0
        struct System_String_o* KeyValuePairsName; // 0x0
        struct System_String_o* ComparerName; // 0x0
    };

    struct BattleUINode_Fields : UnityEngine_MonoBehaviour_Fields {
        struct BattleBaseUI_o* battleBaseUI; // 0x18
        struct System_Collections_Generic_List_BattleUINode_o* childNodes; // 0x20
        struct BattleUINode_o* parentNode; // 0x28
        bool registerBaseUI; // 0x30
        bool isOnShowing; // 0x31
        struct System_Collections_Generic_List_UnityEngine_UI_ScrollRect_o* scrollRectList; // 0x38
        struct System_Collections_Generic_List_RectMask2DEx_o* rectMask2DExList; // 0x40
    };
    struct FrameEngine_Common_Fix64_Fields {
        int64_t rawValue; // 0x10
    };

    struct FrameEngine_Common_FixVector3_Fix64_Fields {
        struct FrameEngine_Common_Fix64_Fields x; // 0x10
        struct FrameEngine_Common_Fix64_Fields y; // 0x18
        struct FrameEngine_Common_Fix64_Fields z; // 0x20
    };
    struct FrameEngine_Common_FixVector3_Fix64_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct FrameEngine_Common_FixVector3_Fix64_c {
        Il2CppClass_1 _1;
        struct FrameEngine_Common_FixVector3_Fix64_StaticFields* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        FrameEngine_Common_FixVector3_Fix64_VTable vtable;
    };
    struct FrameEngine_Common_FixVector3_Fix64_o {
        FrameEngine_Common_FixVector3_Fix64_Fields fields;
    };
    struct FrameEngine_Common_FixVector3_Fix64_StaticFields {
        struct FrameEngine_Common_FixVector3_Fix64_Fields one; // 0x0
        struct FrameEngine_Common_FixVector3_Fix64_Fields zero; // 0x18
        struct FrameEngine_Common_FixVector3_Fix64_Fields back; // 0x30
        struct FrameEngine_Common_FixVector3_Fix64_Fields forward; // 0x48
        struct FrameEngine_Common_FixVector3_Fix64_Fields down; // 0x60
        struct FrameEngine_Common_FixVector3_Fix64_Fields up; // 0x78
        struct FrameEngine_Common_FixVector3_Fix64_Fields left; // 0x90
        struct FrameEngine_Common_FixVector3_Fix64_Fields right; // 0xA8
        struct FrameEngine_Common_FixVector3_Fix64_Fields invalid; // 0xC0
        uint16_t seriaTypeNo_ConfigVisi; // 0x0
    };

    struct FrameEngine_Common_Fix64_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct FrameEngine_Common_Fix64_c {
        Il2CppClass_1 _1;
        struct FrameEngine_Common_Fix64_StaticFields* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        FrameEngine_Common_Fix64_VTable vtable;
    };
    struct FrameEngine_Common_Fix64_o {
        FrameEngine_Common_Fix64_Fields fields;
    };
    struct FrameEngine_Common_Fix64_StaticFields {
        int32_t fractionalBits; // 0x0
        struct FrameEngine_Common_Fix64_Fields maxEpsilonValue; // 0x0
        struct FrameEngine_Common_Fix64_Fields maxValue; // 0x8
        struct FrameEngine_Common_Fix64_Fields minValue; // 0x10
        struct FrameEngine_Common_Fix64_Fields zero; // 0x18
        struct FrameEngine_Common_Fix64_Fields one; // 0x20
        struct FrameEngine_Common_Fix64_Fields two; // 0x28
        struct FrameEngine_Common_Fix64_Fields three; // 0x30
        struct FrameEngine_Common_Fix64_Fields four; // 0x38
        struct FrameEngine_Common_Fix64_Fields five; // 0x40
        struct FrameEngine_Common_Fix64_Fields six; // 0x48
        struct FrameEngine_Common_Fix64_Fields seven; // 0x50
        struct FrameEngine_Common_Fix64_Fields eight; // 0x58
        struct FrameEngine_Common_Fix64_Fields nine; // 0x60
        struct FrameEngine_Common_Fix64_Fields ten; // 0x68
        struct FrameEngine_Common_Fix64_Fields hundred; // 0x70
        struct FrameEngine_Common_Fix64_Fields fivehundred; // 0x78
        struct FrameEngine_Common_Fix64_Fields thousand; // 0x80
        struct FrameEngine_Common_Fix64_Fields myriad; // 0x88
        struct FrameEngine_Common_Fix64_Fields half; // 0x90
        struct FrameEngine_Common_Fix64_Fields third; // 0x98
        struct FrameEngine_Common_Fix64_Fields quarter; // 0xA0
        struct FrameEngine_Common_Fix64_Fields fifth; // 0xA8
        struct FrameEngine_Common_Fix64_Fields sixth; // 0xB0
        struct FrameEngine_Common_Fix64_Fields seventh; // 0xB8
        struct FrameEngine_Common_Fix64_Fields eighth; // 0xC0
        struct FrameEngine_Common_Fix64_Fields ninth; // 0xC8
        struct FrameEngine_Common_Fix64_Fields tenth; // 0xD0
        struct FrameEngine_Common_Fix64_Fields point1; // 0xD8
        struct FrameEngine_Common_Fix64_Fields point2; // 0xE0
        struct FrameEngine_Common_Fix64_Fields point3; // 0xE8
        struct FrameEngine_Common_Fix64_Fields point4; // 0xF0
        struct FrameEngine_Common_Fix64_Fields point5; // 0xF8
        struct FrameEngine_Common_Fix64_Fields point6; // 0x100
        struct FrameEngine_Common_Fix64_Fields point7; // 0x108
        struct FrameEngine_Common_Fix64_Fields point8; // 0x110
        struct FrameEngine_Common_Fix64_Fields point9; // 0x118
        struct FrameEngine_Common_Fix64_Fields pi; // 0x120
        struct FrameEngine_Common_Fix64_Fields pi_mul2; // 0x128
        struct FrameEngine_Common_Fix64_Fields pi_div2; // 0x130
        struct FrameEngine_Common_Fix64_Fields pi_inv; // 0x138
        struct FrameEngine_Common_Fix64_Fields pi_sqr; // 0x140
        struct FrameEngine_Common_Fix64_Fields deg2rad; // 0x148
        struct FrameEngine_Common_Fix64_Fields rad2deg; // 0x150
        struct FrameEngine_Common_Fix64_Fields epsilon; // 0x158
        struct FrameEngine_Common_Fix64_Fields epsilon10; // 0x160
        struct FrameEngine_Common_Fix64_Fields epsilon100; // 0x168
        struct FrameEngine_Common_Fix64_Fields epsilon128; // 0x170
        struct FrameEngine_Common_Fix64_Fields epsilon1000; // 0x178
        struct FrameEngine_Common_Fix64_Fields E; // 0x180
        float divideOfOne; // 0x188
    };

    struct UIMiniIconBaseCtrl_Fields : BattleUINode_Fields {
        struct ImageEx_o* icon; // 0x48
        struct ImageEx_o* border; // 0x50
        struct ImageEx_o* spIcon; // 0x58
        struct FrameEngine_Visual_FixGameObjectVisi_o* followObj; // 0x60
        struct UIMiniMapIconCtrl_o* miniMapIconCtrl; // 0x68
        struct UIMiniMapStrategicPoint_o* miniMapSPCtrl; // 0x70
        struct FrameEngine_Visual_BattleActorVisi_o* actor; // 0x78
        int32_t iconType; // 0x80
        bool isHero; // 0x84
        int32_t baseSortIndex_k_BackingField; // 0x88
        int32_t originalSortIndex_k_BackingField; // 0x8C
        struct UnityEngine_RectTransform_o* cachedTransform; // 0x90
        struct UnityEngine_RectTransform_o* cachedIconTran; // 0x98
        bool isInitiate; // 0xA0
        bool isShowing; // 0xA1
        float baseScale; // 0xA4
        bool isBeRecyle_k_BackingField; // 0xA8
        struct UnityEngine_CanvasRenderer_array* canvasRenderers; // 0xB0
        int32_t lastUpdatePer5FrameCount; // 0xB8
        struct FrameEngine_Common_FixVector3_Fix64_Fields cacheFollowPos; // 0xC0
        struct LAssetGameObject_o* teleportEffectItem; // 0xD8
        struct LAssetGameObject_o* teleportCDEffectItem; // 0xE0
        int32_t teleportCooldownFrame; // 0xE8
        bool bShowTeleport; // 0xEC
        bool bTeleportCooling; // 0xED
        struct LAssetGameObject_o* currentHurtEffectItem; // 0xF0
        struct System_Action_System_Object_o* deleCacheOf_OnFreeHurtEffect; // 0xF8
    };
    struct UIMiniIconBaseCtrl_VTable {
        VirtualInvokeData _0_dummy;
    };
    struct UIMiniIconBaseCtrl_c {
        Il2CppClass_1 _1;
        struct UIMiniIconBaseCtrl_StaticFields* static_fields;
        Il2CppRGCTXData* rgctx_data;
        Il2CppClass_2 _2;
        UIMiniIconBaseCtrl_VTable vtable;
    };
    struct UIMiniIconBaseCtrl_o {
        UIMiniIconBaseCtrl_c *klass;
        void *monitor;
        UIMiniIconBaseCtrl_Fields fields;
    };
    struct UIMiniIconBaseCtrl_StaticFields {
        struct FrameEngine_Common_FixVector3_Fix64_Fields INVALID_POSITION; // 0x0
        struct System_String_array* HURT_EFFECT_PATH_LIST; // 0x18
        struct FrameEngine_Common_Fix64_Fields s_MinSqrMagnitudeToChangePos; // 0x20
        struct System_String_o* teleportSourceEffectPath; // 0x0
        struct System_String_o* teleportCDEffectPath; // 0x0
    };



};

#endif //DOBBY_PROJECT_LOLHEADER_H
