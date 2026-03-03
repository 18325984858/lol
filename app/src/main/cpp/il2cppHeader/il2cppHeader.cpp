//
// Created by Song on 2026/2/3.
//

#include "il2cppHeader.h"
#include "../Log/log.h"
#include <regex>
#include <sstream>
#include <fstream>

std::string IntToHex(uint32_t value) {
    std::stringstream ss;
    // 使用 std::uppercase 可以输出 0x1A 而不是 0x1a
    ss << "0x" << std::uppercase << std::hex << value;
    return ss.str();
}

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

    // 1. 初始化并解析所有类
    Init(m_classMap);

    // 2. 遍历完成后，保存到文件
    // 这里建议使用 SD 卡路径或 Data 目录路径
    SaveToIdaHeader(nullptr);

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

    // 记录上一个字段的末尾位置，用于计算 Padding
    // Il2Cpp 对象头部（klass + monitor）在 64 位下占用 0x10 (16字节)
    uint32_t currentOffset = 0x10;

    while ((field = il2cpp_class_get_fields(klass, &iter))) {
        const char* fieldName = il2cpp_field_get_name(field);
        const Il2CppType* type = il2cpp_field_get_type(field);
        uint32_t flags = il2cpp_field_get_flags(field);
        uint32_t offset = il2cpp_field_get_offset(field);
        bool isStatic = (flags & 0x0010) != 0;

        // 1. 处理实例字段的 Padding
        if (!isStatic && offset > currentOffset) {
            uint32_t paddingSize = offset - currentOffset;
            std::string padName = "pad_0x" + IntToHex(currentOffset);
            unityClass->fields.emplace_back(padName, "uint8_t[" + std::to_string(paddingSize) + "]", currentOffset, false, 0);
        }

        // 2. 获取 IDA 兼容的类型名 (调用之前的 GetIdaTypeName)
        std::string idaTypeName = GetIdaCompatibleType(type);

        // 3. 存储到结构体
        unityClass->fields.emplace_back(
                fieldName ? fieldName : "Unnamed_Field",
                idaTypeName,
                offset,
                isStatic,
                isStatic ? reinterpret_cast<uintptr_t>(field) : 0
        );

        // 4. 更新当前偏移位置 (仅针对实例字段)
        if (!isStatic) {
            // 粗略估算字段占用大小，也可以使用 il2cpp_class_instance_size 处理 ValueType
            // 这里简单处理：指针对齐 8 字节，基础类型按实际大小
            currentOffset = offset + GetTypeSize(type);
        }

        LOG(LOG_LEVEL_INFO, "  [Field] %s | Offset: 0x%X | Type: %s", fieldName, offset, idaTypeName.c_str());
    }
}

std::string li2cppHeader::li2cppHeader::GetIdaCompatibleType(const Il2CppType* type) {
    if (!type) return "void*";

    // 获取底层枚举类型 (IL2CPP_TYPE_I4, IL2CPP_TYPE_CLASS 等)
    Il2CppTypeEnum typeEnum = type->type;

    switch (typeEnum) {
        case IL2CPP_TYPE_VOID:     return "void";
        case IL2CPP_TYPE_BOOLEAN:  return "bool";
        case IL2CPP_TYPE_CHAR:     return "uint16_t"; // C# Char
        case IL2CPP_TYPE_I1:       return "int8_t";
        case IL2CPP_TYPE_U1:       return "uint8_t";
        case IL2CPP_TYPE_I2:       return "int16_t";
        case IL2CPP_TYPE_U2:       return "uint16_t";
        case IL2CPP_TYPE_I4:       return "int32_t";
        case IL2CPP_TYPE_U4:       return "uint32_t";
        case IL2CPP_TYPE_I8:       return "int64_t";
        case IL2CPP_TYPE_U8:       return "uint64_t";
        case IL2CPP_TYPE_R4:       return "float";
        case IL2CPP_TYPE_R8:       return "double";
        case IL2CPP_TYPE_STRING:   return "struct String_o*";
        case IL2CPP_TYPE_PTR:      return "void*";
        case IL2CPP_TYPE_FNPTR:    return "void*";
        case IL2CPP_TYPE_OBJECT:   return "struct System_Object_o*";

        case IL2CPP_TYPE_SZARRAY: {
            // 处理一维数组
            Il2CppClass* elementKlass = il2cpp_class_from_type(type->data.type);
            std::string name = CleanName(elementKlass);
            return "struct " + name + "_array*";
        }

        case IL2CPP_TYPE_CLASS: {
            // 处理引用类型
            Il2CppClass* klass = il2cpp_class_from_type(type);
            return "struct " + CleanName(klass) + "_o*";
        }

        case IL2CPP_TYPE_VALUETYPE: {
            // 关键：识别结构体 (ValueType)
            Il2CppClass* klass = il2cpp_class_from_type(type);
            if (il2cpp_class_is_enum(klass)) {
                // 如果是枚举，IDA 通常识别为 int32_t
                return "int32_t";
            }
            // 注意：作为字段时，结构体是平铺的，不带星号
            return "struct " + CleanName(klass) + "_Fields";
        }

        case IL2CPP_TYPE_GENERICINST: {
            // 处理泛型实例
            Il2CppClass* klass = il2cpp_class_from_type(type);
            return "struct " + CleanName(klass) + "_o*";
        }

        default:
            return "void*";
    }
}

uint32_t li2cppHeader::li2cppHeader::GetTypeSize(const Il2CppType* type) {
    switch (type->type) {
        case IL2CPP_TYPE_BOOLEAN:
        case IL2CPP_TYPE_I1:
        case IL2CPP_TYPE_U1:       return 1;
        case IL2CPP_TYPE_I2:
        case IL2CPP_TYPE_U2:
        case IL2CPP_TYPE_CHAR:     return 2;
        case IL2CPP_TYPE_I4:
        case IL2CPP_TYPE_U4:
        case IL2CPP_TYPE_R4:       return 4;
        case IL2CPP_TYPE_I8:
        case IL2CPP_TYPE_U8:
        case IL2CPP_TYPE_R8:       return 8;
        case IL2CPP_TYPE_VALUETYPE: {
            Il2CppClass* klass = il2cpp_class_from_type(type);
            return il2cpp_class_instance_size(klass) - 0x10; // 减去对象头偏移
        }
        default: return 8; // 指针、类、数组在 64 位下均为 8 字节
    }
}

std::string li2cppHeader::li2cppHeader::CleanName(Il2CppClass* klass) {
    const char* ns = il2cpp_class_get_namespace(klass);
    const char* name = il2cpp_class_get_name(klass);
    std::string fullName = (ns && strlen(ns) > 0) ? (std::string(ns) + "_" + name) : name;

    // 替换所有 IDA 不支持的字符
    for (char &c : fullName) {
        if (c == '.' || c == '`' || c == '<' || c == '>' || c == '/') c = '_';
    }
    return fullName;
}

void li2cppHeader::li2cppHeader::SaveToIdaHeader(const std::string& path) {
    std::ofstream fout(path);
    if (!fout.is_open()) {
        LOG(LOG_LEVEL_ERROR, "Failed to open export file: %s", path.c_str());
        return;
    }

    // 1. 基础环境定义
    fout << "/* Generated by Gemini UnityDump */\n";
    fout << "#include <stdint.h>\n\n";
    fout << "typedef struct VirtualInvokeData { void* methodPtr; void* method; } VirtualInvokeData;\n";
    fout << "typedef struct Il2CppObject { struct Il2CppClass_c *klass; void *monitor; } Il2CppObject;\n";
    fout << "typedef struct Il2CppArrayBounds { uintptr_t length; uintptr_t lower_bound; } Il2CppArrayBounds;\n";
    fout << "typedef struct String_o { Il2CppObject obj; int32_t length; uint16_t chars[0]; } String_o;\n\n";

    // 2. 前置声明：所有类都预先声明其 _o, _c, _Fields 结构
    fout << "/* --- Forward Declarations --- */\n";
    for (auto const& [key, cls] : m_classMap) {
        // 使用你的 CleanName 逻辑生成安全名称
        std::string sn = CleanName(reinterpret_cast<Il2CppClass*>(cls->klassPtr));
        fout << "typedef struct " << sn << "_o " << sn << "_o;\n";
        fout << "typedef struct " << sn << "_c " << sn << "_c;\n";
        fout << "typedef struct " << sn << "_Fields " << sn << "_Fields;\n";
    }

    // 3. 详细定义
    fout << "\n/* --- Type Definitions --- */\n";
    for (auto const& [key, cls] : m_classMap) {
        std::string sn = CleanName(reinterpret_cast<Il2CppClass*>(cls->klassPtr));

        fout << "/* Assembly: " << cls->assembly << " | Namespace: " << cls->namespc << " */\n";

        // A. 字段结构定义 (包含 Padding)
        fout << "struct " << sn << "_Fields {\n";
        for (const auto& field : cls->fields) {
            if (field.isStatic) continue; // 实例结构体不含静态字段
            fout << "    " << field.typeName << " " << field.name << "; // " << IntToHex(field.offset) << "\n";
        }
        fout << "};\n";

        // B. VTable 结构定义 (根据存储的方法列表)
        fout << "struct " << sn << "_VTable {\n";
        if (cls->methods.empty()) {
            fout << "    VirtualInvokeData vtable[32]; // Placeholder\n";
        } else {
            for (size_t i = 0; i < cls->methods.size(); ++i) {
                fout << "    VirtualInvokeData " << CleanName(reinterpret_cast<Il2CppClass*>(cls->klassPtr)) << "_" << i << ";\n";
            }
        }
        fout << "};\n";

        // C. 类结构 (klass/static) 定义
        fout << "struct " << sn << "_c {\n";
        fout << "    void* image;\n";
        fout << "    void* gc_desc;\n";
        fout << "    const char* name;\n";
        fout << "    const char* namespc;\n";
        fout << "    uint32_t flags;\n";
        fout << "    uint32_t instance_size;\n";
        fout << "    struct " << sn << "_VTable vtable;\n";
        fout << "};\n";

        // D. 实例对象结构定义
        fout << "struct " << sn << "_o {\n";
        fout << "    struct " << sn << "_c *klass;\n";
        fout << "    void *monitor;\n";
        fout << "    struct " << sn << "_Fields fields;\n";
        fout << "};\n\n";
    }

    fout.close();
    LOG(LOG_LEVEL_INFO, "[li2cppHeader] Successfully saved %zu classes to %s", m_classMap.size(), path.size());
}