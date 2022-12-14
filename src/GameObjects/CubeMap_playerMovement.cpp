#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "CubeMapMiniMap.hpp"
#include "../States/Level.hpp"

bool CubeMap::canPlayerMove() const {
    return !isSideTransitionAnimationInProgress && !isObjectAnimationInProgress;
}

bool CubeMap::movePlayer(PlayerMoveDirection dir, const Vector<Magnet*>& listGrabbedMagnets) {
    if (!canPlayerMove()) {
        return false;
    }
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
        isSideTransitionAnimationInProgress = true;
    if (!getCurrentSide()->getField(newPlayerPos)->canPlayerEnter()) {
        currentSideId = oldSideId;
        return false;
    }
    bool rollbackPlayerMovement = false;
    bool rollbackMagnets = moved;
    if (newPlayerPos.x != this->playerPos.x || newPlayerPos.y != this->playerPos.y) {
        if (!getCurrentSide()->getField(newPlayerPos)->canPlayerEnter()) {
            rollbackPlayerMovement = true;
            rollbackMagnets = true;
        }
        if (!rollbackMagnets) {
            if (newPlayerPos.x > this->playerPos.x) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToBigX, false, this->playerPos);
                }
            } else if (newPlayerPos.x < this->playerPos.x) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToSmallX, false, this->playerPos);
                }
            } else if (newPlayerPos.y > this->playerPos.y) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToBigY, false, this->playerPos);
                }
            } else if (newPlayerPos.y < this->playerPos.y) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToSmallY, false, this->playerPos);
                }
            }
        }
        if (!rollbackPlayerMovement && getCurrentSide()->getField(newPlayerPos)->canPlayerEnter()) {
            this->playerPos = newPlayerPos;
            doLevelFinishedLogic();
            return true;
        } else { // rollback Magnets
            if (newPlayerPos.x > this->playerPos.x) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToSmallX, true, this->playerPos);
                }
            } else if (newPlayerPos.x < this->playerPos.x) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToBigX, true, this->playerPos);
                }
            } else if (newPlayerPos.y > this->playerPos.y) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToSmallY, true, this->playerPos);
                }
            } else if (newPlayerPos.y < this->playerPos.y) {
                for (auto magnet : listGrabbedMagnets) {
                    magnet->move(MovementDirection::moveToBigY, true, this->playerPos);
                }
            }
        }
    }
    return false;
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
            break;
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
            break;
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
            break;
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
    auto currentSide = getCurrentSide();
    auto oldCubeMapSide = getSide(oldSideId);
    auto newField = currentSide->getField(newPlayerPos);
    auto oldField = oldCubeMapSide->getField(oldPlayerPos);
    if (newField->canPlayerEnter()) {
        oldField->leave();
        newField->enter();
        currentSide->setAllSlidersInMotion();
        if (rollDice) {
            moveCubeInWorld(diceRollDirection);
        }
        if (oldSideId != this->currentSideId) {
            DiceSideRotation rotation = diceData.getDiceSideRotation(this->currentSideId);
            switch (rotation) {
                case DiceSideRotation::UP:
                    this->getCurrentSide()->setGravityDirection(MovementDirection::moveToBigY);
                    break;
                case DiceSideRotation::DOWN:
                    this->getCurrentSide()->setGravityDirection(MovementDirection::moveToSmallY);
                    break;
                case DiceSideRotation::LEFT:
                    this->getCurrentSide()->setGravityDirection(MovementDirection::moveToSmallX);
                    break;
                case DiceSideRotation::RIGHT:
                    this->getCurrentSide()->setGravityDirection(MovementDirection::moveToBigX);
                    break;
                default:
                    break;
            }
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
        CubeMap::getFinishedJingle()->playOnce();
        gameState->returnToLevelSelector(ExitState::FINISHED);
    }
}