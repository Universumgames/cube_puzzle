#include "LevelSelector.hpp"
#include "../CubeGame.hpp"
#include "Level.hpp"
#include "../recthelper.hpp"
#include "../data/paths.hpp"
#include "../data/SpriteStorage.hpp"
#include <utility>
#include <filesystem>
#include "../filehelper.hpp"

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
            /*if (what_key.scancode == SDL_SCANCODE_RIGHT) {
                selectorIndex = (selectorIndex + rows) % (int) levelData.size();
            } else if (what_key.scancode == SDL_SCANCODE_LEFT) {
                selectorIndex = (selectorIndex + 2 * rows) % (int) levelData.size();
            } else */if (what_key.scancode == SDL_SCANCODE_UP) {
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
    debugText->setEnabled(cubeGame.isDebug());
    drawList();
    if (oldSize != game.getWindowSize()) {
        prepareLevelListItems();
        oldSize = game.getWindowSize();
    }
}

void LevelSelector::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
    SDL_RenderClear(render);

    Point headlineSize = headline->getTextSize();
    headline->changePosition({(game.getWindowSize().x - headlineSize.x) / 2, 10});

    int padding = 10;

    Rect drawableListSpace = getListSpace();
    drawableListSpace.y += headline->getDrawedRect().y + headline->getDrawedRect().h + padding;
    drawableListSpace.h -= headline->getDrawedRect().y + headline->getDrawedRect().h + padding;
    int selectWidth = getLevelSelectSize().x;
    int selectHeight = getLevelSelectSize().y;
    Point selectedPos = {drawableListSpace.x, drawableListSpace.y + drawableListSpace.h / 2 - (selectHeight / 2)};

    Rect selectBorder = {selectedPos.x, selectedPos.y, selectWidth, selectHeight};
    drawColoredFilledRect(render, Color{0, 255, 0, 255}, addPadding(selectBorder, -5));

    Point levelRectSize = {selectWidth, selectHeight};
    for (int i = 0; i < levelData.size(); i++) {
        Point rectStartPoint =
                selectedPos + Point{0, (levelRectSize.y + padding) * (i - selectorIndex)};
        Rect drawable = {rectStartPoint.x, rectStartPoint.y, levelRectSize.x, levelRectSize.y};

        SDL_RenderCopy(render, levelData[i].selectorTexture, NULL, &drawable);
    }

    // draw sidebar text
    Rect sideBarSpace = getSideBarSpace(headlineSize.y);
    sideBarText->setMaxWidth(sideBarSpace.w);
    Point sidebarTextSize = sideBarText->getTextSize();
    Rect centeredSideBarTextRect = centerIn(Rect{0, 0, sidebarTextSize.x, sidebarTextSize.y}, sideBarSpace);
    sideBarText->changePosition({centeredSideBarTextRect.x, centeredSideBarTextRect.y});
    sideBarText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);


    debugText->RenderUI(frame, totalMSec, deltaT);
    headline->RenderUI(BASIC_GO_DATA_PASSTHROUGH);


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


    std::sort(levelData.begin(), levelData.end(), LevelData::sort);
}

void LevelSelector::drawList() {
    std::string debugString = "Bitte wähle ein Level aus der Liste\n";
    for (const auto &level: levelData) {
        debugString += std::to_string(level.id) + ": " + level.name + "\n";
    }
    debugText->changeText(debugString);
}

void LevelSelector::Init() {
    GameState::Init();
    oldSize = game.getWindowSize();
    // check if returned from level
    // if level was finished, load to next level, if not proceed
    if (cubeGame.interGameStateData.exitState == ExitState::FINISHED)
        playNextLevel(cubeGame.interGameStateData.sourceStateID);

    // else check if levels are all loaded, if not load them
    if (!levelsLoaded) {
        loadList();
        levelsLoaded = true;
    }
    debugText = new Text(cubeGame, this, render, 500, "level selector", game.getSpriteStorage()->debugFont, {10, 10}, 1,
                         white);
    debugText->Init();

    headline = new Text(cubeGame, this, render, 500, "Select a level to play", game.getSpriteStorage()->basicFont, {});
    headline->Init();

    sideBarText = new Text(cubeGame, this, render, 500,
                           "Use the up/down keys to select a level\n"
                           "Press Enter to start level\n\n"
                           "Want to play the tutorial? Press t",
                           game.getSpriteStorage()->basicFont, {});
    sideBarText->Init();

    prepareLevelListItems();
}

void LevelSelector::UnInit() {
    GameState::UnInit();
}

void LevelSelector::playLevel(const LevelData &level) {
    game.SetNextState(level.allStatesIndex);
    long index = std::find(levelData.begin(), levelData.end(), level) - levelData.begin();
    selectorIndex = (int) index;
}

void LevelSelector::playLevel(int levelId) {
    for (const auto &level: this->levelData) {
        if (levelId == level.id) {
            playLevel(level);
        }
    }
}

void LevelSelector::playNextLevel(int allStatesID) {
    LevelData data;
    for (const auto &level: this->levelData) {
        if (allStatesID == level.allStatesIndex) {
            data = level;
        }
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

void LevelSelector::prepareLevelListItemTexture(LevelData &leveldata, Point drawableRect) {
    if (leveldata.selectorTexture == nullptr) SDL_DestroyTexture(leveldata.selectorTexture);
    leveldata.selectorTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
                                                  SDL_TEXTUREACCESS_TARGET, drawableRect.x, drawableRect.y);
    Texture *oldTarget = SDL_GetRenderTarget(render);
    SDL_SetRenderTarget(render, leveldata.selectorTexture);
    SDL_SetRenderDrawColor(render, 40, 40, 40, 255);
    SDL_RenderClear(render);
    Text *t = new Text(cubeGame, this, render, drawableRect.x - 10 * 2, "", cubeGame.spriteStorage.basicFont, {10, 10});
    t->changeText(std::to_string(leveldata.id) + ". Level \n " + leveldata.name);
    t->setDebug(false);
    t->RenderUI(0, 0, 0);

    SDL_SetRenderTarget(render, oldTarget);
}

void LevelSelector::prepareLevelListItems() {
    Point levelRectSize = getLevelSelectSize();

    for (auto &i: levelData) {
        prepareLevelListItemTexture(i, levelRectSize);
    }
}

Point LevelSelector::getLevelSelectSize() {
    Rect listSpace = getListSpace();
    return {listSpace.w, max(listSpace.h / 10, 100)};
}

Rect LevelSelector::getListSpace() {
    Rect drawableUISpace = getDrawableUISpace();
    drawableUISpace.w = max(100, drawableUISpace.w / 2);
    return drawableUISpace;
}

Rect LevelSelector::getSideBarSpace(int topPadding) {
    Rect drawableUISpace = getDrawableUISpace();
    Rect listSpace = getListSpace();
    return addPadding(
            {drawableUISpace.x + listSpace.w, drawableUISpace.y, drawableUISpace.w - listSpace.w, drawableUISpace.h},
            topPadding, 0, 10, 0);
}

Rect LevelSelector::getDrawableUISpace() {
    return centerIn(addPadding(getDrawableUIRect(), 20), getDrawableUIRect());
}
