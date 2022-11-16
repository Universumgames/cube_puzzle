#pragma once
#include <string>
#include <fstream>

static std::string getFileContent(std::string path) {
    std::ifstream fileStream(path);
    std::stringstream sStream;
    if (fileStream.is_open()){
        sStream << fileStream.rdbuf();
        fileStream.close();
    }
    return sStream.str();
}
