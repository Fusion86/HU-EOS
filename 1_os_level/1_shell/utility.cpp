#include "utility.h"

#include <sstream>
#include <fstream>

bool replace(std::string &str, const std::string &from, const std::string &to)
{
    // Taken from https://stackoverflow.com/a/3418285/2125072
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// Read all lines in file and return a string
std::string slurp(std::ifstream &in)
{
    // Taken from https://stackoverflow.com/a/116220/2125072
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}
