/*
 * =====================================================================================
 *
 *       Filename:  module_loader.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年09月18日 16时17分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <dlfcn.h>

#include <boost/filesystem.hpp>

#include "liq/module.h"

namespace fs = boost::filesystem;


namespace liq {


Module* ModuleManager::load(const std::string &path)
{
    std::string full_path = fs::absolute(fs::path(path)).normalize().string();
    if (modules.find(full_path) == modules.end()) {
        load_file(full_path);
    } 
    return modules[full_path];
}

Module* ModuleManager::load_file(const std::string &path)
{
    Module *module = new Module();
    void *handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        std::cout << "load file error:" << path << dlerror() << std::endl;
        return NULL;
    }
    module->file_path = path;
    module->handle = handle;
    module->create_service = (decltype(module->create_service))dlsym(handle, "create_module");
    module->destory_service = (decltype(module->destory_service))dlsym(handle, "destory_module");

    std::cout << "loaded module " << path << std::endl;
    modules[path] = module;
    return module;
}

}
