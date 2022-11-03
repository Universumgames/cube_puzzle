
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

class CubeData{
public:
    /// default cube is 1up, 5north, 3west
    CubeData();
    void rotate(CubeRollDirection direction);

    /// get dice side facing direction
    int getSide(DiceSide side) const;

    std::string toString() const;

private:
    int up, north, west;

    static int getOpposite(int side);
};


