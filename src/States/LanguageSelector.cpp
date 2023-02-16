#include "LanguageSelector.hpp"
#include "../recthelper.hpp"

void LanguageSelector::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (skip) {
        return;
    }
    for(auto lang: languages){
        Rect flagPos = getDrawRectForLanguage(lang.first);
        lang.second->setLocation({flagPos.x, flagPos.y});
        lang.second->setSize({flagPos.w, flagPos.h});
        lang.second->Update(BASIC_GO_DATA_PASSTHROUGH);
    }
}

void LanguageSelector::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 40, 40, 40, 255);
    SDL_RenderClear(render);
    if (skip) {
        SDL_RenderPresent(render);
        return;
    }
    for(auto lang: languages){
        lang.second->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    }
    SDL_RenderPresent(render);
}

Rect LanguageSelector::getDrawableGameRect() {
    Point wSize = game.getWindowSize();
    return {0, 0, wSize.x, wSize.y};
}

Rect LanguageSelector::getDrawableUIRect() {
    return getDrawableGameRect();
}

Rect LanguageSelector::getGameRenderDst() {
    return getDrawableGameRect();
}

Rect LanguageSelector::getUIRenderDst() {
    return getDrawableGameRect();
}

LanguageSelector::LanguageSelector(CubeGame &cubeGame, Renderer *render) : ComplexGameState(cubeGame, render) {
    skip = false;
}

void LanguageSelector::Init() {
    GameState::Init();

    auto language = getLanguage();
    if (language != Language::UNDEFINED) {
        continueGame();
        skip = true;
    }

    cubeGame.touchController->setScene(TouchController::TouchScene::RUDIMENTARY);

    en = new UIButton(cubeGame, this, render, {0,0}, {0,0}, getTextureForLang(Language::ENGLISH));
    en->setPressedLambda([&](CubeGame &game, TouchObject *downButton) {
        setLanguage(Language::ENGLISH);
        this->continueGame();
    });
    de = new UIButton(cubeGame, this, render, {0,0}, {0,0}, getTextureForLang(Language::GERMAN));
    de->setPressedLambda([&](CubeGame &game, TouchObject *downButton) {
        setLanguage(Language::GERMAN);
        this->continueGame();
    });

    languages = {
            {Language::ENGLISH, en},
            {Language::GERMAN, de}
    };
}

Rect LanguageSelector::getDrawRectForLanguage(Language lang) {
    int count = getAvailableLanguages().size();
    int id = getLanguageIndex(lang);

    Rect allowedRect = addPadding(getDrawableGameRect(), 50);
    int width = (allowedRect.w / 2) - 10;
    Point flagSize = {width, (int) ((2.0 / 3.0) * width)};

    int columnId = id % 2;
    int rowId = id / 2;

    return {allowedRect.x + columnId * flagSize.x, allowedRect.y + rowId * flagSize.y, flagSize.x, flagSize.y};
}

Texture *LanguageSelector::getTextureForLang(Language lang) {
    switch (lang) {
        case Language::ENGLISH:
            return cubeGame.getSpriteStorage()->flagEnglish;
        case Language::GERMAN:
            return cubeGame.getSpriteStorage()->flagGerman;
        default:
            return nullptr;
    }
}

void LanguageSelector::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    ComplexGameState::HandleEvent(frame, totalMSec, deltaT, event);
    for(auto lang : languages){
        lang.second->HandleEvent(BASIC_GO_DATA_PASSTHROUGH, event);
    }
}

void LanguageSelector::continueGame() {
    cubeGame.SetNextState(TITLESCREEN_ID);
}
