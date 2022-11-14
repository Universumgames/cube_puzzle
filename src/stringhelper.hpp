#pragma once
#include <string>
#include <regex>
#include "global.hpp"

// split string by delimiter
static Vector<std::string> split(const std::string &s, char delim)
{
    Vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
static std::string replaceAll(const std::string s, std::string toReplace, std::string with){
    return std::regex_replace(s, std::regex(toReplace), with);
}

static std::string removeAll(const std::string s, std::string toRemove){
    return std::regex_replace(s, std::regex(toRemove), "");
}