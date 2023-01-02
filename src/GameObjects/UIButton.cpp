//
// Created by Tom Arlt on 02.01.23.
//

#include "UIButton.hpp"
#include "../CubeGame.hpp"
#include "../data/SpriteStorage.hpp"
#include "../recthelper.hpp"



void UIButton::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    TouchObject::RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    drawSpriteBorder(game.isDebug(), render, renderRect);
    if (!enabled)return;

    if (!useTexture) {
        drawColoredFilledRect(render, color, renderRect);
    } else {
        if (textureAngle == 0)
            SDL_RenderCopy(render, renderTexture, textureSrcNull ? NULL : &textureSrc, &renderRect);
        else
            SDL_RenderCopyEx(render, renderTexture, textureSrcNull ? NULL : &textureSrc, &renderRect, textureAngle,
                             NULL, SDL_FLIP_NONE);
    }


}

UIButton::UIButton(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render, Point location, Point size,
                   Texture *renderTexture, int touchPadding) : TouchObject(game, gameState, render, location,
                                                                           size, touchPadding),
                                                               renderTexture(renderTexture) {
    this->useTexture = true;
    updateRects();
}

UIButton::UIButton(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render, Point location, Point size,
                   Color color, int touchPadding) : TouchObject(game, gameState, render, location,
                                                                size, touchPadding), color(color) {
    this->useTexture = false;
    updateRects();
}


Rect UIButton::getRenderRect() {
    return renderRect;
}


void UIButton::setTextureSettings(int angle, Rect src) {
    this->textureAngle = angle;
    this->textureSrc = src;
    this->textureSrcNull = src == Rect{0, 0, 0, 0};
}

bool UIButton::getEnabled() {
    return enabled;
}

void UIButton::setEnabled(bool enabled) {
    this->enabled = enabled;
}


void UIButton::setTexture(Texture *texture) {
    this->renderTexture = texture;
    this->useTexture = true;
}

void UIButton::setColor(Color color) {
    this->color = color;
    this->useTexture = false;
}

void UIButton::updateRects() {
    TouchObject::updateRects();
    renderRect = {location.x, location.y, size.x, size.y};
}


