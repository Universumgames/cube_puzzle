#pragma once

#include "../CubeGame.hpp"
#include "GameObject.hpp"
#include "CubeField.hpp"

class CubeObject {
protected:
    DiceData *diceData;
    int sideId;
    
public:
    /// render only method
    // TODO Mina ich brauche irgendeine Möglichkeit um auf diceData und die Würfelseite (beispiel sideID in CubeMapSide)
    virtual void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT);
    /// Handle input events, probably unused
    virtual void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event);
    /// physics, etc. update method
    // TODO mina hier wirst GARANTIERT auch die Dice Data und sideID benötigen.....
    virtual void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT);

    void setDiceData(DiceData* dice_data);
    void setSideId(int sideID);
    
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
    bool isFalling = false;
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    
    [[nodiscard]] bool canPlayerEnter() override;
    [[nodiscard]] bool canAnotherObjectEnter() override;
    [[nodiscard]] bool canEnterPressurePlate() override;
    [[nodiscard]] bool canActivatePressurePlate() override;
};

// ################################# Activatable #####################################################################################

class Activatable : public CubeObject {
};

class Slider : public Activatable {
public:
    enum class MovementDirectionIfActivated {
        moveToBigX,
        moveToSmallX,
        moveToBigY,
        moveToSmallY
    };
private:
    int id;
    bool isActivated = false;
    MovementDirectionIfActivated direction;
public:
    Slider() = delete;
    explicit Slider(MovementDirectionIfActivated movementDirectionEnum, int id, bool activated = false);
    
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    
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
public:
    void Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) override;
    
    [[nodiscard]] bool canPlayerEnter() override;
    [[nodiscard]] bool canAnotherObjectEnter() override;
    [[nodiscard]] bool canEnterPressurePlate() override;
    [[nodiscard]] bool canActivatePressurePlate() override;
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