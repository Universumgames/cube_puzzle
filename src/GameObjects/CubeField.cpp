#include "CubeField.hpp"
#include "GameObject.hpp"
#include "../data/SpriteStorage.hpp"
#include "../data/Colors.hpp"

// ################################# Konstruktoren ###################################################################################

CubeField::CubeField(int sideId, const Vector<CubeObject *>& cubeObjects) {
    this->sideId = sideId;
    this->cubeObjects = cubeObjects;
}

EmptyField::EmptyField(int sideId, const Vector<CubeObject *>& cubeObjects)
        : CubeField(sideId, cubeObjects) {}

Grass::Grass(int sideId, const Vector<CubeObject *>& cubeObjects)
        : EmptyField(sideId, cubeObjects) {}

Static::Static(int sideId, const Vector<CubeObject *>& cubeObjects)
        : CubeField(sideId, cubeObjects) {}

Wall_1::Wall_1(int sideId, const Vector<CubeObject *>& cubeObjects)
        : Static(sideId, cubeObjects) {}

Wall_2::Wall_2(int sideId, const Vector<CubeObject *>& cubeObjects)
    : Static(sideId, cubeObjects) {}
    
Interactable::Interactable(int sideId, const Vector<CubeObject *>& cubeObjects)
    : CubeField(sideId, cubeObjects) {}

PressurePlate::PressurePlate(int sideId, int id, bool activated, const Vector<CubeObject *>& cubeObjects)
    : Interactable(sideId, cubeObjects) {
    this->id = id;
    this->isActivated = activated;
}

// ################################# Alle Render-Methoden ############################################################################

void CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game.isDebug(), render, dst);
    Point locationNew = {location.x + 10, location.y + 10};
    Point sizeNew = {size.x - 20, size.y - 20};
    for (CubeObject* cubeObject : this->cubeObjects) {
        cubeObject->Render(game, render, sizeNew, locationNew, frame, totalMSec, deltaT);
        locationNew = {locationNew.x + 10, locationNew.y + 10};
        sizeNew = {sizeNew.x - 20, sizeNew.y - 20};
    }
}

void EmptyField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Grass::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Wall_1::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 100, 120, 0, 255);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Wall_2::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 50, 84, 65, 255);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void PressurePlate::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 154, 60, 78, 255);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

// ################################# HandleEvent und Update-Methoden #################################################################

void EmptyField::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // maybe add code later?
}

void Wall_1::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // maybe add code later?
}

void Wall_2::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // maybe add code later?
}

void Grass::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // maybe add code later?
}

void PressurePlate::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // maybe add code later?
}

// ################################# Setter & Getter #################################################################################

void CubeField::setSideId(int sideID) {
    this->sideId = sideID;
    for (auto cubeObject : this->cubeObjects) {
        cubeObject->setSideId(this->sideId);
    }
}

void CubeField::setDiceData(DiceData* dice_data) {
    this->diceData = dice_data;
    for (auto cubeObject : this->cubeObjects) {
        cubeObject->setDiceData(this->diceData);
    }
}

void CubeField::setCubeMapSideRef(CubeMapSide* cube_map_side) {
    this->cubeMapSideRef = cube_map_side;
}

bool PressurePlate::getIsActivated() const {
    return this->isActivated;
}

int PressurePlate::getId() const {
    return this->id;
}

// ################################# canPlayerEnter-Methoden #########################################################################

bool EmptyField::canPlayerEnter() {
    for (CubeObject* cubeObject : this->cubeObjects) {
        if (!cubeObject->canPlayerEnter()) {
            return false;
        }
    }
    return true;
}

bool Static::canPlayerEnter() {
    return false;
}

bool PressurePlate::canPlayerEnter() {
    for (CubeObject* cubeObject : this->cubeObjects) {
        if (!cubeObject->canPlayerEnter()) {
            return false;
        }
    }
    return true;
}

// ################################# canObjectEnter-Methoden #########################################################################

bool EmptyField::canObjectEnter(CubeObject *cubeObject) {
    return true;
}

bool Static::canObjectEnter(CubeObject *cubeObject) {
    return false;
}

bool Grass::canObjectEnter(CubeObject *cubeObject) {
    return false;
}

bool PressurePlate::canObjectEnter(CubeObject *cubeObject) {
    return cubeObject->canEnterPressurePlate();
}

// ################################# sonstige Methoden ###############################################################################

bool CubeField::isLevelFinishedIfEntered() {
    for (CubeObject* cubeObject : this->cubeObjects) {
        if (cubeObject->isLevelFinishedIfEntered()) {
            return true;
        }
    }
    return false;
}

bool CubeField::isPressurePlate() {
    return false;
}

bool PressurePlate::isPressurePlate() {
    return true;
}

void CubeField::addObject(CubeObject* cubeObject) {
    this->cubeObjects.push_back(cubeObject);
}

void CubeField::removeObject(CubeObject* cubeObject) {
    // TODO Mina: Methode implementieren.
}


int CubeField::enter() {
    return -1;
}

/// returns the id of the PressurePlate. The Sliders activated by this PressurePlate have the same id.
int PressurePlate::enter() {
    if (!this->canPlayerEnter()) {
        return -1;
    }
    if (!this->isActivated) {
        this->isActivated = true;
        this->activateAllSlidersWithSameId();
        return this->id;
    }
    return -1;
}

int CubeField::leave() {
    return -1;
}

int PressurePlate::leave() {
    if (!this->isActivated) {
        return -1;
    }
    this->isActivated = false;
    this->deactivateAllSlidersWithSameId();
    return this->id;
}

void PressurePlate::activateAllSlidersWithSameId() {
    CubeMap* cubeMap = this->cubeMapSideRef->getCubeMapRef();
    Vector<CubeMapSide*> allCubeMapSides = *cubeMap->getAllCubeMapSides();
    for (auto anyCubeMapSide : allCubeMapSides) {
        for (auto cubeField : anyCubeMapSide->cubeFields) {
            for (auto cubeObject : cubeField->cubeObjects) {
                if (cubeObject->isSlider()) {
                    auto *slider = dynamic_cast<Slider*>(cubeObject);
                    if (slider->getId() == this->id) {
                        slider->activate();
                    }
                }
            }
        }
    }
}

void PressurePlate::deactivateAllSlidersWithSameId() {
    CubeMap* cubeMap = this->cubeMapSideRef->getCubeMapRef();
    Vector<CubeMapSide*> allCubeMapSides = *cubeMap->getAllCubeMapSides();
    for (auto anyCubeMapSide : allCubeMapSides) {
        for (auto cubeField : anyCubeMapSide->cubeFields) {
            for (auto cubeObject : cubeField->cubeObjects) {
                if (cubeObject->isSlider()) {
                    auto *slider = dynamic_cast<Slider*>(cubeObject);
                    if (slider->getId() == this->id) {
                        slider->deactivate();
                    }
                }
            }
        }
    }
}