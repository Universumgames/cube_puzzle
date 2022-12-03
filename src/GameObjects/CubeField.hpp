#pragma once

#include "../CubeGame.hpp"
#include "../stringhelper.hpp"
#include "CubeObject.hpp"
#include "../data/DiceData.hpp"
#include "CubeMapSide.hpp"

class CubeObject;
class CubeMapSide;

/// Field on CubeMap
class CubeField {
protected:
    CubeMapSide* cubeMapSideRef;
    Vector<GameObject *> objects;
    int sideId;
    DiceData* diceData;

public:
    Vector<CubeObject *> cubeObjects;
    CubeField() = delete;
    //explicit CubeField(Vector<CubeObject *> &cubeObjects);
    //CubeField(Vector<CubeObject *> &cubeObjects, int sideId);
    //CubeField(int sideId);
    explicit CubeField(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
    
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) {};
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) = 0;
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);

    void setSideId(int sideID);
    void setDiceData(DiceData* dice_data);
    void setCubeMapSideRef(CubeMapSide* cube_map_side);
    
    virtual bool canPlayerEnter() = 0;
    virtual bool canObjectEnter(CubeObject *cubeObject) = 0;
    bool isLevelFinishedIfEntered();
    virtual bool isPressurePlate();
    void addObject(CubeObject *cubeObject);
    void removeObject(CubeObject *cubeObject);
    virtual int enter();
    virtual int leave();
};

// ################################# EmptyFields #####################################################################################

class EmptyField : public CubeField {
public:
    EmptyField() = delete;
    explicit EmptyField(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;

    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};

class Grass : public EmptyField {
public:
    Grass() = delete;
    explicit Grass(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
    
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
    
    bool canObjectEnter(CubeObject *cubeObject) override;
};

// ################################# Walls ###########################################################################################

class Static : public CubeField { // walls, unmovable obstacles
public:
    explicit Static(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};

class Wall_1 : public Static {
public:
    Wall_1() = delete;
    explicit Wall_1(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                float deltaT) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
};

class Wall_2 : public Static {
public:
    Wall_2() = delete;
    explicit Wall_2(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;
};

// ################################# PressurePlate ###################################################################################

class Interactable : public CubeField {
public:
    explicit Interactable(int sideId, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
};

class PressurePlate : public Interactable {
private:
    int id;
    bool isActivated = false;
public:
    PressurePlate() = delete;
    PressurePlate(int sideId, int id, bool activated = false, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSe, float deltaT) override;
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) override;

    bool getIsActivated();
    int getId();
    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
    bool isPressurePlate() override;
    int enter() override;
    int leave() override;
    void activateAllSlidersWithSameId();
    void deactivateAllSlidersWithSameId();
};