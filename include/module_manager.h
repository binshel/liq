

#pragma once

#include <string>
#include <map>
#include <memory>

#include "module.h"


class module_manager
{
private:
    static module_manager instance;

    std::map<std::string, std::shared_ptr<module_t>> modules;

    module_manager();

public:
    static void init();
    static void regist_module(const std::string&, module_t *);
    static module_t* find_module(const std::string&);
};

extern "C" {
    void regist_module(const std::string& name, module_t *module);
}
