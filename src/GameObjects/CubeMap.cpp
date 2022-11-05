//
// Created by Tom Arlt on 02.11.22.
//

#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"

void CubeMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if(event.type != SDL_KEYDOWN) return;
    const Keysym & what_key = event.key.keysym;
    if( what_key.scancode == SDL_SCANCODE_UP ){

    }
}

void CubeMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void CubeMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void CubeMap::drawMap(const u32 frame, const u32 totalMSec, const float deltaT) {
    for (auto side: sides) {
        side->Render(game, render, BASIC_GO_DATA_PASSTHROUGH);
    }
}


CubeMapSide *CubeMap::getSide(int i) {
    return sides[i - 1];
}

CubeMap::CubeMap(Game &game1, SDL_Renderer *render1, const Vector<CubeMapSide> &sides1, int startSide, Point playerPos)
        : GameObject(game1, render1) {
    for (auto side: sides1) {
        this->sides.push_back(new CubeMapSide(side));
    }
    this->currentSideId = startSide;
    this->playerPos = playerPos;
}

bool CubeMap::movePlayer(PlayerMoveDirection dir) {

    return false;
}

void CubeMap::moveCubeInWorld(DiceRollDirection rollDirection) {
    int temp = 0;
    switch (rollDirection) {
        case DiceRollDirection::NORTH:
            worldMap->cubePos += 1_up;
            break;
        case DiceRollDirection::SOUTH:
            worldMap->cubePos += 1_down;
            break;
        case DiceRollDirection::WEST:
            worldMap->cubePos += 1_left;
            break;
        case DiceRollDirection::EAST:
            worldMap->cubePos += 1_right;
            break;
    }
}

CubeField *CubeMap::getField(int side, int x, int y) {
    return getSide(side)->getField(x, y);
}

CubeField *CubeMap::getField(int side, Point p) {
    return getSide(side)->getField(p.x, p.y);
}

void CubeMap::Init() {
    GameObject::Init();
}

Point CubeMap::cubePositionToScreenPosition(Point cubePos) {
    int anchorSide = DiceData::getAnchorSideOfTopLeftCorner(currentSideId);
    DiceFaceDirection faceDirection = diceData.get
    return Point();
}

void CubeMapSide::HandleEvent(Game &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    for (auto *field: side) {
        field->HandleEvent(game, BASIC_GO_DATA_PASSTHROUGH, event);
    }
}

void CubeMapSide::Update(Game &game, const u32 frame, const u32 totalMSec, const float deltaT) {
    for (auto *field: side) {
        field->Update(game, BASIC_GO_DATA_PASSTHROUGH);
    }
}

void CubeMapSide::Render(Game &game, Renderer *render, const u32 frame, const u32 totalMSec, const float deltaT) {
    int x = 0, y = 0;
    Point size = getFieldSize(game.getWindowSize());
    Point offset = getStartingOffset(game.getWindowSize(), size);
    for (auto *field: side) {
        field->Render(game, render, size, {size.x * x + offset.x, size.y * y + offset.y}, BASIC_GO_DATA_PASSTHROUGH);
        x++;
        if (x >= width) {
            y++;
            x = 0;
        }
    }
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
