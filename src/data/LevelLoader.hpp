#pragma once

#include <string>
#include "../recthelper.hpp"
#include "WorldField.hpp"
#include <string>
#include <fstream>

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

    struct TutLoadedLevelData:public LoadedLevelData{
        std::string sideBarText;

        TutLoadedLevelData(const LoadedLevelData& levelData){
            this->path = levelData.path;
            this->name = levelData.name;
            this->id = levelData.id;
            this->sides = levelData.sides;
            this->worldField = levelData.worldField;
            this->worldField = levelData.worldField;
            this->cubePos = levelData.cubePos;
            this->playerPos = levelData.playerPos;
            this->cubeSide = levelData.cubeSide;
        }

        TutLoadedLevelData() = default;
    };

    static LoadedLevelData loadLevel(const std::string& path);
    static TutLoadedLevelData loadTutLevel(const std::string& path);

private:
    static LoadedLevelData loadLevel(const std::string& path, std::ifstream& is);


};
