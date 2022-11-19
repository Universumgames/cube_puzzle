#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "CubeMapMiniMap.hpp"

void CubeMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    //if (event.type != SDL_KEYDOWN) return;
    const Keysym &what_key = event.key.keysym;
    if (what_key.scancode == SDL_SCANCODE_UP) {

    }
    miniMap->HandleEvent(BASIC_GO_DATA_PASSTHROUGH, event);
}

void CubeMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    debugSideIndicator->setEnabled(game.isDebug());
    debugSideIndicator->changeText("Current Side: " + std::to_string(currentSideId));
    minimapText->setEnabled(game.isDebug());
    debugDiceData->setEnabled(game.isDebug());
    debugDiceData->changeText("DiceSides: n" + std::to_string(diceData.getSide(DiceSide::NORTH)) + " ,w" +
                              std::to_string(diceData.getSide(DiceSide::WEST)) + " , u" +
                              std::to_string(diceData.getSide(DiceSide::TOP)));
    getCurrentSide()->Update(game, BASIC_GO_DATA_PASSTHROUGH);

    miniMap->Update(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    debugSideIndicator->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    debugDiceData->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    miniMap->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}


void CubeMap::drawMap(const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect drawableRect = getDrawableRect();
    getCurrentSide()->Render(game, render, diceData, BASIC_GO_DATA_PASSTHROUGH, drawableRect);
}


CubeMapSide *CubeMap::getSide(int i) {
    return sides[i - 1];
}

CubeMap::CubeMap(CubeGame &game1, SDL_Renderer *render1, const Vector<CubeMapSide *> &sides1, int startSide,
                 Point playerPos)
        : GameObject(game1, render1) {
    this->sides.assign(sides1.begin(), sides1.end());
    this->currentSideId = startSide;
    this->playerPos = playerPos;
    this->debugSideIndicator = new Text(game, render, 400, "", game1.getSpriteStorage()->debugFont, {10, 30});
    this->minimapText = new Text(game, render, 400, "", game.getSpriteStorage()->debugFont, {10, 60});
    this->debugDiceData = new Text(game, render, 400, "", game.getSpriteStorage()->debugFont, {10, 230});
    this->miniMap = new CubeMapMiniMap(game, render, this);
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

Rect CubeMap::getDrawableRect() {
    Point windowSize = game.getWindowSize();
    Point center = windowSize / 2;
    int s = min(windowSize.x, windowSize.y);
    Point totalSize = {s, s};
    Point offset = center - (totalSize / 2);
    return {offset.x, offset.y, s, s};
}

Rect CubeMap::playerDrawPosition() {
    CubeMapSide *side = getCurrentSide();
    auto origSize = side->getFieldSize(getDrawableRect());
    auto size = origSize * 0.8;
    auto gridOffset = getDrawableRect() + ((origSize - size) / 2);
    auto screenGridPos = getCurrentSide()->cubePositionToScreenPosition(diceData, playerPos);
    return {origSize.x * screenGridPos.x + gridOffset.x, origSize.y * screenGridPos.y + gridOffset.y, size.x, size.y};
}