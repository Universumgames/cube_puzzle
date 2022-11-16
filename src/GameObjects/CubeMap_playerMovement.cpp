#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "../matrix.hpp"

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
    worldMap->fixCubePosOutBounds();
    diceData.rotate(rollDirection);
}

PlayerMoveDirection CubeMap::screenDirectionToDirectionOnCubeSide(PlayerMoveDirection direction) {
    DiceSideRotation faceDirection = diceData.getDiceSideRotation(currentSideId);
    switch (faceDirection) {
        case DiceSideRotation::UP:
            return direction;
        case DiceSideRotation::DOWN:
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
        case DiceSideRotation::LEFT:
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
        case DiceSideRotation::RIGHT:
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
    DiceSideRotation oldSideOrientation = diceData.getDiceSideRotation(currentSideId);
    int oldSideId = currentSideId;
    DiceSide oldSide = diceData.getSideFacing(currentSideId);
    if (playerPos.x < 0) { // move left out of side
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::LEFT);
                break;
            case DiceSideRotation::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::RIGHT);
                break;
            case DiceSideRotation::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
            case DiceSideRotation::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
        }
    } else if (playerPos.x >= side->width) { // move right out of side
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::RIGHT);
                break;
            case DiceSideRotation::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::LEFT);
                break;
            case DiceSideRotation::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
            case DiceSideRotation::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
        }
    } else if (playerPos.y < 0) { // move up out of side
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
            case DiceSideRotation::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
            case DiceSideRotation::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::LEFT);
                break;
            case DiceSideRotation::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::RIGHT);
                break;
        }
    } else if (playerPos.y >= side->height) { // move down out of side
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::DOWN);
                moveCubeInWorld(getOppositeDiceRollDirection(sideToRollDirection(oldSide)));
                break;
            case DiceSideRotation::DOWN:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::UP);
                moveCubeInWorld(sideToRollDirection(oldSide));
                break;
            case DiceSideRotation::LEFT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::RIGHT);
                break;
            case DiceSideRotation::RIGHT:
                currentSideId = diceData.getSideWhenMovingInDirX(currentSideId, DiceSideRotation::LEFT);
                break;
        }
    }

    // reposition player
    if (oldSideId != currentSideId) { // 1 bis 6, jeweils bezüglich der Augenzahl auf der Würfelseite
        int maxRow = getCurrentSide()->height - 1; // MAX_ROW_INDEX
        int maxCol = getCurrentSide()->width - 1; // MAX_COLUMN_INDEX
        playerPos = {1, 1};

        // TODO Mina, hier kommt Code hin
    }

    return oldSideId != currentSideId;
}