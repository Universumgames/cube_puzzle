#pragma once

#include <string>
#include <vector>
#include <map>
#include "WorldField.hpp"

/// basic data of level
struct LevelData {
    /// path of file to retrieve leveldata
    std::string path;
    /// id of level
    int id;
    ///  (index in allstates vector in Game)
    size_t allStatesIndex;
    std::string name = "unknown";

    Texture * selectorTexture = nullptr;

    static bool sort(const LevelData& d1, const LevelData& d2){
        return d1.id < d2.id;
    }

    friend bool operator==(const LevelData rhs, const LevelData lhs){
        return rhs.id == lhs.id;
    }
};

struct TutorialLevelData: public LevelData{
    std::string sidebarText;

    TutorialLevelData& operator=(const LevelData& levelData){
        this->path = levelData.path;
        this->id = levelData.id;
        this->allStatesIndex = levelData.allStatesIndex;
        this->name = levelData.name;
        this->selectorTexture = levelData.selectorTexture;
    }
};