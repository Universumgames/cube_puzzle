#pragma once

#include "GameObject.hpp"
#include "../CubeGame.hpp"
#include "../stringhelper.hpp"

/// Field on CubeMap
class CubeField {
protected:
    Vector<GameObject *> objects;
    
public:
    CubeField() = default;

    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) = 0;

    /// physics, etc. update method
    virtual void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) = 0;

    /// render only method
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
           const float deltaT);

    /// return true if player can move to this tile, false otherwise
    virtual bool canPlayerEnter() = 0;

    /// return true if an other object can move to this tile, false otherwise
    virtual bool canObjectEnter() = 0;

    /// has to be implemented in every sub class
    static CubeField *decode(std::string data);

    /// encoding works by creating it layer by layer
    virtual std::string encode() = 0;

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

    bool canObjectEnter() override {
        return true;
    }

    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    void Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                const float deltaT) override {
    }

    static EmptyField* decode(std::string data) {
        return new EmptyField();
    }

    std::string encode() override {
        return charToString((char) CubeField::TYPE::EMPTY);
    }
};

class GravityObject : public CubeField {
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
};

class FallingStone : public GravityObject {
public:
    std::string encode() override { return GravityObject::encode() + "f"; }
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};

class Static: public CubeField {
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
        return charToString((char) CubeField::TYPE::STATIC);
    }

    static Static* decode(std::string data);
};

class Grass : public Static {
public:
    std::string encode() override {
        return Static::encode() + "g";
    }
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};

class Ice : public Static {
public:
    std::string encode() override {
        return Static::encode() + "i";
    }
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};

class Interactable: public CubeField {
public:
    bool canPlayerEnter() override {
        return true;
    }

    bool canObjectEnter() override {
        return true;
    }
    
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

class Button : public Interactable {
public:
    bool canPlayerEnter() override {
        return true;
    }

    bool canObjectEnter() override {
        return true;
    }
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};

class Activatable : public CubeField {
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

    static Activatable* decode(std::string data);

    std::string encode() override {
        return charToString((char) CubeField::TYPE::ACTIVATABLE);
    }
    
    // TODO implement logic
};

class Piston : public Activatable {
public:
    std::string encode() override {
        return Activatable::encode() + "p";
    }
    
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};

class SlidingWall : public Activatable {
public:
    std::string encode() override {
        return Activatable::encode() + "s";
    }
    
    // TODO implement decode method
    // TODO implement logic
    // TODO adjust encode method
};