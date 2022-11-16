#pragma once
#include <string>
#include <fstream>

static std::string getFileContent(std::string path) {
    std::ifstream fileStream(path);
    std::string content;
    if (fileStream.is_open()){
        fileStream >> content;
        fileStream.close();
    }
    return content;
}
