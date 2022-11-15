#pragma once

#include "LevelData.hpp"

class CubeMapSide;

/// This is just an alternative proposal for loading levels
class LevelLoader {
public:
    struct LoadedLevelData {
        std::string path;
        std::string name;
        int id;
        Vector<CubeMapSide*> sides;
        Point worldSize;
        Vector<WorldField::WorldFieldEnum> worldField;
        Point cubePos = {};
        Point playerPos = {};
        int cubeSide = 0;
    };

    static LoadedLevelData loadLevel(std::string path);
};
