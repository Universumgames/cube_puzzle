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
 * @param side side number to get the anchor rom
 * @return the anchor of the side
 */

int DiceData::getAnchorSideOfTopLeftCorner(int side) {
    switch (side) {
        case 1:
            return 5;
        case 2:
        case 3:
        case 4:
        case 5:
            return 1;
        case 6:
            return 2;
        default:
            return 0;
    }
}

DiceSideRotation DiceData::getDiceSideRotation(int side) {
    int anchor = getAnchorSideOfTopLeftCorner(side);
    if (side == north) { // north
        if (anchor == up) {
            return DiceSideRotation::UP;
        } else if (anchor == west) {
            return DiceSideRotation::RIGHT;
        } else if (anchor == getOpposite(up)) {
            return DiceSideRotation::DOWN;
        } else if (anchor == getOpposite(west)) {
            return DiceSideRotation::LEFT;
        }
    } else if (side == up) {
        // undefined
    } else if (side == west) { // west
        if (anchor == north) {
            return DiceSideRotation::LEFT;
        } else if (anchor == up) {
            return DiceSideRotation::UP;
        } else if (anchor == getOpposite(north)) {
            return DiceSideRotation::RIGHT;
        } else if (anchor == getOpposite(up)) {
            return DiceSideRotation::DOWN;
        }
    } else if (side == getOpposite(north)) { // south
        if (anchor == west) {
            return DiceSideRotation::LEFT;
        } else if (anchor == up) {
            return DiceSideRotation::UP;
        } else if (anchor == getOpposite(west)) {
            return DiceSideRotation::RIGHT;
        } else if (anchor == getOpposite(up)) {
            return DiceSideRotation::DOWN;
        }
    } else if (side == getOpposite(up)) {
        // undefined
    } else if (side == getOpposite(west)) { // east
        if (anchor == north) {
            return DiceSideRotation::RIGHT;
        } else if (anchor == up) {
            return DiceSideRotation::UP;
        } else if (anchor == getOpposite(north)) {
            return DiceSideRotation::LEFT;
        } else if (anchor == getOpposite(up)) {
            return DiceSideRotation::DOWN;
        }
    }
    return DiceSideRotation::LEFT;
}

int DiceData::getSideWhenMovingInDirX(int side, DiceSideRotation direction) {
    auto currentFacing = getSideFacing(side);
    DiceSide nextDir = DiceSide::NORTH;
    switch (currentFacing) {
        case DiceSide::NORTH:
            switch (direction) {
                case DiceSideRotation::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceSideRotation::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceSideRotation::LEFT:
                    nextDir = DiceSide::EAST;
                    break;
                case DiceSideRotation::RIGHT:
                    nextDir = DiceSide::WEST;
                    break;
            }
            break;
        case DiceSide::SOUTH:
            switch (direction) {
                case DiceSideRotation::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceSideRotation::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceSideRotation::LEFT:
                    nextDir = DiceSide::WEST;
                    break;
                case DiceSideRotation::RIGHT:
                    nextDir = DiceSide::EAST;
                    break;
            }
            break;
        case DiceSide::WEST:
            switch (direction) {
                case DiceSideRotation::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceSideRotation::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceSideRotation::LEFT:
                    nextDir = DiceSide::NORTH;
                    break;
                case DiceSideRotation::RIGHT:
                    nextDir = DiceSide::SOUTH;
                    break;
            }
            break;
        case DiceSide::EAST:
            switch (direction) {
                case DiceSideRotation::UP:
                    nextDir = DiceSide::TOP;
                    break;
                case DiceSideRotation::DOWN:
                    nextDir = DiceSide::BOTTOM;
                    break;
                case DiceSideRotation::LEFT:
                    nextDir = DiceSide::SOUTH;
                    break;
                case DiceSideRotation::RIGHT:
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
    if (b != nullptr) *b = getOpposite(c);
    if (n != nullptr) *n = getSideWhenMovingInDirX(c, DiceSideRotation::UP);
    if (s != nullptr) *s = getSideWhenMovingInDirX(c, DiceSideRotation::DOWN);
    if (w != nullptr) *w = getSideWhenMovingInDirX(c, DiceSideRotation::LEFT);
    if (e != nullptr) *e = getSideWhenMovingInDirX(c, DiceSideRotation::RIGHT);
}

bool operator==(const DiceData &lhs, const DiceData &rhs) {
    return lhs.up == rhs.up && lhs.north == rhs.north && lhs.west == rhs.west;
}

bool operator!=(const DiceData &lhs, const DiceData &rhs) {
    return !(lhs == rhs);
}
