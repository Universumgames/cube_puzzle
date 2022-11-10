#include "WorldMap.hpp"
#include "CubeMap.hpp"

void WorldMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void WorldMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    mapCoordinates->setEnabled(game.isDebug());
    mapCoordinates->changeText("CubePos in world: x" + std::to_string(cubePos.x) + " y" + std::to_string(cubePos.y));
}

void WorldMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
}

WorldMap::WorldMap(CubeGame &game, SDL_Renderer *render, int xWidth, int yHeight, Vector<WorldField> map, Point cubePos)
        : GameObject(game, render) {
    this->field = map;
    this->xWidth = xWidth;
    this->yHeight = yHeight;
    this->cubePos = cubePos;
    this->mapCoordinates = new Text(game, render, 500, "", game.getSpriteStorage()->debugFont, {10, 200});
}

int WorldMap::getFieldIndex(int x, int y) const {
    return x * xWidth + y;
}

void WorldMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    mapCoordinates->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}
