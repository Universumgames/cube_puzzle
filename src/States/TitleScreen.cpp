#include "TitleScreen.hpp"
#include "../CubeGame.hpp"
#include "config.hpp"
#include "../recthelper.hpp"

void TitleScreen::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    versionInfo->changePosition(cubeGame.getWindowSize() - versionInfo->getTextSize() - Point{10, 10});

}

void TitleScreen::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
    SDL_RenderClear(render);
    versionInfo->RenderUI(BASIC_GO_DATA_PASSTHROUGH);

    SDL_RenderPresent(render);
}

Rect TitleScreen::getDrawableGameRect() {
    Point size = cubeGame.getWindowSize();
    return {0,0,size.x, size.y};
}

Rect TitleScreen::getDrawableUIRect() {
    Point size = cubeGame.getWindowSize();
    return {0,0,size.x, size.y};
}

Rect TitleScreen::getGameRenderDst() {
    Point size = cubeGame.getWindowSize();
    return {0,0,size.x, size.y};
}

Rect TitleScreen::getUIRenderDst() {
    Point size = cubeGame.getWindowSize();
    return {0,0,size.x, size.y};
}

void TitleScreen::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type != SDL_KEYDOWN) return;
    const Keysym &what_key = event.key.keysym;
    if(what_key.scancode == SDL_SCANCODE_RETURN){
        cubeGame.returnToLevelSelector();
    }
}

TitleScreen::TitleScreen(CubeGame& game, Renderer *renderer): ComplexGameState(game, renderer) {

}

void TitleScreen::Init() {
    GameState::Init();
    game.SetPerfDrawMode(Game::PerformanceDrawMode::None);
    cubeGame.setWindowName("Qube Puzzle");

    versionInfo = new Text(cubeGame, this, render, 500, "v" VERSION " b" COMMIT_HASH,
            game.getSpriteStorage()->smallFont, {});
    versionInfo->Init();
}

