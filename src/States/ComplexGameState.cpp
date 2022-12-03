//
// Created by Tom Arlt on 07.11.22.
//
#include "ComplexGameState.hpp"
#include "../CubeGame.hpp"

ComplexGameState::ComplexGameState(CubeGame &cubeGame, Renderer *render) : GameState(cubeGame, render),
                                                                           cubeGame(cubeGame) {
}

void ComplexGameState::returnToLevelSelector(ExitState exitState) {
    cubeGame.interGameStateData.exitState = exitState;
    cubeGame.interGameStateData.sourceStateID = -1;
    cubeGame.returnToLevelSelector();
}
