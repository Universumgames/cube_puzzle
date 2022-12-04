#include "CubeObject.hpp"
#include <utility>

// ################################# Konstruktoren ###################################################################################

Slider::Slider(MovementDirection direction, int id, bool activated) {
    this->directionIfActivated = direction;
    this->id = id;
    this->isActivated = activated;
    if (this->isActivated) {
        this->currentMovementDirection = direction;
    }
    this->type = CubeObject::ObjectType::typeSlider;
    this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
}

// ################################# HandleEvent und Update-Methoden #################################################################

void CubeObject::Update(CubeGame& game, u32 frame, u32 totalMSec, float deltaT) {
    //cout << "CubeObject::Update is being called." << endl;
    CubeField* cubeField = this->cubeFieldRef;
    int oldX = cubeField->getX();
    int oldY = cubeField->getY();
    int newX = oldX;
    int newY = oldY;
    CubeMapSide* cubeMapSide = cubeField->getCubeMapSideRef();
    int height = cubeMapSide->height;
    int width = cubeMapSide->width;
    switch (this->currentMovementDirection) {
        case MovementDirection::moveToBigX:
            if (oldX < width - 1)  {
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
        } else { // Slider reached the end
            cubeMapSide->getCubeMapRef()->setIsAnimating(false);
            this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
        }
    } else {
        cubeMapSide->getCubeMapRef()->setIsAnimating(false);
        this->lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
    }
}

void CubeObject::HandleEvent(CubeGame& game, u32 frame, u32 totalMSec, float deltaT, Event event) {
}

// ################################# Setter & Getter #################################################################################

void CubeObject::setDiceData(DiceData* dice_data) {
    this->diceData = dice_data;
}

void CubeObject::setSideId(int sideID) {
    this->sideId = sideID;
}

void CubeObject::setCubeFieldRef(CubeField* cube_field) {
    this->cubeFieldRef = cube_field;
}

CubeObject::ObjectType CubeObject::getType() {
    return this->type;
}

int Slider::getId() const {
    return this->id;
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
    switch(this->directionIfActivated) {
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

// ################################# Flag Logic Methoden #############################################################################
bool CubeObject::isLevelFinishedIfEntered() {
    return false;
}

bool Flag::isLevelFinishedIfEntered() {
    return true;
}