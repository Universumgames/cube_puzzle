#pragma once


#include "../CubeGame.hpp"
#include "../stringhelper.hpp"
#include "CubeObject.hpp"

/// Field on CubeMap
class CubeField {
protected:
    Vector<GameObject *> objects;
    Vector<CubeObject *> cubeObjects;

public:
    CubeField() = default;
    explicit CubeField(Vector<CubeObject *> &cubeObjects);

    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event){};
    /// physics, etc. update method
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) = 0;
    /// render only method
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);

    virtual bool canPlayerEnter() = 0;
    virtual bool canObjectEnter(CubeObject *cubeObject) = 0;
};

class EmptyField : public CubeField {
public:

    EmptyField() = default;

    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    /// render only method
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;

    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;

};

class Static : public CubeField { // walls, unmovable obstacles
public:
    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};

class Wall_1 : public Static {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Wall_2 : public Static {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Grass : public EmptyField {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Interactable : public CubeField {
public:
};

class PressurePlate : public Interactable {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSe, float deltaT) override;

    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};