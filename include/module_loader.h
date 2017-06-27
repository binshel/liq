#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <memory>

#include "module.h"

typedef module_t* (*fun_create_module)();
typedef void (*fun_destory_module)(module_t*);


struct module_file
{
    int32_t     id;
    std::string file_path;
    fun_create_module create_module;
    fun_destory_module destory_module;
    void *handle;

    module_file();
    module_file(const module_file &info);
    ~module_file();
};

class module_loader : public module_t
{
private:
    static module_loader instance;

    std::map<std::string, std::shared_ptr<module_file>> dl_files;

    module_loader();
    void load_file(const std::string &path);

public:
    static void init();
    static module_t* load_module(const std::string &path);
    static void destory_module(module_t*);
    static void unload_module(const std::string &path);


};
