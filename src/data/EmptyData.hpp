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
inline Vector<CubeObject *> sliderVec = {new Slider(Slider::MovementDirectionIfActivated::moveToBigX, 1)};
inline Point emptyCubeMapSidesSize = {5, 5};
inline Vector<CubeField *> side1 = {
        new Grass(1), new EmptyField(1), new EmptyField(1, sliderVec), new EmptyField(1), new Grass(1),
        new EmptyField(1), new EmptyField(1), new EmptyField(1), new EmptyField(1), new EmptyField(1),
        new EmptyField(1), new EmptyField(1), new EmptyField(1), new EmptyField(1), new EmptyField(1),
        new EmptyField(1), new PressurePlate(1, 1), new EmptyField(1), new EmptyField(1), new EmptyField(1),
        new EmptyField(1), new EmptyField(1), new EmptyField(1), new EmptyField(1), new EmptyField(1)
};

inline Vector<CubeField *> side2 = {
        new Grass(2), new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2),
        new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2),
        new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2),
        new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2),
        new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2), new EmptyField(2)
};

inline Vector<CubeField *> side3 = {
        new Grass(3), new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3),
        new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3),
        new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3),
        new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3),
        new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3), new EmptyField(3)
};

inline Vector<CubeField *> side4 = {
        new Grass(4), new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4),
        new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4),
        new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4),
        new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4),
        new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4), new EmptyField(4)
};

inline Vector<CubeField *> side5 = {
        new Grass(5), new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5),
        new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5),
        new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5),
        new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5),
        new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5), new EmptyField(5)
};

inline Vector<CubeField *> side6 = {
        new Grass(6), new EmptyField(6, flagVec), new EmptyField(6), new EmptyField(6), new EmptyField(6),
        new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6),
        new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6),
        new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6),
        new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6), new EmptyField(6)
};

inline Vector<CubeMapSide*> emptyCubeMapSides = {
        new CubeMapSide(side1, emptyCubeMapSidesSize, 1),
        new CubeMapSide(side2, emptyCubeMapSidesSize, 2),
        new CubeMapSide(side3, emptyCubeMapSidesSize, 3),
        new CubeMapSide(side4, emptyCubeMapSidesSize, 4),
        new CubeMapSide(side5, emptyCubeMapSidesSize, 5),
        new CubeMapSide(side6, emptyCubeMapSidesSize, 6)
};