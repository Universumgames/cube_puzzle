//
// Created by Tom Arlt on 02.11.22.
//

#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"

void CubeMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void CubeMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void CubeMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMap();
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap();
}

void CubeMap::drawMinimap() {

}

void CubeMap::drawMap() {

}


CubeMapSide *CubeMap::getSide(int i) {
    return sides[i - 1];
}

CubeMap::CubeMap(Game &game1, SDL_Renderer *render1, const Vector<CubeMapSide> &sides1, int startSide, Point playerPos)
        : GameObject(game1, render1) {
    for (auto side: sides1) {
        this->sides.push_back(&side);
    }
    this->currentSideId = startSide;
    this->playerPos = playerPos;
}

bool CubeMap::movePlayer(PlayerMoveDirection dir) {

    return false;
}

void CubeMap::rollCube(DiceRollDirection rollDirection) {
    int temp = 0;
    switch (rollDirection) {
        case NORTH:
            worldMap->cubePos += 1_up;
            break;
        case SOUTH:
            worldMap->cubePos += 1_down;
            break;
        case WEST:
            worldMap->cubePos += 1_left;
            break;
        case EAST:
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
    for (auto *field: side) {
        field->Render(game, render, {10,10}, {10,10} ,BASIC_GO_DATA_PASSTHROUGH);
    }
}
