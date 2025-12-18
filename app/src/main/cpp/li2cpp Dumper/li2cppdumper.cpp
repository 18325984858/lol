//
// Created by Song on 2025/11/19.
//

#include "li2cppdumper.h"
#include "../Log/log.h"
#include <sstream>

li2cpp::li2cppDumper::li2cppDumper(void*dqil2cppBase,
                                   void *pCodeRegistration,
                                   void *pMetadataRegistration,
                                   void *pGlobalMetadataHeader,
                                   void*pMetadataImagesTable):
                                   li2cppApi::cUnityApi(dqil2cppBase,
                                                        static_cast<Il2CppCodeRegistration *>(pCodeRegistration),
                                                        static_cast<Il2CppMetadataRegistration *>(pMetadataRegistration),
                                                        static_cast<Il2CppGlobalMetadataHeader *>(pGlobalMetadataHeader),
                                                        static_cast<Il2CppImageGlobalMetadata *>(pMetadataImagesTable)){

    if(m_outDumpCs == nullptr){
        m_outDumpCs = std::make_shared<cMyfile>(m_pathDumpCs);
        if(m_outDumpCs) {
            m_outDumpCs->openFile(cMyfile::FileMode::ReadWrite);
        }
    }

    if(m_outDumpstr == nullptr){
        m_outDumpstr = std::make_shared<cMyfile>(m_pathDumpstr);
        if(m_outDumpstr) {
            m_outDumpstr->openFile(cMyfile::FileMode::ReadWrite);
        }
    }

    if(m_outlog == nullptr){
        m_outlog = std::make_shared<cMyfile>(m_pathlog);
        if(m_outlog){
            m_outlog->openFile(cMyfile::FileMode::ReadWrite);
        }
    }

    if(m_outPuts == nullptr){
         m_outPuts = std::make_shared<std::vector<std::string>>();
    }

    if(m_kIl2CppMetadataUsageStringLiteral == nullptr){
        m_kIl2CppMetadataUsageStringLiteral = std::make_shared<std::map<int,std::string>>();
    }

    if(m_methodList == nullptr){
        m_methodList = std::make_shared<std::list<std::shared_ptr<cMethodDefinitionAndMethodSpec>>>();
    }

}

li2cpp::li2cppDumper::~li2cppDumper(){

    if(m_outDumpCs){
        for (const auto& elem : *m_outPuts) { // const& 避免拷贝，且只读
            m_outDumpCs->writeLine(elem);
        }
        m_outDumpCs->flush();
        m_outDumpCs->closeFile();
    }

    if(m_kIl2CppMetadataUsageStringLiteral){
        for(const auto& elem:*m_kIl2CppMetadataUsageStringLiteral){
            m_outDumpstr->writeLine(elem.second);
        }
        m_outDumpstr->flush();
        m_outDumpstr->closeFile();
    }

    if(m_outlog){
        m_outlog->flush();
        m_outDumpstr->closeFile();
    }


    m_methodList = nullptr;
    m_outPuts = nullptr;
    m_outDumpCs = nullptr;
}

bool li2cpp::li2cppDumper::initInfo(){

    do {
        //判断是否在调试状态
        if(m_pGlobalMetadataHeader == nullptr ||
        m_pil2CppMetadataRegistration == nullptr ||
        il2cpp_is_debugger_attached()){
            break;
        }

        char szbuf[0x1000] = {0};
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sprintf(szbuf,"genericClassesCount : %d",m_pil2CppMetadataRegistration->genericClassesCount);
        writeLog(szbuf);
        sprintf(szbuf,"genericInstsCount : %d",m_pil2CppMetadataRegistration->genericInstsCount);
        writeLog(szbuf);
        sprintf(szbuf,"genericMethodTableCount : %d",m_pil2CppMetadataRegistration->genericMethodTableCount);
        writeLog(szbuf);
        sprintf(szbuf,"typesCount : %d",m_pil2CppMetadataRegistration->typesCount);
        writeLog(szbuf);
        sprintf(szbuf,"fieldOffsetsCount : %d",m_pil2CppMetadataRegistration->fieldOffsetsCount);
        writeLog(szbuf);
        sprintf(szbuf,"genericClassesCount : %d",m_pil2CppMetadataRegistration->genericClassesCount);
        writeLog(szbuf);
        sprintf(szbuf,"typeDefinitionsSizesCount : %d",m_pil2CppMetadataRegistration->typeDefinitionsSizesCount);
        writeLog(szbuf);
        sprintf(szbuf,"metadataUsagesCount : %d",m_pil2CppMetadataRegistration->metadataUsagesCount);
        writeLog(szbuf);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sprintf(szbuf, "reversePInvokeWrapperCount : %d", m_pIl2CppCodeRegistration->reversePInvokeWrapperCount);
        writeLog(szbuf);
        sprintf(szbuf, "genericMethodPointersCount : %d", m_pIl2CppCodeRegistration->genericMethodPointersCount);
        writeLog(szbuf);
        sprintf(szbuf, "invokerPointersCount : %d", m_pIl2CppCodeRegistration->invokerPointersCount);
        writeLog(szbuf);
        sprintf(szbuf, "unresolvedIndirectCallCount : %d", m_pIl2CppCodeRegistration->unresolvedIndirectCallCount);
        writeLog(szbuf);
        sprintf(szbuf, "interopDataCount : %d", m_pIl2CppCodeRegistration->interopDataCount);
        writeLog(szbuf);
        sprintf(szbuf, "windowsRuntimeFactoryCount : %d", m_pIl2CppCodeRegistration->windowsRuntimeFactoryCount);
        writeLog(szbuf);
        sprintf(szbuf, "codeGenModulesCount : %d", m_pIl2CppCodeRegistration->codeGenModulesCount);
        writeLog(szbuf);

        dumpGenericsMethod();
        dumpcs();
        dumpStr();

        return true;
    }while(0);
    return false;
}

std::string li2cpp::li2cppDumper::dumpType(const Il2CppType *type) {
    std::stringstream outPut;
    auto *klass = il2cpp_class_from_type(type);
    outPut << "\n// Namespace: " << il2cpp_class_get_namespace(klass) << "\n";
    auto flags = il2cpp_class_get_flags(klass);
    if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
        outPut << "[Serializable]\n";
    }
    //TODO attribute
    auto is_valuetype = il2cpp_class_is_valuetype(klass);
    auto is_enum = il2cpp_class_is_enum(klass);
    auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
    switch (visibility) {
        case TYPE_ATTRIBUTE_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_PUBLIC:
            outPut << "public ";
            break;
        case TYPE_ATTRIBUTE_NOT_PUBLIC:
        case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
        case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
            outPut << "internal ";
            break;
        case TYPE_ATTRIBUTE_NESTED_PRIVATE:
            outPut << "private ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAMILY:
            outPut << "protected ";
            break;
        case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "static ";
    } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
    } else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED) {
        outPut << "sealed ";
    }
    if (flags & TYPE_ATTRIBUTE_INTERFACE) {
        outPut << "interface ";
    } else if (is_enum) {
        outPut << "enum ";
    } else if (is_valuetype) {
        outPut << "struct ";
    } else {
        outPut << "class ";
    }


    //判断是否是嵌套类型
    std::string fullClassName = il2cpp_class_get_name(klass);
    Il2CppClass* declaring = il2cpp_class_get_declaring_type(klass);
    while (declaring != nullptr) {
        fullClassName = std::string(il2cpp_class_get_name(declaring)) + "." + fullClassName;
        declaring = il2cpp_class_get_declaring_type(declaring);
    }
    outPut << fullClassName;


    //outPut << il2cpp_class_get_name(klass);

    std::vector<std::string> extends;
    auto parent = il2cpp_class_get_parent(klass);
    if (!is_valuetype && !is_enum && parent) {
        auto parent_type = il2cpp_class_get_type(parent);
        if (parent_type->type != IL2CPP_TYPE_OBJECT) {
            extends.emplace_back(il2cpp_class_get_name(parent));
        }
    }
    void *iter = nullptr;
    while (auto itf = il2cpp_class_get_interfaces(klass, &iter)) {
        extends.emplace_back(il2cpp_class_get_name(itf));
    }
    if (!extends.empty()) {
        outPut << " : " << extends[0];
        for (int i = 1; i < extends.size(); ++i) {
            outPut << ", " << extends[i];
        }
    }
    outPut << "\n{";
    outPut << dump_field(klass);
    outPut << dump_property(klass);
    outPut << dump_method(klass);
    //TODO EventInfo
    outPut << "}\n";
    return outPut.str();
}

std::string li2cpp::li2cppDumper::get_type_name(const Il2CppType* type) {
    if (type == nullptr) {
        return "void";
    }

    // 处理指针和数组（简单示例）
    if (type->byref) {
        return get_type_name(type) + "&"; // 简单引用
    }

    // ------------------------------------------
    // 1. 获取 Il2CppClass
    // ------------------------------------------
    auto klass = il2cpp_class_from_type(type);

    //判断类型
    switch (type->type)
    {
        case IL2CPP_TYPE_VOID:
        {
           return "void";
        }
        case IL2CPP_TYPE_CLASS:
        case IL2CPP_TYPE_VALUETYPE:
        {
            return klass && il2cpp_class_get_name(const_cast<Il2CppClass *>(klass)) ? il2cpp_class_get_name(const_cast<Il2CppClass *>(klass)) : "unknown";
        }

        case IL2CPP_TYPE_GENERICINST:
        {
            break;
        }

        case IL2CPP_TYPE_VAR:   // T
            return il2cpp_type_get_name(type);

        case IL2CPP_TYPE_MVAR:  // 方法泛型 T
            return il2cpp_type_get_name(type);
        default:
            return "/* Unknown Type */";;
    }

    // ------------------------------------------
    // 2. 拼接嵌套类名 (如 Display.DisplaysUpdatedDelegate)
    // ------------------------------------------
    std::string typeName = il2cpp_class_get_name(klass);
    Il2CppClass *declaring = il2cpp_class_get_declaring_type(klass);
    while (declaring != nullptr) {
        typeName = std::string(il2cpp_class_get_name(declaring)) + "." + typeName;
        declaring = il2cpp_class_get_declaring_type(declaring);
    }

    // ------------------------------------------
    // 3. 处理泛型 (如 Dictionary<int, RenderInstancedDataLayout>)
    // ------------------------------------------

    // 移除末尾的 `N (例如 Dictionary`2 -> Dictionary)
    size_t backtickPos = typeName.find_last_of('`');
    if (backtickPos != std::string::npos) {
        typeName = typeName.substr(0, backtickPos);
    }

    // 获取类型参数
    auto generic_type = il2cpp_class_get_generic_class(klass);
    if (generic_type) {
        auto context = il2cpp_generic_class_get_context(generic_type);
        auto inst = il2cpp_generic_context_get_generic_class_inst(context);

        if (inst && inst->type_argc > 0) {
            typeName += "<";
            for (uint32_t i = 0; i < inst->type_argc; ++i) {
                if (i  > 0) {
                    typeName += ", ";
                }
                // 递归调用 get_type_name 来处理泛型参数本身可能是泛型或嵌套类的情况
                typeName += get_type_name(inst->type_argv[i]);
            }
            typeName += ">";
        }
    }
    return typeName;
}

std::string li2cpp::li2cppDumper::dump_field(Il2CppClass *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Fields\n";
    auto is_enum = il2cpp_class_is_enum(klass);
    void *iter = nullptr;
    while (auto field = il2cpp_class_get_fields(klass, &iter)) {
        //... [访问修饰符和关键字 (public/static/readonly/const) 保持不变] ...
        outPut << "\t";
        auto attrs = il2cpp_field_get_flags(field);
        auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
        switch (access) {
            //... [省略访问修饰符 switch case] ...
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                outPut << "protected internal ";
                break;
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL) {
            outPut << "const ";
        } else {
            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                outPut << "static ";
            }
            if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                outPut << "readonly ";
            }
        }

        auto field_type = il2cpp_field_get_type(field);

        // --- 修改点：使用新的类型名称解析函数 ---
        // outPut << il2cpp_class_get_name(il2cpp_class_from_type(field_type)) << " " << il2cpp_field_get_name(field); // 原有代码
        outPut << get_type_name(field_type) << " " << il2cpp_field_get_name(field);
        // ------------------------------------

        //TODO 获取构造函数初始化后的字段值
        if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum) {
            uint64_t val = 0;
            il2cpp_field_static_get_value(field, &val);
            outPut << " = " << std::dec << val;
        }
        outPut << "; // 0x" << std::hex << il2cpp_field_get_offset(field) << "\n";
    }
    return outPut.str();
}

std::string li2cpp::li2cppDumper::dump_property(Il2CppClass *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Properties\n";
    void *iter = nullptr;
    while (auto prop_const = il2cpp_class_get_properties(klass, &iter)) {
        //TODO attribute
        auto prop = const_cast<PropertyInfo *>(prop_const);
        auto get = il2cpp_property_get_get_method(prop);
        auto set = il2cpp_property_get_set_method(prop);
        auto prop_name = il2cpp_property_get_name(prop);
        outPut << "\t";
        Il2CppClass *prop_class = nullptr;
        uint32_t iflags = 0;
        if (get) {
            outPut << get_method_modifier(il2cpp_method_get_flags(get, &iflags));
            prop_class = il2cpp_class_from_type(il2cpp_method_get_return_type(get));
        } else if (set) {
            outPut << get_method_modifier(il2cpp_method_get_flags(set, &iflags));
            auto param = il2cpp_method_get_param(set, 0);
            prop_class = il2cpp_class_from_type(param);
        }
        if (prop_class) {
            outPut << il2cpp_class_get_name(prop_class) << " " << prop_name << " { ";
            if (get) {
                outPut << "get; ";
            }
            if (set) {
                outPut << "set; ";
            }
            outPut << "}\n";
        } else {
            if (prop_name) {
                outPut << " // unknown property " << prop_name;
            }
        }
    }
    return outPut.str();
}

std::string li2cpp::li2cppDumper::get_method_modifier(uint32_t flags) {
    std::stringstream outPut;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
    switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        outPut << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            outPut << "virtual ";
        } else {
            outPut << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        outPut << "extern ";
    }
    return outPut.str();
}

std::string li2cpp::li2cppDumper::dump_method(Il2CppClass *klass) {
    std::stringstream outPut;
    outPut << "\n\t// Methods\n";
    void *iter = nullptr;
    while (auto method = il2cpp_class_get_methods(klass, &iter)) {
        //TODO attribute
        if (method->methodPointer) {
            outPut << "\t// RVA: 0x";
            outPut << std::hex << (uint64_t) method->methodPointer - Getil2cppModuleBase();
            outPut << " VA: 0x";
            outPut << std::hex << (uint64_t) method->methodPointer;
        } else {
            outPut << "\t// RVA: -1";
        }
        /*if (method->slot != 65535) {
            outPut << " Slot: " << std::dec << method->slot;
        }*/
        outPut << "\n\t";
        uint32_t iflags = 0;
        auto flags = il2cpp_method_get_flags(method, &iflags);
        outPut << get_method_modifier(flags);
        //TODO genericContainerIndex
        auto return_type = il2cpp_method_get_return_type(method);
        if (_il2cpp_type_is_byref(return_type)) {
            outPut << "ref ";
        }
        auto return_class = il2cpp_class_from_type(return_type);
        outPut << il2cpp_class_get_name(return_class) << " " << il2cpp_method_get_name(method)
               << "(";
        auto param_count = il2cpp_method_get_param_count(method);
        for (int i = 0; i < param_count; ++i) {
            auto param = il2cpp_method_get_param(method, i);
            auto attrs = param->attrs;
            if (_il2cpp_type_is_byref(param)) {
                if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                    outPut << "out ";
                } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                    outPut << "in ";
                } else {
                    outPut << "ref ";
                }
            } else {
                if (attrs & PARAM_ATTRIBUTE_IN) {
                    outPut << "[In] ";
                }
                if (attrs & PARAM_ATTRIBUTE_OUT) {
                    outPut << "[Out] ";
                }
            }
            auto parameter_class = il2cpp_class_from_type(param);
            outPut << il2cpp_class_get_name(parameter_class) << " "
                   << il2cpp_method_get_param_name(method, i);
            outPut << ", ";
        }
        if (param_count > 0) {
            outPut.seekp(-2, outPut.cur);
        }
        outPut << ") { }\n";


        //TODO GenericInstMethod
        if(method->methodPointer == nullptr){
            auto token = method->token;
            auto pMethodList1 = GetMethodToList(token);
            //判断取出的数据是否为空
            if(pMethodList1 && !pMethodList1->empty()){
                for(const auto& elem : *pMethodList1){
                    std::string methodsrcname = il2cpp_method_get_name(method);

                    methodsrcname.append(".");

                    methodsrcname += get_method_space_name(elem->Spec);

                    char szbuf[0x1000] = {0};
                    sprintf(szbuf,"0x%08X",elem->offset);

                    writeLog(szbuf);
                    writeLog(methodsrcname);
                }
            }
        }

    }
    return outPut.str();
}

std::string li2cpp::li2cppDumper::dumpStr() {
    std::stringstream outPut;

    for (size_t i = 0; i < m_pil2CppMetadataRegistration->metadataUsagesCount; i++){

        uintptr_t* metadataPointer = reinterpret_cast<uintptr_t*>(m_pil2CppMetadataRegistration->metadataUsages[i]);
        if(metadataPointer) {

            Il2CppMetadataUsage usage = GetEncodedIndexType(static_cast<uint32_t>(*metadataPointer));
            switch (usage) {
                case kIl2CppMetadataUsageTypeInfo:
                case kIl2CppMetadataUsageMethodDef:
                case kIl2CppMetadataUsageMethodRef:
                    dump_RuntimeMetadata(metadataPointer);
                    break;
                default:
                    break;
            }
        }
    }

    return outPut.str();
}

std::string li2cpp::li2cppDumper::dump_RuntimeMetadata(uintptr_t *metadataPointer) {
    std::string pstring = "";
    do {
        if(!metadataPointer){
            break;
        }
        uintptr_t metadataValue = *(intptr_t*)metadataPointer;
        uint32_t encodedToken = static_cast<uint32_t>(metadataValue);
        Il2CppMetadataUsage usage = GetEncodedIndexType(encodedToken);
        uint32_t decodedIndex = GetDecodedMethodIndex(encodedToken);
        switch (usage)
        {
            case kIl2CppMetadataUsageTypeInfo:
                break;
            case kIl2CppMetadataUsageIl2CppType:
                break;
            case kIl2CppMetadataUsageMethodDef:
            case kIl2CppMetadataUsageMethodRef:
                break;
            case kIl2CppMetadataUsageFieldInfo:
                break;
            case kIl2CppMetadataUsageStringLiteral:
                pstring = il2cpp_Il2CppString_toCString(GetStringLiteralFromIndex(decodedIndex));
                if(m_kIl2CppMetadataUsageStringLiteral){
                    m_kIl2CppMetadataUsageStringLiteral->insert(std::pair<int,std::string>(decodedIndex,pstring));
                }
                break;
            case kIl2CppMetadataUsageFieldRva:
                break;
            case kIl2CppMetadataUsageInvalid:
                break;
        }
    }while(0);
    return pstring;
}

bool li2cpp::li2cppDumper::writeLog(std::string str) {
    std::string out = "[SFK] : ";
    out+=str;
    m_outlog->writeLine(out.c_str());
    m_outlog->flush();
    return true;
}

std::string li2cpp::li2cppDumper::dumpcs() {
    std::string pstring = "";
    do {

        LOG(LOG_LEVEL_ERROR,"dumping...");
        size_t size;
        auto domain = il2cpp_domain_get();
        auto assemblies = il2cpp_domain_get_assemblies(domain, &size);

        std::stringstream imageOutput;

        //遍历获取程序集（SO）名称
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            imageOutput << "// Image " << i << ": " << il2cpp_image_get_name(image) << "\n";
        }

        //遍历每个程序集（SO）的数据情况
        LOG(LOG_LEVEL_INFO,"Version greater than 2023.3");
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            imageStr << "\n// Dll : " << il2cpp_image_get_name(image);
            auto classCount = il2cpp_image_get_class_count(image);

            for (int j = 0; j < classCount; ++j) {
                auto klass = il2cpp_image_get_class(image, j);
                auto type = il2cpp_class_get_type(const_cast<Il2CppClass *>(klass));
                //LOGD("type name : %s", il2cpp_type_get_name(type));
                auto outPut = imageStr.str() + dumpType(type);
                m_outPuts->push_back(outPut);
            }
        }
        return pstring;
    }while(0);
    return pstring;
}

std::string li2cpp::li2cppDumper::dumpGenericsMethod() {
    //参考 GetMethodInfoFromMethodDefinitionIndex
    std::string pstring = "";

    for (int32_t i = 0; i < m_pil2CppMetadataRegistration->genericMethodTableCount; i++){

        const Il2CppGenericMethodFunctionsDefinitions* genericMethodIndices = m_pil2CppMetadataRegistration->genericMethodTable+i;

        if(genericMethodIndices && genericMethodIndices->genericMethodIndex < m_pil2CppMetadataRegistration->methodSpecsCount) {

            const Il2CppMethodSpec *methodSpec = m_pil2CppMetadataRegistration->methodSpecs+genericMethodIndices->genericMethodIndex;

            if(methodSpec && m_pIl2CppCodeRegistration->genericMethodPointersCount>genericMethodIndices->indices.methodIndex) {

                const Il2CppMethodDefinition *methodDefinition = GetMethodDefinitionFromIndex(
                        methodSpec->methodDefinitionIndex);

                const Il2CppMethodPointer *pMethodPointer = &m_pIl2CppCodeRegistration->genericMethodPointers[genericMethodIndices->indices.methodIndex];

                uint64_t offset = 0;
                if (pMethodPointer) {
                    offset = (uint64_t) ((uint64_t) pMethodPointer - Getil2cppModuleBase());
                }

                if (methodSpec && methodDefinition) {
                    if (m_methodList) {
                        auto p = std::make_shared<li2cpp::cMethodDefinitionAndMethodSpec>();

                        p->Spec = methodSpec;
                        p->offset = offset;
                        p->Method = methodDefinition;

                        m_methodList->push_back(p);
                    }
                }
            }
        }
    }
    return pstring;
}

std::shared_ptr<std::list<std::shared_ptr<li2cpp::cMethodDefinitionAndMethodSpec>>>
li2cpp::li2cppDumper::GetMethodToList(uint32_t Token) {

    //申请一个链表 用于返回
    std::shared_ptr<std::list<std::shared_ptr<li2cpp::cMethodDefinitionAndMethodSpec>>> plist =
            std::make_shared<std::list<std::shared_ptr<li2cpp::cMethodDefinitionAndMethodSpec>>>();

    //判断要查找的链表是否为空
    if(m_methodList && !m_methodList->empty()){
        //遍历链表查找一样的 Token 值加入到链表中并返回
        for (const auto&elem :*m_methodList) {

            const Il2CppMethodDefinition *methodDefinition = GetMethodDefinitionFromIndex(
                    elem->Spec->methodDefinitionIndex);

            if(methodDefinition->token == Token){
                plist->push_back(elem);
            }
        }
    }
    return plist;
}


std::string li2cpp::li2cppDumper::get_method_space_name(const Il2CppMethodSpec* spec){

    std::string retstr = "";
    do{
        if(spec == nullptr){
            break;
        }

        const Il2CppMethodDefinition* pmethode = GetMethodDefinitionFromIndex(spec->methodDefinitionIndex);
        if(pmethode == nullptr){
            break;
        }

        const Il2CppTypeDefinition * pmethodetype = GetTypeDefinitionForIndex(pmethode->declaringType);
        if(pmethodetype == nullptr){
            break;
        }

        retstr = get_method_space_class_name(pmethodetype->nameIndex,spec->methodIndexIndex);

    }while(0);

    return retstr;
}

std::string li2cpp::li2cppDumper::get_method_space_class_name(StringIndex stringIndex,
                                                              GenericInstIndex genericInstIndex) {
    std::string retstr = GetStringFromIndex(stringIndex);

    retstr+=".";

    if(retstr.empty()){
        return retstr;
    }

    do {
        //判断是否是泛型函数 并移除`
        size_t backtickPos = retstr.find_last_of('`');
        if (backtickPos != std::string::npos) {
            retstr = retstr.substr(0, backtickPos);
            std::string genericename = get_method_generic_name(genericInstIndex);
            if (genericename.empty()) {
                break;
            }
            retstr+=genericename;
        }
    }while(0);
    return retstr;
}

/*
std::string li2cpp::li2cppDumper::get_method_generic_name(GenericInstIndex genericInstIndex) {

    std::string retstr = "";

    do {
        if(genericInstIndex == kTypeIndexInvalid){
            break;
        }

        IL2CPP_ASSERT(genericInstIndex < m_pil2CppMetadataRegistration->genericInstsCount);
        const Il2CppGenericInst *classInst = m_pil2CppMetadataRegistration->genericInsts[genericInstIndex];
        if(classInst == nullptr){
            break;
        }

        retstr.append("<");
        for (uint32_t i = 0; i < classInst->type_argc; ++i){
            //获取类类型
            const Il2CppType * pil2cctype = classInst->type_argv[i];
            if(pil2cctype == nullptr){
                continue;
            }

            const char* pGenerictypename =  il2cpp_type_get_name(pil2cctype);
            if(pGenerictypename == nullptr){
                break;
            }
            retstr.append(pGenerictypename);

            if(i != classInst->type_argc-1){
                retstr.append(",");
            }
        }
        retstr.append(">");
    }while(0);
    return retstr;
}
*/

std::string li2cpp::li2cppDumper::get_method_generic_name(GenericInstIndex genericInstIndex)
{
    if (genericInstIndex == kTypeIndexInvalid)
        return "";

    IL2CPP_ASSERT(genericInstIndex < m_pil2CppMetadataRegistration->genericInstsCount);

    const Il2CppGenericInst* inst =
            m_pil2CppMetadataRegistration->genericInsts[genericInstIndex];

    if (!inst || inst->type_argc == 0)
        return "";

    std::string ret;
    ret.append("<");

    for (uint32_t i = 0; i < inst->type_argc; ++i)
    {
        ret.append(get_type_name(inst->type_argv[i]));
        if (i + 1 < inst->type_argc)
            ret.append(", ");
    }

    ret.append(">");

    return ret;
}
