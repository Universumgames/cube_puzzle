#include "LevelSelector.hpp"
#include "../CubeGame.hpp"
#include "Level.hpp"
#include "../recthelper.hpp"
#include "../data/paths.hpp"
#include "../data/SpriteStorage.hpp"
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

            // navigation in selector
            if (what_key.scancode == SDL_SCANCODE_RIGHT) {

            } else if (what_key.scancode == SDL_SCANCODE_LEFT) {

            } else if (what_key.scancode == SDL_SCANCODE_UP) {
                selectorIndex--;
            } else if (what_key.scancode == SDL_SCANCODE_DOWN) {
                selectorIndex++;
            } else if (what_key.scancode == SDL_SCANCODE_KP_ENTER || what_key.scancode == SDL_SCANCODE_RETURN) {
                playLevel(levelData[selectorIndex]);
            }
            selectorIndex = (int) (max(0, selectorIndex) % levelData.size());
        }
    }
}

void LevelSelector::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    text->setEnabled(cubeGame.isDebug());
    drawList();
}

void LevelSelector::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 200, 0, 255, 255);
    SDL_RenderFillRect(render, EntireRect);

    int padding = 10;

    Point listStartPoint = {0, 0};
    Rect drawableUISpace = centerIn(addPadding(getDrawableUIRect(), 20), getDrawableUIRect());
    SDL_SetRenderDrawColor(render, 200, 255, 0, 255);
    SDL_RenderFillRect(render, &drawableUISpace);
    Point usedListStartPoint = {listStartPoint.x + drawableUISpace.x, listStartPoint.y + drawableUISpace.y};
    int selectWidth = max(drawableUISpace.w / 2, 100);
    int selectHeight = max(drawableUISpace.h / 6, 50);
    cout << selectHeight << endl;
    rows = drawableUISpace.h / selectHeight;
    cout << rows << endl;
    selectHeight = drawableUISpace.h / rows;
    cout << selectHeight<< endl;
    columns = 2;

    Point levelRectSize = {selectWidth, selectHeight};

    // TODO fix display list when overflowing window
    int column = 0;
    for (int i = 0; i < levelData.size(); i++) {
        int row = i % rows;
        Point rectStartPoint =
                usedListStartPoint + Point{(levelRectSize.x + padding) * column, (levelRectSize.y + padding) * row};
        Rect drawable = {rectStartPoint.x, rectStartPoint.y, levelRectSize.x, levelRectSize.y};

        if (selectorIndex == i) {
            drawColoredFilledRect(render, Color{0, 255, 0, 255}, addPadding(drawable, -5));
        }
        if(row == rows -1) column ++;

        SDL_RenderCopy(render, levelData[i].selectorTexture, NULL, &drawable);
    }


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

    for(int i = 0; i < 5; i++) {
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

    std::sort(levelData.begin(), levelData.end(), LevelData::sort);
}

void LevelSelector::drawList() {
    std::string debugString = "Bitte wähle ein Level aus der Liste\n";
    for (const auto &level: levelData) {
        debugString += std::to_string(level.id) + ": " + level.name + "\n";
    }
    text->changeText(debugString);
}

void LevelSelector::Init() {
    GameState::Init();
    // check if returned from level
    // if level was finished, load to next level, if not proceed
    if (cubeGame.interGameStateData.exitState == ExitState::FINISHED)
        playNextLevel(cubeGame.interGameStateData.sourceStateID);

    // else check if levels are all loaded, if not load them
    if (!levelsLoaded) {
        loadList();
        levelsLoaded = true;
    }
    text = new Text(cubeGame, this, render, 500, "level selector", game.getSpriteStorage()->debugFont, {10, 10}, 1,
                    white);
    text->Init();

    int padding = 10;

    Point listStartPoint = {0, 0};
    Rect drawableUISpace = centerIn(addPadding(getDrawableUIRect(), 20), getDrawableUIRect());
    Point usedListStartPoint = {listStartPoint.x + drawableUISpace.x, listStartPoint.y + drawableUISpace.y};
    Point levelRectSize = {max(100, drawableUISpace.w / columns), max(50, drawableUISpace.h / rows)};

    for (int i = 0; i < levelData.size(); i++) {
        int row = i % rows, column = i / columns;
        Point rectStartPoint =
                usedListStartPoint + Point{(levelRectSize.x + padding) * column, (levelRectSize.y + padding) * row};
        Rect drawable = {rectStartPoint.x, rectStartPoint.y, levelRectSize.x, levelRectSize.y};

        prepareLevelListItemTexture(levelData[i], drawable);

    }
}

void LevelSelector::UnInit() {
    GameState::UnInit();
}

void LevelSelector::playLevel(const LevelData &level) {
    game.SetNextState(level.allStatesIndex);
}

void LevelSelector::playLevel(int levelId) {
    for (const auto &level: levelData) {
        if (levelId == level.id) game.SetNextState(level.allStatesIndex);
    }
}

void LevelSelector::playNextLevel(int allStatesID) {
    LevelData data;
    for (const auto &level: levelData) {
        if (allStatesID == level.allStatesIndex) data = level;
    }
    playLevel(data.id + 1);
}

Rect LevelSelector::getDrawableGameRect() {
    Point wSize = game.getWindowSize();
    return {0, 0, wSize.x, wSize.y};
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

void LevelSelector::prepareLevelListItemTexture(LevelData &leveldata, Rect drawableRect) {
    leveldata.selectorTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
                                                  SDL_TEXTUREACCESS_TARGET, drawableRect.w, drawableRect.h);
    Texture *oldTarget = SDL_GetRenderTarget(render);
    SDL_SetRenderTarget(render, leveldata.selectorTexture);
    SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
    SDL_RenderFillRect(render, NULL);
    Text *t = new Text(cubeGame, this, render, drawableRect.w - 10 * 2, "", cubeGame.spriteStorage.basicFont, {10, 10});
    t->changeText(std::to_string(leveldata.id) + ". Level \n " + leveldata.name);
    t->RenderUI(0, 0, 0);

    SDL_SetRenderTarget(render, oldTarget);
}
