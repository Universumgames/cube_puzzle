#pragma once

#include "../gamebase.hpp"

class CubeGame;

/// complex game state to store additional CubeGame
class ComplexGameState : public GameState {
public:
    ComplexGameState(CubeGame &cubeGame, Renderer *render);

    virtual Rect getDrawableGameRect() = 0;
    virtual Rect getDrawableUIRect() = 0;

protected:
    virtual Rect getGameRenderDst() = 0;
    virtual Rect getUIRenderDst() = 0;

protected:
    CubeGame &cubeGame;
};