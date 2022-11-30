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
    
    explicit CubeField(Vector<CubeObject *>& cubeObjects);

    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) = 0;

    /// physics, etc. update method
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) = 0;

    /// render only method
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);

    /// return true if player can move to this tile, false otherwise
    virtual bool canPlayerEnter() = 0;

    /// return true if an other object can move to this tile, false otherwise
    virtual bool canObjectEnter(CubeObject* cubeObject) = 0;

    /// has to be implemented in every sub class
    static CubeField *decode(std::string data);

    /// encoding works by creating it layer by layer
    virtual std::string encode() = 0;

    void drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst);

    enum class TYPE : char {
        EMPTY = 'e',
        STATIC = 's',
        GRAVITY = 'g',
        INTERACTABLE = 'i',
        ACTIVATABLE = 'a'
    };
};

class EmptyField : public CubeField {
public:
    bool canPlayerEnter() override {
        return true;
    }

    bool canObjectEnter(CubeObject* cubeObject) override {
        return true;
    }

    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    void Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                const float deltaT) override {
        drawSpriteBorder(game, render, {location.x, location.y, size.x, size.y});
    }

    static EmptyField *decode(std::string data) {
        return new EmptyField();
    }

    std::string encode() override {
        return charToString((char) CubeField::TYPE::EMPTY);
    }
};

class Static : public CubeField { // walls, unmovable obstacles
public:
    bool canPlayerEnter() override {
        return false;
    }

    bool canObjectEnter(CubeObject* cubeObject) override {
        return false;
    }

    /// Handle input events
    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    /// physics, etc. update method
    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    std::string encode() override {
        return charToString((char) CubeField::TYPE::STATIC);
    }

    static Static *decode(std::string data);
};

class Wall_1 : public Static {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Wall_2 : public Static {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
};

class Grass : public Static {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    std::string encode() override {
        return Static::encode() + "g";
    }
    
    static Grass *decode(std::string data);
    
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};

class Interactable : public CubeField {
public:
    /// Handle input events
    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    /// physics, etc. update method
    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    std::string encode() override {
        return charToString((char) CubeField::TYPE::INTERACTABLE);
    }

    static Interactable *decode(std::string data);
};

class PressurePlate : public Interactable {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    bool canPlayerEnter() override {
        return true;
    }

    bool canObjectEnter(CubeObject* cubeObject) override {
        return cubeObject->canEnterPressurePlate();
    }
    
    static PressurePlate *decode(std::string data);
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};






// -------------------------- unused code (for now) --------------------------

/*class Ice : public Static {
public:
    std::string encode() override {
        return Static::encode() + "i";
    }
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};*/

/*class GravityObject : public CubeField {
public:
    bool canPlayerEnter() override {
        return false;
    }

    bool canObjectEnter() override {
        return false;
    }

    /// Handle input events
    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    /// physics, etc. update method
    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    std::string encode() override {
        return charToString((char) CubeField::TYPE::GRAVITY);
    }
    // TODO implement logic
};*/

/*class FallingStone : public GravityObject {
public:
    std::string encode() override { return GravityObject::encode() + "f"; }
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};*/

/*class Piston : public Interactable {
public:
    bool canPlayerEnter() override {
        return false;
    }

    bool canObjectEnter() override {
        return false;
    }

    std::string encode() override {
        return Interactable::encode() + "p";
    }

    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};*/

/*class Activatable : public CubeField {
protected:
    bool isOpen;
    bool isDeactivatable;

public:
    explicit Activatable(bool isDeactivatable) : isDeactivatable(isDeactivatable), isOpen(false) {}

    Activatable(bool isDeactivatable, bool isOpen) : isDeactivatable(isDeactivatable), isOpen(isOpen) {}

    bool canPlayerEnter() override {
        return this->isOpen;
    }

    bool canObjectEnter() override {
        return this->isOpen;
    }

    /// Handle input events
    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    /// physics, etc. update method
    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    static Activatable *decode(std::string data);

    std::string encode() override {
        return charToString((char) CubeField::TYPE::ACTIVATABLE);
    }

    bool activate();

    bool deactivate();

    // TODO implement logic
};

class SlidingWall : public Activatable {
public:
    std::string encode() override {
        return Activatable::encode() + "s";
    }

    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};*/