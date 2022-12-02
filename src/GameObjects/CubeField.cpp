#include "CubeField.hpp"
#include "GameObject.hpp"
#include "../data/SpriteStorage.hpp"
#include "../data/Colors.hpp"

// ################################# Konstruktoren ###################################################################################

CubeField::CubeField(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
}

CubeField::CubeField(Vector<CubeObject *> &cubeObjects, int sideId) {
    this->cubeObjects = cubeObjects;
    this->sideId = sideId;
}

EmptyField::EmptyField(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
}

Wall_1::Wall_1(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
}

Wall_2::Wall_2(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
}

Grass::Grass(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
}

PressurePlate::PressurePlate(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
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

void CubeField::addObject(CubeObject* cubeObject) {
    this->cubeObjects.push_back(cubeObject);
}

void CubeField::removeObject(CubeObject* cubeObject) {
    // TODO Mina: Methode implementieren.
}