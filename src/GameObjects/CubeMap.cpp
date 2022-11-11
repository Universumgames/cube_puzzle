#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"

void CubeMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type != SDL_KEYDOWN) return;
    const Keysym &what_key = event.key.keysym;
    if (what_key.scancode == SDL_SCANCODE_UP) {

    }
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
}

void CubeMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
    debugSideIndicator->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    debugDiceData->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

void drawMinimapSide(std::vector<Texture *> diceSideTextures, Renderer *render, DiceData diceData, int side, Rect dst) {
    drawSide(diceSideTextures[side - 1], render, dst,
             diceData.getDiceSideRotation(side));
}

void CubeMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    minimapText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    int c = currentSideId, n = 0, w = 0, e = 0, s = 0, b = 0;
    int sideSize = 32;
    int sideDistance = 10;
    DiceData cloneData = DiceData(diceData);
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    Point offset = {game.getWindowSize().x - (sideSize * 3 + sideDistance), sideDistance};

    drawMinimapSide(game.getSpriteStorage()->sideSprites, render, cloneData, n,
                    {offset.x + sideSize, offset.y, sideSize, sideSize});
    drawMinimapSide(game.getSpriteStorage()->sideSprites, render, cloneData, w,
                    {offset.x, offset.y + sideSize, sideSize, sideSize});
    drawMinimapSide(game.getSpriteStorage()->sideSprites, render, cloneData, c,
                    {offset.x + sideSize, offset.y + sideSize, sideSize, sideSize});
    drawMinimapSide(game.getSpriteStorage()->sideSprites, render, cloneData, e,
                    {offset.x + sideSize * 2, offset.y + sideSize, sideSize, sideSize});
    drawMinimapSide(game.getSpriteStorage()->sideSprites, render, cloneData, s,
                    {offset.x + sideSize, offset.y + sideSize * 2, sideSize, sideSize});
    drawMinimapSide(game.getSpriteStorage()->sideSprites, render, cloneData, b,
                    {offset.x + sideSize, offset.y + sideSize * 3, sideSize, sideSize});
}


void CubeMap::drawMap(const u32 frame, const u32 totalMSec, const float deltaT) {
    getCurrentSide()->Render(game, render, diceData, BASIC_GO_DATA_PASSTHROUGH);
}


CubeMapSide *CubeMap::getSide(int i) {
    return sides[i - 1];
}

CubeMap::CubeMap(CubeGame &game1, SDL_Renderer *render1, const Vector<CubeMapSide> &sides1, int startSide,
                 Point playerPos)
        : GameObject(game1, render1) {
    for (auto side: sides1) {
        this->sides.push_back(new CubeMapSide(side));
    }
    this->currentSideId = startSide;
    this->playerPos = playerPos;
    this->debugSideIndicator = new Text(game, render, 400, "", game1.getSpriteStorage()->debugFont, {10, 30});
    this->minimapText = new Text(game, render, 400, "", game.getSpriteStorage()->debugFont, {10, 60});
    this->debugDiceData = new Text(game, render, 400, "", game.getSpriteStorage()->debugFont, {10, 230});
    updateMinimap();
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

Rect CubeMap::playerDrawPosition() {
    CubeMapSide *side = getCurrentSide();
    auto origSize = side->getFieldSize(game.getWindowSize());
    auto size = origSize * 0.8;
    auto gridOffset = side->getStartingOffset(game.getWindowSize(), origSize) + ((origSize - size) / 2);
    auto screenGridPos = getCurrentSide()->cubePositionToScreenPosition(diceData, playerPos);
    return {origSize.x * screenGridPos.x + gridOffset.x, origSize.y * screenGridPos.y + gridOffset.y, size.x, size.y};
}

CubeMapSide *CubeMap::getCurrentSide() {
    return getSide(currentSideId);
}

void CubeMap::updateMinimap() {
    int n = 0, w = 0, c = 0, e = 0, s = 0, b = 0;
    c = currentSideId;
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    std::string text = "number cube minimap: \n   " + std::to_string(n) + "   \n" +
                       std::to_string(w) + " " + std::to_string(c) + " " + std::to_string(e) + "\n" +
                       "   " + std::to_string(s) + "   \n" +
                       "   " + std::to_string(b) + "   ";
    minimapText->changeText(text);
}