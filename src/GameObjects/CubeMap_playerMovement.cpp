#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"

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