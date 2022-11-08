//
// Created by Tom Arlt on 02.11.22.
//

#include "LevelSelector.hpp"
#include "../CubeGame.hpp"
#include "Level.hpp"
#include "../recthelper.hpp"

void LevelSelector::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event))
            continue;
        if (event.type == SDL_KEYDOWN) {
            const Keysym &what_key = event.key.keysym;
            if (what_key.scancode == SDL_SCANCODE_0) {
                game.SetNextState(1);
            }
        }
    }
}

void LevelSelector::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
}

void LevelSelector::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 200, 0, 255, 255);
    SDL_RenderFillRect(render, EntireRect);

    text->RenderUI(frame, totalMSec, deltaT);

    SDL_RenderPresent(render);
}

LevelSelector::LevelSelector(CubeGame &game, Renderer *render) : ComplexGameState(game, render) {
}

void LevelSelector::loadList() {
//fill levelpaths

    Vector<std::string> paths;

    /// template level
    {
        auto *tempLevel = new Level(cubeGame, render);
        auto levelD = tempLevel->loadTemplateLevel(cubeGame.allStates.size());
        levelData.push_back(levelD);
        cubeGame.allStates.push_back(tempLevel);
    }

// create levels
    for (const auto &path: paths) {
        loadLevel(path);
    }


}

void LevelSelector::drawList() {

}

void LevelSelector::loadLevel(const std::string &path) {
    // add level to allstates of game
    auto *levelX = new Level(cubeGame, render);
    auto level = levelX->load(path, cubeGame.allStates.size());
    levelData.push_back(level);
    cubeGame.allStates.push_back(levelX);
}

void LevelSelector::Init() {
    GameState::Init();
    if (levelsLoaded) return;
    loadList();

    levelsLoaded = true;
    text = new Text(cubeGame, render, 500, "level selector", game.getSpriteStorage()->debugFont, {10, 10}, 1, white);
    text->Init();
}

void LevelSelector::UnInit() {
    GameState::UnInit();
}

void LevelSelector::playLevel(const LevelData &level) {
    game.SetNextState(level.id);
}
