//
// Created by Song on 2025/11/23.
//

#include "Symbol.h"
#include "../Dobby/include/dobby.h"
#include "../Log/log.h"


Symbol::Symbol::Symbol(const std::string & sopath):m_sopath(sopath),m_Mmodule(nullptr) {
    m_addressMap = std::make_shared<std::map<std::string,void*>>();
    if(m_addressMap){
        LOG(LOG_LEVEL_INFO,"m_addressMap Init Success!");
    }
}

void* Symbol::Symbol::find(const std::string& symbolName) {
    void *p = nullptr;

    do {
        //从map缓存中查找是否有此地址
        auto it = m_addressMap.get()->find(symbolName);
        if(it !=  m_addressMap.get()->end()){
            p = it->second;
            break;
        }
        //没查找到使用DobbySymbolResolver函数查找
        if(m_sopath.empty()){
            break;
        }
        p = DobbySymbolResolver(m_sopath.c_str(),symbolName.c_str());
        if(p != nullptr){
            m_addressMap.get()->insert(std::make_pair(symbolName,p));
            LOG(LOG_LEVEL_INFO,"DobbySymbolResolver FindFunction:%s Address:%p Success!",symbolName.c_str(),p);
        }
    }while(0);
    return p;
}

bool Symbol::Symbol::setSoPath(const std::string &sopath) {
    if(sopath.empty()){
        return false;
    }
    m_sopath = sopath;
    return true;
}

