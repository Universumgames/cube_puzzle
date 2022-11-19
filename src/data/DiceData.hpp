#pragma once

#include "MoveDirections.hpp"
#include <string>
#include "../global.hpp"

/**
 *           5
 *
 *   4       1        3
 *
 *           2
 *
 *           6
*/

/// Dice Side direction when viewing from top
enum class DiceSide {
    NORTH, SOUTH, WEST, EAST, TOP, BOTTOM
};

/// Direction when viewing straight on dice side
enum class DiceSideRotation {
    UP = 0, DOWN = 180, LEFT = 270, RIGHT = 90
};

/// Helper Class for Dice Logic
class DiceData {
public:
    /// default cube is 1up, 5north, 3west
    DiceData();

    /**
     * Roll cube in direction
     * @param direction direction to roll to
     * @param side
     * @return new side facing in same direction
     */
    int rotate(DiceRollDirection direction, int side = 1);

    /// get dice side facing direction
    [[nodiscard]] int getSide(DiceSide side) const;

    /// get facing direction of side
    [[nodiscard]] DiceSide getSideFacing(int side) const;

    [[nodiscard]] std::string toString() const;

    /**
     * get the side of the cube where the top left corner is relative to
     * example: the anchor of side 1 is 5, because rotating the cube like above, you can see what is meant with top left corner of side 1
     * @param side side to which you want to get the anchor from
     * @return
     */
    static int getAnchorSideOfTopLeftCorner(int side);

    /**
     * get global orientation of side when looking straight on side
     * @param side
     * @return get rotation of side on global space (no result for top and bottom of dice)
     */
    DiceSideRotation getDiceSideRotation(int side);

    int getSideWhenMovingInDirX(int side, DiceSideRotation direction);

    /// get folded out representation of cube relative to center c
    void get2DRepresentation(int c, int *n, int *w, int *e, int *s, int *b);

    friend bool operator ==(const DiceData& lhs, const DiceData& rhs);
    friend bool operator !=(const DiceData& lhs, const DiceData& rhs);

private:
    int up, north, west;
public:
    /// get opposite side, getOpposite(6) = 1
    static int getOpposite(int side);
};

/// map function to map DiceSide to DiceRollDirection (undefined behavior for DiceSide::TOP and DiceSide::BOTTOM)
inline DiceRollDirection sideToRollDirection(DiceSide side) {
    switch (side) {
        case DiceSide::NORTH:
            return DiceRollDirection::NORTH;
        case DiceSide::SOUTH:
            return DiceRollDirection::SOUTH;
        case DiceSide::WEST:
            return DiceRollDirection::WEST;
        case DiceSide::EAST:
            return DiceRollDirection::EAST;
        case DiceSide::TOP:
            return DiceRollDirection::NORTH;
        case DiceSide::BOTTOM:
            return DiceRollDirection::SOUTH;
    }
    return DiceRollDirection::NORTH;
}

inline void sAndNRotation(int w, int c, int e, int b, int n, int s, DiceSideRotation *nRot, DiceSideRotation *sRot) {
    if (nRot == nullptr || sRot == nullptr) return;
    auto anchorN = DiceData::getAnchorSideOfTopLeftCorner(n);
    auto anchorS = DiceData::getAnchorSideOfTopLeftCorner(s);
    if (anchorN == w) *nRot = DiceSideRotation::LEFT;
    else if (anchorN == c) *nRot = DiceSideRotation::DOWN;
    else if (anchorN == e) *nRot = DiceSideRotation::RIGHT;
    else if (anchorN == b) *nRot = DiceSideRotation::UP;

    if (anchorS == w) *sRot = DiceSideRotation::LEFT;
    else if (anchorS == c) *sRot = DiceSideRotation::UP;
    else if (anchorS == e) *sRot = DiceSideRotation::RIGHT;
    else if (anchorS == b) *sRot = DiceSideRotation::DOWN;
}