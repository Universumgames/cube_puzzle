#include "CubeField.hpp"
#include "GameObject.hpp"
#include "../data/Colors.hpp"

void CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                  const float deltaT) {
    SDL_SetRenderDrawColor(render, 200, 200, 0, 255);

    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game, render, dst);
}

CubeField::TYPE getType(char firstChar) {
    return (CubeField::TYPE) firstChar;
}

CubeField *CubeField::decode(std::string data) {
    TYPE type = getType(data[0]);
    data.pop_back();
    // TODO implement switch cases
    switch (type) {
        case TYPE::EMPTY:
            return EmptyField::decode(data);
        case TYPE::STATIC:
            return Static::decode(data);
        //case TYPE::GRAVITY:
        //    return GravityObject::decode(data);
        case TYPE::INTERACTABLE:
            return Interactable::decode(data);
        //case TYPE::ACTIVATABLE:
        //    return Activatable::decode(data);
    }
    return nullptr;
}

void CubeField::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug()) return;
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}

CubeField::CubeField(Vector<CubeObject *>& cubeObjects) {
    this->cubeObjects = cubeObjects;
}

Static *Static::decode(std::string data) {
    char c = data[0];
    data.pop_back();
    switch (c) {
        case 'g':
            return Grass::decode(data);
        default:
            return nullptr;
    }
    return nullptr;
}

Grass *Grass::decode(std::string data) {
    return new Grass();
}

Interactable *Interactable::decode(std::string data) {
    char c = data[0];
    data.pop_back();
    switch (c) {
        case 'b':
            return PressurePlate::decode(data);
        //case 'p':
        //    return Piston::decode(data);
        default:
            return nullptr;
    }
    return nullptr;
}

PressurePlate *PressurePlate::decode(std::string data) {
    return new PressurePlate();
}



// -------------------------- unused code (for now) --------------------------

/*Activatable *Activatable::decode(std::string data) {
    char c = data[0];
    data.pop_back();
    switch (c) {
        case 's':
            return SlidingWall::decode(data);
        default:
            return nullptr;
    }
    return nullptr;
}

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
