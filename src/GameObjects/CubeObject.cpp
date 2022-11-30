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

void CubeObject::Update(CubeGame& game, u32 frame, u32 totalMSec, float deltaT) {
}

void CubeObject::HandleEvent(CubeGame& game, u32 frame, u32 totalMSec, float deltaT, Event event) {
}

void CubeObject::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug())
        return;
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}

bool CubeObject::canPlayerEnter() {
    return false;
}

bool CubeObject::canEnterPressurePlate() {
    return false;
}

bool CubeObject::canActivatePressurePlate() {
    return false;
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

bool Collectible::canPlayerEnter() {
    return true;
}

bool Flag::canEnterPressurePlate() {
    return false;
}
bool Flag::canActivatePressurePlate() {
    return false;
}





// -------------------------- unused code (for now) --------------------------

/*
/// return-value tells you if the activation took place or not.
bool Activatable::activate() {
    if (!this->isOpen) {
        this->isOpen = true;
        return true;
    }
    return false;
}

/// return-value tells you if the deactivation took place or not.
bool Activatable::deactivate() {
    if (this->isDeactivatable && this->isOpen) {
        this->isOpen = false;
        return true;
    }
    return false;
}*/

// -------------------------- unused code (for now) --------------------------

/*class GravityObject : public CubeField {
public:
    bool canPlayerEnter() override {
        return false;
    }

    bool canObjectEnter() override {
        return false;
    }

    /// Handle input events
    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    /// physics, etc. update method
    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }
    // TODO implement logic
};*/

/*class FallingStone : public GravityObject {
};*/

/*class Piston : public Interactable {
public:
    bool canPlayerEnter() override {
        return false;
    }

    bool canObjectEnter() override {
        return false;
    }
};*/

/*class Activatable : public CubeField {
protected:
    bool isOpen;
    bool isDeactivatable;

public:
    explicit Activatable(bool isDeactivatable) : isDeactivatable(isDeactivatable), isOpen(false) {}

    Activatable(bool isDeactivatable, bool isOpen) : isDeactivatable(isDeactivatable), isOpen(isOpen) {}

    bool canPlayerEnter() override {
        return this->isOpen;
    }

    bool canObjectEnter() override {
        return this->isOpen;
    }

    /// Handle input events
    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    /// physics, etc. update method
    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    bool activate();

    bool deactivate();
};

class SlidingWall : public Activatable {
};*/