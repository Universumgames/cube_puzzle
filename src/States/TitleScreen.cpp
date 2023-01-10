#include "TitleScreen.hpp"
#include "../CubeGame.hpp"
#include "config.hpp"
#include "../recthelper.hpp"
#include "../GameObjects/CubeMap.hpp"
#include "../data/RemoteLevelFetch.hpp"

void TitleScreen::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    TutorialLevel::Update(BASIC_GO_DATA_PASSTHROUGH);
    versionInfo->changePosition(cubeGame.getWindowSize() - versionInfo->getTextSize() - Point{10, 10});
    remoteLevelLoading->changePosition(cubeGame.getWindowSize() - remoteLevelLoading->getTextSize() - Point{10, 30});
    remoteLevelLoading->changeText(
            RemoteLevelFetch::getInstance()->getLoadingState() == RemoteLevelFetch::LoadingState::FINISHED ?
            std::to_string(RemoteLevelFetch::getInstance()->getLevelData().size()) + " remote Levels successfully loaded" :
            RemoteLevelFetch::getInstance()->getLoadingStateString() + " loading remote levels");
    if (totalMSec - lastMovementTotalMSec > 1000) {
        int dir;
        solution >> dir;
        cubeMap->movePlayer((PlayerMoveDirection) dir, {});
        lastMovementTotalMSec = totalMSec;
    }
    startButton->setSize(game.getWindowSize());
}


void TitleScreen::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    const Keysym &what_key = event.key.keysym;
    if ((event.type == SDL_KEYDOWN && what_key.scancode == SDL_SCANCODE_RETURN) && RemoteLevelFetch::getInstance()->getLoadingState() != RemoteLevelFetch::LoadingState::LOADING) {
        cubeGame.returnToLevelSelector();
    }
}

TitleScreen::TitleScreen(CubeGame &game, Renderer *renderer) : TutorialLevel(game, renderer) {
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

    remoteLevelLoading = new Text(cubeGame, this, render, 500, "", game.getSpriteStorage()->smallFont, {});
    remoteLevelLoading->Init();

    startButton = new TouchObject(cubeGame, this, render, {0, 0}, {0, 0});
    startButton->Init();

    cubeGame.touchController->setScene(TouchController::TouchScene::RUDIMENTARY);
}

void TitleScreen::internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) {
    TutorialLevel::internalUIRender(frame, totalMSec, deltaT);
    versionInfo->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    remoteLevelLoading->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    Rect dst = Level::getDrawableUIRect();
    dst = scale(dst, 0.5);
    dst = addPadding(dst, 20);
    dst = centerInVertical(dst, Level::getDrawableUIRect());
    SDL_RenderCopy(render, game.getSpriteStorage()->titleScreenGameBanner, NULL, &dst);
    startButton->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

