//
// Created by Tom Arlt on 03.11.22.
//
#include "DiceData.hpp"

DiceData::DiceData() {
    this->up = 1;
    this->north = 5;
    this->west = 3;
}

void DiceData::rotate(DiceRollDirection direction) {
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

int DiceData::getSide(DiceSide side) const {
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
    return 0;
}

int DiceData::getOpposite(int side) {
    return 7 - side;
}

std::string DiceData::toString() const {
    return "n: " + std::to_string(north) + " u:" + std::to_string(up) + " w:" + std::to_string(west);
}

DiceSide DiceData::getSideFacing(int side) const {
    if(side == north) return DiceSide::NORTH;
    else if(side == west) return DiceSide::WEST;
    else if(side == up) return DiceSide::TOP;
    else if(side == getOpposite(north)) return DiceSide::SOUTH;
    else if(side == getOpposite(west)) return DiceSide::EAST;
    else if(side == getOpposite(up)) return DiceSide::BOTTOM;
    return DiceSide::NORTH;
}
