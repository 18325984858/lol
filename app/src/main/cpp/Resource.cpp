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

struct _Module__Fields {
};
struct _Module__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct _Module__o {
    _Module__c *klass;
    void *monitor;
    _Module__Fields fields;
};
struct Interop_Error_Fields {
    int32_t value__;
};
struct Interop_Error_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct Interop_Error_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Error_VTable vtable;
};
struct Interop_Error_o {
    Interop_Error_Fields fields;
};
struct Interop_ErrorInfo_Fields {
    int32_t _error;
    int32_t _rawErrno;
};
struct Interop_ErrorInfo_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Interop_ErrorInfo_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_ErrorInfo_VTable vtable;
};
struct Interop_ErrorInfo_o {
    Interop_ErrorInfo_Fields fields;
};
struct Interop_Sys_NodeType_Fields {
    int32_t value__;
};
struct Interop_Sys_NodeType_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct Interop_Sys_NodeType_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Sys_NodeType_VTable vtable;
};
struct Interop_Sys_NodeType_o {
    Interop_Sys_NodeType_Fields fields;
};
struct Interop_Sys_DirectoryEntry_Fields {
    uint8_t* Name;
    int32_t NameLength;
    int32_t InodeType;
};
struct Interop_Sys_DirectoryEntry_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Interop_Sys_DirectoryEntry_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Sys_DirectoryEntry_VTable vtable;
};
struct Interop_Sys_DirectoryEntry_o {
    Interop_Sys_DirectoryEntry_Fields fields;
};
struct Interop_Sys_FileStatus_Fields {
    int32_t Flags;
    int32_t Mode;
    uint32_t Uid;
    uint32_t Gid;
    int64_t Size;
    int64_t ATime;
    int64_t ATimeNsec;
    int64_t MTime;
    int64_t MTimeNsec;
    int64_t CTime;
    int64_t CTimeNsec;
    int64_t BirthTime;
    int64_t BirthTimeNsec;
    int64_t Dev;
    int64_t Ino;
    uint32_t UserFlags;
};
struct Interop_Sys_FileStatus_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Interop_Sys_FileStatus_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Sys_FileStatus_VTable vtable;
};
struct Interop_Sys_FileStatus_o {
    Interop_Sys_FileStatus_Fields fields;
};
struct Interop_Sys_FileStatusFlags_Fields {
    int32_t value__;
};
struct Interop_Sys_FileStatusFlags_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct Interop_Sys_FileStatusFlags_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Sys_FileStatusFlags_VTable vtable;
};
struct Interop_Sys_FileStatusFlags_o {
    Interop_Sys_FileStatusFlags_Fields fields;
};
struct Interop_Sys_Permissions_Fields {
    int32_t value__;
};
struct Interop_Sys_Permissions_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct Interop_Sys_Permissions_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Sys_Permissions_VTable vtable;
};
struct Interop_Sys_Permissions_o {
    Interop_Sys_Permissions_Fields fields;
};
struct Interop_Sys_Fields {
};
struct Interop_Sys_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Interop_Sys_c {
    Il2CppClass_1 _1;
    struct Interop_Sys_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_Sys_VTable vtable;
};
struct Interop_Sys_o {
    Interop_Sys_c *klass;
    void *monitor;
    Interop_Sys_Fields fields;
};
struct Interop_Sys_StaticFields {
    bool CanSetHiddenFlag;
};
struct Interop_Fields {
};
struct Interop_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Interop_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Interop_VTable vtable;
};
struct Interop_o {
    Interop_c *klass;
    void *monitor;
    Interop_Fields fields;
};
struct InteropErrorExtensions_Fields {
};
struct InteropErrorExtensions_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct InteropErrorExtensions_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    InteropErrorExtensions_VTable vtable;
};
struct InteropErrorExtensions_o {
    InteropErrorExtensions_c *klass;
    void *monitor;
    InteropErrorExtensions_Fields fields;
};
struct Locale_Fields {
};
struct Locale_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Locale_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Locale_VTable vtable;
};
struct Locale_o {
    Locale_c *klass;
    void *monitor;
    Locale_Fields fields;
};
struct SR_Fields {
};
struct SR_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct SR_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    SR_VTable vtable;
};
struct SR_o {
    SR_c *klass;
    void *monitor;
    SR_Fields fields;
};
struct Mono_Runtime_Fields {
};
struct Mono_Runtime_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Runtime_c {
    Il2CppClass_1 _1;
    struct Mono_Runtime_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Runtime_VTable vtable;
};
struct Mono_Runtime_o {
    Mono_Runtime_c *klass;
    void *monitor;
    Mono_Runtime_Fields fields;
};
struct Mono_Runtime_StaticFields {
    Il2CppObject* dump;
};
struct Mono_RuntimeClassHandle_Fields {
    struct Mono_RuntimeStructs_MonoClass_o* value;
};
struct Mono_RuntimeClassHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeClassHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeClassHandle_VTable vtable;
};
struct Mono_RuntimeClassHandle_o {
    Mono_RuntimeClassHandle_Fields fields;
};
struct Mono_RuntimeRemoteClassHandle_Fields {
    struct Mono_RuntimeStructs_RemoteClass_o* value;
};
struct Mono_RuntimeRemoteClassHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeRemoteClassHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeRemoteClassHandle_VTable vtable;
};
struct Mono_RuntimeRemoteClassHandle_o {
    Mono_RuntimeRemoteClassHandle_Fields fields;
};
struct Mono_RuntimeGenericParamInfoHandle_Fields {
    struct Mono_RuntimeStructs_GenericParamInfo_o* value;
};
struct Mono_RuntimeGenericParamInfoHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeGenericParamInfoHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeGenericParamInfoHandle_VTable vtable;
};
struct Mono_RuntimeGenericParamInfoHandle_o {
    Mono_RuntimeGenericParamInfoHandle_Fields fields;
};
struct Mono_RuntimeEventHandle_Fields {
    intptr_t value;
};
struct Mono_RuntimeEventHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeEventHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeEventHandle_VTable vtable;
};
struct Mono_RuntimeEventHandle_o {
    Mono_RuntimeEventHandle_Fields fields;
};
struct Mono_RuntimePropertyHandle_Fields {
    intptr_t value;
};
struct Mono_RuntimePropertyHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimePropertyHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimePropertyHandle_VTable vtable;
};
struct Mono_RuntimePropertyHandle_o {
    Mono_RuntimePropertyHandle_Fields fields;
};
struct Mono_RuntimeGPtrArrayHandle_Fields {
    struct Mono_RuntimeStructs_GPtrArray_o* value;
};
struct Mono_RuntimeGPtrArrayHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeGPtrArrayHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeGPtrArrayHandle_VTable vtable;
};
struct Mono_RuntimeGPtrArrayHandle_o {
    Mono_RuntimeGPtrArrayHandle_Fields fields;
};
struct Mono_RuntimeMarshal_Fields {
};
struct Mono_RuntimeMarshal_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeMarshal_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeMarshal_VTable vtable;
};
struct Mono_RuntimeMarshal_o {
    Mono_RuntimeMarshal_c *klass;
    void *monitor;
    Mono_RuntimeMarshal_Fields fields;
};
struct Mono_RuntimeStructs_RemoteClass_Fields {
    intptr_t default_vtable;
    intptr_t xdomain_vtable;
    struct Mono_RuntimeStructs_MonoClass_o* proxy_class;
    intptr_t proxy_class_name;
    uint32_t interface_count;
};
struct Mono_RuntimeStructs_RemoteClass_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeStructs_RemoteClass_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeStructs_RemoteClass_VTable vtable;
};
struct Mono_RuntimeStructs_RemoteClass_o {
    Mono_RuntimeStructs_RemoteClass_Fields fields;
};
struct Mono_RuntimeStructs_MonoClass_Fields {
};
struct Mono_RuntimeStructs_MonoClass_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeStructs_MonoClass_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeStructs_MonoClass_VTable vtable;
};
struct Mono_RuntimeStructs_MonoClass_o {
    Mono_RuntimeStructs_MonoClass_Fields fields;
};
struct Mono_RuntimeStructs_GenericParamInfo_Fields {
    struct Mono_RuntimeStructs_MonoClass_o* pklass;
    intptr_t name;
    uint16_t flags;
    uint32_t token;
    struct Mono_RuntimeStructs_MonoClass_o** constraints;
};
struct Mono_RuntimeStructs_GenericParamInfo_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeStructs_GenericParamInfo_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeStructs_GenericParamInfo_VTable vtable;
};
struct Mono_RuntimeStructs_GenericParamInfo_o {
    Mono_RuntimeStructs_GenericParamInfo_Fields fields;
};
struct Mono_RuntimeStructs_GPtrArray_Fields {
    intptr_t* data;
    int32_t len;
};
struct Mono_RuntimeStructs_GPtrArray_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeStructs_GPtrArray_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeStructs_GPtrArray_VTable vtable;
};
struct Mono_RuntimeStructs_GPtrArray_o {
    Mono_RuntimeStructs_GPtrArray_Fields fields;
};
struct Mono_RuntimeStructs_Fields {
};
struct Mono_RuntimeStructs_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_RuntimeStructs_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_RuntimeStructs_VTable vtable;
};
struct Mono_RuntimeStructs_o {
    Mono_RuntimeStructs_c *klass;
    void *monitor;
    Mono_RuntimeStructs_Fields fields;
};
struct Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_Fields {
    uint8_t FixedElementField;
};
struct Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_VTable vtable;
};
struct Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_o {
    Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_Fields fields;
};
struct Mono_MonoAssemblyName_Fields {
    intptr_t name;
    intptr_t culture;
    intptr_t hash_value;
    intptr_t public_key;
    struct Mono_MonoAssemblyName__public_key_token_e__FixedBuffer_o public_key_token;
    uint32_t hash_alg;
    uint32_t hash_len;
    uint32_t flags;
    uint16_t major;
    uint16_t minor;
    uint16_t build;
    uint16_t revision;
    uint16_t arch;
};
struct Mono_MonoAssemblyName_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_MonoAssemblyName_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_MonoAssemblyName_VTable vtable;
};
struct Mono_MonoAssemblyName_o {
    Mono_MonoAssemblyName_Fields fields;
};
struct Mono_ValueTuple_Fields {
};
struct Mono_ValueTuple_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_ValueTuple_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_ValueTuple_VTable vtable;
};
struct Mono_ValueTuple_o {
    Mono_ValueTuple_Fields fields;
};
struct Mono_ValueTuple_T1__Fields {
    Il2CppObject* Item1;
};
struct Mono_ValueTuple_T1__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_ValueTuple_T1__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_ValueTuple_T1__VTable vtable;
};
struct Mono_ValueTuple_T1__o {
    Mono_ValueTuple_T1__Fields fields;
};
struct Mono_ValueTuple_T1__T2__Fields {
    Il2CppObject* Item1;
    Il2CppObject* Item2;
};
struct Mono_ValueTuple_T1__T2__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_ValueTuple_T1__T2__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_ValueTuple_T1__T2__VTable vtable;
};
struct Mono_ValueTuple_T1__T2__o {
    Mono_ValueTuple_T1__T2__Fields fields;
};
struct Mono_ValueTuple_T1__T2__T3__Fields {
    Il2CppObject* Item1;
    Il2CppObject* Item2;
    Il2CppObject* Item3;
};
struct Mono_ValueTuple_T1__T2__T3__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_ValueTuple_T1__T2__T3__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_ValueTuple_T1__T2__T3__VTable vtable;
};
struct Mono_ValueTuple_T1__T2__T3__o {
    Mono_ValueTuple_T1__T2__T3__Fields fields;
};
struct Mono_ValueTuple_T1__T2__T3__T4__Fields {
    Il2CppObject* Item1;
    Il2CppObject* Item2;
    Il2CppObject* Item3;
    Il2CppObject* Item4;
};
struct Mono_ValueTuple_T1__T2__T3__T4__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_ValueTuple_T1__T2__T3__T4__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_ValueTuple_T1__T2__T3__T4__VTable vtable;
};
struct Mono_ValueTuple_T1__T2__T3__T4__o {
    Mono_ValueTuple_T1__T2__T3__T4__Fields fields;
};
struct Mono_ValueTuple_T1__T2__T3__T4__T5__Fields {
    Il2CppObject* Item1;
    Il2CppObject* Item2;
    Il2CppObject* Item3;
    Il2CppObject* Item4;
    Il2CppObject* Item5;
};
struct Mono_ValueTuple_T1__T2__T3__T4__T5__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_ValueTuple_T1__T2__T3__T4__T5__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_ValueTuple_T1__T2__T3__T4__T5__VTable vtable;
};
struct Mono_ValueTuple_T1__T2__T3__T4__T5__o {
    Mono_ValueTuple_T1__T2__T3__T4__T5__Fields fields;
};
struct Mono_SafeGPtrArrayHandle_Fields {
    struct Mono_RuntimeGPtrArrayHandle_o handle;
};
struct Mono_SafeGPtrArrayHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Dispose;
};
struct Mono_SafeGPtrArrayHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_SafeGPtrArrayHandle_VTable vtable;
};
struct Mono_SafeGPtrArrayHandle_o {
    Mono_SafeGPtrArrayHandle_Fields fields;
};
struct Mono_SafeStringMarshal_Fields {
    struct System_String_o* str;
    intptr_t marshaled_string;
};
struct Mono_SafeStringMarshal_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Dispose;
};
struct Mono_SafeStringMarshal_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_SafeStringMarshal_VTable vtable;
};
struct Mono_SafeStringMarshal_o {
    Mono_SafeStringMarshal_Fields fields;
};
struct Mono_Xml_SmallXmlParser_IContentHandler_Fields {
};
struct Mono_Xml_SmallXmlParser_IContentHandler_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct Mono_Xml_SmallXmlParser_IContentHandler_o {
    Mono_Xml_SmallXmlParser_IContentHandler_c *klass;
    void *monitor;
    Mono_Xml_SmallXmlParser_IContentHandler_Fields fields;
};
struct Mono_Xml_SmallXmlParser_IAttrList_Fields {
};
struct Mono_Xml_SmallXmlParser_IAttrList_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct Mono_Xml_SmallXmlParser_IAttrList_o {
    Mono_Xml_SmallXmlParser_IAttrList_c *klass;
    void *monitor;
    Mono_Xml_SmallXmlParser_IAttrList_Fields fields;
};
struct Mono_Xml_SmallXmlParser_AttrListImpl_Fields {
    struct System_Collections_Generic_List_string__o* attrNames;
    struct System_Collections_Generic_List_string__o* attrValues;
};
struct Mono_Xml_SmallXmlParser_AttrListImpl_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_GetValue;
    VirtualInvokeData _5_GetValue;
    VirtualInvokeData _6_get_Names;
    VirtualInvokeData _7_get_Values;
};
struct Mono_Xml_SmallXmlParser_AttrListImpl_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Xml_SmallXmlParser_AttrListImpl_VTable vtable;
};
struct Mono_Xml_SmallXmlParser_AttrListImpl_o {
    Mono_Xml_SmallXmlParser_AttrListImpl_c *klass;
    void *monitor;
    Mono_Xml_SmallXmlParser_AttrListImpl_Fields fields;
};
struct Mono_Xml_SmallXmlParser_Fields {
    struct Mono_Xml_SmallXmlParser_IContentHandler_o* handler;
    struct System_IO_TextReader_o* reader;
    struct System_Collections_Stack_o* elementNames;
    struct System_Collections_Stack_o* xmlSpaces;
    struct System_String_o* xmlSpace;
    struct System_Text_StringBuilder_o* buffer;
    struct System_Char_array* nameBuffer;
    bool isWhitespace;
    struct Mono_Xml_SmallXmlParser_AttrListImpl_o* attributes;
    int32_t line;
    int32_t column;
    bool resetColumn;
};
struct Mono_Xml_SmallXmlParser_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Xml_SmallXmlParser_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Xml_SmallXmlParser_VTable vtable;
};
struct Mono_Xml_SmallXmlParser_o {
    Mono_Xml_SmallXmlParser_c *klass;
    void *monitor;
    Mono_Xml_SmallXmlParser_Fields fields;
};
struct System_Exception_Fields {
    struct System_String_o* _className;
    struct System_String_o* _message;
    struct System_Collections_IDictionary_o* _data;
    struct System_Exception_o* _innerException;
    struct System_String_o* _helpURL;
    Il2CppObject* _stackTrace;
    struct System_String_o* _stackTraceString;
    struct System_String_o* _remoteStackTraceString;
    int32_t _remoteStackIndex;
    Il2CppObject* _dynamicMethods;
    int32_t _HResult;
    struct System_String_o* _source;
    struct System_Runtime_Serialization_SafeSerializationManager_o* _safeSerializationManager;
    struct System_Diagnostics_StackTrace_array* captured_traces;
    struct System_IntPtr_array* native_trace_ips;
    int32_t caught_in_unmanaged;
};
struct System_Exception_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_Exception_c {
    Il2CppClass_1 _1;
    struct System_Exception_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Exception_VTable vtable;
};
struct System_Exception_o {
    System_Exception_c *klass;
    void *monitor;
    System_Exception_Fields fields;
};
struct System_Exception_StaticFields {
    Il2CppObject* s_EDILock;
};
struct System_SystemException_Fields : System_Exception_Fields {
};
struct System_SystemException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_SystemException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_SystemException_VTable vtable;
};
struct System_SystemException_o {
    System_SystemException_c *klass;
    void *monitor;
    System_SystemException_Fields fields;
};
struct Mono_Xml_SmallXmlParserException_Fields : System_SystemException_Fields {
    int32_t line;
    int32_t column;
};
struct Mono_Xml_SmallXmlParserException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct Mono_Xml_SmallXmlParserException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Xml_SmallXmlParserException_VTable vtable;
};
struct Mono_Xml_SmallXmlParserException_o {
    Mono_Xml_SmallXmlParserException_c *klass;
    void *monitor;
    Mono_Xml_SmallXmlParserException_Fields fields;
};
struct System_Attribute_Fields {
};
struct System_Attribute_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Attribute_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Attribute_VTable vtable;
};
struct System_Attribute_o {
    System_Attribute_c *klass;
    void *monitor;
    System_Attribute_Fields fields;
};
struct Mono_Interop_MonoPInvokeCallbackAttribute_Fields : System_Attribute_Fields {
};
struct Mono_Interop_MonoPInvokeCallbackAttribute_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Interop_MonoPInvokeCallbackAttribute_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Interop_MonoPInvokeCallbackAttribute_VTable vtable;
};
struct Mono_Interop_MonoPInvokeCallbackAttribute_o {
    Mono_Interop_MonoPInvokeCallbackAttribute_c *klass;
    void *monitor;
    Mono_Interop_MonoPInvokeCallbackAttribute_Fields fields;
};
struct Mono_Globalization_Unicode_CodePointIndexer_TableRange_Fields {
    int32_t Start;
    int32_t End;
    int32_t Count;
    int32_t IndexStart;
    int32_t IndexEnd;
};
struct Mono_Globalization_Unicode_CodePointIndexer_TableRange_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_CodePointIndexer_TableRange_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_CodePointIndexer_TableRange_VTable vtable;
};
struct Mono_Globalization_Unicode_CodePointIndexer_TableRange_o {
    Mono_Globalization_Unicode_CodePointIndexer_TableRange_Fields fields;
};
struct Mono_Globalization_Unicode_CodePointIndexer_Fields {
    struct Mono_Globalization_Unicode_CodePointIndexer_TableRange_array* ranges;
    int32_t TotalCount;
    int32_t defaultIndex;
    int32_t defaultCP;
};
struct Mono_Globalization_Unicode_CodePointIndexer_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_CodePointIndexer_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_CodePointIndexer_VTable vtable;
};
struct Mono_Globalization_Unicode_CodePointIndexer_o {
    Mono_Globalization_Unicode_CodePointIndexer_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_CodePointIndexer_Fields fields;
};
struct Mono_Globalization_Unicode_TailoringInfo_Fields {
    int32_t LCID;
    int32_t TailoringIndex;
    int32_t TailoringCount;
    bool FrenchSort;
};
struct Mono_Globalization_Unicode_TailoringInfo_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_TailoringInfo_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_TailoringInfo_VTable vtable;
};
struct Mono_Globalization_Unicode_TailoringInfo_o {
    Mono_Globalization_Unicode_TailoringInfo_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_TailoringInfo_Fields fields;
};
struct Mono_Globalization_Unicode_Contraction_Fields {
    int32_t Index;
    struct System_Char_array* Source;
    struct System_String_o* Replacement;
    struct System_Byte_array* SortKey;
};
struct Mono_Globalization_Unicode_Contraction_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_Contraction_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_Contraction_VTable vtable;
};
struct Mono_Globalization_Unicode_Contraction_o {
    Mono_Globalization_Unicode_Contraction_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_Contraction_Fields fields;
};
struct Mono_Globalization_Unicode_ContractionComparer_Fields {
};
struct Mono_Globalization_Unicode_ContractionComparer_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Compare;
};
struct Mono_Globalization_Unicode_ContractionComparer_c {
    Il2CppClass_1 _1;
    struct Mono_Globalization_Unicode_ContractionComparer_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_ContractionComparer_VTable vtable;
};
struct Mono_Globalization_Unicode_ContractionComparer_o {
    Mono_Globalization_Unicode_ContractionComparer_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_ContractionComparer_Fields fields;
};
struct Mono_Globalization_Unicode_ContractionComparer_StaticFields {
    struct Mono_Globalization_Unicode_ContractionComparer_o* Instance;
};
struct Mono_Globalization_Unicode_Level2Map_Fields {
    uint8_t Source;
    uint8_t Replace;
};
struct Mono_Globalization_Unicode_Level2Map_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_Level2Map_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_Level2Map_VTable vtable;
};
struct Mono_Globalization_Unicode_Level2Map_o {
    Mono_Globalization_Unicode_Level2Map_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_Level2Map_Fields fields;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_Fields {
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_c {
    Il2CppClass_1 _1;
    struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_MSCompatUnicodeTable___c_VTable vtable;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_o {
    Mono_Globalization_Unicode_MSCompatUnicodeTable___c_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_MSCompatUnicodeTable___c_Fields fields;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_StaticFields {
    struct Mono_Globalization_Unicode_MSCompatUnicodeTable___c_o* __9;
    struct System_Comparison_Level2Map__o* __9__17_0;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable_Fields {
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable_c {
    Il2CppClass_1 _1;
    struct Mono_Globalization_Unicode_MSCompatUnicodeTable_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_MSCompatUnicodeTable_VTable vtable;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable_o {
    Mono_Globalization_Unicode_MSCompatUnicodeTable_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_MSCompatUnicodeTable_Fields fields;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTable_StaticFields {
    int32_t MaxExpansionLength;
    uint8_t* ignorableFlags;
    uint8_t* categories;
    uint8_t* level1;
    uint8_t* level2;
    uint8_t* level3;
    uint8_t* cjkCHScategory;
    uint8_t* cjkCHTcategory;
    uint8_t* cjkJAcategory;
    uint8_t* cjkKOcategory;
    uint8_t* cjkCHSlv1;
    uint8_t* cjkCHTlv1;
    uint8_t* cjkJAlv1;
    uint8_t* cjkKOlv1;
    uint8_t* cjkKOlv2;
    struct System_Char_array* tailoringArr;
    struct Mono_Globalization_Unicode_TailoringInfo_array* tailoringInfos;
    Il2CppObject* forLock;
    bool isReady;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_Fields {
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_c {
    Il2CppClass_1 _1;
    struct Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_VTable vtable;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_o {
    Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_Fields fields;
};
struct Mono_Globalization_Unicode_MSCompatUnicodeTableUtil_StaticFields {
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Ignorable;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Category;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Level1;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Level2;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Level3;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* CjkCHS;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Cjk;
};
struct Mono_Globalization_Unicode_NormalizationTableUtil_Fields {
};
struct Mono_Globalization_Unicode_NormalizationTableUtil_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_NormalizationTableUtil_c {
    Il2CppClass_1 _1;
    struct Mono_Globalization_Unicode_NormalizationTableUtil_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_NormalizationTableUtil_VTable vtable;
};
struct Mono_Globalization_Unicode_NormalizationTableUtil_o {
    Mono_Globalization_Unicode_NormalizationTableUtil_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_NormalizationTableUtil_Fields fields;
};
struct Mono_Globalization_Unicode_NormalizationTableUtil_StaticFields {
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Prop;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Map;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Combining;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Composite;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* Helper;
};
struct Mono_Globalization_Unicode_SimpleCollator_Context_Fields {
    int32_t Option;
    uint8_t* NeverMatchFlags;
    uint8_t* AlwaysMatchFlags;
    uint8_t* Buffer1;
    uint8_t* Buffer2;
    int32_t PrevCode;
    uint8_t* PrevSortKey;
};
struct Mono_Globalization_Unicode_SimpleCollator_Context_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_SimpleCollator_Context_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_SimpleCollator_Context_VTable vtable;
};
struct Mono_Globalization_Unicode_SimpleCollator_Context_o {
    Mono_Globalization_Unicode_SimpleCollator_Context_Fields fields;
};
struct Mono_Globalization_Unicode_SimpleCollator_PreviousInfo_Fields {
    int32_t Code;
    uint8_t* SortKey;
};
struct Mono_Globalization_Unicode_SimpleCollator_PreviousInfo_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_SimpleCollator_PreviousInfo_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_SimpleCollator_PreviousInfo_VTable vtable;
};
struct Mono_Globalization_Unicode_SimpleCollator_PreviousInfo_o {
    Mono_Globalization_Unicode_SimpleCollator_PreviousInfo_Fields fields;
};
struct Mono_Globalization_Unicode_SimpleCollator_Escape_Fields {
    struct System_String_o* Source;
    int32_t Index;
    int32_t Start;
    int32_t End;
    int32_t Optional;
};
struct Mono_Globalization_Unicode_SimpleCollator_Escape_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_SimpleCollator_Escape_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_SimpleCollator_Escape_VTable vtable;
};
struct Mono_Globalization_Unicode_SimpleCollator_Escape_o {
    Mono_Globalization_Unicode_SimpleCollator_Escape_Fields fields;
};
struct Mono_Globalization_Unicode_SimpleCollator_ExtenderType_Fields {
    int32_t value__;
};
struct Mono_Globalization_Unicode_SimpleCollator_ExtenderType_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct Mono_Globalization_Unicode_SimpleCollator_ExtenderType_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_SimpleCollator_ExtenderType_VTable vtable;
};
struct Mono_Globalization_Unicode_SimpleCollator_ExtenderType_o {
    Mono_Globalization_Unicode_SimpleCollator_ExtenderType_Fields fields;
};
struct Mono_Globalization_Unicode_SimpleCollator_Fields {
    struct System_Globalization_TextInfo_o* textInfo;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* cjkIndexer;
    struct Mono_Globalization_Unicode_Contraction_array* contractions;
    struct Mono_Globalization_Unicode_Level2Map_array* level2Maps;
    struct System_Byte_array* unsafeFlags;
    uint8_t* cjkCatTable;
    uint8_t* cjkLv1Table;
    uint8_t* cjkLv2Table;
    struct Mono_Globalization_Unicode_CodePointIndexer_o* cjkLv2Indexer;
    int32_t lcid;
    bool frenchSort;
};
struct Mono_Globalization_Unicode_SimpleCollator_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_GetSortKey;
    VirtualInvokeData _5_System_Globalization_ISimpleCollator_Compare;
    VirtualInvokeData _6_IsPrefix;
    VirtualInvokeData _7_IsSuffix;
    VirtualInvokeData _8_IndexOf;
    VirtualInvokeData _9_LastIndexOf;
};
struct Mono_Globalization_Unicode_SimpleCollator_c {
    Il2CppClass_1 _1;
    struct Mono_Globalization_Unicode_SimpleCollator_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_SimpleCollator_VTable vtable;
};
struct Mono_Globalization_Unicode_SimpleCollator_o {
    Mono_Globalization_Unicode_SimpleCollator_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_SimpleCollator_Fields fields;
};
struct Mono_Globalization_Unicode_SimpleCollator_StaticFields {
    struct Mono_Globalization_Unicode_SimpleCollator_o* invariant;
};
struct Mono_Globalization_Unicode_SortKeyBuffer_Fields {
    struct System_Byte_array* l1b;
    struct System_Byte_array* l2b;
    struct System_Byte_array* l3b;
    struct System_Byte_array* l4sb;
    struct System_Byte_array* l4tb;
    struct System_Byte_array* l4kb;
    struct System_Byte_array* l4wb;
    struct System_Byte_array* l5b;
    struct System_String_o* source;
    int32_t l1;
    int32_t l2;
    int32_t l3;
    int32_t l4s;
    int32_t l4t;
    int32_t l4k;
    int32_t l4w;
    int32_t l5;
    int32_t lcid;
    int32_t options;
    bool processLevel2;
    bool frenchSort;
    bool frenchSorted;
};
struct Mono_Globalization_Unicode_SortKeyBuffer_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Globalization_Unicode_SortKeyBuffer_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Globalization_Unicode_SortKeyBuffer_VTable vtable;
};
struct Mono_Globalization_Unicode_SortKeyBuffer_o {
    Mono_Globalization_Unicode_SortKeyBuffer_c *klass;
    void *monitor;
    Mono_Globalization_Unicode_SortKeyBuffer_Fields fields;
};
struct Mono_Security_BitConverterLE_Fields {
};
struct Mono_Security_BitConverterLE_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Mono_Security_BitConverterLE_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Mono_Security_BitConverterLE_VTable vtable;
};
struct Mono_Security_BitConverterLE_o {
    Mono_Security_BitConverterLE_c *klass;
    void *monitor;
    Mono_Security_BitConverterLE_Fields fields;
};
struct Microsoft_Win32_Win32Native_Fields {
};
struct Microsoft_Win32_Win32Native_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Microsoft_Win32_Win32Native_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Microsoft_Win32_Win32Native_VTable vtable;
};
struct Microsoft_Win32_Win32Native_o {
    Microsoft_Win32_Win32Native_c *klass;
    void *monitor;
    Microsoft_Win32_Win32Native_Fields fields;
};
struct System_Runtime_ConstrainedExecution_CriticalFinalizerObject_Fields {
};
struct System_Runtime_ConstrainedExecution_CriticalFinalizerObject_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Runtime_ConstrainedExecution_CriticalFinalizerObject_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Runtime_ConstrainedExecution_CriticalFinalizerObject_VTable vtable;
};
struct System_Runtime_ConstrainedExecution_CriticalFinalizerObject_o {
    System_Runtime_ConstrainedExecution_CriticalFinalizerObject_c *klass;
    void *monitor;
    System_Runtime_ConstrainedExecution_CriticalFinalizerObject_Fields fields;
};
struct System_Runtime_InteropServices_SafeHandle_Fields : System_Runtime_ConstrainedExecution_CriticalFinalizerObject_Fields {
    intptr_t handle;
    int32_t _state;
    bool _ownsHandle;
    bool _fullyInitialized;
};
struct System_Runtime_InteropServices_SafeHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Dispose;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Dispose;
};
struct System_Runtime_InteropServices_SafeHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Runtime_InteropServices_SafeHandle_VTable vtable;
};
struct System_Runtime_InteropServices_SafeHandle_o {
    System_Runtime_InteropServices_SafeHandle_c *klass;
    void *monitor;
    System_Runtime_InteropServices_SafeHandle_Fields fields;
};
struct Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_Fields : System_Runtime_InteropServices_SafeHandle_Fields {
};
struct Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Dispose;
    VirtualInvokeData _5_get_IsInvalid;
    VirtualInvokeData _6_Dispose;
};
struct Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_VTable vtable;
};
struct Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_o {
    Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_c *klass;
    void *monitor;
    Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_Fields fields;
};
struct Microsoft_Win32_SafeHandles_SafeFileHandle_Fields : Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_Fields {
};
struct Microsoft_Win32_SafeHandles_SafeFileHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Dispose;
    VirtualInvokeData _5_get_IsInvalid;
    VirtualInvokeData _6_Dispose;
    VirtualInvokeData _7_ReleaseHandle;
};
struct Microsoft_Win32_SafeHandles_SafeFileHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Microsoft_Win32_SafeHandles_SafeFileHandle_VTable vtable;
};
struct Microsoft_Win32_SafeHandles_SafeFileHandle_o {
    Microsoft_Win32_SafeHandles_SafeFileHandle_c *klass;
    void *monitor;
    Microsoft_Win32_SafeHandles_SafeFileHandle_Fields fields;
};
struct Microsoft_Win32_SafeHandles_SafeWaitHandle_Fields : Microsoft_Win32_SafeHandles_SafeHandleZeroOrMinusOneIsInvalid_Fields {
};
struct Microsoft_Win32_SafeHandles_SafeWaitHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Dispose;
    VirtualInvokeData _5_get_IsInvalid;
    VirtualInvokeData _6_Dispose;
    VirtualInvokeData _7_ReleaseHandle;
};
struct Microsoft_Win32_SafeHandles_SafeWaitHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Microsoft_Win32_SafeHandles_SafeWaitHandle_VTable vtable;
};
struct Microsoft_Win32_SafeHandles_SafeWaitHandle_o {
    Microsoft_Win32_SafeHandles_SafeWaitHandle_c *klass;
    void *monitor;
    Microsoft_Win32_SafeHandles_SafeWaitHandle_Fields fields;
};
struct Internal_Threading_Tasks_Tracing_TaskTrace_Fields {
};
struct Internal_Threading_Tasks_Tracing_TaskTrace_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Internal_Threading_Tasks_Tracing_TaskTrace_c {
    Il2CppClass_1 _1;
    struct Internal_Threading_Tasks_Tracing_TaskTrace_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Internal_Threading_Tasks_Tracing_TaskTrace_VTable vtable;
};
struct Internal_Threading_Tasks_Tracing_TaskTrace_o {
    Internal_Threading_Tasks_Tracing_TaskTrace_c *klass;
    void *monitor;
    Internal_Threading_Tasks_Tracing_TaskTrace_Fields fields;
};
struct Internal_Threading_Tasks_Tracing_TaskTrace_StaticFields {
    struct Internal_Runtime_Augments_TaskTraceCallbacks_o* s_callbacks;
};
struct Internal_Runtime_Augments_AsyncStatus_Fields {
    int32_t value__;
};
struct Internal_Runtime_Augments_AsyncStatus_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct Internal_Runtime_Augments_AsyncStatus_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Internal_Runtime_Augments_AsyncStatus_VTable vtable;
};
struct Internal_Runtime_Augments_AsyncStatus_o {
    Internal_Runtime_Augments_AsyncStatus_Fields fields;
};
struct Internal_Runtime_Augments_TaskTraceCallbacks_Fields {
};
struct Internal_Runtime_Augments_TaskTraceCallbacks_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Internal_Runtime_Augments_TaskTraceCallbacks_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Internal_Runtime_Augments_TaskTraceCallbacks_VTable vtable;
};
struct Internal_Runtime_Augments_TaskTraceCallbacks_o {
    Internal_Runtime_Augments_TaskTraceCallbacks_c *klass;
    void *monitor;
    Internal_Runtime_Augments_TaskTraceCallbacks_Fields fields;
};
struct Internal_Runtime_Augments_RuntimeAugments_Fields {
};
struct Internal_Runtime_Augments_RuntimeAugments_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Internal_Runtime_Augments_RuntimeAugments_c {
    Il2CppClass_1 _1;
    struct Internal_Runtime_Augments_RuntimeAugments_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Internal_Runtime_Augments_RuntimeAugments_VTable vtable;
};
struct Internal_Runtime_Augments_RuntimeAugments_o {
    Internal_Runtime_Augments_RuntimeAugments_c *klass;
    void *monitor;
    Internal_Runtime_Augments_RuntimeAugments_Fields fields;
};
struct Internal_Runtime_Augments_RuntimeAugments_StaticFields {
    struct Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_o* s_reflectionExecutionDomainCallbacks;
};
struct Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_Fields {
};
struct Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_VTable vtable;
};
struct Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_o {
    Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_c *klass;
    void *monitor;
    Internal_Runtime_Augments_ReflectionExecutionDomainCallbacks_Fields fields;
};
struct Internal_Runtime_Augments_RuntimeThread_Fields {
    struct System_Threading_Thread_o* thread;
};
struct Internal_Runtime_Augments_RuntimeThread_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct Internal_Runtime_Augments_RuntimeThread_c {
    Il2CppClass_1 _1;
    struct Internal_Runtime_Augments_RuntimeThread_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    Internal_Runtime_Augments_RuntimeThread_VTable vtable;
};
struct Internal_Runtime_Augments_RuntimeThread_o {
    Internal_Runtime_Augments_RuntimeThread_c *klass;
    void *monitor;
    Internal_Runtime_Augments_RuntimeThread_Fields fields;
};
struct Internal_Runtime_Augments_RuntimeThread_StaticFields {
    int32_t OptimalMaxSpinWaitsPerSpinIteration;
};
struct string_TrimType_Fields {
    int32_t value__;
};
struct string_TrimType_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct string_TrimType_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    string_TrimType_VTable vtable;
};
struct string_TrimType_o {
    string_TrimType_Fields fields;
};
struct string_ProbabilisticMap_Fields {
};
struct string_ProbabilisticMap_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct string_ProbabilisticMap_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    string_ProbabilisticMap_VTable vtable;
};
struct string_ProbabilisticMap_o {
    string_ProbabilisticMap_Fields fields;
};
struct System_String_Fields {
    int32_t _stringLength;
    uint16_t _firstChar;
};
struct System_String_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_System_Collections_IEnumerable_GetEnumerator;
    VirtualInvokeData _6_System_Collections_Generic_IEnumerable_System_Char__GetEnumerator;
    VirtualInvokeData _7_CompareTo;
    VirtualInvokeData _8_Equals;
    VirtualInvokeData _9_GetTypeCode;
    VirtualInvokeData _10_System_IConvertible_ToBoolean;
    VirtualInvokeData _11_System_IConvertible_ToChar;
    VirtualInvokeData _12_System_IConvertible_ToSByte;
    VirtualInvokeData _13_System_IConvertible_ToByte;
    VirtualInvokeData _14_System_IConvertible_ToInt16;
    VirtualInvokeData _15_System_IConvertible_ToUInt16;
    VirtualInvokeData _16_System_IConvertible_ToInt32;
    VirtualInvokeData _17_System_IConvertible_ToUInt32;
    VirtualInvokeData _18_System_IConvertible_ToInt64;
    VirtualInvokeData _19_System_IConvertible_ToUInt64;
    VirtualInvokeData _20_System_IConvertible_ToSingle;
    VirtualInvokeData _21_System_IConvertible_ToDouble;
    VirtualInvokeData _22_System_IConvertible_ToDecimal;
    VirtualInvokeData _23_System_IConvertible_ToDateTime;
    VirtualInvokeData _24_ToString;
    VirtualInvokeData _25_System_IConvertible_ToType;
    VirtualInvokeData _26_Clone;
};
struct System_String_c {
    Il2CppClass_1 _1;
    struct System_String_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_String_VTable vtable;
};
struct System_String_o {
    System_String_c *klass;
    void *monitor;
    System_String_Fields fields;
};
struct System_String_StaticFields {
    struct System_String_o* Empty;
};
struct System_TimeSpan_Fields {
    int64_t _ticks;
};
struct System_TimeSpan_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_CompareTo;
    VirtualInvokeData _6_Equals;
    VirtualInvokeData _7_ToString;
    VirtualInvokeData _8_TryFormat;
};
struct System_TimeSpan_c {
    Il2CppClass_1 _1;
    struct System_TimeSpan_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeSpan_VTable vtable;
};
struct System_TimeSpan_o {
    System_TimeSpan_Fields fields;
};
struct System_TimeSpan_StaticFields {
    struct System_TimeSpan_o Zero;
    struct System_TimeSpan_o MaxValue;
    struct System_TimeSpan_o MinValue;
};
struct System_TimeZoneInfo_TZifType_Fields {
    struct System_TimeSpan_o UtcOffset;
    bool IsDst;
    uint8_t AbbreviationIndex;
};
struct System_TimeZoneInfo_TZifType_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_TimeZoneInfo_TZifType_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_TZifType_VTable vtable;
};
struct System_TimeZoneInfo_TZifType_o {
    System_TimeZoneInfo_TZifType_Fields fields;
};
struct System_TimeZoneInfo_TZifHead_Fields {
    uint32_t Magic;
    uint8_t Version;
    uint32_t IsGmtCount;
    uint32_t IsStdCount;
    uint32_t LeapCount;
    uint32_t TimeCount;
    uint32_t TypeCount;
    uint32_t CharCount;
};
struct System_TimeZoneInfo_TZifHead_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_TimeZoneInfo_TZifHead_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_TZifHead_VTable vtable;
};
struct System_TimeZoneInfo_TZifHead_o {
    System_TimeZoneInfo_TZifHead_Fields fields;
};
struct System_TimeZoneInfo_TZVersion_Fields {
    uint8_t value__;
};
struct System_TimeZoneInfo_TZVersion_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_TimeZoneInfo_TZVersion_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_TZVersion_VTable vtable;
};
struct System_TimeZoneInfo_TZVersion_o {
    System_TimeZoneInfo_TZVersion_Fields fields;
};
struct System_DateTime_Fields {
    uint64_t _dateData;
};
struct System_DateTime_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
    VirtualInvokeData _23_CompareTo;
    VirtualInvokeData _24_Equals;
    VirtualInvokeData _25_System_Runtime_Serialization_ISerializable_GetObjectData;
    VirtualInvokeData _26_TryFormat;
};
struct System_DateTime_c {
    Il2CppClass_1 _1;
    struct System_DateTime_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTime_VTable vtable;
};
struct System_DateTime_o {
    System_DateTime_Fields fields;
};
struct System_DateTime_StaticFields {
    struct System_Int32_array* s_daysToMonth365;
    struct System_Int32_array* s_daysToMonth366;
    struct System_DateTime_o MinValue;
    struct System_DateTime_o MaxValue;
    struct System_DateTime_o UnixEpoch;
};
struct System_TimeZoneInfo_TransitionTime_Fields {
    struct System_DateTime_o _timeOfDay;
    uint8_t _month;
    uint8_t _week;
    uint8_t _day;
    int32_t _dayOfWeek;
    bool _isFixedDateRule;
};
struct System_TimeZoneInfo_TransitionTime_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Equals;
    VirtualInvokeData _5_System_Runtime_Serialization_ISerializable_GetObjectData;
    VirtualInvokeData _6_System_Runtime_Serialization_IDeserializationCallback_OnDeserialization;
};
struct System_TimeZoneInfo_TransitionTime_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_TransitionTime_VTable vtable;
};
struct System_TimeZoneInfo_TransitionTime_o {
    System_TimeZoneInfo_TransitionTime_Fields fields;
};
struct System_TimeZoneInfo_AdjustmentRule_Fields {
    struct System_DateTime_o _dateStart;
    struct System_DateTime_o _dateEnd;
    struct System_TimeSpan_o _daylightDelta;
    struct System_TimeZoneInfo_TransitionTime_o _daylightTransitionStart;
    struct System_TimeZoneInfo_TransitionTime_o _daylightTransitionEnd;
    struct System_TimeSpan_o _baseUtcOffsetDelta;
    bool _noDaylightTransitions;
};
struct System_TimeZoneInfo_AdjustmentRule_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Equals;
    VirtualInvokeData _5_System_Runtime_Serialization_ISerializable_GetObjectData;
    VirtualInvokeData _6_System_Runtime_Serialization_IDeserializationCallback_OnDeserialization;
};
struct System_TimeZoneInfo_AdjustmentRule_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_AdjustmentRule_VTable vtable;
};
struct System_TimeZoneInfo_AdjustmentRule_o {
    System_TimeZoneInfo_AdjustmentRule_c *klass;
    void *monitor;
    System_TimeZoneInfo_AdjustmentRule_Fields fields;
};
struct System_TimeZoneInfo_CachedData_Fields {
    struct System_TimeZoneInfo_o* _localTimeZone;
};
struct System_TimeZoneInfo_CachedData_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_TimeZoneInfo_CachedData_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_CachedData_VTable vtable;
};
struct System_TimeZoneInfo_CachedData_o {
    System_TimeZoneInfo_CachedData_c *klass;
    void *monitor;
    System_TimeZoneInfo_CachedData_Fields fields;
};
struct System_TimeZoneInfo___c__DisplayClass16_0_Fields {
    struct System_String_o* localtimeFilePath;
    struct System_String_o* posixrulesFilePath;
    struct System_Byte_array* buffer;
    struct System_Byte_array* rawData;
    struct System_String_o* id;
    struct System_String_o* timeZoneDirectory;
};
struct System_TimeZoneInfo___c__DisplayClass16_0_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_TimeZoneInfo___c__DisplayClass16_0_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo___c__DisplayClass16_0_VTable vtable;
};
struct System_TimeZoneInfo___c__DisplayClass16_0_o {
    System_TimeZoneInfo___c__DisplayClass16_0_c *klass;
    void *monitor;
    System_TimeZoneInfo___c__DisplayClass16_0_Fields fields;
};
struct System_TimeZoneInfo___c_Fields {
};
struct System_TimeZoneInfo___c_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_TimeZoneInfo___c_c {
    Il2CppClass_1 _1;
    struct System_TimeZoneInfo___c_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo___c_VTable vtable;
};
struct System_TimeZoneInfo___c_o {
    System_TimeZoneInfo___c_c *klass;
    void *monitor;
    System_TimeZoneInfo___c_Fields fields;
};
struct System_TimeZoneInfo___c_StaticFields {
    struct System_TimeZoneInfo___c_o* __9;
    struct System_Func_char__bool__o* __9__34_1;
    struct System_Func_char__bool__o* __9__34_0;
    struct System_Func_char__bool__o* __9__35_0;
    struct System_Func_char__bool__o* __9__37_0;
    struct System_Func_char__bool__o* __9__38_0;
    struct System_Comparison_TimeZoneInfo_AdjustmentRule__o* __9__161_0;
};
struct System_TimeZoneInfo_Fields {
    struct System_String_o* _id;
    struct System_String_o* _displayName;
    struct System_String_o* _standardDisplayName;
    struct System_String_o* _daylightDisplayName;
    struct System_TimeSpan_o _baseUtcOffset;
    bool _supportsDaylightSavingTime;
    struct System_TimeZoneInfo_AdjustmentRule_array* _adjustmentRules;
};
struct System_TimeZoneInfo_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_Equals;
    VirtualInvokeData _5_System_Runtime_Serialization_ISerializable_GetObjectData;
    VirtualInvokeData _6_System_Runtime_Serialization_IDeserializationCallback_OnDeserialization;
};
struct System_TimeZoneInfo_c {
    Il2CppClass_1 _1;
    struct System_TimeZoneInfo_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZoneInfo_VTable vtable;
};
struct System_TimeZoneInfo_o {
    System_TimeZoneInfo_c *klass;
    void *monitor;
    System_TimeZoneInfo_Fields fields;
};
struct System_TimeZoneInfo_StaticFields {
    struct System_TimeZoneInfo_o* s_utcTimeZone;
    struct System_TimeZoneInfo_CachedData_o* s_cachedData;
    struct System_DateTime_o s_maxDateOnly;
    struct System_DateTime_o s_minDateOnly;
    struct System_TimeSpan_o MaxOffset;
    struct System_TimeSpan_o MinOffset;
};
struct System_Delegate_Fields {
    intptr_t method_ptr;
    intptr_t invoke_impl;
    Il2CppObject* m_target;
    intptr_t method;
    intptr_t delegate_trampoline;
    intptr_t extra_arg;
    intptr_t method_code;
    intptr_t interp_method;
    intptr_t interp_invoke_impl;
    struct System_Reflection_MethodInfo_o* method_info;
    struct System_Reflection_MethodInfo_o* original_method_info;
    struct System_DelegateData_o* data;
    bool method_is_virtual;
};
struct System_Delegate_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
};
struct System_Delegate_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Delegate_VTable vtable;
};
struct System_Delegate_o {
    System_Delegate_c *klass;
    void *monitor;
    System_Delegate_Fields fields;
};
struct System_MulticastDelegate_Fields : System_Delegate_Fields {
    struct System_Delegate_array* delegates;
};
struct System_MulticastDelegate_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
};
struct System_MulticastDelegate_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_MulticastDelegate_VTable vtable;
};
struct System_MulticastDelegate_o {
    System_MulticastDelegate_c *klass;
    void *monitor;
    System_MulticastDelegate_Fields fields;
};
struct System_Action_Fields : System_MulticastDelegate_Fields {
};
struct System_Action_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_VTable vtable;
};
struct System_Action_o {
    System_Action_c *klass;
    void *monitor;
    System_Action_Fields fields;
};
struct System_Action_T__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T__VTable vtable;
};
struct System_Action_T__o {
    System_Action_T__c *klass;
    void *monitor;
    System_Action_T__Fields fields;
};
struct System_Action_T1__T2__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__VTable vtable;
};
struct System_Action_T1__T2__o {
    System_Action_T1__T2__c *klass;
    void *monitor;
    System_Action_T1__T2__Fields fields;
};
struct System_Action_T1__T2__T3__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__T3__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__T3__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__T3__VTable vtable;
};
struct System_Action_T1__T2__T3__o {
    System_Action_T1__T2__T3__c *klass;
    void *monitor;
    System_Action_T1__T2__T3__Fields fields;
};
struct System_Action_T1__T2__T3__T4__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__T3__T4__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__T3__T4__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__T3__T4__VTable vtable;
};
struct System_Action_T1__T2__T3__T4__o {
    System_Action_T1__T2__T3__T4__c *klass;
    void *monitor;
    System_Action_T1__T2__T3__T4__Fields fields;
};
struct System_Action_T1__T2__T3__T4__T5__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__T3__T4__T5__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__T3__T4__T5__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__T3__T4__T5__VTable vtable;
};
struct System_Action_T1__T2__T3__T4__T5__o {
    System_Action_T1__T2__T3__T4__T5__c *klass;
    void *monitor;
    System_Action_T1__T2__T3__T4__T5__Fields fields;
};
struct System_Action_T1__T2__T3__T4__T5__T6__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__T3__T4__T5__T6__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__T3__T4__T5__T6__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__T3__T4__T5__T6__VTable vtable;
};
struct System_Action_T1__T2__T3__T4__T5__T6__o {
    System_Action_T1__T2__T3__T4__T5__T6__c *klass;
    void *monitor;
    System_Action_T1__T2__T3__T4__T5__T6__Fields fields;
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__T3__T4__T5__T6__T7__VTable vtable;
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__o {
    System_Action_T1__T2__T3__T4__T5__T6__T7__c *klass;
    void *monitor;
    System_Action_T1__T2__T3__T4__T5__T6__T7__Fields fields;
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__T8__Fields : System_MulticastDelegate_Fields {
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__T8__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__T8__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Action_T1__T2__T3__T4__T5__T6__T7__T8__VTable vtable;
};
struct System_Action_T1__T2__T3__T4__T5__T6__T7__T8__o {
    System_Action_T1__T2__T3__T4__T5__T6__T7__T8__c *klass;
    void *monitor;
    System_Action_T1__T2__T3__T4__T5__T6__T7__T8__Fields fields;
};
struct System_Func_TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_TResult__VTable vtable;
};
struct System_Func_TResult__o {
    System_Func_TResult__c *klass;
    void *monitor;
    System_Func_TResult__Fields fields;
};
struct System_Func_T__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T__TResult__VTable vtable;
};
struct System_Func_T__TResult__o {
    System_Func_T__TResult__c *klass;
    void *monitor;
    System_Func_T__TResult__Fields fields;
};
struct System_Func_T1__T2__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__TResult__VTable vtable;
};
struct System_Func_T1__T2__TResult__o {
    System_Func_T1__T2__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__TResult__Fields fields;
};
struct System_Func_T1__T2__T3__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__T3__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__T3__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__T3__TResult__VTable vtable;
};
struct System_Func_T1__T2__T3__TResult__o {
    System_Func_T1__T2__T3__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__T3__TResult__Fields fields;
};
struct System_Func_T1__T2__T3__T4__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__T3__T4__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__T3__T4__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__T3__T4__TResult__VTable vtable;
};
struct System_Func_T1__T2__T3__T4__TResult__o {
    System_Func_T1__T2__T3__T4__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__T3__T4__TResult__Fields fields;
};
struct System_Func_T1__T2__T3__T4__T5__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__T3__T4__T5__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__T3__T4__T5__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__T3__T4__T5__TResult__VTable vtable;
};
struct System_Func_T1__T2__T3__T4__T5__TResult__o {
    System_Func_T1__T2__T3__T4__T5__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__T3__T4__T5__TResult__Fields fields;
};
struct System_Func_T1__T2__T3__T4__T5__T6__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__T3__T4__T5__T6__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__T3__T4__T5__T6__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__T3__T4__T5__T6__TResult__VTable vtable;
};
struct System_Func_T1__T2__T3__T4__T5__T6__TResult__o {
    System_Func_T1__T2__T3__T4__T5__T6__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__T3__T4__T5__T6__TResult__Fields fields;
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__VTable vtable;
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__o {
    System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__T3__T4__T5__T6__T7__TResult__Fields fields;
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__Fields : System_MulticastDelegate_Fields {
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__VTable vtable;
};
struct System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__o {
    System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__c *klass;
    void *monitor;
    System_Func_T1__T2__T3__T4__T5__T6__T7__T8__TResult__Fields fields;
};
struct System_Comparison_T__Fields : System_MulticastDelegate_Fields {
};
struct System_Comparison_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Comparison_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Comparison_T__VTable vtable;
};
struct System_Comparison_T__o {
    System_Comparison_T__c *klass;
    void *monitor;
    System_Comparison_T__Fields fields;
};
struct System_Converter_TInput__TOutput__Fields : System_MulticastDelegate_Fields {
};
struct System_Converter_TInput__TOutput__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Converter_TInput__TOutput__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Converter_TInput__TOutput__VTable vtable;
};
struct System_Converter_TInput__TOutput__o {
    System_Converter_TInput__TOutput__c *klass;
    void *monitor;
    System_Converter_TInput__TOutput__Fields fields;
};
struct System_Predicate_T__Fields : System_MulticastDelegate_Fields {
};
struct System_Predicate_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_Predicate_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Predicate_T__VTable vtable;
};
struct System_Predicate_T__o {
    System_Predicate_T__c *klass;
    void *monitor;
    System_Predicate_T__Fields fields;
};
struct System_AggregateException_Fields : System_Exception_Fields {
    struct System_Collections_ObjectModel_ReadOnlyCollection_Exception__o* m_innerExceptions;
};
struct System_AggregateException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_AggregateException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_AggregateException_VTable vtable;
};
struct System_AggregateException_o {
    System_AggregateException_c *klass;
    void *monitor;
    System_AggregateException_Fields fields;
};
struct System_ApplicationException_Fields : System_Exception_Fields {
};
struct System_ApplicationException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ApplicationException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ApplicationException_VTable vtable;
};
struct System_ApplicationException_o {
    System_ApplicationException_c *klass;
    void *monitor;
    System_ApplicationException_Fields fields;
};
struct System_ArgumentException_Fields : System_SystemException_Fields {
    struct System_String_o* _paramName;
};
struct System_ArgumentException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ArgumentException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ArgumentException_VTable vtable;
};
struct System_ArgumentException_o {
    System_ArgumentException_c *klass;
    void *monitor;
    System_ArgumentException_Fields fields;
};
struct System_ArgumentNullException_Fields : System_ArgumentException_Fields {
};
struct System_ArgumentNullException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ArgumentNullException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ArgumentNullException_VTable vtable;
};
struct System_ArgumentNullException_o {
    System_ArgumentNullException_c *klass;
    void *monitor;
    System_ArgumentNullException_Fields fields;
};
struct System_ArgumentOutOfRangeException_Fields : System_ArgumentException_Fields {
    Il2CppObject* _actualValue;
};
struct System_ArgumentOutOfRangeException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ArgumentOutOfRangeException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ArgumentOutOfRangeException_VTable vtable;
};
struct System_ArgumentOutOfRangeException_o {
    System_ArgumentOutOfRangeException_c *klass;
    void *monitor;
    System_ArgumentOutOfRangeException_Fields fields;
};
struct System_ArithmeticException_Fields : System_SystemException_Fields {
};
struct System_ArithmeticException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ArithmeticException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ArithmeticException_VTable vtable;
};
struct System_ArithmeticException_o {
    System_ArithmeticException_c *klass;
    void *monitor;
    System_ArithmeticException_Fields fields;
};
struct System_ArraySegment_Enumerator_T__Fields {
    struct System_Object_array* _array;
    int32_t _start;
    int32_t _end;
    int32_t _current;
};
struct System_ArraySegment_Enumerator_T__RGCTXs {
    Il2CppClass* _0_System_ArraySegment_T_;
    MethodInfo* _1_System_ArraySegment_T__get_Array;
    Il2CppClass* _2_System_ArraySegment_T_;
    Il2CppClass* _3_T__;
    Il2CppClass* _4_System_ArraySegment_Enumerator_T_;
    MethodInfo* _5_System_ArraySegment_T__get_Offset;
    MethodInfo* _6_System_ArraySegment_T__get_Count;
    Il2CppClass* _7_T;
    MethodInfo* _8_System_ArraySegment_Enumerator_T__get_Current;
    Il2CppClass* _9_System_ArraySegment_Enumerator_T_;
};
struct System_ArraySegment_Enumerator_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_get_Current;
    VirtualInvokeData _5_Dispose;
    VirtualInvokeData _6_MoveNext;
    VirtualInvokeData _7_System_Collections_IEnumerator_get_Current;
    VirtualInvokeData _8_System_Collections_IEnumerator_Reset;
};
struct System_ArraySegment_Enumerator_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    System_ArraySegment_Enumerator_T__RGCTXs* rgctx_data;
    Il2CppClass_2 _2;
    System_ArraySegment_Enumerator_T__VTable vtable;
};
struct System_ArraySegment_Enumerator_T__o {
    System_ArraySegment_Enumerator_T__Fields fields;
};
struct System_ArraySegment_T__Fields {
    struct System_Object_array* _array;
    int32_t _offset;
    int32_t _count;
};
struct System_ArraySegment_T__RGCTXs {
    Il2CppClass* _0_T__;
    Il2CppClass* _1_System_ArraySegment_T_;
    MethodInfo* _2_System_ArraySegment_T__ThrowInvalidOperationIfDefault;
    Il2CppClass* _3_System_ArraySegment_T_;
    Il2CppClass* _4_System_ArraySegment_Enumerator_T_;
    MethodInfo* _5_System_ArraySegment_Enumerator_T___ctor;
    MethodInfo* _6_System_ArraySegment_T__Equals;
    Il2CppClass* _7_T;
    MethodInfo* _8_System_Array_IndexOf_T_;
    MethodInfo* _9_System_ArraySegment_T__GetEnumerator;
    Il2CppClass* _10_System_Collections_Generic_IEnumerator_T_;
    Il2CppClass* _11_T__;
    MethodInfo* _12_System_ArraySegment_T___ctor;
};
struct System_ArraySegment_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_System_Collections_Generic_IList_T__get_Item;
    VirtualInvokeData _5_System_Collections_Generic_IList_T__set_Item;
    VirtualInvokeData _6_System_Collections_Generic_IList_T__IndexOf;
    VirtualInvokeData _7_System_Collections_Generic_IList_T__Insert;
    VirtualInvokeData _8_System_Collections_Generic_IList_T__RemoveAt;
    VirtualInvokeData _9_unknown;
    VirtualInvokeData _10_System_Collections_Generic_ICollection_T__get_IsReadOnly;
    VirtualInvokeData _11_System_Collections_Generic_ICollection_T__Add;
    VirtualInvokeData _12_System_Collections_Generic_ICollection_T__Clear;
    VirtualInvokeData _13_System_Collections_Generic_ICollection_T__Contains;
    VirtualInvokeData _14_CopyTo;
    VirtualInvokeData _15_System_Collections_Generic_ICollection_T__Remove;
    VirtualInvokeData _16_System_Collections_Generic_IEnumerable_T__GetEnumerator;
    VirtualInvokeData _17_System_Collections_IEnumerable_GetEnumerator;
    VirtualInvokeData _18_System_Collections_Generic_IReadOnlyList_T__get_Item;
    VirtualInvokeData _19_get_Count;
};
struct System_ArraySegment_T__c {
    Il2CppClass_1 _1;
    struct System_ArraySegment_T__StaticFields* static_fields;
    System_ArraySegment_T__RGCTXs* rgctx_data;
    Il2CppClass_2 _2;
    System_ArraySegment_T__VTable vtable;
};
struct System_ArraySegment_T__o {
    System_ArraySegment_T__Fields fields;
};
struct System_ArraySegment_T__StaticFields {
    struct System_ArraySegment_T__o _Empty_k__BackingField;
};
struct System_ArrayTypeMismatchException_Fields : System_SystemException_Fields {
};
struct System_ArrayTypeMismatchException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ArrayTypeMismatchException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ArrayTypeMismatchException_VTable vtable;
};
struct System_ArrayTypeMismatchException_o {
    System_ArrayTypeMismatchException_c *klass;
    void *monitor;
    System_ArrayTypeMismatchException_Fields fields;
};
struct System_EventArgs_Fields {
};
struct System_EventArgs_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_EventArgs_c {
    Il2CppClass_1 _1;
    struct System_EventArgs_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_EventArgs_VTable vtable;
};
struct System_EventArgs_o {
    System_EventArgs_c *klass;
    void *monitor;
    System_EventArgs_Fields fields;
};
struct System_EventArgs_StaticFields {
    struct System_EventArgs_o* Empty;
};
struct System_AssemblyLoadEventArgs_Fields : System_EventArgs_Fields {
    struct System_Reflection_Assembly_o* _LoadedAssembly_k__BackingField;
};
struct System_AssemblyLoadEventArgs_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_AssemblyLoadEventArgs_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_AssemblyLoadEventArgs_VTable vtable;
};
struct System_AssemblyLoadEventArgs_o {
    System_AssemblyLoadEventArgs_c *klass;
    void *monitor;
    System_AssemblyLoadEventArgs_Fields fields;
};
struct System_AssemblyLoadEventHandler_Fields : System_MulticastDelegate_Fields {
};
struct System_AssemblyLoadEventHandler_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_AssemblyLoadEventHandler_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_AssemblyLoadEventHandler_VTable vtable;
};
struct System_AssemblyLoadEventHandler_o {
    System_AssemblyLoadEventHandler_c *klass;
    void *monitor;
    System_AssemblyLoadEventHandler_Fields fields;
};
struct System_AsyncCallback_Fields : System_MulticastDelegate_Fields {
};
struct System_AsyncCallback_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
    VirtualInvokeData _13_BeginInvoke;
    VirtualInvokeData _14_EndInvoke;
};
struct System_AsyncCallback_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_AsyncCallback_VTable vtable;
};
struct System_AsyncCallback_o {
    System_AsyncCallback_c *klass;
    void *monitor;
    System_AsyncCallback_Fields fields;
};
struct System_AttributeTargets_Fields {
    int32_t value__;
};
struct System_AttributeTargets_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_AttributeTargets_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_AttributeTargets_VTable vtable;
};
struct System_AttributeTargets_o {
    System_AttributeTargets_Fields fields;
};
struct System_AttributeUsageAttribute_Fields : System_Attribute_Fields {
    int32_t _attributeTarget;
    bool _allowMultiple;
    bool _inherited;
};
struct System_AttributeUsageAttribute_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_AttributeUsageAttribute_c {
    Il2CppClass_1 _1;
    struct System_AttributeUsageAttribute_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_AttributeUsageAttribute_VTable vtable;
};
struct System_AttributeUsageAttribute_o {
    System_AttributeUsageAttribute_c *klass;
    void *monitor;
    System_AttributeUsageAttribute_Fields fields;
};
struct System_AttributeUsageAttribute_StaticFields {
    struct System_AttributeUsageAttribute_o* Default;
};
struct System_BadImageFormatException_Fields : System_SystemException_Fields {
    struct System_String_o* _fileName;
    struct System_String_o* _fusionLog;
};
struct System_BadImageFormatException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_BadImageFormatException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_BadImageFormatException_VTable vtable;
};
struct System_BadImageFormatException_o {
    System_BadImageFormatException_c *klass;
    void *monitor;
    System_BadImageFormatException_Fields fields;
};
struct System_BitConverter___c_Fields {
};
struct System_BitConverter___c_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_BitConverter___c_c {
    Il2CppClass_1 _1;
    struct System_BitConverter___c_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_BitConverter___c_VTable vtable;
};
struct System_BitConverter___c_o {
    System_BitConverter___c_c *klass;
    void *monitor;
    System_BitConverter___c_Fields fields;
};
struct System_BitConverter___c_StaticFields {
    struct System_BitConverter___c_o* __9;
    struct System_Buffers_SpanAction_char__ValueTuple_byte____int__int___o* __9__38_0;
};
struct System_BitConverter_Fields {
};
struct System_BitConverter_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_BitConverter_c {
    Il2CppClass_1 _1;
    struct System_BitConverter_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_BitConverter_VTable vtable;
};
struct System_BitConverter_o {
    System_BitConverter_c *klass;
    void *monitor;
    System_BitConverter_Fields fields;
};
struct System_BitConverter_StaticFields {
    bool IsLittleEndian;
};
struct System_Boolean_Fields {
    bool m_value;
};
struct System_Boolean_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
    VirtualInvokeData _22_CompareTo;
    VirtualInvokeData _23_Equals;
};
struct System_Boolean_c {
    Il2CppClass_1 _1;
    struct System_Boolean_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Boolean_VTable vtable;
};
struct System_Boolean_o {
    System_Boolean_Fields fields;
};
struct System_Boolean_StaticFields {
    struct System_String_o* TrueString;
    struct System_String_o* FalseString;
};
struct System_Byte_Fields {
    uint8_t m_value;
};
struct System_Byte_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
    VirtualInvokeData _22_ToString;
    VirtualInvokeData _23_CompareTo;
    VirtualInvokeData _24_Equals;
    VirtualInvokeData _25_TryFormat;
};
struct System_Byte_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Byte_VTable vtable;
};
struct System_Byte_o {
    System_Byte_Fields fields;
};
struct System_CLSCompliantAttribute_Fields : System_Attribute_Fields {
    bool _compliant;
};
struct System_CLSCompliantAttribute_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_CLSCompliantAttribute_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_CLSCompliantAttribute_VTable vtable;
};
struct System_CLSCompliantAttribute_o {
    System_CLSCompliantAttribute_c *klass;
    void *monitor;
    System_CLSCompliantAttribute_Fields fields;
};
struct System_Char_Fields {
    uint16_t m_value;
};
struct System_Char_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_CompareTo;
    VirtualInvokeData _6_Equals;
    VirtualInvokeData _7_GetTypeCode;
    VirtualInvokeData _8_System_IConvertible_ToBoolean;
    VirtualInvokeData _9_System_IConvertible_ToChar;
    VirtualInvokeData _10_System_IConvertible_ToSByte;
    VirtualInvokeData _11_System_IConvertible_ToByte;
    VirtualInvokeData _12_System_IConvertible_ToInt16;
    VirtualInvokeData _13_System_IConvertible_ToUInt16;
    VirtualInvokeData _14_System_IConvertible_ToInt32;
    VirtualInvokeData _15_System_IConvertible_ToUInt32;
    VirtualInvokeData _16_System_IConvertible_ToInt64;
    VirtualInvokeData _17_System_IConvertible_ToUInt64;
    VirtualInvokeData _18_System_IConvertible_ToSingle;
    VirtualInvokeData _19_System_IConvertible_ToDouble;
    VirtualInvokeData _20_System_IConvertible_ToDecimal;
    VirtualInvokeData _21_System_IConvertible_ToDateTime;
    VirtualInvokeData _22_ToString;
    VirtualInvokeData _23_System_IConvertible_ToType;
};
struct System_Char_c {
    Il2CppClass_1 _1;
    struct System_Char_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Char_VTable vtable;
};
struct System_Char_o {
    System_Char_Fields fields;
};
struct System_Char_StaticFields {
    struct System_Byte_array* s_categoryForLatin1;
};
struct System_CharEnumerator_Fields {
    struct System_String_o* _str;
    int32_t _index;
    uint16_t _currentElement;
};
struct System_CharEnumerator_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_MoveNext;
    VirtualInvokeData _5_System_Collections_IEnumerator_get_Current;
    VirtualInvokeData _6_Reset;
    VirtualInvokeData _7_get_Current;
    VirtualInvokeData _8_Dispose;
    VirtualInvokeData _9_Clone;
};
struct System_CharEnumerator_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_CharEnumerator_VTable vtable;
};
struct System_CharEnumerator_o {
    System_CharEnumerator_c *klass;
    void *monitor;
    System_CharEnumerator_Fields fields;
};
struct System_Convert_Fields {
};
struct System_Convert_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Convert_c {
    Il2CppClass_1 _1;
    struct System_Convert_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Convert_VTable vtable;
};
struct System_Convert_o {
    System_Convert_c *klass;
    void *monitor;
    System_Convert_Fields fields;
};
struct System_Convert_StaticFields {
    struct System_SByte_array* s_decodingMap;
    struct System_Type_array* ConvertTypes;
    struct System_Type_o* EnumType;
    struct System_Char_array* base64Table;
    Il2CppObject* DBNull;
};
struct System_TimeZone_Fields {
};
struct System_TimeZone_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_TimeZone_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TimeZone_VTable vtable;
};
struct System_TimeZone_o {
    System_TimeZone_c *klass;
    void *monitor;
    System_TimeZone_Fields fields;
};
struct System_CurrentSystemTimeZone_Fields : System_TimeZone_Fields {
    int64_t m_ticksOffset;
    struct System_String_o* m_standardName;
    struct System_String_o* m_daylightName;
    struct System_Collections_Hashtable_o* m_CachedDaylightChanges;
};
struct System_CurrentSystemTimeZone_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_CurrentSystemTimeZone_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_CurrentSystemTimeZone_VTable vtable;
};
struct System_CurrentSystemTimeZone_o {
    System_CurrentSystemTimeZone_c *klass;
    void *monitor;
    System_CurrentSystemTimeZone_Fields fields;
};
struct System_DBNull_Fields {
};
struct System_DBNull_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_GetObjectData;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
};
struct System_DBNull_c {
    Il2CppClass_1 _1;
    struct System_DBNull_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DBNull_VTable vtable;
};
struct System_DBNull_o {
    System_DBNull_c *klass;
    void *monitor;
    System_DBNull_Fields fields;
};
struct System_DBNull_StaticFields {
    struct System_DBNull_o* Value;
};
struct System_DateTimeKind_Fields {
    int32_t value__;
};
struct System_DateTimeKind_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_DateTimeKind_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeKind_VTable vtable;
};
struct System_DateTimeKind_o {
    System_DateTimeKind_Fields fields;
};
struct System_DateTimeOffset_Fields {
    struct System_DateTime_o _dateTime;
    int16_t _offsetMinutes;
};
struct System_DateTimeOffset_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_System_IComparable_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_CompareTo;
    VirtualInvokeData _7_Equals;
    VirtualInvokeData _8_System_Runtime_Serialization_ISerializable_GetObjectData;
    VirtualInvokeData _9_System_Runtime_Serialization_IDeserializationCallback_OnDeserialization;
    VirtualInvokeData _10_TryFormat;
};
struct System_DateTimeOffset_c {
    Il2CppClass_1 _1;
    struct System_DateTimeOffset_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeOffset_VTable vtable;
};
struct System_DateTimeOffset_o {
    System_DateTimeOffset_Fields fields;
};
struct System_DateTimeOffset_StaticFields {
    struct System_DateTimeOffset_o MinValue;
    struct System_DateTimeOffset_o MaxValue;
    struct System_DateTimeOffset_o UnixEpoch;
};
struct System_DayOfWeek_Fields {
    int32_t value__;
};
struct System_DayOfWeek_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_DayOfWeek_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DayOfWeek_VTable vtable;
};
struct System_DayOfWeek_o {
    System_DayOfWeek_Fields fields;
};
struct System_DivideByZeroException_Fields : System_ArithmeticException_Fields {
};
struct System_DivideByZeroException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_DivideByZeroException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DivideByZeroException_VTable vtable;
};
struct System_DivideByZeroException_o {
    System_DivideByZeroException_c *klass;
    void *monitor;
    System_DivideByZeroException_Fields fields;
};
struct System_TypeLoadException_Fields : System_SystemException_Fields {
    struct System_String_o* ClassName;
    struct System_String_o* AssemblyName;
    struct System_String_o* MessageArg;
    int32_t ResourceId;
};
struct System_TypeLoadException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_TypeLoadException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TypeLoadException_VTable vtable;
};
struct System_TypeLoadException_o {
    System_TypeLoadException_c *klass;
    void *monitor;
    System_TypeLoadException_Fields fields;
};
struct System_DllNotFoundException_Fields : System_TypeLoadException_Fields {
};
struct System_DllNotFoundException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_DllNotFoundException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DllNotFoundException_VTable vtable;
};
struct System_DllNotFoundException_o {
    System_DllNotFoundException_c *klass;
    void *monitor;
    System_DllNotFoundException_Fields fields;
};
struct System_Double_Fields {
    double m_value;
};
struct System_Double_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
    VirtualInvokeData _22_ToString;
    VirtualInvokeData _23_CompareTo;
    VirtualInvokeData _24_Equals;
    VirtualInvokeData _25_TryFormat;
};
struct System_Double_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Double_VTable vtable;
};
struct System_Double_o {
    System_Double_Fields fields;
};
struct System_EntryPointNotFoundException_Fields : System_TypeLoadException_Fields {
};
struct System_EntryPointNotFoundException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_EntryPointNotFoundException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_EntryPointNotFoundException_VTable vtable;
};
struct System_EntryPointNotFoundException_o {
    System_EntryPointNotFoundException_c *klass;
    void *monitor;
    System_EntryPointNotFoundException_Fields fields;
};
struct System_EventHandler_Fields : System_MulticastDelegate_Fields {
};
struct System_EventHandler_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_EventHandler_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_EventHandler_VTable vtable;
};
struct System_EventHandler_o {
    System_EventHandler_c *klass;
    void *monitor;
    System_EventHandler_Fields fields;
};
struct System_EventHandler_TEventArgs__Fields : System_MulticastDelegate_Fields {
};
struct System_EventHandler_TEventArgs__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_unknown;
    VirtualInvokeData _6_Clone;
    VirtualInvokeData _7_GetMethodImpl;
    VirtualInvokeData _8_GetObjectData;
    VirtualInvokeData _9_GetInvocationList;
    VirtualInvokeData _10_CombineImpl;
    VirtualInvokeData _11_RemoveImpl;
    VirtualInvokeData _12_Invoke;
};
struct System_EventHandler_TEventArgs__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_EventHandler_TEventArgs__VTable vtable;
};
struct System_EventHandler_TEventArgs__o {
    System_EventHandler_TEventArgs__c *klass;
    void *monitor;
    System_EventHandler_TEventArgs__Fields fields;
};
struct System_ExecutionEngineException_Fields : System_SystemException_Fields {
};
struct System_ExecutionEngineException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_ExecutionEngineException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ExecutionEngineException_VTable vtable;
};
struct System_ExecutionEngineException_o {
    System_ExecutionEngineException_c *klass;
    void *monitor;
    System_ExecutionEngineException_Fields fields;
};
struct System_MemberAccessException_Fields : System_SystemException_Fields {
};
struct System_MemberAccessException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_MemberAccessException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_MemberAccessException_VTable vtable;
};
struct System_MemberAccessException_o {
    System_MemberAccessException_c *klass;
    void *monitor;
    System_MemberAccessException_Fields fields;
};
struct System_FieldAccessException_Fields : System_MemberAccessException_Fields {
};
struct System_FieldAccessException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_FieldAccessException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_FieldAccessException_VTable vtable;
};
struct System_FieldAccessException_o {
    System_FieldAccessException_c *klass;
    void *monitor;
    System_FieldAccessException_Fields fields;
};
struct System_FlagsAttribute_Fields : System_Attribute_Fields {
};
struct System_FlagsAttribute_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_FlagsAttribute_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_FlagsAttribute_VTable vtable;
};
struct System_FlagsAttribute_o {
    System_FlagsAttribute_c *klass;
    void *monitor;
    System_FlagsAttribute_Fields fields;
};
struct System_FormatException_Fields : System_SystemException_Fields {
};
struct System_FormatException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_FormatException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_FormatException_VTable vtable;
};
struct System_FormatException_o {
    System_FormatException_c *klass;
    void *monitor;
    System_FormatException_Fields fields;
};
struct System_Runtime_InteropServices_GCHandle_Fields {
    intptr_t handle;
};
struct System_Runtime_InteropServices_GCHandle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Runtime_InteropServices_GCHandle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Runtime_InteropServices_GCHandle_VTable vtable;
};
struct System_Runtime_InteropServices_GCHandle_o {
    System_Runtime_InteropServices_GCHandle_Fields fields;
};
struct System_Gen2GcCallback_Fields : System_Runtime_ConstrainedExecution_CriticalFinalizerObject_Fields {
    struct System_Func_object__bool__o* _callback;
    struct System_Runtime_InteropServices_GCHandle_o _weakTargetObj;
};
struct System_Gen2GcCallback_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Gen2GcCallback_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Gen2GcCallback_VTable vtable;
};
struct System_Gen2GcCallback_o {
    System_Gen2GcCallback_c *klass;
    void *monitor;
    System_Gen2GcCallback_Fields fields;
};
struct System_DateTimeFormat_Fields {
};
struct System_DateTimeFormat_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_DateTimeFormat_c {
    Il2CppClass_1 _1;
    struct System_DateTimeFormat_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeFormat_VTable vtable;
};
struct System_DateTimeFormat_o {
    System_DateTimeFormat_c *klass;
    void *monitor;
    System_DateTimeFormat_Fields fields;
};
struct System_DateTimeFormat_StaticFields {
    struct System_TimeSpan_o NullOffset;
    struct System_Char_array* allStandardFormats;
    struct System_Globalization_DateTimeFormatInfo_o* InvariantFormatInfo;
    struct System_String_array* InvariantAbbreviatedMonthNames;
    struct System_String_array* InvariantAbbreviatedDayNames;
    struct System_String_array* fixedNumberFormats;
};
struct System_DateTimeParse_DTT_Fields {
    int32_t value__;
};
struct System_DateTimeParse_DTT_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_DateTimeParse_DTT_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeParse_DTT_VTable vtable;
};
struct System_DateTimeParse_DTT_o {
    System_DateTimeParse_DTT_Fields fields;
};
struct System_DateTimeParse_TM_Fields {
    int32_t value__;
};
struct System_DateTimeParse_TM_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_DateTimeParse_TM_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeParse_TM_VTable vtable;
};
struct System_DateTimeParse_TM_o {
    System_DateTimeParse_TM_Fields fields;
};
struct System_DateTimeParse_DS_Fields {
    int32_t value__;
};
struct System_DateTimeParse_DS_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_DateTimeParse_DS_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeParse_DS_VTable vtable;
};
struct System_DateTimeParse_DS_o {
    System_DateTimeParse_DS_Fields fields;
};
struct System_DateTimeParse_Fields {
};
struct System_DateTimeParse_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_DateTimeParse_c {
    Il2CppClass_1 _1;
    struct System_DateTimeParse_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeParse_VTable vtable;
};
struct System_DateTimeParse_o {
    System_DateTimeParse_c *klass;
    void *monitor;
    System_DateTimeParse_Fields fields;
};
struct System_DateTimeParse_StaticFields {
    struct System_DateTimeParse_DS_array_array* dateParsingStates;
};
struct System_ByReference_T__Fields {
    intptr_t _value;
};
struct System_ByReference_T__RGCTXs {
    Il2CppClass* _0_T;
};
struct System_ByReference_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_ByReference_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    System_ByReference_T__RGCTXs* rgctx_data;
    Il2CppClass_2 _2;
    System_ByReference_T__VTable vtable;
};
struct System_ByReference_T__o {
    System_ByReference_T__Fields fields;
};
struct System_ReadOnlySpan_char__Fields {
    struct System_ByReference_T__o _pointer;
    int32_t _length;
};
struct System_ReadOnlySpan_char__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_ReadOnlySpan_char__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ReadOnlySpan_char__VTable vtable;
};
struct System_ReadOnlySpan_char__o {
    System_ReadOnlySpan_char__Fields fields;
};
struct System___DTString_Fields {
    struct System_ReadOnlySpan_char__o Value;
    int32_t Index;
    uint16_t m_current;
    struct System_Globalization_CompareInfo_o* m_info;
    bool m_checkDigitToken;
};
struct System___DTString_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System___DTString_c {
    Il2CppClass_1 _1;
    struct System___DTString_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System___DTString_VTable vtable;
};
struct System___DTString_o {
    System___DTString_Fields fields;
};
struct System___DTString_StaticFields {
    struct System_Char_array* WhiteSpaceChecks;
};
struct System_DTSubStringType_Fields {
    int32_t value__;
};
struct System_DTSubStringType_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_DTSubStringType_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DTSubStringType_VTable vtable;
};
struct System_DTSubStringType_o {
    System_DTSubStringType_Fields fields;
};
struct System_DTSubString_Fields {
    struct System_ReadOnlySpan_char__o s;
    int32_t index;
    int32_t length;
    int32_t type;
    int32_t value;
};
struct System_DTSubString_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_DTSubString_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DTSubString_VTable vtable;
};
struct System_DTSubString_o {
    System_DTSubString_Fields fields;
};
struct System_DateTimeToken_Fields {
    int32_t dtt;
    int32_t suffix;
    int32_t num;
};
struct System_DateTimeToken_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_DateTimeToken_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeToken_VTable vtable;
};
struct System_DateTimeToken_o {
    System_DateTimeToken_Fields fields;
};
struct System_DateTimeRawInfo_Fields {
    int32_t* num;
    int32_t numCount;
    int32_t month;
    int32_t year;
    int32_t dayOfWeek;
    int32_t era;
    int32_t timeMark;
    double fraction;
    bool hasSameDateAndTimeSeparators;
};
struct System_DateTimeRawInfo_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_DateTimeRawInfo_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeRawInfo_VTable vtable;
};
struct System_DateTimeRawInfo_o {
    System_DateTimeRawInfo_Fields fields;
};
struct System_ParseFailureKind_Fields {
    int32_t value__;
};
struct System_ParseFailureKind_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_ParseFailureKind_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ParseFailureKind_VTable vtable;
};
struct System_ParseFailureKind_o {
    System_ParseFailureKind_Fields fields;
};
struct System_ParseFlags_Fields {
    int32_t value__;
};
struct System_ParseFlags_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_ParseFlags_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_ParseFlags_VTable vtable;
};
struct System_ParseFlags_o {
    System_ParseFlags_Fields fields;
};
struct System_DateTimeResult_Fields {
    int32_t Year;
    int32_t Month;
    int32_t Day;
    int32_t Hour;
    int32_t Minute;
    int32_t Second;
    double fraction;
    int32_t era;
    int32_t flags;
    struct System_TimeSpan_o timeZoneOffset;
    struct System_Globalization_Calendar_o* calendar;
    struct System_DateTime_o parsedDate;
    int32_t failure;
    struct System_String_o* failureMessageID;
    Il2CppObject* failureMessageFormatArgument;
    struct System_String_o* failureArgumentName;
    struct System_ReadOnlySpan_char__o originalDateTimeString;
    struct System_ReadOnlySpan_char__o failedFormatSpecifier;
};
struct System_DateTimeResult_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_DateTimeResult_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_DateTimeResult_VTable vtable;
};
struct System_DateTimeResult_o {
    System_DateTimeResult_Fields fields;
};
struct System_TokenType_Fields {
    int32_t value__;
};
struct System_TokenType_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_TokenType_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_TokenType_VTable vtable;
};
struct System_TokenType_o {
    System_TokenType_Fields fields;
};
struct System_Guid_GuidStyles_Fields {
    int32_t value__;
};
struct System_Guid_GuidStyles_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_Guid_GuidStyles_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Guid_GuidStyles_VTable vtable;
};
struct System_Guid_GuidStyles_o {
    System_Guid_GuidStyles_Fields fields;
};
struct System_Guid_GuidParseThrowStyle_Fields {
    int32_t value__;
};
struct System_Guid_GuidParseThrowStyle_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_Guid_GuidParseThrowStyle_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Guid_GuidParseThrowStyle_VTable vtable;
};
struct System_Guid_GuidParseThrowStyle_o {
    System_Guid_GuidParseThrowStyle_Fields fields;
};
struct System_Guid_ParseFailureKind_Fields {
    int32_t value__;
};
struct System_Guid_ParseFailureKind_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_Guid_ParseFailureKind_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Guid_ParseFailureKind_VTable vtable;
};
struct System_Guid_ParseFailureKind_o {
    System_Guid_ParseFailureKind_Fields fields;
};
struct System_Guid_Fields {
    int32_t _a;
    int16_t _b;
    int16_t _c;
    uint8_t _d;
    uint8_t _e;
    uint8_t _f;
    uint8_t _g;
    uint8_t _h;
    uint8_t _i;
    uint8_t _j;
    uint8_t _k;
};
struct System_Guid_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_ToString;
    VirtualInvokeData _5_CompareTo;
    VirtualInvokeData _6_CompareTo;
    VirtualInvokeData _7_Equals;
    VirtualInvokeData _8_System_ISpanFormattable_TryFormat;
};
struct System_Guid_c {
    Il2CppClass_1 _1;
    struct System_Guid_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Guid_VTable vtable;
};
struct System_Guid_o {
    System_Guid_Fields fields;
};
struct System_Guid_StaticFields {
    struct System_Guid_o Empty;
};
struct System_Guid_GuidResult_Fields {
    struct System_Guid_o _parsedGuid;
    int32_t _throwStyle;
    int32_t _failure;
    struct System_String_o* _failureMessageID;
    Il2CppObject* _failureMessageFormatArgument;
    struct System_String_o* _failureArgumentName;
    struct System_Exception_o* _innerException;
};
struct System_Guid_GuidResult_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Guid_GuidResult_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Guid_GuidResult_VTable vtable;
};
struct System_Guid_GuidResult_o {
    System_Guid_GuidResult_Fields fields;
};
struct System_HashCode_Fields {
    uint32_t _v1;
    uint32_t _v2;
    uint32_t _v3;
    uint32_t _v4;
    uint32_t _queue1;
    uint32_t _queue2;
    uint32_t _queue3;
    uint32_t _length;
};
struct System_HashCode_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_HashCode_c {
    Il2CppClass_1 _1;
    struct System_HashCode_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_HashCode_VTable vtable;
};
struct System_HashCode_o {
    System_HashCode_Fields fields;
};
struct System_HashCode_StaticFields {
    uint32_t s_seed;
};
struct System_IAsyncResult_Fields {
};
struct System_IAsyncResult_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IAsyncResult_o {
    System_IAsyncResult_c *klass;
    void *monitor;
    System_IAsyncResult_Fields fields;
};
struct System_ICloneable_Fields {
};
struct System_ICloneable_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_ICloneable_o {
    System_ICloneable_c *klass;
    void *monitor;
    System_ICloneable_Fields fields;
};
struct System_IComparable_Fields {
};
struct System_IComparable_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IComparable_o {
    System_IComparable_c *klass;
    void *monitor;
    System_IComparable_Fields fields;
};
struct System_IComparable_T__Fields {
};
struct System_IComparable_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IComparable_T__o {
    System_IComparable_T__c *klass;
    void *monitor;
    System_IComparable_T__Fields fields;
};
struct System_IConvertible_Fields {
};
struct System_IConvertible_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IConvertible_o {
    System_IConvertible_c *klass;
    void *monitor;
    System_IConvertible_Fields fields;
};
struct System_ICustomFormatter_Fields {
};
struct System_ICustomFormatter_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_ICustomFormatter_o {
    System_ICustomFormatter_c *klass;
    void *monitor;
    System_ICustomFormatter_Fields fields;
};
struct System_IDisposable_Fields {
};
struct System_IDisposable_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IDisposable_o {
    System_IDisposable_c *klass;
    void *monitor;
    System_IDisposable_Fields fields;
};
struct System_IEquatable_T__Fields {
};
struct System_IEquatable_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IEquatable_T__o {
    System_IEquatable_T__c *klass;
    void *monitor;
    System_IEquatable_T__Fields fields;
};
struct System_IFormatProvider_Fields {
};
struct System_IFormatProvider_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IFormatProvider_o {
    System_IFormatProvider_c *klass;
    void *monitor;
    System_IFormatProvider_Fields fields;
};
struct System_IFormattable_Fields {
};
struct System_IFormattable_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_IFormattable_o {
    System_IFormattable_c *klass;
    void *monitor;
    System_IFormattable_Fields fields;
};
struct System_ISpanFormattable_Fields {
};
struct System_ISpanFormattable_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[32];
};
struct System_ISpanFormattable_o {
    System_ISpanFormattable_c *klass;
    void *monitor;
    System_ISpanFormattable_Fields fields;
};
struct System_IndexOutOfRangeException_Fields : System_SystemException_Fields {
};
struct System_IndexOutOfRangeException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_IndexOutOfRangeException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_IndexOutOfRangeException_VTable vtable;
};
struct System_IndexOutOfRangeException_o {
    System_IndexOutOfRangeException_c *klass;
    void *monitor;
    System_IndexOutOfRangeException_Fields fields;
};
struct System_InsufficientExecutionStackException_Fields : System_SystemException_Fields {
};
struct System_InsufficientExecutionStackException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_InsufficientExecutionStackException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_InsufficientExecutionStackException_VTable vtable;
};
struct System_InsufficientExecutionStackException_o {
    System_InsufficientExecutionStackException_c *klass;
    void *monitor;
    System_InsufficientExecutionStackException_Fields fields;
};
struct System_Int16_Fields {
    int16_t m_value;
};
struct System_Int16_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
    VirtualInvokeData _22_ToString;
    VirtualInvokeData _23_CompareTo;
    VirtualInvokeData _24_Equals;
    VirtualInvokeData _25_TryFormat;
};
struct System_Int16_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Int16_VTable vtable;
};
struct System_Int16_o {
    System_Int16_Fields fields;
};
struct System_Int32_Fields {
    int32_t m_value;
};
struct System_Int32_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
    VirtualInvokeData _22_ToString;
    VirtualInvokeData _23_CompareTo;
    VirtualInvokeData _24_Equals;
    VirtualInvokeData _25_TryFormat;
};
struct System_Int32_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Int32_VTable vtable;
};
struct System_Int32_o {
    System_Int32_Fields fields;
};
struct System_Int64_Fields {
    int64_t m_value;
};
struct System_Int64_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_GetTypeCode;
    VirtualInvokeData _6_System_IConvertible_ToBoolean;
    VirtualInvokeData _7_System_IConvertible_ToChar;
    VirtualInvokeData _8_System_IConvertible_ToSByte;
    VirtualInvokeData _9_System_IConvertible_ToByte;
    VirtualInvokeData _10_System_IConvertible_ToInt16;
    VirtualInvokeData _11_System_IConvertible_ToUInt16;
    VirtualInvokeData _12_System_IConvertible_ToInt32;
    VirtualInvokeData _13_System_IConvertible_ToUInt32;
    VirtualInvokeData _14_System_IConvertible_ToInt64;
    VirtualInvokeData _15_System_IConvertible_ToUInt64;
    VirtualInvokeData _16_System_IConvertible_ToSingle;
    VirtualInvokeData _17_System_IConvertible_ToDouble;
    VirtualInvokeData _18_System_IConvertible_ToDecimal;
    VirtualInvokeData _19_System_IConvertible_ToDateTime;
    VirtualInvokeData _20_ToString;
    VirtualInvokeData _21_System_IConvertible_ToType;
    VirtualInvokeData _22_ToString;
    VirtualInvokeData _23_CompareTo;
    VirtualInvokeData _24_Equals;
    VirtualInvokeData _25_TryFormat;
};
struct System_Int64_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_Int64_VTable vtable;
};
struct System_Int64_o {
    System_Int64_Fields fields;
};
struct System_InvalidCastException_Fields : System_SystemException_Fields {
};
struct System_InvalidCastException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_InvalidCastException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_InvalidCastException_VTable vtable;
};
struct System_InvalidCastException_o {
    System_InvalidCastException_c *klass;
    void *monitor;
    System_InvalidCastException_Fields fields;
};
struct System_InvalidOperationException_Fields : System_SystemException_Fields {
};
struct System_InvalidOperationException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_InvalidOperationException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_InvalidOperationException_VTable vtable;
};
struct System_InvalidOperationException_o {
    System_InvalidOperationException_c *klass;
    void *monitor;
    System_InvalidOperationException_Fields fields;
};
struct System_InvalidProgramException_Fields : System_SystemException_Fields {
};
struct System_InvalidProgramException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_InvalidProgramException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_InvalidProgramException_VTable vtable;
};
struct System_InvalidProgramException_o {
    System_InvalidProgramException_c *klass;
    void *monitor;
    System_InvalidProgramException_Fields fields;
};
struct System_InvalidTimeZoneException_Fields : System_Exception_Fields {
};
struct System_InvalidTimeZoneException_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_unknown;
    VirtualInvokeData _5_get_Message;
    VirtualInvokeData _6_get_Data;
    VirtualInvokeData _7_get_InnerException;
    VirtualInvokeData _8_get_StackTrace;
    VirtualInvokeData _9_get_Source;
    VirtualInvokeData _10_GetObjectData;
    VirtualInvokeData _11_GetType;
};
struct System_InvalidTimeZoneException_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_InvalidTimeZoneException_VTable vtable;
};
struct System_InvalidTimeZoneException_o {
    System_InvalidTimeZoneException_c *klass;
    void *monitor;
    System_InvalidTimeZoneException_Fields fields;
};
struct System_LazyState_Fields {
    int32_t value__;
};
struct System_LazyState_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_CompareTo;
    VirtualInvokeData _5_ToString;
    VirtualInvokeData _6_GetTypeCode;
    VirtualInvokeData _7_System_IConvertible_ToBoolean;
    VirtualInvokeData _8_System_IConvertible_ToChar;
    VirtualInvokeData _9_System_IConvertible_ToSByte;
    VirtualInvokeData _10_System_IConvertible_ToByte;
    VirtualInvokeData _11_System_IConvertible_ToInt16;
    VirtualInvokeData _12_System_IConvertible_ToUInt16;
    VirtualInvokeData _13_System_IConvertible_ToInt32;
    VirtualInvokeData _14_System_IConvertible_ToUInt32;
    VirtualInvokeData _15_System_IConvertible_ToInt64;
    VirtualInvokeData _16_System_IConvertible_ToUInt64;
    VirtualInvokeData _17_System_IConvertible_ToSingle;
    VirtualInvokeData _18_System_IConvertible_ToDouble;
    VirtualInvokeData _19_System_IConvertible_ToDecimal;
    VirtualInvokeData _20_System_IConvertible_ToDateTime;
    VirtualInvokeData _21_ToString;
    VirtualInvokeData _22_System_IConvertible_ToType;
};
struct System_LazyState_c {
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_LazyState_VTable vtable;
};
struct System_LazyState_o {
    System_LazyState_Fields fields;
};
struct System_LazyHelper_Fields {
    int32_t _State_k__BackingField;
    struct System_Runtime_ExceptionServices_ExceptionDispatchInfo_o* _exceptionDispatch;
};
struct System_LazyHelper_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_LazyHelper_c {
    Il2CppClass_1 _1;
    struct System_LazyHelper_StaticFields* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    System_LazyHelper_VTable vtable;
};
struct System_LazyHelper_o {
    System_LazyHelper_c *klass;
    void *monitor;
    System_LazyHelper_Fields fields;
};
struct System_LazyHelper_StaticFields {
    struct System_LazyHelper_o* NoneViaConstructor;
    struct System_LazyHelper_o* NoneViaFactory;
    struct System_LazyHelper_o* PublicationOnlyViaConstructor;
    struct System_LazyHelper_o* PublicationOnlyViaFactory;
    struct System_LazyHelper_o* PublicationOnlyWaitForOtherThreadToPublish;
};
struct System_Lazy_T__Fields {
    struct System_LazyHelper_o* _state;
    struct System_Func_T__o* _factory;
    Il2CppObject* _value;
};
struct System_Lazy_T__RGCTXs {
    Il2CppType* _0_T;
    Il2CppClass* _1_T;
    MethodInfo* _2_System_Lazy_T___ctor;
    Il2CppClass* _3_System_Func_T_;
    Il2CppClass* _4_System_Lazy_T_;
    MethodInfo* _5_System_Lazy_T__CreateViaDefaultConstructor;
    Il2CppClass* _6_System_Lazy_T_;
    MethodInfo* _7_System_Func_T__Invoke;
    MethodInfo* _8_System_Lazy_T__ViaConstructor;
    MethodInfo* _9_System_Lazy_T__ViaFactory;
    MethodInfo* _10_System_Lazy_T__PublicationOnly;
    MethodInfo* _11_System_Lazy_T__PublicationOnlyWaitForOtherThreadToPublish;
    MethodInfo* _12_System_Lazy_T__PublicationOnlyViaConstructor;
    MethodInfo* _13_System_Lazy_T__PublicationOnlyViaFactory;
    MethodInfo* _14_System_Lazy_T__ExecutionAndPublication;
    MethodInfo* _15_System_Lazy_T__get_Value;
    MethodInfo* _16_System_Lazy_T__get_IsValueCreated;
    MethodInfo* _18_System_Lazy_T__CreateValue;
};
struct System_Lazy_T__VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
};
struct System_Lazy_T__c {
    Il2CppClass_1 _1;
    void* static_fields;
    System_Lazy_T__RGCTXs* rgctx_data;
    Il2CppClass_2 _2;
    System_Lazy_T__VTable vtable;
};
struct System_Lazy_T__o {
    System_Lazy_T__c *klass;
    void *monitor;
    System_Lazy_T__Fields fields;
};