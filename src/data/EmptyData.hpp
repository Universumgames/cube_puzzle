#pragma once

#include "../global.hpp"
#include "WorldField.hpp"
#include "../GameObjects/CubeField.hpp"

inline Point emptyWorldFieldSize = {5, 5};

inline Vector<WorldField> emptyWorldField = {
        WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT,
        WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT,
        WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT,
        WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT,
        WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT, WorldField::DEFAULT,
};

inline Point emptyCubeMapSidesSize = {5, 5};
inline Vector<CubeField *> emptySide = {
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(),
        new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField(), new EmptyField()
};

inline Vector<CubeMapSide> emptyCubeMapSides = {
        CubeMapSide(emptySide, emptyCubeMapSidesSize),
        CubeMapSide(emptySide, emptyCubeMapSidesSize),
        CubeMapSide(emptySide, emptyCubeMapSidesSize),
        CubeMapSide(emptySide, emptyCubeMapSidesSize),
        CubeMapSide(emptySide, emptyCubeMapSidesSize),
        CubeMapSide(emptySide, emptyCubeMapSidesSize)
};