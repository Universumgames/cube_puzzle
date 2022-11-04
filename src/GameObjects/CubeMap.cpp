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
    return sides[i-1];
}

CubeMap::CubeMap(Game &game1, SDL_Renderer *render1, const Vector<CubeMapSide> &sides1) : GameObject(game1, render1) {
    for (auto side: sides1) {
        this->sides.push_back(&side);
    }
}

bool CubeMap::movePlayer(PlayerMoveDirection dir) {
    return false;
}

void CubeMap::rollCube(DiceRollDirection rollDirection) {
    int temp = 0;
    switch(rollDirection){
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
