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

    minimapText->changePosition({game.getWindowSize().x - 80, 0});
    int n = 0, w = 0, c = 0, e = 0, s = 0, b = 0;
    c = currentSideId;
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    std::string text = "   " + std::to_string(n) + "   \n" +
                       std::to_string(w) + " " + std::to_string(c) + " " + std::to_string(e) + "\n" +
                       "   " + std::to_string(s) + "   \n" +
                       "   " + std::to_string(b) + "   ";
    minimapText->changeText(text);
}

void CubeMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
    if (game.isDebug())
        debugSideIndicator->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    minimapText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMap::drawMap(const u32 frame, const u32 totalMSec, const float deltaT) {
    for (auto side: sides) {
        side->Render(game, render, diceData, BASIC_GO_DATA_PASSTHROUGH);
    }
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
    this->debugSideIndicator = new Text(game, render, 400, "", game1.getSpriteStorage()->basicFont, {0, 40});
    this->minimapText = new Text(game, render, 400, "", game.getSpriteStorage()->basicFont, {0, 0});
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
    // move dir relative to screen not world
    auto tmp = 0;
    switch (diceData.getDiceSideRotation(currentSideId)) {
        case DiceFaceDirection::UP:
            break;
        case DiceFaceDirection::DOWN:
            moveDir = moveDir * -1;
            break;
        case DiceFaceDirection::LEFT:
            tmp = moveDir.x;
            moveDir.x = moveDir.y;
            moveDir.y = tmp * -1;
            break;
        case DiceFaceDirection::RIGHT:
            tmp = moveDir.x;
            moveDir.x = moveDir.y * -1;
            moveDir.y = tmp;
            break;
    }
    Point newPlayerPos = playerPos + moveDir;
    bool edge = checkCubeSideEdgeOverstepping(newPlayerPos);
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
    // TODO implement edge overstepping
    // done: change current Side
    // done: rotate cube
    // TODO move player to correct location
    auto *side = getSide(currentSideId);
    if (playerPos.x < 0) {
        currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::LEFT);
        playerPos = {2, 2};
        return true;
    } else if (playerPos.x >= side->width) {
        currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::RIGHT);
        playerPos = {2, 2};
        return true;
    } else if (playerPos.y < 0) {
        auto oldFacing = diceData.getSideFacing(currentSideId);
        currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::UP);
        moveCubeInWorld(diceSideToRollDir(oldFacing));
        playerPos = {2, 2};
        return true;
    } else if (playerPos.y >= side->height) {
        auto oldFacing = diceData.getSideFacing(currentSideId);
        currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceFaceDirection::DOWN);
        moveCubeInWorld(diceSideToRollDir(oldFacing));
        playerPos = {2, 2};
        return true;
    }
    return false;
}

Rect CubeMap::playerDrawPosition() {
    CubeMapSide *side = getCurrentSide();
    auto size = side->getFieldSize(game.getWindowSize());
    auto offset = side->getStartingOffset(game.getWindowSize(), size);
    return {size.x * playerPos.x + offset.x, size.y * playerPos.y + offset.y, size.x, size.y};
}

CubeMapSide *CubeMap::getCurrentSide() {
    return getSide(currentSideId);
}

Point CubeMapSide::cubePositionToScreenPosition(DiceData diceData, Point cubePos) const {
    DiceFaceDirection faceDirection = diceData.getDiceSideRotation(sideID);
    Point res = {};
    switch (faceDirection) {
        case DiceFaceDirection::UP:
            return cubePos;
        case DiceFaceDirection::DOWN:
            return {this->width - cubePos.x - 1, this->height - cubePos.y - 1};
        case DiceFaceDirection::LEFT:
            return {cubePos.y, this->width - cubePos.x - 1};
        case DiceFaceDirection::RIGHT:
            return {this->height - cubePos.y - 1, cubePos.x};
    }
    return res;
}

void CubeMapSide::HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    for (auto *field: side) {
        field->HandleEvent(game, BASIC_GO_DATA_PASSTHROUGH, event);
    }
}

void CubeMapSide::Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) {
    for (auto *field: side) {
        field->Update(game, BASIC_GO_DATA_PASSTHROUGH);
    }
}

void CubeMapSide::Render(CubeGame &game, Renderer *render, DiceData diceData, const u32 frame, const u32 totalMSec,
                         const float deltaT) {
    int x = 0, y = 0;
    Point size = getFieldSize(game.getWindowSize());
    Point offset = getStartingOffset(game.getWindowSize(), size);
    for (auto *field: side) {
        field->Render(game, render, size, {size.x * x + offset.x, size.y * y + offset.y}, BASIC_GO_DATA_PASSTHROUGH);
        x++;
        if (x >= width) {
            y++;
            x = 0;
        }
    }
}

Point CubeMapSide::getFieldSize(Point windowSize) {
    int w = min(windowSize.x, windowSize.y) / width;
    return {w, w};
}

Point CubeMapSide::getStartingOffset(Point windowSize, Point fieldSize) {
    Point center = windowSize / 2;
    Point totalSize = {width * fieldSize.x, height * fieldSize.y};
    return center - (totalSize / 2);
}
