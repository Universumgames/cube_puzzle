#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "CubeMapMiniMap.hpp"
#include "../States/Level.hpp"

// ################################# File-internal Helper-Function ###################################################################

void updateAnimationSidePosition(Rect& oldSide, Rect& newSide, double transitionState, PlayerMoveDirection moveDir);

// ################################# Konstruktoren ###################################################################################

CubeMap::CubeMap(CubeGame &game1, ComplexGameState *gameState, SDL_Renderer *render1,
                 const Vector<CubeMapSide *> &sides, int startSide,
                 Point playerPos)
        : GameObject(game1, gameState, render1) {
    this->sides = sides;
    this->currentSideId = startSide;
    this->playerPos = playerPos;
    this->debugSideIndicator = new Text(game, gameState, render, 400, "", game1.getSpriteStorage()->debugFont,
                                        {10, 30});
    this->minimapText = new Text(game, gameState, render, 400, "", game.getSpriteStorage()->debugFont, {10, 60});
    this->debugDiceData = new Text(game, gameState, render, 400, "", game.getSpriteStorage()->debugFont, {10, 230});
    this->miniMap = new CubeMapMiniMap(game, gameState, render, this);
    
    for(auto side : sides){
        side->setDiceData(&diceData);
        side->setCubeMapRef(this);
    }
}

// ################################# Alle Render-Methoden ############################################################################

void CubeMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    renderTarget = SDL_GetRenderTarget(render);
    drawMap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    debugSideIndicator->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    debugDiceData->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    miniMap->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

// ################################# HandleEvent, Update- und Init-Methoden ##########################################################

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
    if (!isSideTransitionAnimationInProgress) {
        saveCurrentFrame();
        sideTransitionState = 0;
    }
}

void CubeMap::Init() {
    GameObject::Init();
}

// ################################# Setter & Getter #################################################################################
// ####### public #######

void CubeMap::SetWorldMap(WorldMap *world_map) {
    this->worldMap = world_map;
}

void CubeMap::setIsAnimating(bool isAnimating) {
    this->isObjectAnimationInProgress = isAnimating;
}

Rect CubeMap::getPlayerDrawPosition() {
    CubeMapSide *side = getCurrentSide();
    auto origSize = side->getFieldSize(getDrawableRect());
    auto size = origSize * 0.8;
    auto gridOffset = getDrawableRect() + ((origSize - size) / 2);
    auto screenGridPos = getCurrentSide()->cubePositionToScreenPosition(playerPos);
    return {origSize.x * screenGridPos.x + gridOffset.x, origSize.y * screenGridPos.y + gridOffset.y, size.x, size.y};
}

CubeMapSide *CubeMap::getCurrentSide() {
    return getSide(this->currentSideId);
}

Vector<CubeMapSide*>* CubeMap::getAllCubeMapSides() {
    return &(this->sides);
}

Point CubeMap::getCurrentPlayerPos() const {
    return this->playerPos;
}

Vector<Magnet*> CubeMap::getAllNeighboringMagnets() {
    CubeMapSide* currentSide = this->getCurrentSide();
    Vector<Magnet*> listMagnets = currentSide->getAllMagnetsSurroundingPlayer(this->playerPos.x, this->playerPos.y);
    return listMagnets;
}

// ####### private ######

CubeMapSide *CubeMap::getSide(int i) {
    return sides[i - 1];
}

CubeField *CubeMap::getField(int side, int x, int y) {
    return getSide(side)->getField(x, y);
}

CubeField *CubeMap::getField(int side, Point p) {
    return getSide(side)->getField(p.x, p.y);
}

/// gibt den Spielbereich zurück (ohne den schwarzen Rand drumrum)
Rect CubeMap::getDrawableRect() {
    return gameState->getDrawableGameRect();
}

// ################################# sonstige Methoden ###############################################################################
// ####### public #######

bool CubeMap::isAnimating() const {
    return isSideTransitionAnimationInProgress;
}

// ####### private ######

void CubeMap::moveCubeInWorld(DiceRollDirection rollDirection) {
    switch (rollDirection) {
        case DiceRollDirection::NORTH:
            this->worldMap->cubePos += 1_up;
            break;
        case DiceRollDirection::SOUTH:
            this->worldMap->cubePos += 1_down;
            break;
        case DiceRollDirection::WEST:
            this->worldMap->cubePos += 1_left;
            break;
        case DiceRollDirection::EAST:
            this->worldMap->cubePos += 1_right;
            break;
    }
    worldMap->fixCubePosOutBounds();
    diceData.rotate(rollDirection);
}

void CubeMap::drawMap(const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect drawableRect = getDrawableRect();
    if (isSideTransitionAnimationInProgress) {
        Rect oldSide = {0,0, game.getWindowSize().x, game.getWindowSize().y};
        Rect newSide = getDrawableRect();
        updateAnimationSidePosition(oldSide, newSide, sideTransitionState, lastNormalizedMove);
        SDL_RenderCopy(render, oldSideFrame, nullptr, &oldSide);
        sideTransitionState += deltaT * 10;
        if (sideTransitionState >= 1) {
            sideTransitionState = 0;
            isSideTransitionAnimationInProgress = false;
        }
        getCurrentSide()->Render(game, gameState, render, BASIC_GO_DATA_PASSTHROUGH, newSide);
    } else
        getCurrentSide()->Render(game, gameState, render, BASIC_GO_DATA_PASSTHROUGH, drawableRect);
}

void CubeMap::saveCurrentFrame() {
    Texture *currTarget = renderTarget;
    if (oldSideFrameSize != game.getCurrentRenderTargetSize() || oldSideFrame == nullptr) {
        if (oldSideFrame != nullptr) SDL_DestroyTexture(oldSideFrame);
        Point targetSize = game.getCurrentRenderTargetSize();
        oldSideFrame = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, targetSize.x, targetSize.y);
        oldSideFrameSize = game.getCurrentRenderTargetSize();
    }
    SDL_SetRenderTarget(render, oldSideFrame);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    SDL_SetTextureBlendMode(oldSideFrame, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(render, currTarget, nullptr, nullptr);
    SDL_SetRenderTarget(render, currTarget);
}

// ################################# File-internal Helper-Function ###################################################################

void updateAnimationSidePosition(Rect& oldSide, Rect& newSide, double transitionState, PlayerMoveDirection moveDir) {
    switch(moveDir) {
        case PlayerMoveDirection::UP:
            newSide.y -= newSide.h * (1-transitionState);
            oldSide.y += newSide.h * transitionState;
            break;
        case PlayerMoveDirection::DOWN:
            newSide.y += newSide.h * (1-transitionState);
            oldSide.y -= newSide.h * transitionState;
            break;
        case PlayerMoveDirection::LEFT:
            newSide.x -= newSide.w * (1-transitionState);
            oldSide.x += newSide.w * transitionState;
            break;
        case PlayerMoveDirection::RIGHT:
            newSide.x += newSide.w * (1-transitionState);
            oldSide.x -= newSide.w * transitionState;
            break;
    }
}