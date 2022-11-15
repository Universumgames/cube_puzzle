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
    /// id of level (index in allstates vector in Game)
    size_t id;
    std::string name = "unknown";
};