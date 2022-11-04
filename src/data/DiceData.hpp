
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

enum class DiceSide{
    NORTH, SOUTH, WEST, EAST, TOP, BOTTOM
};

class DiceData{
public:
    /// default cube is 1up, 5north, 3west
    DiceData();
    void rotate(DiceRollDirection direction);

    /// get dice side facing direction
    [[nodiscard]] int getSide(DiceSide side) const;

    /// get facing direction of side
    [[nodiscard]] DiceSide getSideFacing(int side) const;

    [[nodiscard]] std::string toString() const;

private:
    int up, north, west;

    static int getOpposite(int side);
};


