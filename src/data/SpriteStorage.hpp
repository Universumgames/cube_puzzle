#pragma once

#include "../global.hpp"

struct SpriteStorage {
    Texture *cubeMapSpriteSheet = nullptr;
    Texture *worldMapSpriteSheet = nullptr;
    Texture *playerSpriteSheet = nullptr;
    Texture* temp = nullptr;
};