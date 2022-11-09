#pragma once

#include "../global.hpp"

#define SPRITE_TILE_SIZE 16
#define SIDE_SIZE 48

inline void drawSprite(Texture *texture, Renderer *render, Point imageIndex, Rect dst) {
    Rect src = {imageIndex.x * SPRITE_TILE_SIZE, imageIndex.y * SPRITE_TILE_SIZE, SPRITE_TILE_SIZE, SPRITE_TILE_SIZE};
    SDL_RenderCopy(render, texture, &src, &dst);
}

inline void drawSide(Texture *texture, Renderer *render, Rect dst) {
    Rect src = {0,0, SIDE_SIZE, SIDE_SIZE};
    SDL_RenderCopy(render, texture, &src, &dst);
}

#define ROBOTO_FONT_FILEPATH "./asset/font/RobotoSlab-Bold.ttf"
#define ROBOTO_FONT_LIGHT_FILEPATH "./asset/font/Roboto/Roboto-Regular.ttf"

struct SpriteStorage {
    Texture *cubeMapSpriteSheet = nullptr;
    Texture *worldMapSpriteSheet = nullptr;
    Texture *playerSpriteSheet = nullptr;
    Texture *temp = nullptr;
    Font* basicFont = nullptr;
    Font* debugFont = nullptr;

    std::vector<Texture*> sideSprites;
};