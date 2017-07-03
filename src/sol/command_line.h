#pragma once

#include <map>
#include <set>
#include <vector>

class command_line
{
private:
    static command_line instance;

    std::map<std::string, std::string> values;
    std::set<std::string> options;
    std::vector<std::string> files;

    command_line();

public:
    static void init(int argc, const char *argv[]);
    static std::string get_value(std::string &key);
    static bool has_option(std::string &option);
    static std::vector<std::string> get_files();
    static void debug();
};
