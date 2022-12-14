#include "CubeMap.hpp"
#include "WorldMap.hpp"
#include "CubeField.hpp"
#include "../recthelper.hpp"
#include "CubeMapMiniMap.hpp"
#include "../States/Level.hpp"

bool CubeMap::canPlayerMove() const {
    return !this->isSideTransitionAnimationInProgress && !this->isObjectAnimationInProgress;
}

bool CubeMap::movePlayer(PlayerMoveDirection dir, const Vector<Magnet*>& listGrabbedMagnets) {
    if (!canPlayerMove()) {
        return false;
    }
    PlayerMoveDirection normalizedDirection = screenDirectionToDirectionOnCubeSide(dir);

    Point moveDirection = {};
    switch (normalizedDirection) {
        case PlayerMoveDirection::UP:
            moveDirection += 1_up;
            break;
        case PlayerMoveDirection::DOWN:
            moveDirection += 1_down;
            break;
        case PlayerMoveDirection::LEFT:
            moveDirection += 1_left;
            break;
        case PlayerMoveDirection::RIGHT:
            moveDirection += 1_right;
            break;
    }
    Point newPlayerPos = this->playerPos + moveDirection;
    int oldSideId = this->currentSideId;
    DiceRollDirection diceRollDirection;
    bool isCubeRotationNecessary = this->isCubeRotationNecessary(newPlayerPos, dir, diceRollDirection);
    bool isCubeSideTransitionNecessary = this->currentSideId != oldSideId;
    if (isCubeSideTransitionNecessary) {
        dropAllMagnets(listGrabbedMagnets);
    }
    auto currentSide = getCurrentSide();
    auto oldCubeMapSide = getSide(oldSideId);
    auto newField = currentSide->getField(newPlayerPos);
    auto oldField = oldCubeMapSide->getField(this->playerPos);
    if (newField->canPlayerEnter()) {
        oldField->leave();
        newField->enter();
        currentSide->setAllSlidersInMotion();
        if (isCubeRotationNecessary) {
            moveCubeInWorld(diceRollDirection);
        }
        if (isCubeSideTransitionNecessary) {
            this->isSideTransitionAnimationInProgress = true;
            setGravityOnNewCubeMapSide();
        }
    } else { // if player cannot enter new field, rollback all the changes made
        this->currentSideId = oldSideId;
        return false;
    }

    if (newPlayerPos.x > this->playerPos.x) {
        for (auto magnet : listGrabbedMagnets) {
            magnet->move(MovementDirection::moveToBigX);
        }
    } else if (newPlayerPos.x < this->playerPos.x) {
        for (auto magnet : listGrabbedMagnets) {
            magnet->move(MovementDirection::moveToSmallX);
        }
    } else if (newPlayerPos.y > this->playerPos.y) {
        for (auto magnet : listGrabbedMagnets) {
            magnet->move(MovementDirection::moveToBigY);
        }
    } else if (newPlayerPos.y < this->playerPos.y) {
        for (auto magnet : listGrabbedMagnets) {
            magnet->move(MovementDirection::moveToSmallY);
        }
    }

    this->playerPos = newPlayerPos;
    doLevelFinishedLogic();
    return true;
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
bool CubeMap::isCubeRotationNecessary(Point& newPlayerPos, PlayerMoveDirection moveDirection, DiceRollDirection& diceRollDirection) {
    auto *side = getSide(this->currentSideId);
    DiceSideRotation oldSideOrientation = diceData.getDiceSideRotation(this->currentSideId);
    int oldSideId = this->currentSideId;
    DiceSide oldSide = this->diceData.getSideFacing(this->currentSideId);
    Point oldPlayerPos = this->playerPos;
    bool rollDice = false;
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

    if (oldSideId == this->currentSideId) { // we do not need to change the CubeMapSide
        return false;
    }

    // correct newPlayerPos if cube cubeFields transition took place
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
    return rollDice;
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

void CubeMap::dropAllMagnets(const Vector<Magnet*>& listGrabbedMagnets) {
    for (auto magnet : listGrabbedMagnets) {
        magnet->setIsGrabbed(false);
    }
}

void CubeMap::setGravityOnNewCubeMapSide() {
    DiceSideRotation rotation = this->diceData.getDiceSideRotation(this->currentSideId);
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