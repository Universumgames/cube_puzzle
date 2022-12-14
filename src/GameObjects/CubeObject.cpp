#include "CubeObject.hpp"
#include <utility>

// ################################# Konstruktoren ###################################################################################

Stone::Stone() {
    this->fallingDirection = MovementDirection::none;
    this->currentMovementDirection = this->fallingDirection;
    this->type = CubeObject::ObjectType::typeStone;
}

Slider::Slider(MovementDirection movementDirectionIfActivated, int id, bool activated) {
    this->directionIfActivated = movementDirectionIfActivated;
    this->id = id;
    this->isActivated = activated;
    if (this->isActivated) {
        this->currentMovementDirection = movementDirectionIfActivated;
    } else {
        this->currentMovementDirection = getOppositeMovementDirection(movementDirectionIfActivated);
    }
    this->type = CubeObject::ObjectType::typeSlider;
    this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
}

Magnet::Magnet() {
    this->type = CubeObject::ObjectType::typeMagnet;
}

// ################################# HandleEvent und Update-Methoden #################################################################

void CubeObject::Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) {
    CubeField *cubeField = this->cubeFieldRef;
    int oldX = cubeField->getX();
    int oldY = cubeField->getY();
    int newX = oldX;
    int newY = oldY;
    CubeMapSide *cubeMapSide = cubeField->getCubeMapSideRef();
    int height = cubeMapSide->height;
    int width = cubeMapSide->width;
    switch (this->currentMovementDirection) {
        case MovementDirection::moveToBigX:
            if (oldX < width - 1) {
                newX = oldX + 1;
            }
            break;
        case MovementDirection::moveToSmallX:
            if (oldX > 0) {
                newX = oldX - 1;
            }
            break;
        case MovementDirection::moveToBigY:
            if (oldY < height - 1) {
                newY = oldY + 1;
            }
            break;
        case MovementDirection::moveToSmallY:
            if (oldY > 0) {
                newY = oldY - 1;
            }
            break;
        default:
            break;
    }
    if (oldX != newX || oldY != newY) {
        if (cubeMapSide->canObjectEnterFieldAt(this, newX, newY)) {
            this->lastMovementCountdown = std::max(0.0, static_cast<double>(this->lastMovementCountdown)) - deltaT;
            if (this->lastMovementCountdown > 0) {
                return;
            }
            this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
            cubeMapSide->getCubeMapRef()->setIsAnimating(true);
            auto newField = cubeMapSide->getField(newX, newY);
            if (cubeField->removeObject(this)) {
                newField->addObject(this);
                this->cubeFieldRef = newField;
            }
        } else { // Slider or Stone reached the end
            cubeMapSide->getCubeMapRef()->setIsAnimating(false);
            this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
        }
    } else { // Slider or Stone reached the end
        cubeMapSide->getCubeMapRef()->setIsAnimating(false);
        this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
    }
}

void CubeObject::HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
}

// ################################# Setter & Getter #################################################################################

void CubeObject::setDiceData(DiceData *dice_data) {
    this->diceData = dice_data;
}

void CubeObject::setSideId(int sideID) {
    this->sideId = sideID;
}

void CubeObject::setCubeFieldRef(CubeField *cube_field) {
    this->cubeFieldRef = cube_field;
}

CubeObject::ObjectType CubeObject::getType() {
    return this->type;
}

void Stone::setFallingDirection(MovementDirection dir) {
    this->fallingDirection = dir;
    this->currentMovementDirection = this->fallingDirection;
}

CubeObject::ObjectType Stone::getType() {
    return this->type;
}

int Slider::getId() const {
    return this->id;
}

void Magnet::setIsGrabbed(bool is_grabbed) {
    this->isGrabbed = is_grabbed;
}

bool Magnet::getIsGrabbed() const {
    return this->isGrabbed;
}

// ################################# can...Enter-Methoden ############################################################################

bool CubeObject::canPlayerEnter() {
    return false;
}

bool CubeObject::canAnotherObjectEnter() {
    return false;
}

bool Stone::canPlayerEnter() {
    return false;
}

bool Stone::canAnotherObjectEnter() {
    return false;
}

bool Slider::canPlayerEnter() {
    return false;
}

bool Slider::canAnotherObjectEnter() {
    return false;
}

bool Magnet::canPlayerEnter() {
    if (this->isGrabbed) {
        CubeField* oldMagnetField = this->cubeFieldRef;
        Point oldMagnetFieldCoordinates = oldMagnetField->getCoordinates();
        CubeField* newMagnetField = nullptr;
        CubeMapSide* currentCubeMapSide = oldMagnetField->getCubeMapSideRef();
        CubeMap* cubeMap = currentCubeMapSide->getCubeMapRef();
        Point oldPlayerPosition = cubeMap->getCurrentPlayerPos();
        if (oldPlayerPosition.x < oldMagnetFieldCoordinates.x) {
            newMagnetField = currentCubeMapSide->getField(oldMagnetFieldCoordinates.x + 1, oldMagnetFieldCoordinates.y);
        } else if (oldPlayerPosition.x > oldMagnetFieldCoordinates.x) {
            newMagnetField = currentCubeMapSide->getField(oldMagnetFieldCoordinates.x - 1, oldMagnetFieldCoordinates.y);
        } else if (oldPlayerPosition.y > oldMagnetFieldCoordinates.y) {
            newMagnetField = currentCubeMapSide->getField(oldMagnetFieldCoordinates.x, oldMagnetFieldCoordinates.y - 1);
        } else if (oldPlayerPosition.y < oldMagnetFieldCoordinates.y) {
            newMagnetField = currentCubeMapSide->getField(oldMagnetFieldCoordinates.x, oldMagnetFieldCoordinates.y + 1);
        }
        if (newMagnetField->canObjectEnter(this)) {
            return true;
        }
    }
    return false;
}

bool Magnet::canAnotherObjectEnter() {
    return false;
}

bool Flag::canPlayerEnter() {
    return true;
}

bool Flag::canAnotherObjectEnter() {
    return false;
}

// ################################# canEnter...-Methoden ############################################################################

bool CubeObject::canEnterPressurePlate() {
    return true;
}

bool Stone::canEnterPressurePlate() {
    return true;
}

bool Slider::canEnterPressurePlate() {
    return true;
}

bool Magnet::canEnterPressurePlate() {
    return true;
}

bool Flag::canEnterPressurePlate() {
    return false;
}

// ################################# canActivatePressurePlate-Methoden ###############################################################

bool CubeObject::canActivatePressurePlate() {
    return true;
}

bool Stone::canActivatePressurePlate() {
    return true;
}

bool Slider::canActivatePressurePlate() {
    return true;
}

bool Magnet::canActivatePressurePlate() {
    return true;
}

bool Flag::canActivatePressurePlate() {
    return false;
}

// ################################# Slider Logic Methoden ###########################################################################

bool CubeObject::isSlider() {
    return false;
}

bool Slider::isSlider() {
    return true;
}

void Slider::activate() {
    this->isActivated = true;
    this->currentMovementDirection = this->directionIfActivated;
}

void Slider::deactivate() {
    this->isActivated = false;
    switch (this->directionIfActivated) {
        case MovementDirection::moveToBigX:
            this->currentMovementDirection = MovementDirection::moveToSmallX;
            break;
        case MovementDirection::moveToSmallX:
            this->currentMovementDirection = MovementDirection::moveToBigX;
            break;
        case MovementDirection::moveToBigY:
            this->currentMovementDirection = MovementDirection::moveToSmallY;
            break;
        case MovementDirection::moveToSmallY:
            this->currentMovementDirection = MovementDirection::moveToBigY;
            break;
        default:
            break;
    }
}

// ################################# Magnet Logic Methoden ###########################################################################

void Magnet::move(MovementDirection direction) {
    CubeField *cubeField = this->cubeFieldRef;
    int oldX = cubeField->getX();
    int oldY = cubeField->getY();
    int newX = oldX;
    int newY = oldY;
    CubeMapSide *cubeMapSide = cubeField->getCubeMapSideRef();
    int height = cubeMapSide->height;
    int width = cubeMapSide->width;
    switch (direction) {
        case MovementDirection::moveToBigX:
            if (oldX < width - 1) {
                newX = oldX + 1;
            }
            break;
        case MovementDirection::moveToSmallX:
            if (oldX > 0) {
                newX = oldX - 1;
            }
            break;
        case MovementDirection::moveToBigY:
            if (oldY < height - 1) {
                newY = oldY + 1;
            }
            break;
        case MovementDirection::moveToSmallY:
            if (oldY > 0) {
                newY = oldY - 1;
            }
            break;
        default:
            break;
    }
    if (oldX == newX && oldY == newY) {
        this->isGrabbed = false;
        return;
    }
    if (!cubeMapSide->canObjectEnterFieldAt(this, newX, newY)) {
        this->isGrabbed = false;
        return;
    }
    auto newField = cubeMapSide->getField(newX, newY);
    if (!cubeField->removeObject(this)) {
        this->isGrabbed = false;
        return;
    }
    newField->addObject(this);
    this->cubeFieldRef = newField;
    this->isGrabbed = true;
}

// ################################# Flag Logic Methoden #############################################################################

bool CubeObject::isLevelFinishedIfEntered() {
    return false;
}

bool Flag::isLevelFinishedIfEntered() {
    return true;
}
