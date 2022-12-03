#include "CubeObject.hpp"
#include "../data/spriteDefs.hpp"

// ################################# Alle Render-Methoden ############################################################################

void drawCubeObject(Renderer* render, Texture* sheet, Point size, Point location, Point imageIndex, DiceSideRotation rotation){
    drawSprite(sheet, render, imageIndex, {location.x, location.y, size.x, size.y}, (int)rotation);
}

void CubeObject::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game, render, dst);
}

void Stone::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_STONE_INDEX, diceData->getDiceSideRotation(sideId));
}

void Slider::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO implement rotated rendering depending on orientation of slider
    switch(direction){
        case MovementDirectionIfActivated::moveToBigX:
            break;
        case MovementDirectionIfActivated::moveToSmallX:
            break;
        case MovementDirectionIfActivated::moveToBigY:
            break;
        case MovementDirectionIfActivated::moveToSmallY:
            break;
    }
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_SLIDER_INDEX, diceData->getDiceSideRotation(sideId));
}

void Magnet::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_MAGNET_INDEX, diceData->getDiceSideRotation(sideId));
}

void Flag::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    //Flag should always be render upright
    drawCubeObject(render, game.getSpriteStorage()->cubeObjectSpriteSheet, size, location, SPRITE_FLAG_INDEX, DiceSideRotation::UP);
}

void CubeObject::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug()) {
        return;
    }
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}