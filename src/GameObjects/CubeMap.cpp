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

void sAndNRotation(int w, int c, int e, int b, int n, int s, DiceSideRotation *nRot, DiceSideRotation *sRot) {
    if (nRot == nullptr || sRot == nullptr) return;
    auto anchorN = DiceData::getAnchorSideOfTopLeftCorner(n);
    auto anchorS = DiceData::getAnchorSideOfTopLeftCorner(s);
    if (anchorN == w) *nRot = DiceSideRotation::LEFT;
    else if (anchorN == c) *nRot = DiceSideRotation::DOWN;
    else if (anchorN == e) *nRot = DiceSideRotation::RIGHT;
    else if (anchorN == b) *nRot = DiceSideRotation::UP;

    if (anchorS == w) *sRot = DiceSideRotation::LEFT;
    else if (anchorS == c) *sRot = DiceSideRotation::UP;
    else if (anchorS == e) *sRot = DiceSideRotation::RIGHT;
    else if (anchorS == b) *sRot = DiceSideRotation::DOWN;
}

void CubeMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    minimapText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    int c = currentSideId, n = 0, w = 0, e = 0, s = 0, b = 0;
    int relativeToWindow = max(game.getWindowSize().x, game.getWindowSize().y) / 20.0;
    int sideSize = max(relativeToWindow, 32);
    int sideDistance = 10;
    DiceData cloneData = DiceData(diceData);
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    Point offset = {game.getWindowSize().x - (sideSize * 3 + sideDistance), sideDistance};
    Color borderColor = black;
    int borderWidth = 1;

    auto nRot = DiceSideRotation::UP;
    auto sRot = DiceSideRotation::UP;
    sAndNRotation(w, c, e, b, n, s, &nRot, &sRot);

    // border of y-axis of cross
    drawColoredFilledRect(render, borderColor,
                          {offset.x + sideSize - borderWidth, offset.y - borderWidth, sideSize + borderWidth * 2,
                           sideSize * 4 + borderWidth * 2});
    // border of x-axis of cross
    drawColoredFilledRect(render, borderColor,
                          {offset.x - borderWidth, offset.y + sideSize - borderWidth, sideSize * 3 + borderWidth * 2,
                           sideSize + borderWidth * 2});
    drawSide(game.getSpriteStorage()->sideSprites[n - 1], render, {offset.x + sideSize, offset.y, sideSize, sideSize},
             nRot);
    drawSide(game.getSpriteStorage()->sideSprites[w - 1], render, {offset.x, offset.y + sideSize, sideSize, sideSize},
             cloneData.getDiceSideRotation(w));
    drawSide(game.getSpriteStorage()->sideSprites[c - 1], render,
             {offset.x + sideSize, offset.y + sideSize, sideSize, sideSize}, cloneData.getDiceSideRotation(c));
    drawSide(game.getSpriteStorage()->sideSprites[e - 1], render,
             {offset.x + sideSize * 2, offset.y + sideSize, sideSize, sideSize}, cloneData.getDiceSideRotation(e));
    drawSide(game.getSpriteStorage()->sideSprites[s - 1], render,
             {offset.x + sideSize, offset.y + sideSize * 2, sideSize, sideSize}, sRot);
    drawSide(game.getSpriteStorage()->sideSprites[b - 1], render,
             {offset.x + sideSize, offset.y + sideSize * 3, sideSize, sideSize}, cloneData.getDiceSideRotation(b));
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