#pragma once

#include <string>
#include <vector>
#include <map>
#include "WorldField.hpp"

/// basic data of level
// TODO extend data
struct LevelData {
    Map<int, Map<int, Vector<WorldField::WorldFieldEnum>>> levelDataMap;
    /// path of file to retrieve leveldata
    std::string path;
    /// id of level
    int id;
    ///  (index in allstates vector in Game)
    size_t allStatesIndex;
    std::string name = "unknown";
};