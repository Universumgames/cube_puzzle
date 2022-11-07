//
// Created by Tom Arlt on 03.11.22.
//
#include "DiceData.hpp"

DiceData::DiceData() {
    this->up = 1;
    this->north = 5;
    this->west = 3;
}

int DiceData::rotate(DiceRollDirection direction, int side) {
    auto oldUp = up;
    auto oldNorth = north;
    auto oldWest = west;
    auto oldSideFacing = getSideFacing(side);
    switch (direction) {
        case DiceRollDirection::NORTH:
            north = oldUp;
            up = getOpposite(oldNorth);
            break;
        case DiceRollDirection::SOUTH:
            up = oldNorth;
            north = getOpposite(oldUp);
            break;
        case DiceRollDirection::WEST:
            west = oldUp;
            up = getOpposite(oldWest);
            break;
        case DiceRollDirection::EAST:
            up = oldWest;
            west = getOpposite(oldUp);
            break;
    }
    return getSide(oldSideFacing);
}

int DiceData::getSide(DiceSide side) const {
    switch (side) {
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
            return 7 - up;
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
    if (side == north) return DiceSide::NORTH;
    else if (side == west) return DiceSide::WEST;
    else if (side == up) return DiceSide::TOP;
    else if (side == getOpposite(north)) return DiceSide::SOUTH;
    else if (side == getOpposite(west)) return DiceSide::EAST;
    else if (side == getOpposite(up)) return DiceSide::BOTTOM;
    return DiceSide::NORTH;
}


/**
 * Anchors
 *          5
 *         ´|`
 *     4 -> 1 <-3
 *         ´|`
 *          2
 *         ´|`
 *          6
 * @param side
 * @return
 */

int DiceData::getAnchorSideOfTopLeftCorner(int side) {
    switch (side) {
        case 1:
            return 5;
        case 2:
            return 1;
        case 3:
            return 1;
        case 4:
            return 1;
        case 5:
            return 1;
        case 6:
            return 2;
        default:
            return 0;
    }
}

DiceFaceDirection DiceData::getDiceSideOrientation(int side) {
    int anchor = getAnchorSideOfTopLeftCorner(side);
    if(side == north){
        if(anchor == up){
            return DiceFaceDirection::UP;
        }else if(anchor == west){
            return DiceFaceDirection::LEFT;
        }else if(anchor == getOpposite(up)){
            return DiceFaceDirection::DOWN;
        }else if(anchor == getOpposite(west)){
            return DiceFaceDirection::RIGHT;
        }
    } else if (side == up){
        // undefined
    }else if(side == west){
        if(anchor == north){
            return DiceFaceDirection::LEFT;
        }else if(anchor == up){
            return DiceFaceDirection::UP;
        }else if(anchor == getOpposite(north)){
            return DiceFaceDirection::RIGHT;
        }else if(anchor == getOpposite(up)){
            return DiceFaceDirection::DOWN;
        }
    }else if(side == getOpposite(north)){
        if(anchor == west){
            return DiceFaceDirection::RIGHT;
        }else if(anchor == up){
            return DiceFaceDirection::UP;
        }else if(anchor == getOpposite(west)){
            return DiceFaceDirection::LEFT;
        }else if(anchor == getOpposite(up)){
            return DiceFaceDirection::DOWN;
        }
    }else if(side == getOpposite(up)){
        // undefined
    }else if(side == getOpposite(west)){
        if(anchor == north){
            return DiceFaceDirection::RIGHT;
        }else if(anchor == up){
            return DiceFaceDirection::UP;
        }else if(anchor == getOpposite(north)){
            return DiceFaceDirection::LEFT;
        }else if(anchor == getOpposite(up)){
            return DiceFaceDirection::DOWN;
        }
    }
    return DiceFaceDirection::LEFT;
}
