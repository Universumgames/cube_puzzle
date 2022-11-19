#include "CubeMap.hpp"
#include "../recthelper.hpp"

Point CubeMapSide::cubePositionToScreenPosition(DiceData diceData, Point cubePos) const {
    DiceSideRotation faceDirection = diceData.getDiceSideRotation(sideID);
    Point res = {};
    switch (faceDirection) {
        case DiceSideRotation::UP:
            return cubePos;
        case DiceSideRotation::DOWN:
            return {this->width - cubePos.x - 1, this->height - cubePos.y - 1};
        case DiceSideRotation::LEFT:
            return {cubePos.y, this->width - cubePos.x - 1};
        case DiceSideRotation::RIGHT:
            return {this->height - cubePos.y - 1, cubePos.x};
    }
    return res;
}

void CubeMapSide::HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    for (auto *field: side) {
        field->HandleEvent(game, BASIC_GO_DATA_PASSTHROUGH, event);
    }
}

void CubeMapSide::Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) {
    for (auto *field: side) {
        field->Update(game, BASIC_GO_DATA_PASSTHROUGH);
    }
    if (overlay != nullptr) overlay->setEnabled(game.isDebug());
}

void CubeMapSide::Render(CubeGame &game, Renderer *render, DiceData diceData, const u32 frame, const u32 totalMSec,
                         const float deltaT) {
    int x = 0, y = 0;
    Point size = getFieldSize(game.getWindowSize());
    Point offset = getStartingOffset(game.getWindowSize(), size);
    Rect drawableRect = getDrawableRect(game.getWindowSize());
    DiceSideRotation rotation = diceData.getDiceSideRotation(sideID);
    int dimm = sin(frame / 120.0) * 20;
    SDL_SetTextureColorMod(game.getSpriteStorage()->sideSprites[sideID - 1], 230 + dimm,230 + dimm,230 + dimm);
    drawSide(game.getSpriteStorage()->sideSprites[sideID - 1], render, drawableRect, rotation);
    SDL_SetTextureColorMod(game.getSpriteStorage()->sideSprites[sideID - 1], 255,255,255);

    if (overlay == nullptr) {
        overlay = new Text(game, render, 400, "", game.getSpriteStorage()->debugFont, {0, 0});
    }
    for (auto *field: side) {
        Point pos = cubePositionToScreenPosition(diceData, {x, y});
        field->Render(game, render, size, {size.x * pos.x + offset.x, size.y * pos.y + offset.y},
                      BASIC_GO_DATA_PASSTHROUGH);
        overlay->changePosition({size.x * pos.x + offset.x, size.y * pos.y + offset.y});
        overlay->changeText(std::to_string(x) + "," + std::to_string(y));
        overlay->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
        x++;
        if (x >= width) {
            y++;
            x = 0;
        }
    }
    renderGridOverlay(game, render, diceData, BASIC_GO_DATA_PASSTHROUGH);
}

Point CubeMapSide::getFieldSize(Point windowSize) {
    int w = min(windowSize.x, windowSize.y) / width;
    return {w, w};
}

Point CubeMapSide::getStartingOffset(Point windowSize, Point fieldSize) {
    Point center = windowSize / 2;
    Point totalSize = {width * fieldSize.x, height * fieldSize.y};
    return center - (totalSize / 2);
}

void CubeMapSide::renderGridOverlay(CubeGame &game, Renderer *render, DiceData diceData, const u32 frame,
                                    const u32 totalMSec, const float deltaT) {
    Point size = getFieldSize(game.getWindowSize());
    Point offset = getStartingOffset(game.getWindowSize(), size);
    Rect drawableRect = getDrawableRect(game.getWindowSize());
    if (game.isDebug()) {
        // grid lines
        double lineWidth = max(max(size.x, size.y) / 40.0, 2.0);
        for (int x = 1; x < width; x++) {
            Rect dst = {(int) (offset.x + size.x * x - lineWidth / 2), offset.y, (int) lineWidth, drawableRect.h};
            SDL_SetRenderDrawColor(render, 0, 0, 0, 100);
            SDL_RenderFillRect(render, &dst);
        }
        for (int y = 1; y < height; y++) {
            Rect dst = {offset.x, (int) (offset.y + size.y * y - lineWidth / 2), drawableRect.w, (int) lineWidth};
            SDL_SetRenderDrawColor(render, 0, 0, 0, 100);
            SDL_RenderFillRect(render, &dst);
        }


        // colored rectangle
        auto sideOrientation = diceData.getDiceSideRotation(sideID);
        Rect dst = {0, 0, 0, 0};
        switch (sideOrientation) {
            case DiceSideRotation::UP:
                dst = {(int) (offset.x + (drawableRect.w / 2.0) - 25), offset.y, 50, 50};
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
                break;
            case DiceSideRotation::DOWN:
                dst = {(int) (offset.x + (drawableRect.w / 2.0) - 25), offset.y + drawableRect.h - 50, 50, 50};
                SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
                break;
            case DiceSideRotation::LEFT:
                dst = {(int) offset.x, (int) (offset.y + (drawableRect.h / 2.0) - 25), 50, 50};
                SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
                break;
            case DiceSideRotation::RIGHT:
                dst = {offset.x + drawableRect.w - 25, (int) (offset.y + (drawableRect.h / 2.0) - 25), 50, 50};
                SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
                break;
        }
        SDL_RenderFillRect(render, &dst);
    }
}

Rect CubeMapSide::getDrawableRect(Point windowSize) {
    Point size = getFieldSize(windowSize);
    Point offset = getStartingOffset(windowSize, size);
    return {offset.x, offset.y, size.x * width, size.y * height};
}

Point CubeMapSide::screenPositionToCubePosition(DiceData diceData, Point screenPos) const {
    Point p = {};
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            p = {x,y};
            Point s = cubePositionToScreenPosition(diceData, p);
            if( s.x == screenPos.x && s.y == screenPos.y) break;
        }
    }
    return p;
}
