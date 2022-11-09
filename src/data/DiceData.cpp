//
// Created by Tom Arlt on 03.11.22.
//
#include "DiceData.hpp"

DiceData::DiceData() {
    this->up = 1;
    this->north = 5;
    this->west = 4;
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

DiceFaceDirection DiceData::getDiceSideRotation(int side) {
    int anchor = getAnchorSideOfTopLeftCorner(side);
    if(side == north){ // north
        if(anchor == up){
            return DiceFaceDirection::UP;
        }else if(anchor == west){
            return DiceFaceDirection::RIGHT;
        }else if(anchor == getOpposite(up)){
            return DiceFaceDirection::DOWN;
        }else if(anchor == getOpposite(west)){
            return DiceFaceDirection::LEFT;
        }
    } else if (side == up){
        // undefined
    }else if(side == west){ // west
        if(anchor == north){
            return DiceFaceDirection::LEFT;
        }else if(anchor == up){
            return DiceFaceDirection::UP;
        }else if(anchor == getOpposite(north)){
            return DiceFaceDirection::RIGHT;
        }else if(anchor == getOpposite(up)){
            return DiceFaceDirection::DOWN;
        }
    }else if(side == getOpposite(north)){ // south
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
    }else if(side == getOpposite(west)){ // east
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

int DiceData::getSideWhenMovingInDirX(int side, DiceFaceDirection direction) {
    auto currentFacing = getSideFacing(side);
    DiceSide nextDir = DiceSide::NORTH;
    switch(currentFacing){
        case DiceSide::NORTH:
            switch (direction) {
                case DiceFaceDirection::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceFaceDirection::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceFaceDirection::LEFT:
                    nextDir = DiceSide::EAST;
                    break;
                case DiceFaceDirection::RIGHT:
                    nextDir = DiceSide::WEST;
                    break;
            }
            break;
        case DiceSide::SOUTH:
            switch (direction) {
                case DiceFaceDirection::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceFaceDirection::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceFaceDirection::LEFT:
                    nextDir = DiceSide::WEST;
                    break;
                case DiceFaceDirection::RIGHT:
                    nextDir = DiceSide::EAST;
                    break;
            }
            break;
        case DiceSide::WEST:
            switch (direction) {
                case DiceFaceDirection::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceFaceDirection::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceFaceDirection::LEFT:
                    nextDir = DiceSide::NORTH;
                    break;
                case DiceFaceDirection::RIGHT:
                    nextDir = DiceSide::SOUTH;
                    break;
            }
            break;
        case DiceSide::EAST:
            switch (direction) {
                case DiceFaceDirection::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceFaceDirection::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceFaceDirection::LEFT:
                    nextDir = DiceSide::SOUTH;
                    break;
                case DiceFaceDirection::RIGHT:
                    nextDir = DiceSide::NORTH;
                    break;
            }
            break;
        case DiceSide::TOP:
            break;
        case DiceSide::BOTTOM:
            break;
    }
    return getSide(nextDir);
}

void DiceData::get2DRepresentation(int c, int *n, int *w, int *e, int *s, int *b) {
    *b = getOpposite(c);
    *n = getSideWhenMovingInDirX(c, DiceFaceDirection::UP);
    *s = getSideWhenMovingInDirX(c, DiceFaceDirection::DOWN);
    *w = getSideWhenMovingInDirX(c, DiceFaceDirection::LEFT);
    *e = getSideWhenMovingInDirX(c, DiceFaceDirection::RIGHT);
}