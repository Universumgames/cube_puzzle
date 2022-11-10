//
// Created by Tom Arlt on 02.11.22.
//

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

void CubeMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    minimapText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
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

bool CubeMap::movePlayer(PlayerMoveDirection dir) {
    PlayerMoveDirection normalizedDirection = screenDirectionToDirectionOnCubeSide(dir);

    Point moveDir = {};
    switch (normalizedDirection) {
        case PlayerMoveDirection::UP:
            moveDir += 1_up;
            break;
        case PlayerMoveDirection::DOWN:
            moveDir += 1_down;
            break;
        case PlayerMoveDirection::LEFT:
            moveDir += 1_left;
            break;
        case PlayerMoveDirection::RIGHT:
            moveDir += 1_right;
            break;
    }
    Point newPlayerPos = playerPos + moveDir;
    bool edge = checkCubeSideEdgeOverstepping(newPlayerPos);
    if (edge) {
        updateMinimap();
    }
    if (!getCurrentSide()->getField(newPlayerPos)->isPlayerMovableTo()) return false;
    playerPos = newPlayerPos;
    return true;
}

void CubeMap::moveCubeInWorld(DiceRollDirection rollDirection) {
    switch (rollDirection) {
        case DiceRollDirection::NORTH:
            worldMap->cubePos += 1_up;
            break;
        case DiceRollDirection::SOUTH:
            worldMap->cubePos += 1_down;
            break;
        case DiceRollDirection::WEST:
            worldMap->cubePos += 1_left;
            break;
        case DiceRollDirection::EAST:
            worldMap->cubePos += 1_right;
            break;
    }
    diceData.rotate(rollDirection);
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

PlayerMoveDirection CubeMap::screenDirectionToDirectionOnCubeSide(PlayerMoveDirection direction) {
    DiceFaceDirection faceDirection = diceData.getDiceSideRotation(currentSideId);
    switch (faceDirection) {
        case DiceFaceDirection::UP:
            return direction;
        case DiceFaceDirection::DOWN:
            switch (direction) {
                case PlayerMoveDirection::UP:
                    return PlayerMoveDirection::DOWN;
                case PlayerMoveDirection::DOWN:
                    return PlayerMoveDirection::UP;
                case PlayerMoveDirection::LEFT:
                    return PlayerMoveDirection::RIGHT;
                case PlayerMoveDirection::RIGHT:
                    return PlayerMoveDirection::LEFT;
            }
        case DiceFaceDirection::LEFT:
            switch (direction) {
                case PlayerMoveDirection::UP:
                    return PlayerMoveDirection::RIGHT;
                case PlayerMoveDirection::DOWN:
                    return PlayerMoveDirection::LEFT;
                case PlayerMoveDirection::LEFT:
                    return PlayerMoveDirection::UP;
                case PlayerMoveDirection::RIGHT:
                    return PlayerMoveDirection::DOWN;
            }
        case DiceFaceDirection::RIGHT:
            switch (direction) {
                case PlayerMoveDirection::UP:
                    return PlayerMoveDirection::LEFT;
                case PlayerMoveDirection::DOWN:
                    return PlayerMoveDirection::RIGHT;
                case PlayerMoveDirection::LEFT:
                    return PlayerMoveDirection::DOWN;
                case PlayerMoveDirection::RIGHT:
                    return PlayerMoveDirection::UP;
            }
    }
    return PlayerMoveDirection::LEFT;
}

bool CubeMap::checkCubeSideEdgeOverstepping(Point &playerPos) {
    // done: implement edge overstepping
    // done: change current Side
    // done: rotate cube
    // TODO move player to correct location
    auto *side = getSide(currentSideId);
    DiceFaceDirection oldSideOrientation = diceData.getDiceSideRotation(currentSideId);
    DiceSide oldSide = diceData.getSideFacing(currentSideId);
    if (playerPos.x < 0) {
        // TODO move cube missing
        switch (oldSideOrientation) {
            case DiceFaceDirection::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::LEFT);
                break;
            case DiceFaceDirection::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::RIGHT);
                break;
            case DiceFaceDirection::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
            case DiceFaceDirection::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
        }
        playerPos = {2, 2};
        return true;
    } else if (playerPos.x >= side->width) {
        switch (oldSideOrientation) {
            case DiceFaceDirection::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::RIGHT);
                break;
            case DiceFaceDirection::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::LEFT);
                break;
            case DiceFaceDirection::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
            case DiceFaceDirection::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
        }
        playerPos = {2, 2};
        return true;
    } else if (playerPos.y < 0) {
        auto oldFacing = diceData.getSideFacing(currentSideId);
        auto oldOrientation = diceData.getDiceSideRotation(currentSideId);
        // TODO move cube missing
        switch (oldSideOrientation) {
            case DiceFaceDirection::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
            case DiceFaceDirection::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
            case DiceFaceDirection::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::LEFT);
                break;
            case DiceFaceDirection::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::RIGHT);
                break;
        }
        playerPos = {2, 2};
        return true;
    } else if (playerPos.y >= side->height) {
        auto oldFacing = diceData.getSideFacing(currentSideId);
        // TODO move cube missing
        //moveCubeInWorld(diceSideToRollDir(oldFacing));
        switch (oldSideOrientation) {
            case DiceFaceDirection::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
            case DiceFaceDirection::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
            case DiceFaceDirection::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::RIGHT);
                break;
            case DiceFaceDirection::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::LEFT);
                break;
        }
        playerPos = {2, 2};
        return true;
    }
    return false;
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