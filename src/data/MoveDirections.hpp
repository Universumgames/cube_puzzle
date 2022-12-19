#pragma once

/// player movement relative to screen
enum class PlayerMoveDirection : int {
    UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3
};

/// Directions when viewing on monitor (north is up, west is left - on monitor)
enum class DiceRollDirection {
    NORTH, SOUTH, WEST, EAST
};

/// map function to get opposite of roll direction (NORTH -> SOUTH)
inline DiceRollDirection getOppositeDiceRollDirection(DiceRollDirection dir) {
    switch (dir) {
        case DiceRollDirection::NORTH:
            return DiceRollDirection::SOUTH;
        case DiceRollDirection::SOUTH:
            return DiceRollDirection::NORTH;
        case DiceRollDirection::WEST:
            return DiceRollDirection::EAST;
        case DiceRollDirection::EAST:
            return DiceRollDirection::WEST;
        default:
            return DiceRollDirection::NORTH;
    }
}

/// Movement directions for slider
enum class MovementDirection {
    moveToBigX,
    moveToSmallX,
    moveToBigY,
    moveToSmallY,
    none
};

/// map function to get opposite of movement direction
inline MovementDirection getOppositeMovementDirection(MovementDirection dir) {
    switch (dir) {
        case MovementDirection::moveToBigY:
            return MovementDirection::moveToSmallY;
        case MovementDirection::moveToSmallY:
            return MovementDirection::moveToBigY;
        case MovementDirection::moveToBigX:
            return MovementDirection::moveToSmallX;
        case MovementDirection::moveToSmallX:
            return MovementDirection::moveToBigX;
        default:
            return MovementDirection::none;
    }
}