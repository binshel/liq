#pragma once

#include <map>
#include <set>
#include <vector>

class CommandLine
{
private:
    static CommandLine instance;

    std::map<std::string, std::string> values;
    std::set<std::string> options;
    std::vector<std::string> files;

    CommandLine();

public:
    static void init(int argc, char * const argv[]);
    static std::string* get_value(const std::string &key);
    static bool has_option(const std::string &option);
    static std::vector<std::string> get_files();
    static void debug();
};
