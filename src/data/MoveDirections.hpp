//
// Created by Tom Arlt on 03.11.22.
//
#pragma once

/// player movement relative to screen
enum class PlayerMoveDirection {
    UP, DOWN, LEFT, RIGHT
};

/// Directions when viewing on monitor (north is up, west is left - on monitor)
enum class DiceRollDirection{
    NORTH, SOUTH, WEST, EAST
};

/// map function to get opposite of roll direction (NORTH -> SOUTH)
inline DiceRollDirection getOppositeDiceRollDirection(DiceRollDirection dir){
    switch(dir){
        case DiceRollDirection::NORTH:
            return DiceRollDirection::SOUTH;
        case DiceRollDirection::SOUTH:
            return DiceRollDirection::NORTH;
        case DiceRollDirection::WEST:
            return DiceRollDirection::EAST;
        case DiceRollDirection::EAST:
            return DiceRollDirection::WEST;
    }
}