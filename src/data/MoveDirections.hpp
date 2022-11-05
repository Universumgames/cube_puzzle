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

enum class DiceFaceDirection{
    UP, DOWN, LEFT, RIGHT
};