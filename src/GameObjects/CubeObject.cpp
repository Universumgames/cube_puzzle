#include "CubeObject.hpp"

void CubeObject::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                        float deltaT) {
    // TODO implement method
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

FallingGravityObject* RestingGravityObject::switchToFallingGravityObject() {
    return new FallingGravityObject();
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

RestingGravityObject* FallingGravityObject::switchToRestingGravityObject() {
    return new RestingGravityObject();
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
