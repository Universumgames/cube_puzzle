//
// Created by Tom Arlt on 03.11.22.
//

#pragma once
#include <string>

/// basic data of level
// TODO extend data
struct LevelData{
    /// path of file to retrieve leveldata
    std::string path;
    /// id of level (index in allstates vector in Game)
    size_t id;
};