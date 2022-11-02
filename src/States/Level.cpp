//
// Created by Tom Arlt on 02.11.22.
//

#include "Level.hpp"

#define iterateGameObjects(method) for(auto gameobject:gameObjects){gameobject->method;}

void Level::Events(const u32 frame, const u32 totalMSec, const float deltaT)
{
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event))
    {
        if (game.HandleEvent(event))
            continue;
        iterateGameObjects(HandleEvent(frame, totalMSec, deltaT, event));
    }
}

void Level::Update(const u32 frame, const u32 totalMSec, const float deltaT)
{
    iterateGameObjects(Update(frame, totalMSec, deltaT));
}

void Level::Render(const u32 frame, const u32 totalMSec, const float deltaT)
{
    iterateGameObjects(Render(frame, totalMSec, deltaT))
    iterateGameObjects(RenderUI(frame, totalMSec, deltaT))
}

Level::Level(Game &game, Renderer *render) : GameState(game, render) {}

void Level::Init() {
    GameState::Init();
    iterateGameObjects(Init())
}

void Level::UnInit() {
    GameState::UnInit();
    iterateGameObjects(UnInit())
}
