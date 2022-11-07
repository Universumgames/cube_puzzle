//
// Created by Tom Arlt on 02.11.22.
//

#include "Level.hpp"
#include "../GameObjects/Player.hpp"
#include "../GameObjects/CubeMap.hpp"
#include "../GameObjects/WorldMap.hpp"
#include "../data/EmptyData.hpp"
#include "../recthelper.hpp"
#include "../CubeGame.hpp"

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

u32 lastMSec = 0;
u32 lastFrame = 0;

void Level::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    iterateGameObjects(Update(BASIC_GO_DATA_PASSTHROUGH));
    if (frame % 50 == 0) {
        u32 deltaSec = totalMSec - lastMSec;
        u32 deltaFrame = frame - lastFrame;
        text->changeText("test level " + std::to_string((double)deltaFrame / ((double)deltaSec / 1000)) + "fps");
        lastMSec = totalMSec;
        lastFrame = frame;
    }
}

void Level::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
    SDL_RenderFillRect(render, EntireRect);
    iterateGameObjects(Render(BASIC_GO_DATA_PASSTHROUGH))
    iterateGameObjects(RenderUI(BASIC_GO_DATA_PASSTHROUGH))
    SDL_RenderPresent(render);
}

Level::Level(CubeGame &game, Renderer *render) : ComplexGameState(game, render) {
    this->text = new Text(game, render, 500, "test level", ROBOT_FONT_FILEPATH, 30, {0, 0}, 1, white);
}

void Level::Init() {
    GameState::Init();
    gameObjects.push_back(worldMap);
    gameObjects.push_back(cubeMap);
    gameObjects.push_back(player);
    gameObjects.push_back(text);
    iterateGameObjects(Init())
    game.SetPerfDrawMode(Game::PerformanceDrawMode::None);
}

void Level::UnInit() {
    GameState::UnInit();
    iterateGameObjects(UnInit())
}

LevelData Level::load(const std::string &path, size_t id) {
    worldMap = new WorldMap(cubeGame, render, 10, 10, {WorldField::DEFAULT}, {0, 0});
    cubeMap = new CubeMap(cubeGame, render, {});
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(cubeGame, render);
    return {.path = path, .id = id};
}

LevelData Level::loadTemplateLevel(size_t id) {
    worldMap = new WorldMap(cubeGame, render, emptyWorldFieldSize.x, emptyWorldFieldSize.y, emptyWorldField, {0, 0});
    cubeMap = new CubeMap(cubeGame, render, emptyCubeMapSides);
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(cubeGame, render);
    return {.path = "", .id = id};
}