//
// Created by Tom Arlt on 02.11.22.
//

#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"

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
    return sides[i];
}

CubeMap::CubeMap(Game &game1, SDL_Renderer *render1, const Vector<CubeMapSide> &sides1) : GameObject(game1, render1) {
    for (auto side: sides1) {
        this->sides.push_back(&side);
    }
}
