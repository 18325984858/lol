//
// Created by Song on 2026/2/3.
//

#include "il2cppHeader.h"
#include "../Log/log.h"
#include <regex>


li2cppHeader::li2cppHeader::li2cppHeader(void *dqil2cppBase, void *pCodeRegistration,
                                         void *pMetadataRegistration, void *pGlobalMetadataHeader,
                                         void *pMetadataImagesTable) :
        li2cppApi::cUnityApi(dqil2cppBase,
                             static_cast<Il2CppCodeRegistration *>(pCodeRegistration),
                             static_cast<Il2CppMetadataRegistration *>(pMetadataRegistration),
                             static_cast<Il2CppGlobalMetadataHeader *>(pGlobalMetadataHeader),
                             static_cast<Il2CppImageGlobalMetadata *>(pMetadataImagesTable)),
        m_il2cppbase(reinterpret_cast<uint64_t>(dqil2cppBase)),
        m_pGlobalMetadata(pGlobalMetadataHeader),
        m_pGlobalMetadataHeader(static_cast<Il2CppGlobalMetadataHeader *>(pGlobalMetadataHeader)),
        m_pil2CppMetadataRegistration(static_cast<Il2CppMetadataRegistration *>(pMetadataRegistration)),
        m_pIl2CppCodeRegistration(static_cast<Il2CppCodeRegistration *>(pCodeRegistration)),
        m_pMetadataImagesTable(static_cast<Il2CppImageGlobalMetadata *>(pMetadataImagesTable))
{

}

li2cppHeader::li2cppHeader::~li2cppHeader() {

}

void li2cppHeader::li2cppHeader::start() {
    LOG(LOG_LEVEL_INFO, "[li2cppHeader] >>> Entering: %s", __FUNCTION__);

    Init(m_classMap);


    LOG(LOG_LEVEL_INFO, "[li2cppHeader] <<< Exiting: %s", __FUNCTION__);
}

std::string li2cppHeader::li2cppHeader::GetClassUniqueName(
        std::map<std::string, std::shared_ptr<UnityClass>> &classMap,
        std::shared_ptr<UnityClass>& targetClass,
        const Il2CppType *type) {

    if (!type) return "Unknown_Class";



    // 1. 获取当前类型的基本信息
    Il2CppClass* klass = il2cpp_type_get_class_or_element_class(type);

    // --- 核心过滤逻辑 ---
    // 1. 过滤掉枚举 (Enum)
    if (il2cpp_class_is_enum(klass)) return "Enum_Ignored";

    // 2. 过滤掉原始内置类型 (如 int, float, bool 等)
    // 通常这些类型没有父类或者父类是 ValueType
    auto flags = il2cpp_class_get_flags(klass);
    if (flags & 0x00000020) return "Interface_Ignored"; // 如果你也不想要接口

    const char* rawTypeName = il2cpp_type_get_name(type);
    std::string baseName = rawTypeName ? rawTypeName : "Unknown";

    std::string uniqueIdentifier = baseName;

    // 2. 特殊逻辑：如果是类（非值类型）且有父类，进行拼接
    if (klass && !il2cpp_class_is_valuetype(klass)) {
        Il2CppClass* parentKlass = il2cpp_class_get_parent(klass);
        if (parentKlass) {
            const char* parentName = il2cpp_class_get_name(parentKlass);
            if (parentName && std::string(parentName) != "Object") { // 忽略最顶层的 Object
                uniqueIdentifier += "_" + std::string(parentName);
            }
        }
    }

    // 3. 统一字符清洗 (点号转下划线，过滤非字母数字)
    std::replace(uniqueIdentifier.begin(), uniqueIdentifier.end(), '.', '_');
    uniqueIdentifier = std::regex_replace(uniqueIdentifier, std::regex("[^a-zA-Z0-9]"), "_");

    // 4. 清理连续下划线并去除首尾多余部分
    uniqueIdentifier = std::regex_replace(uniqueIdentifier, std::regex("_{2,}"), "_");
    if (!uniqueIdentifier.empty() && uniqueIdentifier[0] == '_') uniqueIdentifier.erase(0, 1);
    if (!uniqueIdentifier.empty() && uniqueIdentifier.back() == '_') uniqueIdentifier.pop_back();

    // 5. 冲突处理
    if (targetClass != nullptr) {
        if (classMap.count(uniqueIdentifier) && classMap[uniqueIdentifier]->klassPtr != targetClass->klassPtr) {
            uniqueIdentifier += "_" + std::to_string(targetClass->klassPtr);
        }
    }

    return uniqueIdentifier;
}


void li2cppHeader::li2cppHeader::Init(
        std::map<std::string, std::shared_ptr<UnityClass>> &classMap) { // 注意：这里去掉了 const 因为我们要填充它

    LOG(LOG_LEVEL_INFO, "[li2cppHeader] >>> Entering: %s", __FUNCTION__);

    auto domain = il2cpp_domain_get();
    size_t size;
    auto assemblies = il2cpp_domain_get_assemblies(domain, &size);

    for (size_t i = 0; i < size; ++i) {
        auto image = il2cpp_assembly_get_image(assemblies[i]);
        auto classCount = il2cpp_image_get_class_count(image);
        const char* imageName = il2cpp_image_get_name(image);

        for (int j = 0; j < classCount; ++j) {
            auto klass = il2cpp_image_get_class(image, j);
            if (!klass) continue;

            // 1. 基本信息提取
            const char* namespc = il2cpp_class_get_namespace(const_cast<Il2CppClass *>(klass));
            const char* name = il2cpp_class_get_name(const_cast<Il2CppClass *>(klass));

            // 2. 创建 UnityClass 实例 (使用智能指针)
            auto unityClass = std::make_shared<UnityClass>(
                    namespc ? namespc : "",
                    name ? name : "",
                    reinterpret_cast<uintptr_t>(klass)
            );

            unityClass->assembly = imageName;
            unityClass->instanceSize = il2cpp_class_instance_size(const_cast<Il2CppClass *>(klass));
            unityClass->flags = il2cpp_class_get_flags(klass);

            // 3. 处理父类继承关系
            auto parent = il2cpp_class_get_parent(const_cast<Il2CppClass *>(klass));
            if (parent) {
                unityClass->parentPtr = reinterpret_cast<uintptr_t>(parent);
                // 如果需要记录父类名字，可以进一步调用 il2cpp_class_get_name(parent)
            }

            // 4. 处理类型属性（是否是值类型/结构体）
            // --- 替换原有的 bool 赋值 ---

            // 获取 IL2CPP 原生状态
            bool isValueType = il2cpp_class_is_valuetype(klass);
            bool isEnum = il2cpp_class_is_enum(klass);

            // 使用位域进行存储
            unityClass->typeAttr.bits.isValueType = isValueType;
            unityClass->typeAttr.bits.isEnum = isEnum;

            // 在 Unity/C# 中，Struct 是 ValueType 但不是 Enum
            unityClass->typeAttr.bits.isStruct = isValueType && !isEnum;

            // 补充：判断是否为接口 (Interface)
            // TYPE_ATTRIBUTE_INTERFACE 通常为 0x00000020
            unityClass->typeAttr.bits.isInterface = (unityClass->flags & 0x00000020);

            // --- 这样你的 typeAttr.raw 就会自动根据这些 bits 的设置而改变 ---

            // 5. 获取详细类型名（含泛型参数）
            // il2cpp_type_get_name 会返回类似 "List<int>" 的字符串
            auto type = il2cpp_class_get_type(const_cast<Il2CppClass *>(klass));
            const char* typeName = il2cpp_type_get_name(type);
            if (typeName) {
                // 如果是泛型，这里的 typeName 会包含 <T> 信息
                // 你可以把它存入你的 unityClass 中
                // il2cpp_free(typeName); // 注意：某些版本需要手动释放该字符串
            }

            // 6. 生成唯一键并存入 Map
            std::string uniqueName = GetClassUniqueName(classMap, unityClass,type);

            LOG(LOG_LEVEL_INFO,"uniqueName %s",uniqueName.c_str());

            //获取成员
            DumpFields(const_cast<Il2CppClass *>(klass), unityClass);

            classMap[uniqueName] = unityClass;
            
            // 打印调试信息（可选，类太多时建议关闭）
            // LOG(LOG_LEVEL_DEBUG, "Dumped class: %s", uniqueName.c_str());
        }
    }

    LOG(LOG_LEVEL_INFO, "[li2cppHeader] <<< Exiting: %s, Total Classes: %zu",
        __FUNCTION__, classMap.size());
}

void li2cppHeader::li2cppHeader::DumpFields(Il2CppClass* klass, std::shared_ptr<UnityClass>& unityClass) {
    if (!klass || !unityClass) return;

    void* iter = nullptr;
    FieldInfo* field = nullptr;

    // 记录开始处理该类的字段
    LOG(LOG_LEVEL_INFO, "  [Fields] Dumping fields for class: %s", unityClass->className.c_str());

    // 1. 使用 il2cpp 迭代器遍历
    while ((field = il2cpp_class_get_fields(klass, &iter))) {
        // 2. 提取基础元数据
        const char* fieldName = il2cpp_field_get_name(field);
        auto type = il2cpp_field_get_type(field);
        uint32_t flags = il2cpp_field_get_flags(field);

        // 3. 提取类型名称
        std::string typeName = "Unknown_Type";
        if (type) {
            char* rawTypeName = const_cast<char *>(il2cpp_type_get_name(type));
            if (rawTypeName) {
                typeName = rawTypeName;
                // il2cpp_free(rawTypeName); // 根据你的 NDK 版本决定是否释放
            }
        }

        // 4. 计算偏移与静态属性
        // FIELD_ATTRIBUTE_STATIC = 0x0010
        bool isStatic = (flags & 0x0010) != 0;
        uint32_t offset = il2cpp_field_get_offset(field);
        uintptr_t staticAddr = 0;

        if (isStatic) {
            // 获取静态字段所在的内存地址 (注意：只有在类初始化后此地址才有效)
            staticAddr = reinterpret_cast<uintptr_t>(field);
        }

        // 5. 封装到你定义的 UnityField 结构体中
        // 使用你定义的构造函数: UnityField(name, typeName, offset, isStatic, staticAddr)
        unityClass->fields.emplace_back(
                fieldName ? fieldName : "Unnamed_Field",
                typeName,
                offset,
                isStatic,
                staticAddr
        );

        // 6. 打印日志
        // 格式：[Field] [S] Offset: 0x10 | Type: int | Name: _count
        LOG(LOG_LEVEL_INFO, "    [Field]%s Offset: 0x%03X | Type: %-20s | Name: %s",
            isStatic ? " [Static]" : "         ",
            offset,
            typeName.c_str(),
            fieldName ? fieldName : "Unnamed");
    }
}