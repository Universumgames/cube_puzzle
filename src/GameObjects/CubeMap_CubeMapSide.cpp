#include "CubeMap.hpp"
#include "../recthelper.hpp"

Point CubeMapSide::cubePositionToScreenPosition(DiceData diceData, Point cubePos) const {
    DiceFaceDirection faceDirection = diceData.getDiceSideRotation(sideID);
    Point res = {};
    switch (faceDirection) {
        case DiceFaceDirection::UP:
            return cubePos;
        case DiceFaceDirection::DOWN:
            return {this->width - cubePos.x - 1, this->height - cubePos.y - 1};
        case DiceFaceDirection::LEFT:
            return {cubePos.y, this->width - cubePos.x - 1};
        case DiceFaceDirection::RIGHT:
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
    drawSide(game.getSpriteStorage()->sideSprites[sideID - 1], render, drawableRect);
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


        auto sideOrientation = diceData.getDiceSideRotation(sideID);
        Rect dst = {0, 0, 0, 0};
        switch (sideOrientation) {
            case DiceFaceDirection::UP:
                dst = {(int) (offset.x + (drawableRect.w / 2.0) - 25), offset.y, 50, 50};
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
                break;
            case DiceFaceDirection::DOWN:
                dst = {(int) (offset.x + (drawableRect.w / 2.0) - 25), offset.y + drawableRect.h - 50, 50, 50};
                SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
                break;
            case DiceFaceDirection::LEFT:
                dst = {(int) offset.x, (int) (offset.y + (drawableRect.h / 2.0) - 25), 50, 50};
                SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
                break;
            case DiceFaceDirection::RIGHT:
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
