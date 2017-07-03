/*
 * =====================================================================================
 *
 *       Filename:  command_line.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年09月18日 17时16分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>

#include "command_line.h"

command_line command_line::instance;

command_line::command_line()
{
    this->values.clear();
    this->options.clear();
    this->files.clear();
}

void command_line::init(int argc, const char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                if (i + 1 < argc) {
                    instance.values[argv[i]+2] = argv[i+1];
                    ++i;
                }
            } else {
                instance.options.insert(argv[i]+1);
            }
        } else {
            instance.files.push_back(argv[i]);
        }
    }
}

std::string* command_line::get_value(std::string &key)
{
    auto it = instance.values.find(key);
    if (it == instance.values.end()) {
        return NULL;
    } else {
        return &(it->second);
    }
}
bool command_line::has_option(std::string &option)
{
    auto it = instance.options.find(option);
    if (it == instance.options.end()) {
        return false;
    } else {
        return true;
    }

}
std::vector<std::string> command_line::get_files()
{
    return instance.files;
}
void command_line::debug()
{
    std::cout << "command line values" << std::endl;
    for (auto it = instance.values.begin(); 
            it != instance.values.end();
            ++it) {
        std::cout << it->first << " ==> " << it->second << std::endl;
    }
    std::cout << "command line options" << std::endl;
    for (auto it = instance.options.begin(); 
            it != instance.options.end();
            ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "command line files" << std::endl;
    for (auto it = instance.files.begin();
            it != instance.files.end();
            ++it) {
        std::cout << *it << std::endl;
    }
}

