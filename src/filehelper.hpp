#pragma once
#include <string>
#include <fstream>

static std::string getFileContent(std::string path) {
    std::string fileString;
    std::ifstream fileStream(path);
    if (fileStream.is_open()) {
        fileString.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

        fileStream.close();
    }
    return fileString;
}
