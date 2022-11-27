#pragma once

#include "GameObject.hpp"

class CubeObject {
public:
    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) = 0;
    
    /// physics, etc. update method
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) = 0;
    
    /// render only method
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
           float deltaT);
    
    virtual bool canEnterPressurePlate() = 0;
    
    virtual bool canActivatePressurePlate() = 0;
};

class GravityObject : CubeObject {
public:
    bool canEnterPressurePlate() override;
};

class FallingGravityObject;

class RestingGravityObject : GravityObject {
public:
    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    RestingGravityObject() = default;
    FallingGravityObject* switchToFallingGravityObject();
    bool canActivatePressurePlate() override;
};

class RestingStone : RestingGravityObject {

};

class FallingGravityObject : GravityObject {
public:
    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    FallingGravityObject() = default;
    RestingGravityObject* switchToRestingGravityObject();
    bool canActivatePressurePlate() override;

};

class FallingStone : FallingGravityObject {

};

class Activatable : CubeObject {
public:
    bool canEnterPressurePlate() override;
    bool canActivatePressurePlate() override;
    
};

class Slider : Activatable {

};

class Moveable : CubeObject {
public:
    bool canEnterPressurePlate() override;
};

class RestingMoveable : Moveable {

};

class RestingMagnet : RestingMoveable {

};

class MovingMoveable : Moveable {

};

class MovingMagnet : MovingMoveable {

};