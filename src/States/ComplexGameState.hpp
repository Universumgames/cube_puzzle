#pragma once

#include "../gamebase.hpp"

class CubeGame;

/// complex game state to store additional CubeGame
class ComplexGameState : public GameState {
public:
    ComplexGameState(CubeGame &cubeGame, Renderer *render);

protected:
    CubeGame &cubeGame;
};