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
    int x, y;
    CubeMapSide* cubeMapSideRef;
    Vector<GameObject *> objects;
    int sideId;
    DiceData* diceData;

public:
    Vector<CubeObject *> cubeObjects;
    CubeField() = delete;
    explicit CubeField(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
    
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event);
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT);
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);

    void setSideId(int sideID);
    void setDiceData(DiceData* dice_data);
    void setCubeMapSideRef(CubeMapSide* cube_map_side);
    void setGravityDirection(MovementDirection dir);
    CubeMapSide* getCubeMapSideRef();
    [[nodiscard]] Point getCoordinates();
    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;
    [[nodiscard]] Magnet* getMagnetIfPresent();
    
    virtual bool canPlayerEnter() = 0;
    virtual bool canObjectEnter(CubeObject *cubeObject) = 0;
    bool isLevelFinishedIfEntered();
    virtual bool isPressurePlate();
    void addObject(CubeObject *cubeObject);
    bool removeObject(CubeObject *cubeObject);
    virtual int enter();
    virtual int leave();
};

// ################################# EmptyFields #####################################################################################

class EmptyField : public CubeField {
public:
    EmptyField() = delete;
    explicit EmptyField(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;

    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};

class ObjectBarrier : public EmptyField {
public:
    ObjectBarrier() = delete;
    explicit ObjectBarrier(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
    
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    
    bool canObjectEnter(CubeObject *cubeObject) override;
};

class ArrowField : public EmptyField {
private:
    MovementDirection arrowDirection;

public:
    ArrowField() = delete;
    explicit ArrowField(int sideId, int x, int y, MovementDirection arrowDirection, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject *>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

// ################################# Walls ###########################################################################################

class Static : public CubeField { // walls, unmovable obstacles
public:
    explicit Static(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
};

class Wall : public Static {
public:
    Wall() = delete;
    explicit Wall(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                float deltaT) override;
};

// ################################# PressurePlate ###################################################################################

class Interactable : public CubeField {
public:
    explicit Interactable(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());
};

class PressurePlate : public Interactable {
private:
    int id;
    bool isActivated = false;
public:
    PressurePlate() = delete;
    PressurePlate(int sideId, int x, int y, int id, bool activated = false, const Vector<CubeObject *>& cubeObjects = Vector<CubeObject*>());

    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSe, float deltaT) override;

    [[nodiscard]] bool getIsActivated() const;
    [[nodiscard]] int getId() const;
    bool canPlayerEnter() override;
    bool canObjectEnter(CubeObject *cubeObject) override;
    bool isPressurePlate() override;
    int enter() override;
    int leave() override;
    int activate();
    int deactivate();
    void activateAllSlidersWithSameId();
    void deactivateAllSlidersWithSameId();

    static AudioPlayer* getAudioEnter();
};