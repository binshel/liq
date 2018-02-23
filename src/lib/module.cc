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
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>

#include "liq/module.h"
#include "liq/logger.h"


namespace liq {

static char *findpath(const std::string &path)
{
    static const char *prefix = getenv("FRAME_HOME");
    std::string full_path = "";
    if ((path.compare(0, 1, "/") != 0)
            && (path.compare(0, 1, "~") != 0))
    {
        full_path.append(prefix);
        full_path.append("/lib/liq/");
        full_path.append(path);
    } else {
        full_path.append(path);
    }
    return realpath(full_path.c_str(), NULL);
}

void ModuleManager::load_dir(const std::string &path, const std::string &suf)
{
    char *abs_path = findpath(path);
    if (!abs_path) {
        ERRORF("module dir not exists %s", path.c_str());
        exit(-1);
    }
    DIR *dir = opendir(abs_path);
    if (!dir) {
        ERRORF("can't open module dir %s:%s", path.c_str(), strerror(errno));
        exit(-1);
    }
    struct dirent *ent;
    while ((ent = readdir(dir))) {
        if (strcmp(ent->d_name, ".") == 0 
                || strcmp(ent->d_name, "..") == 0) 
        {
            continue;
        }
        /*
         * 一些系统的 d_type 无效，使用 stat 获得
         */
        std::string d_path = "";
        d_path = d_path + abs_path + "/" + ent->d_name;
        struct stat d_stat;
        stat(d_path.c_str(), &d_stat);

        std::string npath;
        if (path == "") npath = ent->d_name;
        else npath = path + "/" + ent->d_name;

        if (S_ISDIR(d_stat.st_mode)) {
            load_dir(npath, suf);
        } else if (S_ISREG(d_stat.st_mode)) {
            int pre_len = strlen(ent->d_name) - suf.length();
            if (pre_len > 0 && suf.compare(ent->d_name + pre_len) == 0)
                load(npath);
        } else {
            INFOF("module dir not dir or regular file %s:%s:%d", 
                    npath.c_str(), 
                    d_path.c_str(), 
                    d_stat.st_mode);
        }
    }

    closedir(dir);
    free(abs_path);
    return;
}

Module::Module(const std::string &path, void* handle):
    file_path(path), handle(handle)
{
    pcreate_service = (decltype(pcreate_service))dlsym(handle, "create_service");
    pdestory_service = (decltype(pdestory_service))dlsym(handle, "destory_service");
}


Module* ModuleManager::load(const std::string &path)
{
    char *abs_path = findpath(path);
    if (!abs_path) {
        ERRORF("module file not exists %s", path.c_str());
        exit(-1);
    }
    
    if (modules.find(abs_path) == modules.end()) {
        load_file(abs_path);
    } 
    auto module = modules[abs_path];
    free(abs_path);
    return module;
}

Module* ModuleManager::load_file(const std::string &path)
{
    INFOF("loading file %s", path.c_str());
    void *handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle) {
        ERROR << "load file error:" << path << dlerror() << EOL;
        exit(-1);
    }
    Module *module = new Module(path, handle);
    INFO << "loaded module " << path << EOL;
    modules[path] = module;
    return module;
}

}
