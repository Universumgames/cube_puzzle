#pragma once

#include "../global.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeField.hpp"

// Template file for storing example level data

inline Point emptyWorldFieldSize = {5, 5};

inline Vector<WorldField::WorldFieldEnum> emptyWorldField = {
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
        WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT, WorldField::WorldFieldEnum::DEFAULT,
};

inline Point emptyCubeMapSidesSize = {5, 5};
inline Vector<CubeField *> emptySide = {
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField()
};

inline Vector<CubeMapSide*> emptyCubeMapSides = {
        new CubeMapSide(emptySide, emptyCubeMapSidesSize, 1),
        new CubeMapSide(emptySide, emptyCubeMapSidesSize, 2),
        new CubeMapSide(emptySide, emptyCubeMapSidesSize, 3),
        new CubeMapSide(emptySide, emptyCubeMapSidesSize, 4),
        new CubeMapSide(emptySide, emptyCubeMapSidesSize, 5),
        new CubeMapSide(emptySide, emptyCubeMapSidesSize, 6)
};