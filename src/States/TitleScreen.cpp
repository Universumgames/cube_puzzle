#include "TitleScreen.hpp"
#include "../CubeGame.hpp"
#include "config.hpp"
#include "../recthelper.hpp"
#include "../GameObjects/CubeMap.hpp"

void TitleScreen::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    TutorialLevel::Update(BASIC_GO_DATA_PASSTHROUGH);
    versionInfo->changePosition(cubeGame.getWindowSize() - versionInfo->getTextSize() - Point{10, 10});
    if(totalMSec - lastMovementTotalMSec > 1000) {
        int dir;
        solution >> dir;
        cubeMap->movePlayer((PlayerMoveDirection) dir, {});
        lastMovementTotalMSec = totalMSec;
    }
}


void TitleScreen::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type != SDL_KEYDOWN) return;
    const Keysym &what_key = event.key.keysym;
    if(what_key.scancode == SDL_SCANCODE_RETURN){
        cubeGame.returnToLevelSelector();
    }
}

TitleScreen::TitleScreen(CubeGame& game, Renderer *renderer): TutorialLevel(game, renderer) {
    auto level = LevelLoader::loadTutLevel(TITLESCREEN_LEVEL_PATH);
    TutorialLevel::load(level, cubeGame.allStates.size());
    solution = std::ifstream(TITLESCREEN_LEVEL_SOLUTION_PATH);
}

void TitleScreen::Init() {
    TutorialLevel::Init();
    game.SetPerfDrawMode(Game::PerformanceDrawMode::None);
    cubeGame.setWindowName("Qube Puzzle");

    versionInfo = new Text(cubeGame, this, render, 500, "v" VERSION " " COMMIT_HASH,
            game.getSpriteStorage()->smallFont, {});
    versionInfo->Init();
    versionInfo->setDebug(true);
}

void TitleScreen::internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) {
    TutorialLevel::internalUIRender(frame, totalMSec, deltaT);
    versionInfo->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

