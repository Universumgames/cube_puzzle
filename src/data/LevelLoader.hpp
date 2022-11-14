#pragma once
#include <filesystem>
#include "LevelData.hpp"

class CubeMapSide;

class LevelLoader {
public:
    struct LoadedLevelData{
        Vector<CubeMapSide> sides;
        int worldW =0, worldH = 0;
        Vector<WorldField::WorldFieldEnum> worldField;
        Point cubePos = {};
        Point playerPos = {};
        int cubeSide = 0;
    };

    static LoadedLevelData loadLevel(std::filesystem::directory_entry path);
};
