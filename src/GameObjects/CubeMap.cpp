#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "CubeMapMiniMap.hpp"
#include "../States/Level.hpp"

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
    renderTarget = SDL_GetRenderTarget(render);

    drawMap(BASIC_GO_DATA_PASSTHROUGH);
    if (!sideTransitionAnimating) {
        saveCurrentFrame();
        sideTransitionState = 0;
    }
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    debugSideIndicator->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    debugDiceData->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    miniMap->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}


void CubeMap::drawMap(const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect drawableRect = getDrawableRect();
    if (sideTransitionAnimating) {
        SDL_RenderCopy(render, oldSideFrame, NULL, NULL);
        sideTransitionState += deltaT * 10;
        if (sideTransitionState >= 1) {
            sideTransitionState = 0;
            sideTransitionAnimating = false;
        }
    } else
        getCurrentSide()->Render(game, gameState, render, diceData, BASIC_GO_DATA_PASSTHROUGH, drawableRect);
}


CubeMapSide *CubeMap::getSide(int i) {
    return sides[i - 1];
}

CubeMap::CubeMap(CubeGame &game1, ComplexGameState *gameState, SDL_Renderer *render1,
                 const Vector<CubeMapSide *> &sides, int startSide,
                 Point playerPos)
        : GameObject(game1, gameState, render1) {
    this->sides.assign(sides.begin(), sides.end());
    this->currentSideId = startSide;
    this->playerPos = playerPos;
    this->debugSideIndicator = new Text(game, gameState, render, 400, "", game1.getSpriteStorage()->debugFont,
                                        {10, 30});
    this->minimapText = new Text(game, gameState, render, 400, "", game.getSpriteStorage()->debugFont, {10, 60});
    this->debugDiceData = new Text(game, gameState, render, 400, "", game.getSpriteStorage()->debugFont, {10, 230});
    this->miniMap = new CubeMapMiniMap(game, gameState, render, this);
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
    return gameState->getDrawableGameRect();
}

Rect CubeMap::playerDrawPosition() {
    CubeMapSide *side = getCurrentSide();
    auto origSize = side->getFieldSize(getDrawableRect());
    auto size = origSize * 0.8;
    auto gridOffset = getDrawableRect() + ((origSize - size) / 2);
    auto screenGridPos = getCurrentSide()->cubePositionToScreenPosition(diceData, playerPos);
    return {origSize.x * screenGridPos.x + gridOffset.x, origSize.y * screenGridPos.y + gridOffset.y, size.x, size.y};
}

void CubeMap::saveCurrentFrame() {
    Texture *currTarget = SDL_GetRenderTarget(render);
    if (oldSideFrameSize != game.getCurrentRenderTargetSize() || oldSideFrame == NULL) {
        if (oldSideFrame != NULL) SDL_DestroyTexture(oldSideFrame);
        Point targetSize = game.getCurrentRenderTargetSize();
        oldSideFrame = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, targetSize.x, targetSize.y);
        oldSideFrameSize = game.getCurrentRenderTargetSize();
    }
    SDL_SetRenderTarget(render, oldSideFrame);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    SDL_SetTextureBlendMode(oldSideFrame, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(render, currTarget, NULL, NULL);
    SDL_SetRenderTarget(render, currTarget);
}
