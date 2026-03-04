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
    LOG(LOG_LEVEL_INFO, "[li2cppHeader] >>> Exiting and Cleaning up resources...");

    // 1. 清空类信息 Map
    // 由于使用了 std::shared_ptr，clear() 会导致引用计数减 1
    // 当计数归零时，UnityClass 及其内部的 vector (fields, methods) 会自动释放内存
    m_classMap.clear();

    // 2. 指针置空（虽然对象即将销毁，但这是个好习惯，防止野指针）
    m_pGlobalMetadata = nullptr;
    m_pGlobalMetadataHeader = nullptr;
    m_pil2CppMetadataRegistration = nullptr;
    m_pIl2CppCodeRegistration = nullptr;
    m_pMetadataImagesTable = nullptr;

    LOG(LOG_LEVEL_INFO, "[li2cppHeader] Cleanup finished.");
}

void li2cppHeader::li2cppHeader::start() {
    LOG(LOG_LEVEL_INFO, "[li2cppHeader] >>> Entering: %s", __FUNCTION__);

    // 1. 初始化并解析所有类
    Init(m_classMap);

    // 2. 遍历完成后，保存到文件
    // 这里建议使用 SD 卡路径或 Data 目录路径
    SaveToIdaHeader("/data/data/"+m_packname+"/files/il2cpp.h");

    SaveToMethodJson("/data/data/"+m_packname+"/files/script.json");

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
        std::map<std::string, std::shared_ptr<UnityClass>> &classMap) {

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

            // 1. 统一获取经过清洗的唯一名称 (内部已处理 Namespace 和点号)
            // 这一步是解决 "前缀冗余" 的关键：所有地方都必须用这个函数获取名字
            std::string uniqueName = GetSafeUniqueName(const_cast<Il2CppClass *>(klass));

            // 2. 提取基本信息
            const char* namespc = il2cpp_class_get_namespace(const_cast<Il2CppClass *>(klass));
            const char* name = il2cpp_class_get_name(const_cast<Il2CppClass *>(klass));

            // 3. 创建 UnityClass 实例
            auto unityClass = std::make_shared<UnityClass>(
                    namespc ? namespc : "",
                    name ? name : "",
                    reinterpret_cast<uintptr_t>(klass)
            );

            unityClass->assembly = imageName;
            unityClass->instanceSize = il2cpp_class_instance_size(const_cast<Il2CppClass *>(klass));
            unityClass->flags = il2cpp_class_get_flags(klass);

            // 4. 处理父类继承
            auto parent = il2cpp_class_get_parent(const_cast<Il2CppClass *>(klass));
            if (parent) {
                unityClass->parentPtr = reinterpret_cast<uintptr_t>(parent);
            }

            // 5. 处理类型属性 (位域存储)
            bool isValueType = il2cpp_class_is_valuetype(klass);
            bool isEnum = il2cpp_class_is_enum(klass);
            unityClass->typeAttr.bits.isValueType = isValueType;
            unityClass->typeAttr.bits.isEnum = isEnum;
            unityClass->typeAttr.bits.isStruct = isValueType && !isEnum;
            unityClass->typeAttr.bits.isInterface = (unityClass->flags & 0x00000020);

            // 6. 获取成员 (注意：DumpFields 内部调用的 GetIdaCompatibleType 也要改)
            // 确保 DumpFields 内部引用其他类时，也是调用 GetSafeUniqueName
            DumpFields(const_cast<Il2CppClass *>(klass), unityClass);

            // --- 新增：获取成员方法 ---
            DumpMethods(const_cast<Il2CppClass *>(klass), unityClass);

            //存入 Map
            // uniqueName 是经过 CleanIdentifier 处理的，例如 "FrameEngine_Logic_BattleTeam"
            classMap[uniqueName] = unityClass;

            LOG(LOG_LEVEL_INFO, "[li2cppHeader] Processed: %s", uniqueName.c_str());
        }
    }

    LOG(LOG_LEVEL_INFO, "[li2cppHeader] <<< Exiting: %s, Total: %zu", __FUNCTION__, classMap.size());
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
        case IL2CPP_TYPE_STRING:   return "String_o*";
        case IL2CPP_TYPE_PTR:      return "void*";
        case IL2CPP_TYPE_FNPTR:    return "void*";
        case IL2CPP_TYPE_OBJECT:  return "struct System_Object_o*";

        case IL2CPP_TYPE_SZARRAY: {
            Il2CppClass* elementKlass = il2cpp_class_from_type(type->data.type);
            // 直接使用 GetSafeUniqueName，不再手动拼前缀
            return "struct " + GetSafeUniqueName(elementKlass) + "_array*";
        }

        case IL2CPP_TYPE_CLASS:
        case IL2CPP_TYPE_GENERICINST: {
            Il2CppClass* klass = il2cpp_class_from_type(type);
            return "struct " + GetSafeUniqueName(klass) + "_o*"; // 直接用，不要拼
        }

        case IL2CPP_TYPE_VALUETYPE: {
            Il2CppClass* klass = il2cpp_class_from_type(type);
            if (il2cpp_class_is_enum(klass)) return "int32_t";
            return "struct " + GetSafeUniqueName(klass) + "_Fields";
        }

        default: return "void*";
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

/**
 * 递归解析 IL2CPP 类型名，并转换为 IDA 安全的标识符
 * 目标：处理嵌套泛型，如 Dictionary<string, List<int>> -> Dictionary_string_List_int_
 */
std::string li2cppHeader::li2cppHeader::GetSafeGenericName(const Il2CppType* type) {
    if (!type) return "UnknownType";

    // 1. 获取基础类型名 (IL2CPP 会返回类似 List<int> 或 List`1 的字符串)
    char* rawName = const_cast<char *>(il2cpp_type_get_name(type));
    std::string fullName = rawName ? rawName : "Unnamed";
    // 注意：某些环境下 il2cpp_type_get_name 返回的内存需要释放，根据你的 il2cpp 版本决定是否调用 il2cpp_free

    // 2. 特殊符号全量清洗
    // 我们需要把所有 C 语言结构体命名中非法的字符替换为下划线
    // 关键字符：< > (泛型), [ ] (数组/泛型), , (参数分隔), ` (泛型计数), . (命名空间)
    for (char &c : fullName) {
        if (c == '<' || c == '>' || c == '[' || c == ']' ||
            c == ',' || c == '`' || c == '.' || c == '+' ||
            c == '/' || c == ' ' || c == '*' || c == ':') {
            c = '_';
        }
    }

    // 3. 规范化处理：去除连续下划线，使名字更美观
    // 正则替换： "___" -> "_"
    fullName = std::regex_replace(fullName, std::regex("_{2,}"), "_");

    // 4. 去除首尾可能多余的下划线
    if (!fullName.empty() && fullName[0] == '_') fullName.erase(0, 1);
    if (!fullName.empty() && fullName.back() == '_') fullName.pop_back();

    return fullName;
}


/**
 * 优化后的唯一名获取：内部直接调用清洗函数
 */
std::string li2cppHeader::li2cppHeader::GetSafeUniqueName(Il2CppClass* klass) {
    if (!klass) return "Il2CppObject";

    const Il2CppType* type = il2cpp_class_get_type(klass);
    char* rawName = const_cast<char *>(il2cpp_type_get_name(type));

    // 如果 rawName 已经包含命名空间(通常点号分隔)，CleanIdentifier 会把它变下划线
    std::string fullName = rawName ? rawName : il2cpp_class_get_name(klass);

    // 如果发现 rawName 不带命名空间，再手动补一下
    if (fullName.find('.') == std::string::npos) {
        const char* ns = il2cpp_class_get_namespace(klass);
        if (ns && strlen(ns) > 0) {
            fullName = std::string(ns) + "_" + fullName;
        }
    }

    return CleanIdentifier(fullName);
}

/**
 * 核心清洗函数：将所有非法字符替换为下划线，并压缩连续下划线
 */
std::string li2cppHeader::li2cppHeader::CleanIdentifier(std::string name) {
    for (char &c : name) {
        // 替换点号、尖括号、方括号、逗号、空格等所有 C 语法不支持的符号
        if (c == '.' || c == '<' || c == '>' || c == '`' ||
            c == ',' || c == '[' || c == ']' || c == '+' ||
            c == '/' || c == ' ' || c == '*' || c == ':' || c == '-') {
            c = '_';
        }
    }
    // 使用正则压缩连续的下划线，防止出现 ____
    name = std::regex_replace(name, std::regex("_{2,}"), "_");

    // 移除首尾多余的下划线
    if (!name.empty() && name[0] == '_') name.erase(0, 1);
    if (!name.empty() && name.back() == '_') name.pop_back();

    return name;
}

void li2cppHeader::li2cppHeader::SaveToIdaHeader(const std::string& path) {
    std::ofstream fout(path);
    if (!fout.is_open()) return;

    // 1. 基础结构定义
    fout << "/* Generated by Gemini | Corrected Version */\n";
    fout << "#include <stdint.h>\n\n";
    fout << "typedef struct VirtualInvokeData { void* methodPtr; void* method; } VirtualInvokeData;\n";
    fout << "typedef struct Il2CppClass_c Il2CppClass_c;\n";
    fout << "typedef struct Il2CppObject { struct Il2CppClass_c *klass; void *monitor; } Il2CppObject;\n";
    fout << "typedef struct Il2CppArrayBounds { uintptr_t length; uintptr_t lower_bound; } Il2CppArrayBounds;\n";
    fout << "typedef struct String_o { Il2CppObject obj; int32_t length; uint16_t chars[0]; } String_o;\n\n";

    // 2. 前置声明：使用 uniqueKey (它是已经 CleanIdentifier 过的)
    fout << "/* --- Forward Declarations --- */\n";
    for (auto const& [uniqueKey, cls] : m_classMap) {
        fout << "typedef struct " << uniqueKey << "_o " << uniqueKey << "_o;\n";
        fout << "typedef struct " << uniqueKey << "_c " << uniqueKey << "_c;\n";
        fout << "typedef struct " << uniqueKey << "_Fields " << uniqueKey << "_Fields;\n";
        fout << "typedef struct " << uniqueKey << "_StaticFields " << uniqueKey << "_StaticFields;\n";
        fout << "typedef struct " << uniqueKey << "_array " << uniqueKey << "_array;\n";
    }

    // 3. 详细结构定义
    for (auto const& [uniqueKey, cls] : m_classMap) {
        // 修正 1: 使用 cls->className 代替 cls->name
        fout << "\n/* --- " << (cls->namespc.empty() ? "Global" : cls->namespc) << "::" << cls->className << " --- */\n";

        // A. 静态字段
        fout << "struct " << uniqueKey << "_StaticFields {\n";
        bool hasStatic = false;
        for (const auto& field : cls->fields) {
            if (field.isStatic) {
                // 修正 2: 使用 field.staticAddr 代替 field.metadataPtr
                fout << "    " << field.typeName << " " << field.name << "; // Addr: " << IntToHex(field.staticAddr) << "\n";
                hasStatic = true;
            }
        }
        if (!hasStatic) fout << "    char unused;\n";
        fout << "};\n";

        // B. 实例字段
        fout << "struct " << uniqueKey << "_Fields {\n";
        if (cls->fields.empty()) {
            fout << "    char padding[1];\n";
        } else {
            for (const auto& field : cls->fields) {
                if (field.isStatic) continue;
                // 确保 field.typeName 在生成时也调用了 CleanIdentifier 处理
                fout << "    " << field.typeName << " " << field.name << "; // Offset: " << IntToHex(field.offset) << "\n";
            }
        }
        fout << "};\n";

        fout << "/* Methods for " << uniqueKey << " */\n";
        for (const auto& method : cls->methods) {
            fout << "// " << method.returnType << " " << method.name << "(";
            for (size_t k = 0; k < method.parameterTypes.size(); ++k) {
                fout << method.parameterTypes[k] << (k == method.parameterTypes.size() - 1 ? "" : ", ");
            }
            // 打印相对于 il2cpp 模块基址的偏移 (假设 m_il2cppbase 已设置)
            uintptr_t offset = (method.methodPointer != 0) ? (method.methodPointer - m_il2cppbase) : 0;
            fout << "); // RVA: " << IntToHex(offset) << " | Token: " << IntToHex(method.token) << "\n";
        }

        // C. 类信息
        fout << "struct " << uniqueKey << "_c {\n";
        fout << "    void* image; void* gc_desc; const char* name; const char* namespc;\n";
        fout << "    uint32_t flags; uint16_t instance_size; uint16_t actualSize;\n";
        fout << "    struct " << uniqueKey << "_StaticFields* static_fields;\n";
        fout << "    VirtualInvokeData vtable[32];\n";
        fout << "};\n";

        // D. 实例对象
        fout << "struct " << uniqueKey << "_o {\n";
        fout << "    struct " << uniqueKey << "_c *klass;\n";
        fout << "    void *monitor;\n";
        fout << "    struct " << uniqueKey << "_Fields fields;\n";
        fout << "};\n";

        // E. 数组对象
        fout << "struct " << uniqueKey << "_array {\n";
        fout << "    Il2CppObject obj; Il2CppArrayBounds *bounds; uintptr_t max_length;\n";
        fout << "    struct " << uniqueKey << "_o* m_Items[0];\n";
        fout << "};\n";
    }

    fout.close();
}

std::string li2cppHeader::li2cppHeader::GetTypeAbbreviation(const std::string& type) {
    if (type == "void") return "v";
    if (type == "bool") return "b";
    if (type == "int32_t" || type == "uint32_t") return "i";
    if (type == "int64_t" || type == "uint64_t") return "j";
    if (type == "float") return "f";
    if (type == "double") return "d";
    if (type.find('*') != std::string::npos) return "i"; // 指针在 IL2CPP 中通常视为 native int (i)
    return "i"; // 默认作为对象/指针处理
}

void li2cppHeader::li2cppHeader::DumpMethods(Il2CppClass* klass, std::shared_ptr<UnityClass>& unityClass) {
    void* iter = nullptr;
    const MethodInfo* method = nullptr;
    std::string classSafeName = GetSafeUniqueName(klass);

    while ((method = il2cpp_class_get_methods(klass, &iter))) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(method->methodPointer);
        if (addr == 0) continue; // 跳过没有实现的虚方法或抽象方法

        const char* mName = il2cpp_method_get_name(method);
        const Il2CppType* retType = il2cpp_method_get_return_type(method);
        std::string retTypeName = GetIdaCompatibleType(retType);

        UnityMethod uMethod(mName, retTypeName, addr, method->token);

        // --- 生成 TypeSignature (如 viii) ---
        uMethod.typeSignature = GetTypeAbbreviation(retTypeName);

        // --- 生成 Signature ---
        // 格式：ReturnType Class_Method (ThisType __this, Params...);
        std::string sig = retTypeName + " " + classSafeName + "_" + CleanIdentifier(mName) + " (";

        // 注入 __this 指针 (实例方法)
        sig += classSafeName + "_o* __this";
        uMethod.typeSignature += "i"; // __this 也是一个指针

        uint32_t pCount = il2cpp_method_get_param_count(method);
        for (uint32_t i = 0; i < pCount; ++i) {
            const Il2CppType* pType = il2cpp_method_get_param(method, i);
            std::string pTypeName = GetIdaCompatibleType(pType);

            sig += ", " + pTypeName + " p" + std::to_string(i);
            uMethod.typeSignature += GetTypeAbbreviation(pTypeName);
        }
        sig += ", const MethodInfo* method);";
        uMethod.typeSignature += "i"; // 结尾的 MethodInfo*

        uMethod.signature = sig;
        unityClass->methods.push_back(uMethod);
    }
}

void li2cppHeader::li2cppHeader::SaveToMethodJson(const std::string& path) {
    std::ofstream fout(path);
    if (!fout.is_open()) return;

    // 1. 在最前面加上 {"ScriptMethod": [ 结构
    fout << "{\n";
    fout << "  \"ScriptMethod\": [\n";

    bool firstEntry = true;

    for (auto const& [uniqueKey, cls] : m_classMap) {
        for (const auto& method : cls->methods) {
            if (!firstEntry) fout << ",\n";

            fout << "    {\n";
            fout << "      \"Address\": " << method.methodPointer << ",\n";
            // 这里使用 cls->namespc 和 cls->className 组合成 Interop.ErrorInfo 风格
            fout << "      \"Name\": \"" << (cls->namespc.empty() ? "" : cls->namespc + ".") << cls->className << "$$." << method.name << "\",\n";
            fout << "      \"Signature\": \"" << method.signature << "\",\n";
            fout << "      \"TypeSignature\": \"" << method.typeSignature << "\"\n";
            fout << "    }";

            firstEntry = false;
        }
    }

    // 2. 闭合 JSON 结构
    fout << "\n  ]\n";
    fout << "}";

    fout.close();
}