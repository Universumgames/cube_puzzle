//
// Created by Tom Arlt on 02.11.22.
//

#include "Level.hpp"
#include "../GameObjects/Player.hpp"
#include "../GameObjects/CubeMap.hpp"
#include "../GameObjects/WorldMap.hpp"
#include "../data/EmptyData.hpp"

#define iterateGameObjects(method) for(auto gameobject:gameObjects){gameobject->method;}

void Level::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event))
            continue;
        iterateGameObjects(HandleEvent(BASIC_GO_DATA_PASSTHROUGH, event));
    }
}

void Level::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    iterateGameObjects(Update(BASIC_GO_DATA_PASSTHROUGH));
}

void Level::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    iterateGameObjects(Render(BASIC_GO_DATA_PASSTHROUGH))
    iterateGameObjects(RenderUI(BASIC_GO_DATA_PASSTHROUGH))
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

LevelData Level::load(const std::string &path, size_t id) {
    worldMap = new WorldMap(game, render, 10, 10, {WorldField::DEFAULT}, {0,0});
    cubeMap = new CubeMap(game, render, {});
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(game, render);
    return {.path = path, .id = id};
}

LevelData Level::loadTemplateLevel() {
    worldMap = new WorldMap(game, render, emptyWorldFieldSize.x, emptyWorldFieldSize.y, emptyWorldField, {0,0});
    cubeMap = new CubeMap(game, render, emptyCubeMapSides);
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(game, render);
    return {.path = "", .id = 1};
}
