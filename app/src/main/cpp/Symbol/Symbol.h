//
// Created by Song on 2025/11/23.
//

#ifndef DOBBY_PROJECT_SYMBOL_H
#define DOBBY_PROJECT_SYMBOL_H



#include <map>
#include <string>

namespace Symbol{

    class Symbol{

    public:
        Symbol(const std::string & sopath);
        void* find(const std::string& symbolName);
    public:
        bool setSoPath(const std::string & sopath);
    private:
        void* m_Mmodule;
        std::shared_ptr<std::map<std::string,void*>> m_addressMap;
        std::string m_sopath = nullptr;
    };
};

#endif //DOBBY_PROJECT_SYMBOL_H