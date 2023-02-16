#include "TouchObject.hpp"
#include "../CubeGame.hpp"
#include "../recthelper.hpp"
#include "../touchhelper.hpp"

bool TouchObject::isPressed() {
    bool pressed = std::any_of(lastTouchEvents.begin(), lastTouchEvents.end(),
                               [this](std::pair<SDL_FingerID, SDL_TouchFingerEvent> e) {
                                   SDL_Point touchPoint = uvToPixel(game.getWindowSize(), {e.second.x, e.second.y});
                                   return e.second.type != SDL_FINGERUP && SDL_PointInRect(&touchPoint, &touchRect);
                               });
    return pressed;
}

bool TouchObject::wasJustReleased() {
    bool released = std::any_of(lastTouchEvents.begin(), lastTouchEvents.end(),
                                [this](std::pair<SDL_FingerID, SDL_TouchFingerEvent> e) {
                                    SDL_Point touchPoint = uvToPixel(game.getWindowSize(), {e.second.x, e.second.y});
                                    return e.second.type == SDL_FINGERUP && SDL_PointInRect(&touchPoint, &touchRect);
                                });
    return released;
}


void TouchObject::updateRects() {
    touchRect = {location.x - touchPadding, location.y - touchPadding, size.x + 2 * touchPadding,
                 size.y + 2 * touchPadding};
}

Point TouchObject::getLocation() {
    return location;
}

Point TouchObject::getSize() {
    return size;
}

int TouchObject::getTouchPadding() {
    return touchPadding;
}

Rect TouchObject::getTouchRect() {
    return touchRect;
}

void TouchObject::setLocation(Point location) {
    this->location = location;
    updateRects();
}

void TouchObject::setSize(Point size) {
    if (this->size == size) return;
    this->size = size;
    updateRects();
}

void TouchObject::setTouchPadding(int padding) {
    this->touchPadding = padding;
    updateRects();
}

void TouchObject::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void TouchObject::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    updateLambda(game, this);
    if (wasJustReleased())
        justPressedLambda(game, this);
    if (isPressed())
        holdingLambda(game, this);
}

void TouchObject::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        Point location = {event.motion.x, event.motion.y};
        if(SDL_PointInRect(&location, &touchRect)){
            justPressedLambda(game, this);
        }
    }
}

TouchObject::TouchObject(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render, Point location, Point size,
                         int touchPadding) : GameObject(game, gameState, render), location(location), size(size),
                                             touchPadding(touchPadding) {
    updateRects();
}

void TouchObject::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawSpriteBorder(game.isDebug(), render, touchRect, red);
}


void TouchObject::setPressedLambda(
        std::function<void(CubeGame &, TouchObject *)> pressedLambda) {
    this->justPressedLambda = pressedLambda;
}

void TouchObject::setUpdateLambda(std::function<void(CubeGame &, TouchObject *)> updateLambda) {
    this->updateLambda = updateLambda;
}

void TouchObject::setHoldingLambda(std::function<void(CubeGame &, TouchObject *)> holdingLambda) {
    this->holdingLambda = holdingLambda;
}
