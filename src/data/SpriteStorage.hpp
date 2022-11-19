#pragma once

#include "../global.hpp"
#include "DiceData.hpp"
#include "paths.hpp"

#define SPRITE_TILE_SIZE 16
#define SIDE_SIZE 48

/**
 * simple draw sprite
 * @param texture texture to draw from
 * @param render sdl renderer
 * @param imageIndex texture divided into SPRITE_TILE_SIZE x SPRITE_TILE_SIZE tiles, the index of x,y coordinate
 * @param dst destination rectangle to draw to on screen
 */
inline void drawSprite(Texture *texture, Renderer *render, Point imageIndex, Rect dst, double angle = 0,
                       const SDL_RendererFlip flip = SDL_FLIP_NONE) {
    Rect src = {imageIndex.x * SPRITE_TILE_SIZE, imageIndex.y * SPRITE_TILE_SIZE, SPRITE_TILE_SIZE, SPRITE_TILE_SIZE};
    SDL_RenderCopyEx(render, texture, &src, &dst, angle, nullptr, flip);
}

/**
 * draw full texture
 * @param texture texture to draw
 * @param render sdl renderer
 * @param dst destination rectangle to draw to on screen
 * @param angle angle of rotation
 */
inline void drawSide(Texture *texture, Renderer *render, Rect dst, double angle = 0.0) {
    SDL_RenderCopyEx(render, texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}

/**
 * draw full texture
 * @param texture texture to draw
 * @param render sdl renderer
 * @param dst destination rectangle to draw to on screen
 */
inline void drawSide(Texture *texture, Renderer *render, Rect dst, DiceSideRotation rotation = DiceSideRotation::UP) {
    //Rect src = {0,0, SIDE_SIZE, SIDE_SIZE};
    double angle = (double) rotation;
    drawSide(texture, render, dst, angle);
}

inline void drawColoredFilledRect(Renderer *render, Color color, Rect *dst) {
    SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(render, dst);
}

inline void drawColoredFilledRect(Renderer *render, Color color, Rect dst) {
    SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(render, &dst);
}

/// Storage object to store all global textures and fonts
struct SpriteStorage {
    Texture *cubeMapSpriteSheet = nullptr;
    Texture *worldMapSpriteSheet = nullptr;
    Texture *playerSpriteSheet = nullptr;
    Texture *arrowSemiCircle = nullptr;
    Texture *arrowStraight = nullptr;
    Texture *temp = nullptr;
    Font *basicFont = nullptr;
    Font *debugFont = nullptr;

    /// dice backgrounds, side x is stored in [x-1]
    std::vector<Texture *> sideSprites;
};