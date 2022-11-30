#include "CubeObject.hpp"
#include "../data/Colors.hpp"

void CubeObject::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game, render, dst);
}

void RestingStone::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 100, 100, 255, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void FallingStone::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 150, 150, 0, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Slider::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 100, 0, 255, 60);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void RestingMagnet::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 50, 200, 255, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void MovingMagnet::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 80, 90, 100, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Flag::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}


void CubeObject::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug())
        return;
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}

bool GravityObject::canEnterPressurePlate() {
    return true;
}

void RestingGravityObject::HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) {
    // TODO implement method
}

/// physics, etc. update method
void RestingGravityObject::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // TODO implement method
}

FallingGravityObject* RestingStone::switchToFallingGravityObject() {
    return new FallingStone();
}

bool RestingGravityObject::canActivatePressurePlate() {
    return true;
}

void FallingGravityObject::HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) {
    // TODO implement method
}

/// physics, etc. update method
void FallingGravityObject::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    // TODO implement method
}

RestingGravityObject* FallingStone::switchToRestingGravityObject() {
    return new RestingStone();
}

bool FallingGravityObject::canActivatePressurePlate() {
    return false;
}

bool Activatable::canEnterPressurePlate() {
    return false;
}

bool Activatable::canActivatePressurePlate() {
    return false;
}

bool Moveable::canEnterPressurePlate() {
    return true;
}

bool Flag::canEnterPressurePlate() {
    return false;
}
bool Flag::canActivatePressurePlate() {
    return false;
}
