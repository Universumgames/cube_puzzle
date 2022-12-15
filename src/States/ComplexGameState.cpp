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

void ComplexGameState::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (cubeGame.HandleEvent(event))
            continue;
        HandleEvent(BASIC_GO_DATA_PASSTHROUGH, event);
    }
}
