#pragma once

#include "../CubeGame.hpp"
#include "GameObject.hpp"

class CubeObject {
public:
    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event);
    /// physics, etc. update method
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT);
    /// render only method
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);

    void drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst);
    virtual bool canPlayerEnter();
    virtual bool canEnterPressurePlate();
    virtual bool canActivatePressurePlate();
    virtual bool isLevelFinishedIfEntered();
};

class GravityObject : public CubeObject {
public:
    bool canEnterPressurePlate() override;
};

class Stone : public GravityObject {
private:
    bool isFalling = false;
public:
    Stone() = default;
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    bool canActivatePressurePlate() override;
};

class Activatable : public CubeObject {
public:
    bool canActivatePressurePlate() override;
};

class Slider : public Activatable {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Moveable : public CubeObject {
};

class Magnet : public Moveable {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Collectible : public CubeObject {
public:
    bool canPlayerEnter() override;
};

class Flag : public Collectible {
public:
    // Level:: void returnToLevelSelector(ExitState exitState);
    // ExitState::FINISHED
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;

    bool canEnterPressurePlate() override;
    bool canActivatePressurePlate() override;
    
    bool isLevelFinishedIfEntered() override;
};