#pragma once

#include "../global.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeField.hpp"

// Template file for storing example level data

inline Point emptyWorldFieldSize = {0, 0};

inline Vector<WorldField::WorldFieldEnum> emptyWorldField = {/*
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
*/};

inline Vector<CubeObject *> flagVec = {new Flag()};
inline Vector<CubeObject *> stoneVec = {new Stone()};
inline Vector<CubeObject *> sliderVec = {new Slider(MovementDirection::moveToBigX, 1)};
inline Point emptyCubeMapSidesSize = {5, 5};
inline Vector<CubeField *> side1 = {
        new EmptyField(1, 0, 0), new EmptyField(1, 0, 1), new EmptyField(1, 0, 2), new EmptyField(1, 0, 3), new EmptyField(1, 0, 4),
        new EmptyField(1, 1, 0), new EmptyField(1, 1, 1), new EmptyField(1, 1, 2), new EmptyField(1, 1, 3), new EmptyField(1, 1, 4),
        new EmptyField(1, 2, 0), new EmptyField(1, 2, 1), new EmptyField(1, 2, 2), new EmptyField(1, 2, 3), new EmptyField(1, 2, 4),
        new EmptyField(1, 3, 0), new EmptyField(1, 3, 1), new EmptyField(1, 3, 2), new EmptyField(1, 3, 3), new EmptyField(1, 3, 4),
        new EmptyField(1, 4, 0), new EmptyField(1, 4, 1), new EmptyField(1, 4, 2), new EmptyField(1, 4, 3), new EmptyField(1, 4, 4)
};

inline Vector<CubeField *> side2 = {
        new EmptyField(2, 0, 0), new EmptyField(2, 0, 1), new EmptyField(2, 0, 2), new EmptyField(2, 0, 3), new EmptyField(2, 0, 4),
        new EmptyField(2, 1, 0), new EmptyField(2, 1, 1), new EmptyField(2, 1, 2), new EmptyField(2, 1, 3), new EmptyField(2, 1, 4),
        new EmptyField(2, 2, 0), new EmptyField(2, 2, 1), new EmptyField(2, 2, 2), new EmptyField(2, 2, 3), new EmptyField(2, 2, 4),
        new EmptyField(2, 3, 0), new EmptyField(2, 3, 1), new EmptyField(2, 3, 2), new EmptyField(2, 3, 3), new EmptyField(2, 3, 4),
        new EmptyField(2, 4, 0), new EmptyField(2, 4, 1), new EmptyField(2, 4, 2), new EmptyField(2, 4, 3), new EmptyField(2, 4, 4)
};

inline Vector<CubeField *> side3 = {
        new EmptyField(3, 0, 0), new EmptyField(3, 0, 1), new EmptyField(3, 0, 2), new EmptyField(3, 0, 3), new EmptyField(3, 0, 4),
        new EmptyField(3, 1, 0), new EmptyField(3, 1, 1), new EmptyField(3, 1, 2), new EmptyField(3, 1, 3), new EmptyField(3, 1, 4),
        new EmptyField(3, 2, 0), new EmptyField(3, 2, 1), new EmptyField(3, 2, 2), new EmptyField(3, 2, 3), new EmptyField(3, 2, 4),
        new EmptyField(3, 3, 0), new EmptyField(3, 3, 1), new EmptyField(3, 3, 2), new EmptyField(3, 3, 3), new EmptyField(3, 3, 4),
        new EmptyField(3, 4, 0), new EmptyField(3, 4, 1), new EmptyField(3, 4, 2), new EmptyField(3, 4, 3), new EmptyField(3, 4, 4)
};

inline Vector<CubeField *> side4 = {
        new EmptyField(4, 0, 0), new EmptyField(4, 0, 1), new EmptyField(4, 0, 2), new EmptyField(4, 0, 3), new EmptyField(4, 0, 4),
        new EmptyField(4, 1, 0), new EmptyField(4, 1, 1), new EmptyField(4, 1, 2), new EmptyField(4, 1, 3), new EmptyField(4, 1, 4),
        new EmptyField(4, 2, 0), new EmptyField(4, 2, 1), new EmptyField(4, 2, 2), new EmptyField(4, 2, 3), new EmptyField(4, 2, 4),
        new EmptyField(4, 3, 0), new EmptyField(4, 3, 1), new EmptyField(4, 3, 2), new EmptyField(4, 3, 3), new EmptyField(4, 3, 4),
        new EmptyField(4, 4, 0), new EmptyField(4, 4, 1), new EmptyField(4, 4, 2), new EmptyField(4, 4, 3), new EmptyField(4, 4, 4)
};

inline Vector<CubeField *> side5 = {
        new EmptyField(5, 0, 0), new EmptyField(5, 0, 1), new EmptyField(5, 0, 2), new EmptyField(5, 0, 3), new EmptyField(5, 0, 4),
        new EmptyField(5, 1, 0), new EmptyField(5, 1, 1), new EmptyField(5, 1, 2), new EmptyField(5, 1, 3), new EmptyField(5, 1, 4),
        new EmptyField(5, 2, 0), new EmptyField(5, 2, 1), new EmptyField(5, 2, 2), new EmptyField(5, 2, 3), new EmptyField(5, 2, 4),
        new EmptyField(5, 3, 0), new EmptyField(5, 3, 1), new EmptyField(5, 3, 2), new EmptyField(5, 3, 3), new EmptyField(5, 3, 4),
        new EmptyField(5, 4, 0), new EmptyField(5, 4, 1), new EmptyField(5, 4, 2), new EmptyField(5, 4, 3), new EmptyField(5, 4, 4)
};

inline Vector<CubeField *> side6 = {
        new EmptyField(6, 0, 0), new EmptyField(6, 0, 1), new EmptyField(6, 0, 2), new EmptyField(6, 0, 3), new EmptyField(6, 0, 4),
        new EmptyField(6, 1, 0), new EmptyField(6, 1, 1), new EmptyField(6, 1, 2), new EmptyField(6, 1, 3), new EmptyField(6, 1, 4),
        new EmptyField(6, 2, 0), new EmptyField(6, 2, 1), new EmptyField(6, 2, 2), new EmptyField(6, 2, 3), new EmptyField(6, 2, 4),
        new EmptyField(6, 3, 0), new EmptyField(6, 3, 1), new EmptyField(6, 3, 2), new EmptyField(6, 3, 3), new EmptyField(6, 3, 4),
        new EmptyField(6, 4, 0), new EmptyField(6, 4, 1), new EmptyField(6, 4, 2), new EmptyField(6, 4, 3), new EmptyField(6, 4, 4)
};

inline Vector<CubeMapSide*> emptyCubeMapSides = {
        new CubeMapSide(side1, emptyCubeMapSidesSize, 1),
        new CubeMapSide(side2, emptyCubeMapSidesSize, 2),
        new CubeMapSide(side3, emptyCubeMapSidesSize, 3),
        new CubeMapSide(side4, emptyCubeMapSidesSize, 4),
        new CubeMapSide(side5, emptyCubeMapSidesSize, 5),
        new CubeMapSide(side6, emptyCubeMapSidesSize, 6)
};