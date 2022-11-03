//
// Created by Tom Arlt on 03.11.22.
//
#pragma once
#include "MoveDirections.hpp"
#include <string>

/**
 *           5
 *
 *   4       1        3
 *
 *           2
 *
 *           6
*/

// 1 2 6 5
// 1 4 6 3
// 1 5 6 2
// 1 3 6 4

enum class DiceSide{
    NORTH, SOUTH, WEST, EAST, TOP, BOTTOM
};

class CubeData{
public:
    CubeData();
    void rotate(CubeRollDirection direction);

    int getSide(DiceSide side);

    std::string toString();

private:
    int up, north, west;

    int getOpposite(int side);
};

CubeData::CubeData() {
    this->up = 1;
    this->north = 5;
    this->west = 3;
}

void CubeData::rotate(CubeRollDirection direction) {
    auto oldUp = up;
    auto oldNorth = north;
    auto oldWest = west;
    switch(direction){
        case NORTH:
            north = oldUp;
            up = getOpposite(oldNorth);
            break;
        case SOUTH:
            up = oldNorth;
            north = getOpposite(oldUp);
            break;
        case WEST:
            west = oldUp;
            up = getOpposite(oldWest);
            break;
        case EAST:
            up = oldWest;
            west = getOpposite(oldUp);
            break;
    }
}

int CubeData::getSide(DiceSide side) {
    switch(side){
        case DiceSide::NORTH:
            return north;
        case DiceSide::SOUTH:
            return 7 - north;
        case DiceSide::WEST:
            return west;
        case DiceSide::EAST:
            return 7 - west;
        case DiceSide::TOP:
            return up;
        case DiceSide::BOTTOM:
            return 7-up;
    }
}

int CubeData::getOpposite(int side) {
    return 7 - side;
}

std::string CubeData::toString() {
    return "n: " + std::to_string(north) + " u:" + std::to_string(up) + " w:" + std::to_string(west);
}


