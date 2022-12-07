#pragma once

#include "../CubeGame.hpp"
#include "GameObject.hpp"
#include "CubeField.hpp"

#define OBJECT_MOVEMENT_COUNTDOWN_MILLIS 100

class CubeField;

class CubeObject {
public:
    enum class ObjectType {
        typeStone,
        typeSlider,
        typeMagnet,
        typeFlag,
        none
    };
    
protected:
    double lastMovementCountdown = OBJECT_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
    MovementDirection currentMovementDirection = MovementDirection::none;
    DiceData *diceData;
    int sideId;
    CubeField* cubeFieldRef;
    ObjectType type = ObjectType::none;
    
public:
    /// render only method
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);
    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event);
    /// physics, etc. update method
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT);

    void setDiceData(DiceData* dice_data);
    void setSideId(int sideID);
    void setCubeFieldRef(CubeField* cube_field);
    [[nodiscard]] virtual ObjectType getType();
    
    void drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst);
    [[nodiscard]] virtual bool canPlayerEnter();
    [[nodiscard]] virtual bool canAnotherObjectEnter();
    [[nodiscard]] virtual bool canEnterPressurePlate();
    [[nodiscard]] virtual bool canActivatePressurePlate();
    [[nodiscard]] virtual bool isLevelFinishedIfEntered();
    [[nodiscard]] virtual bool isSlider();
};

// ################################# Gravity Object ##################################################################################

class GravityObject : public CubeObject {
};

class Stone : public GravityObject {
private:
    MovementDirection fallingDirection;
public:
    Stone();
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    
    void setFallingDirection(MovementDirection dir);
    [[nodiscard]] ObjectType getType() override;
    
    [[nodiscard]] bool canPlayerEnter() override;
    [[nodiscard]] bool canAnotherObjectEnter() override;
    [[nodiscard]] bool canEnterPressurePlate() override;
    [[nodiscard]] bool canActivatePressurePlate() override;
};

// ################################# Activatable #####################################################################################

class Activatable : public CubeObject {
};

class Slider : public Activatable {
private:
    int id;
    bool isActivated = false;
    MovementDirection directionIfActivated;
    
public:
    Slider() = delete;
    explicit Slider(MovementDirection movementDirectionIfActivated, int id, bool activated = false);
    
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;

    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) override;

    [[nodiscard]] int getId() const;
    
    [[nodiscard]] bool canPlayerEnter() override;
    [[nodiscard]] bool canAnotherObjectEnter() override;
    [[nodiscard]] bool canEnterPressurePlate() override;
    [[nodiscard]] bool canActivatePressurePlate() override;
    [[nodiscard]] bool isSlider() override;
    void activate();
    void deactivate();
};

// ################################# Moveable ########################################################################################

class Moveable : public CubeObject {
};

class Magnet : public Moveable {
private:
    bool isGrabbed = false;
public:
    Magnet();
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;

    void setIsGrabbed(bool isGrabbed);
    [[nodiscard]] bool getIsGrabbed() const;
    
    [[nodiscard]] bool canPlayerEnter() override;
    [[nodiscard]] bool canAnotherObjectEnter() override;
    [[nodiscard]] bool canEnterPressurePlate() override;
    [[nodiscard]] bool canActivatePressurePlate() override;
    
    void move(MovementDirection direction);
};

// ################################# Collectible #####################################################################################

class Collectible : public CubeObject {
};

class Flag : public Collectible {
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    
    [[nodiscard]] bool canPlayerEnter() override;
    [[nodiscard]] bool canAnotherObjectEnter() override;
    [[nodiscard]] bool canEnterPressurePlate() override;
    [[nodiscard]] bool canActivatePressurePlate() override;
    [[nodiscard]] bool isLevelFinishedIfEntered() override;
};