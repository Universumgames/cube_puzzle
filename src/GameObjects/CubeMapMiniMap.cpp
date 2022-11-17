#include "CubeMapMiniMap.hpp"
#include "../data/Colors.hpp"
#include "CubeMap.hpp"

void CubeMapMiniMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void CubeMapMiniMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (oldDiceData != diceData || oldSide != cubeMap->currentSideId) {
        // cube rotated
        updateDebugText();
        updateMinimap(BASIC_GO_DATA_PASSTHROUGH);
    }
}

void CubeMapMiniMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

CubeMapMiniMap::CubeMapMiniMap(CubeGame &game, SDL_Renderer *render, CubeMap *cubeMap) : GameObject(game, render),
                                                                                         diceData(cubeMap->diceData) {
    this->cubeMap = cubeMap;
    this->oldSide = cubeMap->currentSideId;
    updateMinimap(0,0,0);
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

void CubeMapMiniMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    cubeMap->minimapText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);

    Color borderColor = black;

    // border of y-axis of cross
    drawColoredFilledRect(render, borderColor, bgYDst);
    // border of x-axis of cross
    drawColoredFilledRect(render, borderColor, bgXDst);

    for (auto side: sidesDst) {
        drawSide(game.getSpriteStorage()->sideSprites[side.side - 1], render, side.dst, side.rot);
    }
}

void CubeMapMiniMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMapMiniMap::updateMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (animationState == AnimationState::FINISHED) {
        animationState = AnimationState::TRANSITIONING;
        animationProgress = 0;
    }

    int c = cubeMap->currentSideId, n = 0, w = 0, e = 0, s = 0, b = 0;
    int relativeToWindow = max(game.getWindowSize().x, game.getWindowSize().y) / 20.0;
    int sideSize = max(relativeToWindow, 32);
    int sideDistance = 10;
    auto cloneData = DiceData(diceData);
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    Point offset = {game.getWindowSize().x - (sideSize * 3 + sideDistance), sideDistance};
    int borderWidth = 1;

    auto nRot = DiceSideRotation::UP;
    auto sRot = DiceSideRotation::UP;
    sAndNRotation(w, c, e, b, n, s, &nRot, &sRot);

    bgYDst = {offset.x + sideSize - borderWidth, offset.y - borderWidth, sideSize + borderWidth * 2,
              sideSize * 4 + borderWidth * 2};
    bgXDst = {offset.x - borderWidth, offset.y + sideSize - borderWidth, sideSize * 3 + borderWidth * 2,
              sideSize + borderWidth * 2};


    /*
     * struct RenderData{
        int side;
        Rect dst;
        DiceSideRotation rot;
    };
     */
    sidesDst = {
            {.side = n, .dst = {offset.x + sideSize, offset.y, sideSize, sideSize}, .rot = nRot},
            {.side = w, .dst = {offset.x, offset.y + sideSize, sideSize,
                                sideSize}, .rot = cloneData.getDiceSideRotation(w)},
            {.side = c, .dst = {offset.x + sideSize, offset.y + sideSize, sideSize, sideSize}, .rot = cloneData.getDiceSideRotation(c)},
            {.side = e, .dst = {offset.x + sideSize * 2, offset.y + sideSize, sideSize, sideSize}, .rot= cloneData.getDiceSideRotation(e)},
            {.side = s, .dst = {offset.x + sideSize, offset.y + sideSize * 2, sideSize, sideSize}, .rot = sRot},
            {.side = b, .dst = {offset.x + sideSize, offset.y + sideSize * 3, sideSize, sideSize}, .rot = cloneData.getDiceSideRotation(b)}
    };
    animationState = AnimationState::FINISHED;


    if (animationState == AnimationState::FINISHED) {
        oldDiceData = DiceData(diceData);
        oldSide = cubeMap->currentSideId;
    }
}

void CubeMapMiniMap::updateDebugText() {
    int n = 0, w = 0, c = 0, e = 0, s = 0, b = 0;
    c = cubeMap->currentSideId;
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    std::string text = "number cube minimap: \n   " + std::to_string(n) + "   \n" +
                       std::to_string(w) + " " + std::to_string(c) + " " + std::to_string(e) + "\n" +
                       "   " + std::to_string(s) + "   \n" +
                       "   " + std::to_string(b) + "   ";
    cubeMap->minimapText->changeText(text);
}

