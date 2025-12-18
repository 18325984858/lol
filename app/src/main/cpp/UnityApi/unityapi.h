//
// Created by Song on 2025/12/7.
//

#ifndef DOBBY_PROJECT_UNITYAPI_H
#define DOBBY_PROJECT_UNITYAPI_H

#endif //DOBBY_PROJECT_UNITYAPI_H

#include "../UnityStruct/UnityInfo.h"
#include "../Symbol/Symbol.h"

namespace li2cppApi {

    class cUnityApi{
    public:
        cUnityApi(void*dqil2cppBase=nullptr,
                  Il2CppCodeRegistration *pCodeRegistration=nullptr,
                  Il2CppMetadataRegistration *pMetadataRegistration=nullptr,
                  Il2CppGlobalMetadataHeader *pGlobalMetadataHeader=nullptr,
                  Il2CppImageGlobalMetadata*pMetadataImagesTable=nullptr);
        virtual ~cUnityApi();
    public:
        uint64_t Getil2cppModuleBase();
    public:
        Il2CppClass* il2cpp_class_from_type(const Il2CppType* type);//将Il2CppType类型转换成Il2CppClass类型
        const char*  il2cpp_class_get_name(Il2CppClass *klass);
        const Il2CppAssembly** il2cpp_domain_get_assemblies(const Il2CppDomain* domain, size_t* size);//获取所有已加载代码模块的核心函数
        Il2CppDomain* il2cpp_domain_get();//获取当前进程中 托管代码运行时环境 的全局根对象
        const Il2CppImage* il2cpp_assembly_get_image(const Il2CppAssembly *assembly);//获取模块对象
        const char* il2cpp_image_get_name(const Il2CppImage *image);//获取模块名称
        const char* il2cpp_class_get_namespace(Il2CppClass *klass); //获取命名空间
        Il2CppClass* il2cpp_class_get_parent(Il2CppClass *klass); //获取父类型
        Il2CppClass* il2cpp_class_get_declaring_type(Il2CppClass* klass); //获取外围类型
        bool il2cpp_class_is_abstract(const Il2CppClass *klass);//判断是否是抽象类
        bool il2cpp_class_is_interface(const Il2CppClass *klass);//判断是否是接口
        bool il2cpp_class_is_enum(const Il2CppClass *klass);//判断是否是枚举
        const Il2CppClass* il2cpp_image_get_class(const Il2CppImage * image, size_t index);
        const Il2CppType* il2cpp_class_get_type(Il2CppClass *klass);
        size_t il2cpp_image_get_class_count(const Il2CppImage * image);
        bool il2cpp_class_is_inited(const Il2CppClass *klass);
        const Il2CppType* GetIl2CppTypeFromIndex(TypeIndex index);
        void* il2cpp_api_lookup_symbol(const char* name);//遍历符号
        bool il2cpp_is_debugger_attached();//获取调试状态
        const char* il2cpp_field_get_name(FieldInfo *field);//获取一个给定字段（Field）的 源代码名称。
        int il2cpp_class_get_flags(const Il2CppClass *klass);//获取一个给定 C# 类型的 修饰符标志 (Flags)
        bool il2cpp_class_is_valuetype(const Il2CppClass* klass);//用于确定一个给定的 C# 类型是 值类型 (Value Type) 还是 引用类型 (Reference Type)。
        Il2CppClass* il2cpp_class_get_interfaces(Il2CppClass *klass, void* *iter);//用于实现 反射 (Reflection) 和 类型查询 的关键函数之一。它的主要目的是以一种迭代（Iteration）的方式，获取一个给定 C# 类型所实现（Implement）或继承（Inherit）的所有接口（Interface）
        FieldInfo* il2cpp_class_get_fields(Il2CppClass *klass, void* *iter);//用于 元数据反射（Metadata Reflection） 的一个核心函数。它的作用是以一种 迭代（Iteration） 的方式，获取一个给定 C# 类型（Il2CppClass）中声明的 所有字段（Fields） 的元数据。
        int il2cpp_field_get_flags(FieldInfo *field);//用于获取一个给定 字段（Field） 的 修饰符标志（Flags）。
        const Il2CppType* il2cpp_field_get_type(FieldInfo *field);//用于获取一个给定 字段（Field） 的 数据类型（Type） 信息。
        void il2cpp_field_static_get_value(FieldInfo *field, void *value);//用于获取 静态字段（Static Field） 在内存中的值。
        size_t il2cpp_field_get_offset(FieldInfo *field);//用于获取一个给定 字段（Field） 在其 父类型实例 中内存起始位置的 偏移量（Offset）。
        const PropertyInfo* il2cpp_class_get_properties(Il2CppClass *klass, void* *iter);//用于 元数据反射（Metadata Reflection） 的关键函数。它的作用是以一种 迭代（Iteration） 的方式，获取一个给定 C# 类型（Il2CppClass）中定义的所有 属性（Properties） 的元数据。
        const MethodInfo* il2cpp_property_get_get_method(PropertyInfo *prop);//属性名称获取
        const MethodInfo* il2cpp_property_get_set_method(PropertyInfo *prop);//属性名称获取
        const char* il2cpp_property_get_name(PropertyInfo *prop);//获取属性名
        uint32_t il2cpp_method_get_flags(const MethodInfo *method, uint32_t *iflags);//获取方法的返回值类型
        const Il2CppType* il2cpp_method_get_return_type(const MethodInfo* method);//获取方法的返回值类型
        const Il2CppType* il2cpp_method_get_param(const MethodInfo *method, uint32_t index);//获取方法的参数类型
        const MethodInfo* il2cpp_class_get_methods(Il2CppClass *klass, void* *iter);//遍历类的方法
        uint32_t il2cpp_method_get_param_count(const MethodInfo *method);//获取方法的参数数量
        const char* il2cpp_method_get_name(const MethodInfo *method);//获取方法的名称
        const char *il2cpp_method_get_param_name(const MethodInfo *method, uint32_t index);//获取方法参数的名称
        bool _il2cpp_type_is_byref(const Il2CppType *type);
        Il2CppClass* il2cpp_class_get_nested_types(Il2CppClass *klass, void* *iter);//获取嵌套类型
        bool il2cpp_type_is_byref(const Il2CppType *type);//用于判断一个给定的 C# 类型 (Il2CppType) 是否是 引用传递类型（ByRef Type），即是否使用了 C# 中的 ref 关键字。
        bool il2cpp_method_is_generic(const MethodInfo *method);//判断给定的方法元数据结构 （MethodInfo*） 是否代表一个“泛型方法的定义”（Generic Method Definition）。
        bool il2cpp_method_is_instance(const MethodInfo *method);//判断给定的方法元数据结构 （MethodInfo*） 是否代表一个“实例方法”（Instance Method）。
        Il2CppMetadataUsage GetEncodedIndexType(EncodedMethodIndex index);
        uint32_t GetDecodedMethodIndex(EncodedMethodIndex index);
        Il2CppString* GetStringLiteralFromIndex(StringLiteralIndex index);
        Il2CppString* il2cpp_string_new_len(const char* str, uint32_t length);
        std::string Utf16ToUtf8(const Il2CppChar *utf16String);
        std::string Utf16ToUtf8(const Il2CppChar *utf16String, int maximumSize);
        std::string il2cpp_Il2CppString_toCString(const Il2CppString* pstr);
        const Il2CppGenericMethod* GetGenericMethodFromIndex(GenericMethodIndex index);
        const MethodInfo* GetMethodInfoFromMethodDefinitionIndex(MethodIndex index);
        const Il2CppMethodDefinition* GetMethodDefinitionFromIndex(MethodIndex index);
        const char* GetStringFromIndex(StringIndex index);
        const char* GetWindowsRuntimeStringFromIndex(StringIndex index);
        const Il2CppGenericClass* il2cpp_class_get_generic_class(const Il2CppClass *klass);
        const Il2CppGenericContext* il2cpp_generic_class_get_context(const Il2CppGenericClass* klass);
        const Il2CppGenericInst * il2cpp_generic_context_get_generic_class_inst(const Il2CppGenericContext* context);
        const Il2CppTypeDefinition* GetTypeDefinitionForIndex(TypeDefinitionIndex index);
        const char* il2cpp_type_get_name(const Il2CppType *type);
    public:
        template<typename T>
        static inline bool IsRuntimeMetadataInitialized(T item);
        template <typename u16bit_iterator, typename octet_iterator>
        static octet_iterator utf16to8 (u16bit_iterator start, u16bit_iterator end, octet_iterator result);
        template <typename octet_iterator>
        static octet_iterator append(uint32_t cp, octet_iterator result);
        template<typename T>
        static T MetadataOffset(const void* metadata, size_t sectionOffset, size_t itemIndex);
    public:
        template <typename u16>
        inline static bool is_trail_surrogate(u16 cp);
        template<typename u16_type>
        inline static uint16_t mask16(u16_type oc);
        template <typename u16>
        inline static bool is_lead_surrogate(u16 cp);
    public:
        const std::string m_il2cppStr = "libil2cpp.so";
        std::shared_ptr<Symbol::Symbol> m_li2cppso = nullptr;
        uint64_t m_il2cppbase = 0;
    public:
        void* m_pGlobalMetadata =nullptr;
        Il2CppGlobalMetadataHeader *m_pGlobalMetadataHeader = nullptr;
        Il2CppMetadataRegistration *m_pil2CppMetadataRegistration = nullptr;
        Il2CppCodeRegistration *m_pIl2CppCodeRegistration = nullptr;
        Il2CppImageGlobalMetadata* m_pMetadataImagesTable = nullptr;
    };
};

