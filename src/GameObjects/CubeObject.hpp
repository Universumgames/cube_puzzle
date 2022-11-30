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
};

class GravityObject : public CubeObject {
public:
    bool canEnterPressurePlate() override;
};

class FallingGravityObject;

class RestingGravityObject : public GravityObject {
public:
    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    RestingGravityObject() = default;
    virtual FallingGravityObject* switchToFallingGravityObject() = 0;
    bool canActivatePressurePlate() override;
};

class RestingStone : public RestingGravityObject {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    FallingGravityObject* switchToFallingGravityObject() override;
};

class FallingGravityObject : public GravityObject {
public:
    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    FallingGravityObject() = default;
    virtual RestingGravityObject* switchToRestingGravityObject() = 0;
    bool canActivatePressurePlate() override;

};

class FallingStone : public FallingGravityObject {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    RestingGravityObject* switchToRestingGravityObject() override;
};

class Activatable : public CubeObject {
public:
    bool canEnterPressurePlate() override;
    bool canActivatePressurePlate() override;
    
};

class Slider : public Activatable {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;

};

class Moveable : public CubeObject {
public:
    bool canEnterPressurePlate() override;
};

class RestingMoveable : public Moveable {

};

class RestingMagnet : public RestingMoveable {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class MovingMoveable : public Moveable {

};

class MovingMagnet : public MovingMoveable {
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
};