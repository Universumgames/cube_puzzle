#include "LevelSelector.hpp"
#include "../CubeGame.hpp"
#include "Level.hpp"
#include "../recthelper.hpp"
#include "../data/paths.hpp"

#include <utility>
#include <filesystem>
#include "../filehelper.hpp"

#define FIRST_ROW 1

void LevelSelector::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event)) // es wurde sich bereits um das Event gekümmert, deswegen nächstes Event laden
            continue;
        if (event.type == SDL_KEYDOWN) {
            const Keysym &what_key = event.key.keysym;
            if (what_key.scancode >= SDL_SCANCODE_1 && what_key.scancode < SDL_SCANCODE_0) {
                int id = what_key.scancode + 1 - SDL_SCANCODE_1;
                    playLevel(id);
            } else if (what_key.scancode >= SDL_SCANCODE_KP_1 && what_key.scancode < SDL_SCANCODE_KP_0) {
                int id = what_key.scancode + 1 - SDL_SCANCODE_KP_1;
                    playLevel(id);
            }
        }
    }
}

void LevelSelector::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawList();
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

    /// first add template level
    {
        auto *tempLevel = new Level(cubeGame, render);
        auto levelD = tempLevel->loadTemplateLevel(cubeGame.allStates.size());
        levelData.push_back(levelD);
        cubeGame.allStates.push_back(tempLevel);
    }

    const std::filesystem::path levels{LEVELS_DIR};

    if (!std::filesystem::exists(levels)) {
        return;
    }

    // level loading
    for (auto const &dirEntry: std::filesystem::directory_iterator{levels}) {
        std::string path = dirEntry.path().string();
        if (!dirEntry.is_regular_file() || dirEntry.path().extension() != ".level") {
            continue;
        }
        auto data = LevelLoader::loadLevel(path);
        auto *levelX = new Level(cubeGame, render);
        auto levelD = levelX->load(data, cubeGame.allStates.size());
        levelData.push_back(levelD);
        cubeGame.allStates.push_back(levelX);
    }
}

void LevelSelector::drawList() {
    std::string debugString = "Bitte wähle ein Level aus der Liste\n";
    for (auto level: levelData) {
        debugString += std::to_string(level.id) + ": " + level.name + "\n";
    }
    text->changeText(debugString);
}

void LevelSelector::Init() {
    GameState::Init();
    if (levelsLoaded)
        return;
    loadList();

    levelsLoaded = true;
    text = new Text(cubeGame, this, render, 500, "level selector", game.getSpriteStorage()->debugFont, {10, 10}, 1, white);
    text->Init();
}

void LevelSelector::UnInit() {
    GameState::UnInit();
}

void LevelSelector::playLevel(const LevelData &level) {
    game.SetNextState(level.allStatesIndex);
}

void LevelSelector::playLevel(int levelId){
    for(auto level: levelData){
        if(levelId == level.id) game.SetNextState(level.allStatesIndex);
    }
}

Map<int, Map<int, Vector<WorldField::WorldFieldEnum>>> LevelSelector::getLevelDataMap(std::string &fileString) {
    Map<int, Map<int, Vector<WorldField::WorldFieldEnum>>> levelDataMap;
    Map<int, Vector<WorldField::WorldFieldEnum>> cubeSide1, cubeSide2, cubeSide3, cubeSide4, cubeSide5, cubeSide6;
    int cubeSide = 5;
    int rowOfCubeSide = FIRST_ROW;
    Vector<WorldField::WorldFieldEnum> row;
    for (char &c: fileString) {
        if (c == ';' || c == ',' || c == '-') {
            switch (cubeSide) {
                case 1:
                    cubeSide1.insert_or_assign(rowOfCubeSide, row);
                    break;
                case 2:
                    cubeSide2.insert_or_assign(rowOfCubeSide, row);
                    break;
                case 3:
                    cubeSide3.insert_or_assign(rowOfCubeSide, row);
                    break;
                case 4:
                    cubeSide4.insert_or_assign(rowOfCubeSide, row);
                    break;
                case 5:
                    cubeSide5.insert_or_assign(rowOfCubeSide, row);
                    break;
                case 6:
                    cubeSide6.insert_or_assign(rowOfCubeSide, row);
                    break;
            }
            row.clear();
            if (c == ';') {
                rowOfCubeSide = FIRST_ROW;
                if (cubeSide == 5) {
                    cubeSide = 4;
                } else if (cubeSide == 3) {
                    cubeSide = 2;
                } else {
                    cubeSide = 6;
                }
            } else if (c == ',') {
                if (cubeSide == 4) {
                    cubeSide = 1;
                } else if (cubeSide == 1) {
                    cubeSide = 3;
                }
            } else if (c == '-') {
                rowOfCubeSide++;
                if (cubeSide == 3) {
                    cubeSide = 4;
                }
            }
        } else {
            row.emplace_back(WorldField::convertCharToEnum(c));
        }
    }
    levelDataMap.insert_or_assign(1, cubeSide1);
    levelDataMap.insert_or_assign(2, cubeSide2);
    levelDataMap.insert_or_assign(3, cubeSide3);
    levelDataMap.insert_or_assign(4, cubeSide4);
    levelDataMap.insert_or_assign(5, cubeSide5);
    levelDataMap.insert_or_assign(6, cubeSide6);

    return levelDataMap;
}

Rect LevelSelector::getDrawableGameRect() {
    Point wSize = game.getWindowSize();
    return {0,0, wSize.x, wSize.y};
}

Rect LevelSelector::getDrawableUIRect() {
    return getDrawableGameRect();
}

Rect LevelSelector::getGameRenderDst() {
    return getDrawableGameRect();
}

Rect LevelSelector::getUIRenderDst() {
    return getDrawableUIRect();
}
