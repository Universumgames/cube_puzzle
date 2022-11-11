#pragma once

#include "../global.hpp"
#include "DiceData.hpp"

#define SPRITE_TILE_SIZE 16
#define SIDE_SIZE 48

/**
 * simple draw sprite
 * @param texture texture to draw from
 * @param render sdl renderer
 * @param imageIndex texture divided into SPRITE_TILE_SIZE x SPRITE_TILE_SIZE tiles, the index of x,y coordinate
 * @param dst destination rectangle to draw to on screen
 */
inline void drawSprite(Texture *texture, Renderer *render, Point imageIndex, Rect dst) {
    Rect src = {imageIndex.x * SPRITE_TILE_SIZE, imageIndex.y * SPRITE_TILE_SIZE, SPRITE_TILE_SIZE, SPRITE_TILE_SIZE};
    SDL_RenderCopy(render, texture, &src, &dst);
}

/**
 * draw full texture
 * @param texture texture to draw
 * @param render sdl renderer
 * @param dst destination rectangle to draw to on screen
 */
inline void drawSide(Texture *texture, Renderer *render, Rect dst, DiceSideRotation rotation = DiceSideRotation::UP) {
    //Rect src = {0,0, SIDE_SIZE, SIDE_SIZE};
    double angle = 0;
    switch (rotation) {
        case DiceSideRotation::UP:
            angle = 0;
            break;
        case DiceSideRotation::DOWN:
            angle = 180;
            break;
        case DiceSideRotation::LEFT:
            angle = 270;
            break;
        case DiceSideRotation::RIGHT:
            angle = 90;
            break;
    }
    Point center = {dst.w / 2, dst.h / 2};
    SDL_RenderCopyEx(render, texture, nullptr, &dst, angle, &center, SDL_FLIP_NONE);
}

#define ROBOTO_FONT_FILEPATH "./asset/font/RobotoSlab-Bold.ttf"
#define ROBOTO_FONT_LIGHT_FILEPATH "./asset/font/Roboto/Roboto-Regular.ttf"

/// Storage object to store all global textures and fonts
struct SpriteStorage {
    Texture *cubeMapSpriteSheet = nullptr;
    Texture *worldMapSpriteSheet = nullptr;
    Texture *playerSpriteSheet = nullptr;
    Texture *temp = nullptr;
    Font *basicFont = nullptr;
    Font *debugFont = nullptr;

    /// dice backgrounds, side x is stored in [x-1]
    std::vector<Texture *> sideSprites;
};