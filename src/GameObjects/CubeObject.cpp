#include "CubeObject.hpp"

// ################################# Konstruktoren ###################################################################################


Slider::Slider(MovementDirectionIfActivated movementDirectionEnum, int id, bool activated) {
    this->direction = movementDirectionEnum;
    this->id = id;
    this->isActivated = activated;
}

// ################################# HandleEvent und Update-Methoden #################################################################

void CubeObject::Update(CubeGame& game, u32 frame, u32 totalMSec, float deltaT) {
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

// ################################# sonstige Methoden ###############################################################################

bool CubeObject::isLevelFinishedIfEntered() {
    return false;
}

bool Flag::isLevelFinishedIfEntered() {
    return true;
}

bool CubeObject::isSlider() {
    return false;
}

bool Slider::isSlider() {
    return true;
}

void Slider::activate() {
    cout << "ich wurde aktiviert yeeeha!!" << endl;
    this->isActivated = true;
}

void Slider::deactivate() {
    cout << "Oh noes, ich wurde deaktiviert :((((" << endl;
    this->isActivated = false;
}