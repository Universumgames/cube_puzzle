//
// Created by Tom Arlt on 02.11.22.
//

#include "CubeGame.hpp"
#include "States/LevelSelector.hpp"
#include "States/Level.hpp"
#include "States/LevelSelector.hpp"


CubeGame::CubeGame() : Game("CubeGame")
{
    loadSprites();
    // Level selctor loads all levels and adds them procedually to the states
    allStates = {new LevelSelector(*this, render)};
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
        else if(what_key.scancode == SDL_SCANCODE_F3){
            debugView = !debugView;
            return true;
        }
        break;
    }

    default:
        break;
    }
    return Game::HandleEvent(event);
}

SpriteStorage *CubeGame::getSpriteStorage() {
    return &spriteStorage;
}

#define loadPNGTexture(variable, render, path) { \
                                        Surface* temp = IMG_Load(path); \
                                        variable =  SDL_CreateTextureFromSurface(render, temp); \
                                        SDL_FreeSurface(temp);         \
                                        }

#define loadFont(variable, path, size) { \
                                        variable = TTF_OpenFont(path, size);         \
                                        }

void CubeGame::loadSprites() {
    loadPNGTexture(spriteStorage.temp, render, "./asset/graphic/AllTestTileWater.png")
    loadFont(spriteStorage.basicFont, ROBOTO_FONT_FILEPATH, 30)
}

bool CubeGame::isDebug() {
    return debugView;
}
