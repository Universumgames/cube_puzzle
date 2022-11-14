#pragma once

#include <filesystem>
#include "LevelData.hpp"

class CubeMapSide;

/// This is just an alternative proposal for loading levels
class [[deprecated]] LevelLoader {
public:
    struct LoadedLevelData {
        Vector<CubeMapSide> sides;
        int worldW = 0, worldH = 0;
        Vector<WorldField::WorldFieldEnum> worldField;
        Point cubePos = {};
        Point playerPos = {};
        int cubeSide = 0;
    };

    [[deprecated]]
    static LoadedLevelData loadLevel(std::filesystem::directory_entry path);
};
