#include "CubeField.hpp"
#include "GameObject.hpp"
#include "../data/SpriteStorage.hpp"
#include "../data/Colors.hpp"

// ################################# Konstruktoren ###################################################################################

CubeField::CubeField(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = std::move(cubeObjects);
}

// ################################# Alle Render-Methoden ############################################################################

void CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game.isDebug(), render, dst);
    Point locationNew = {location.x + 1, location.y + 1};
    Point sizeNew = {size.x - 2, size.y - 2};
    for (CubeObject* cube_object : this->cubeObjects) {
        cube_object->Render(game, render, sizeNew, locationNew, frame, totalMSec, deltaT);
        locationNew = {locationNew.x + 1, locationNew.y + 1};
        sizeNew = {sizeNew.x - 2, sizeNew.y - 2};
    }
}

void EmptyField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec, const float deltaT) {
    drawSpriteBorder(game.isDebug(), render, {location.x, location.y, size.x, size.y});
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

// ################################# canPlayerEnter-Methoden #########################################################################

bool EmptyField::canPlayerEnter() {
    return true;
}

bool Static::canPlayerEnter() {
    return false;
}

bool PressurePlate::canPlayerEnter() {
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