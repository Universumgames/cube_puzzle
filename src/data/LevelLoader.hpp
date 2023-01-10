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
    /// loaded data from level file
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

    /// loaded level data + tutorial data from file
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

    /// load normal level from file
    static LoadedLevelData loadLevel(const std::string& path);
    static LoadedLevelData loadLevelString(const std::string& levelString);
    /// load tutorial level
    static TutLoadedLevelData loadTutLevel(const std::string& path);

private:
    /// internal loading method for identical loading processes between normal levels and tutorial levels
    static LoadedLevelData loadLevel(const std::string& path, std::istream& is);


};
