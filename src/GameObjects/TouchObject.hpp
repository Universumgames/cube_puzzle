#pragma once

#include "GameObject.hpp"


class TouchObject : public GameObject {
public:
    TouchObject(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render, Point location, Point size,
                int touchPadding = 10);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

public:
    bool isPressed();

    bool wasJustReleased();

    Point getLocation();
    Point getSize();
    int getTouchPadding();

    Rect getTouchRect();

    void setLocation(Point location);
    void setSize(Point size);
    void setTouchPadding(int padding);

    void setPressedLambda(std::function<void(CubeGame &, TouchObject *)> pressedLambda);

    void setHoldingLambda(std::function<void(CubeGame &, TouchObject *)> holdingLambda);

    void setUpdateLambda(std::function<void(CubeGame &, TouchObject *)> updateLambda);


protected:
    virtual void updateRects();


protected:
    std::function<void(CubeGame &, TouchObject *)> justPressedLambda = [](CubeGame &, TouchObject *) {};
    std::function<void(CubeGame &, TouchObject *)> updateLambda = [](CubeGame &, TouchObject *) {};
    std::function<void(CubeGame &, TouchObject *)> holdingLambda = [](CubeGame &, TouchObject *) {};
    Point location;
    Point size;
    int touchPadding;
    Rect touchRect;
};
