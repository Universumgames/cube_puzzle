#include "CubeObject.hpp"

// ################################# Konstruktoren ###################################################################################


Slider::Slider(MovementDirectionIfActivated movementDirectionEnum, int id, bool activated) {
    this->diretion = movementDirectionEnum;
    this->id = id;
    this->isActivated = activated;
}

// ################################# Alle Render-Methoden ############################################################################

void CubeObject::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game, render, dst);
}

void Stone::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls einen Stein
    SDL_SetRenderDrawColor(render, 100, 100, 255, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Slider::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls einen Slider, wobei ich noch implementieren muss, in welche Richtung der slidet,
    //  also vll erstmal noch warten mit dem Sprite, die sollten dann ja auch unterschiedlich aussehen.
    SDL_SetRenderDrawColor(render, 100, 0, 255, 60);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Magnet::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls einen "Magneten" oder wie auch immer Leo ihn benannt haben will.
    SDL_SetRenderDrawColor(render, 50, 200, 255, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Flag::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls eine Flagge
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void CubeObject::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug()) {
        return;
    }
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
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