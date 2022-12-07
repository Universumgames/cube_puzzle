#include "CubeObject.hpp"
#include "../data/spriteDefs.hpp"

// ################################# Alle Render-Methoden ############################################################################

void drawCubeObject(Renderer *render, Texture *sheet, Point size, Point location, Point imageIndex,
                    DiceSideRotation rotation) {
    drawSprite(sheet, render, imageIndex, {location.x, location.y, size.x, size.y}, (int) rotation);
}

void drawCubeObject(Renderer *render, Texture *sheet, Point size, Point location, Point imageIndex) {
    drawSprite(sheet, render, imageIndex, {location.x, location.y, size.x, size.y});
}

void CubeObject::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                        float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game, render, dst);
}

void
Stone::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_STONE_INDEX);
}


void
Slider::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    DiceSideRotation rotation = diceData->getDiceSideRotation(sideId);

    // TODO implement rotated rendering depending on orientation of slider
    Point imageIndex = {};
    switch (directionIfActivated) {
        case MovementDirection::moveToBigX:
            imageIndex = SPRITE_SLIDER_RIGHT_INDEX;
            break;
        case MovementDirection::moveToSmallX:
            imageIndex = SPRITE_SLIDER_LEFT_INDEX;
            break;
        case MovementDirection::moveToBigY:
            imageIndex = SPRITE_SLIDER_DOWN_INDEX;
            break;
        case MovementDirection::moveToSmallY:
            imageIndex = SPRITE_SLIDER_UP_INDEX;
            break;
        case MovementDirection::none:
            imageIndex = SPRITE_SLIDER_RIGHT_INDEX;
            break;
    }
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, imageIndex, rotation);
}

void
Magnet::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_MAGNET_INDEX);
}

void
Flag::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    //Flag should always be rendered upright
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_FLAG_INDEX,
                   DiceSideRotation::UP);
}

void CubeObject::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug()) {
        return;
    }
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}