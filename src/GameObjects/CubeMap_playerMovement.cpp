#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "../matrix.hpp"
#include "CubeMapMiniMap.hpp"
#include "../States/Level.hpp"

bool CubeMap::playerCanMove() {
    return !sideTransitionAnimating;
}

bool CubeMap::movePlayer(PlayerMoveDirection dir) {
    if (!playerCanMove()) return false;
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
    Point newPlayerPos = this->playerPos + moveDir;
    int oldSideId = currentSideId;
    bool moved = rotateCubeIfNecessary(newPlayerPos, dir);
    if (moved)
        sideTransitionAnimating = true;

    if (!getCurrentSide()->getField(newPlayerPos)->canPlayerEnter()) {
        currentSideId = oldSideId;
        return false;
    }
    this->playerPos = newPlayerPos;
    doLevelFinishedLogic();
    return true;
}

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

/// if player overstepped an edge: sets the new cubeFields, rotates the cube, and sets the new player position
bool CubeMap::rotateCubeIfNecessary(Point &newPlayerPos, PlayerMoveDirection moveDirection) {
    auto *side = getSide(this->currentSideId);
    DiceSideRotation oldSideOrientation = diceData.getDiceSideRotation(this->currentSideId);
    int oldSideId = this->currentSideId;
    DiceSide oldSide = this->diceData.getSideFacing(this->currentSideId);
    Point oldPlayerPos = this->playerPos;
    bool rollDice = false;
    DiceRollDirection diceRollDirection;
    if (newPlayerPos.x < 0) { // move left out of cubeFields
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::LEFT);
                break;
            case DiceSideRotation::DOWN:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::RIGHT);
                break;
            case DiceSideRotation::LEFT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::DOWN);
                rollDice = true;
                diceRollDirection = getOppositeDiceRollDirection(sideToRollDirection(oldSide));
                break;
            case DiceSideRotation::RIGHT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId, DiceSideRotation::UP);
                rollDice = true;
                diceRollDirection = sideToRollDirection(oldSide);
                break;
        }
    } else if (newPlayerPos.x >= side->width) { // move right out of cubeFields
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::RIGHT);
                break;
            case DiceSideRotation::DOWN:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::LEFT);
                break;
            case DiceSideRotation::LEFT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId, DiceSideRotation::UP);
                rollDice = true;
                diceRollDirection = sideToRollDirection(oldSide);
                break;
            case DiceSideRotation::RIGHT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::DOWN);
                rollDice = true;
                diceRollDirection = getOppositeDiceRollDirection(sideToRollDirection(oldSide));
                break;
        }
    } else if (newPlayerPos.y < 0) { // move up out of cubeFields
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId, DiceSideRotation::UP);
                rollDice = true;
                diceRollDirection = sideToRollDirection(oldSide);
                break;
            case DiceSideRotation::DOWN:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::DOWN);
                rollDice = true;
                diceRollDirection = getOppositeDiceRollDirection(sideToRollDirection(oldSide));
                break;
            case DiceSideRotation::LEFT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::LEFT);
                break;
            case DiceSideRotation::RIGHT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::RIGHT);
                break;
        }
    } else if (newPlayerPos.y >= side->height) { // move down out of cubeFields
        switch (oldSideOrientation) {
            case DiceSideRotation::UP:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::DOWN);
                rollDice = true;
                diceRollDirection = getOppositeDiceRollDirection(sideToRollDirection(oldSide));
                break;
            case DiceSideRotation::DOWN:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId, DiceSideRotation::UP);
                rollDice = true;
                diceRollDirection = sideToRollDirection(oldSide);
                break;
            case DiceSideRotation::LEFT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::RIGHT);
                break;
            case DiceSideRotation::RIGHT:
                this->currentSideId = this->diceData.getSideWhenMovingInDirX(this->currentSideId,
                                                                             DiceSideRotation::LEFT);
                break;
        }
    }

    // reposition player if cube cubeFields transition took place
    if (oldSideId != this->currentSideId) {
        this->lastNormalizedMove = moveDirection;
        int maxRow = getCurrentSide()->height - 1;
        int maxCol = getCurrentSide()->width - 1;

        if (checkCubeSideTransition(1, 2, oldSideId) || checkCubeSideTransition(2, 6, oldSideId)) {
            newPlayerPos.x = oldPlayerPos.x;
            newPlayerPos.y = maxRow - oldPlayerPos.y;
        } else if (checkCubeSideTransition(1, 4, oldSideId) || checkCubeSideTransition(3, 6, oldSideId)) {
            newPlayerPos.x = oldPlayerPos.y;
            newPlayerPos.y = oldPlayerPos.x;
        } else if (checkCubeSideTransition(1, 3, oldSideId) || checkCubeSideTransition(4, 6, oldSideId)) {
            newPlayerPos.x = maxCol - oldPlayerPos.y;
            newPlayerPos.y = maxRow - oldPlayerPos.x;
        } else {
            newPlayerPos.x = maxCol - oldPlayerPos.x;
            newPlayerPos.y = oldPlayerPos.y;
        }
    }

    // check if cubeFields transition is allowed
    if (getCurrentSide()->getField(newPlayerPos)->canPlayerEnter()) {
        if (rollDice) {
            moveCubeInWorld(diceRollDirection);
        }
    } else if (oldSideId != this->currentSideId) { // if not, rollback
        this->currentSideId = oldSideId;
        newPlayerPos.x = oldPlayerPos.x;
        newPlayerPos.y = oldPlayerPos.y;
    }

    return oldSideId != this->currentSideId;
}

/// checks if the player transitioned from cubeFields A to cubeFields B or the other way around
bool CubeMap::checkCubeSideTransition(int sideAId, int sideBId, int oldSideId) const {
    return ((sideAId == oldSideId && sideBId == this->currentSideId) ||
            (sideBId == oldSideId && sideAId == this->currentSideId));
}

void CubeMap::doLevelFinishedLogic() {
    CubeMapSide *currentSide = getCurrentSide();
    CubeField *currentCubeField = currentSide->getField(this->playerPos);
    if (currentCubeField->isLevelFinishedIfEntered()) {
        reinterpret_cast<Level*>(this->gameState)->returnToLevelSelector(ExitState::FINISHED);
        this->playerPos = {0, 0};
        this->currentSideId = 2;
    }
}