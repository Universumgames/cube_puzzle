//
// Created by Tom Arlt on 02.11.22.
//

#include "WorldMap.hpp"
#include "CubeMap.hpp"

void WorldMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void WorldMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void WorldMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

}

WorldMap::WorldMap(Game &game, SDL_Renderer *render, int xWidth, int yHeight, Vector<WorldField> map) : GameObject(game, render) {
    this->field = map;
    this->xWidth = xWidth;
    this->yHeight = yHeight;
}

int WorldMap::getFieldIndex(int x, int y) const {
    return x * xWidth + y;
}
