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
    }
}

void LevelSelector::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
}

void LevelSelector::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 200, 0, 255, 255);
    SDL_RenderFillRect(render, EntireRect);
    SDL_RenderPresent(render);
}

LevelSelector::LevelSelector(CubeGame &game, Renderer *render) : GameState(game, render) {
}

void LevelSelector::loadList() {
//fill levelpaths

    Vector<std::string> paths;

    /// template level
    {
        auto* tempLevel = new Level(game, render);
        auto levelD = tempLevel->loadTemplateLevel();
        levelData.push_back(levelD);
        getCubeGame().allStates.push_back(tempLevel);
    }

// create levels
    for (auto path: paths) {
        loadLevel(path);
    }


}

void LevelSelector::drawList() {

}

void LevelSelector::loadLevel(const std::string &path) {
    // add level to allstates of game
    auto *levelX = new Level(game, render);
    auto level = levelX->load(path, getCubeGame().allStates.size());
    levelData.push_back(level);
    getCubeGame().allStates.push_back(levelX);
}

void LevelSelector::Init() {
    GameState::Init();
    if (levelsLoaded) return;
    loadList();

    levelsLoaded = true;
}

void LevelSelector::UnInit() {
    GameState::UnInit();
}

void LevelSelector::playLevel(const LevelData& level) {
    game.SetNextState(level.id);
}
