#pragma once

#include "../gamebase.hpp"
#include "../TouchController.hpp"

class CubeGame;

enum class ExitState;

/// complex game state to store additional CubeGame
class ComplexGameState : public GameState {
public:
    ComplexGameState(CubeGame &cubeGame, Renderer *render);

    virtual Rect getDrawableGameRect() = 0;
    virtual Rect getDrawableUIRect() = 0;

    virtual void returnToLevelSelector(ExitState exitState);

    virtual void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event){}

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void afterRenderCycle();

protected:
    virtual Rect getGameRenderDst() = 0;
    virtual Rect getUIRenderDst() = 0;

protected:
    CubeGame &cubeGame;
};