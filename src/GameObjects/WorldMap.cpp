#include "WorldMap.hpp"
#include "CubeMap.hpp"
#include "../recthelper.hpp"

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
    int effectX = (x < 0) ? xWidth - x - 2 : x;
    int effectY = (y < 0) ? yHeight - y - 2 : y;
    return effectX * xWidth + effectY;
}

void WorldMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    mapCoordinates->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    drawMinimap();
}

void WorldMap::drawMinimap() {
    // minimap preferences
    int xyMapDelta = 1;
    int wh = 32;
    int edgeOffset = 50;
    Point offset = game.getWindowSize() - Point{wh * 3 + edgeOffset, wh * 3 + edgeOffset};
    int startX = cubePos.x - xyMapDelta;
    int startY = cubePos.y - xyMapDelta;
    int borderWidth = 1;
    Color borderColor = black;
    // draw border around minimap
    SDL_SetRenderDrawColor(render, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    Rect border = {offset.x - borderWidth, offset.y - borderWidth, wh * 3 + borderWidth * 2, wh * 3 + borderWidth * 2};
    SDL_RenderFillRect(render, &border);
    // draw minimap
    for (int x = startX; x <= cubePos.x + xyMapDelta; x++) {
        for (int y = startY; y <= cubePos.y + xyMapDelta; y++) {
            int fIndex = getFieldIndex(x, y);
            WorldField wField = field[fIndex];
            drawWorldField(render, game.getSpriteStorage(), wField,
                           {offset.x + wh * (x - startX),
                            offset.y + wh * (y - startY),
                            wh, wh});
        }
    }
    // draw Cube Position in WorldMap as red dot
    Rect dst = {offset.x + (int) (wh * 1.25), offset.y + (int) (wh * 1.25), wh / 2, wh / 2};
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderFillRect(render, &dst);
}
