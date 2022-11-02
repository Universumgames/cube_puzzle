//
// Created by Tom Arlt on 02.11.22.
//

#include "CubeGame.hpp"
#include "States/LevelSelector.hpp"
#include "States/Level.hpp"

CubeGame::CubeGame() : Game("CubeGame")
{
    allStates = {
        new LevelSelector(*this, render),
        new Level(*this, render)};
    SetNextState(0);
    SetPerfDrawMode(PerformanceDrawMode::Title);
}

bool CubeGame::HandleEvent(const Event event)
{
    // Global Keybindings, will be checked before the ones in GameState
    switch (event.type)
    {
    case SDL_KEYDOWN:
    {
        const Keysym &what_key = event.key.keysym;

        if (what_key.scancode == SDL_SCANCODE_1)
        {
            // Both Game and GameState can change the State,
            // but it will only be changed after the current frame
            SetNextState(0);
            return true;
        }
        else if (what_key.scancode == SDL_SCANCODE_2)
        {
            SetNextState(1);
            return true;
        }
        break;
    }

    default:
        break;
    }
    return Game::HandleEvent(event);
}