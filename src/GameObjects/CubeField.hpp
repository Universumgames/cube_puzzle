#pragma once


#include "../CubeGame.hpp"
#include "../stringhelper.hpp"
#include "CubeObject.hpp"

/// Field on CubeMap
class CubeField {
protected:
    Vector<GameObject *> objects;

public:
    Vector<CubeObject *> cubeObjects;

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

    void addObject(CubeObject* cubeObject);
};

class EmptyField : public CubeField {
public:
    EmptyField() = default;
    explicit EmptyField(Vector<CubeObject *> &cubeObjects);


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
    Wall_1() = default;
    explicit Wall_1(Vector<CubeObject *> &cubeObjects);
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Wall_2 : public Static {
public:
    Wall_2() = default;
    explicit Wall_2(Vector<CubeObject *> &cubeObjects);
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Grass : public EmptyField {
public:
    Grass() = default;
    explicit Grass(Vector<CubeObject *> &cubeObjects);
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Interactable : public CubeField {

};

class PressurePlate : public Interactable {
public:
    PressurePlate() = default;
    explicit PressurePlate(Vector<CubeObject *> &cubeObjects);

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSe, float deltaT) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;

    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};