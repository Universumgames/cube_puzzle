#pragma once

#include "../gamebase.hpp"

class CubeGame;

enum class ExitState;

/// complex game state to store additional CubeGame
class ComplexGameState : public GameState {
public:
    ComplexGameState(CubeGame &cubeGame, Renderer *render);

    virtual Rect getDrawableGameRect() = 0;
    virtual Rect getDrawableUIRect() = 0;

    virtual void returnToLevelSelector(ExitState exitState);

protected:
    virtual Rect getGameRenderDst() = 0;
    virtual Rect getUIRenderDst() = 0;

protected:
    CubeGame &cubeGame;
};