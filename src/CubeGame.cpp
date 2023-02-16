#include "CubeGame.hpp"
#include "States/LevelSelector.hpp"
#include "States/Level.hpp"
#include "States/LevelSelector.hpp"
#include "States/TitleScreen.hpp"
#include "touchhelper.hpp"
#include "TouchController.hpp"
#include "States/LanguageSelector.hpp"

#define loadPNGTexture(variable, render, path)                 \
    {                                                          \
        Surface *temp = IMG_Load(path);                        \
        variable = SDL_CreateTextureFromSurface(render, temp); \
        SDL_FreeSurface(temp);                                 \
    }

#define loadFont(variable, path, size)       \
    {                                        \
        variable = TTF_OpenFont(path, size); \
    }

CubeGame::CubeGame() : Game("CubeGame") {
    loadSprites();
    // Level selector loads all levels and adds them procedually to the states
    auto languageSelector = new LanguageSelector(*this, render);
    auto titleScreen = new TitleScreen(*this, render);
    auto levelSelector = new LevelSelector(*this, render);
    titleScreen->levelSelector = levelSelector;
    allStates = {languageSelector, titleScreen, levelSelector};
    audioHandler = AudioHandler::getInstance();
    audioHandler->init();
    backgroundMusic = new AudioPlayer(MUSIC_BACKGROUND_PATH);
    backgroundMusic->playLoop();
    language = getLanguage();
    SetNextState(0);

    touchController = new TouchController(*this, titleScreen, render);

    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    setWindowIcon();
    // audioHandler.playBackground();
}

bool CubeGame::HandleEvent(const Event event) {
    bool handled = false;
    // Global Keybindings, will be checked before the ones in GameState
    switch (event.type) {
        case SDL_KEYDOWN: {
            const Keysym &what_key = event.key.keysym;
            if (what_key.scancode == SDL_SCANCODE_ESCAPE) {
                interGameStateData = {.sourceStateID = currentStateIdx, .exitState = ExitState::CANCELLED};
                returnToLevelSelector();
                handled = true;
            }
            if (what_key.scancode == SDL_SCANCODE_F3) {
                debugView = !debugView;
                handled = true;
            }
            if (what_key.scancode == SDL_SCANCODE_M) {
                audioHandler->enableAudio(!audioHandler->getAudioEnabled());
                handled = true;
            }
            break;
        }
        case SDL_FINGERMOTION: {
            cout << "MOVE id: " << event.tfinger.fingerId << " x: " << event.tfinger.x << " y: " << event.tfinger.y
                 << " dx: " << event.tfinger.dx << " dy: " << event.tfinger.dy << endl;
            break;
        }
        case SDL_FINGERUP: {
            cout << "UP id: " << event.tfinger.fingerId << " x: " << event.tfinger.x << " y: " << event.tfinger.y
                 << endl;
            auto pos = uvToPixel(getWindowSize(), FPoint{event.tfinger.x, event.tfinger.y});
            cout << "UP PIXEL x: " << pos.x << " y: " << pos.y << endl;
            break;
        }
        case SDL_FINGERDOWN: {
            cout << "DOWN id: " << event.tfinger.fingerId << " x: " << event.tfinger.x << " y: " << event.tfinger.y
                 << endl;
            break;
        }
        default:
            break;
    }
    touchController->HandleEvent(0, 0, 0, event);
    if (handled)
        return true;
    return Game::HandleEvent(event);
}

SpriteStorage *CubeGame::getSpriteStorage() {
    return &spriteStorage;
}

void CubeGame::loadSprites() {
    loadFont(spriteStorage.basicFont, ROBOTO_FONT_FILEPATH, 30);
    loadFont(spriteStorage.smallFont, ROBOTO_FONT_LIGHT_FILEPATH, 18);
    loadFont(spriteStorage.debugFont, ROBOTO_FONT_LIGHT_FILEPATH, 18);
    loadPNGTexture(spriteStorage.playerSpriteSheet, render, PLAYER_SPRITE_SHEET_PATH);
    loadPNGTexture(spriteStorage.arrowSemiCircle, render, ARROW_SEMICIRCLE_PATH);
    loadPNGTexture(spriteStorage.arrowStraight, render, ARROW_STRAIGHT_PATH);
    loadPNGTexture(spriteStorage.cubeFieldSpriteSheet, render, CUBE_FIELD_PATH);
    loadPNGTexture(spriteStorage.cubeObjectSpriteSheet, render, CUBE_OBJECT_PATH);
    loadPNGTexture(spriteStorage.titleScreenGameBanner, render, TITLESCREEEN_GAME_BANNER_PATH);

    loadPNGTexture(spriteStorage.touchArrow, render, TOUCH_ARROW_PATH);
    loadPNGTexture(spriteStorage.touchExit, render, TOUCH_EXIT_PATH);
    loadPNGTexture(spriteStorage.touchMusic, render, TOUCH_MUSIC_PATH);
    loadPNGTexture(spriteStorage.touchTutorial, render, TOUCH_TUTORIAL_PATH);
    loadPNGTexture(spriteStorage.touchGrab, render, TOUCH_SHIFT_PATH);
    loadPNGTexture(spriteStorage.touchEnter, render, TOUCH_ENTER_PATH);

    loadPNGTexture(spriteStorage.flagEnglish, render, FLAG_PATH(Language::ENGLISH));
    loadPNGTexture(spriteStorage.flagGerman, render, FLAG_PATH(Language::GERMAN))

    for (int i = 1; i <= 6; i++) {
        Texture *tmp;
        loadPNGTexture(tmp, render, CUBE_SIDE_BACKGROUND_PATH(i));
        spriteStorage.sideSprites.push_back(tmp);
    }
}

bool CubeGame::isDebug() const {
    return debugView;
}

void CubeGame::setWindowIcon() {
    SetPerfDrawMode(PerformanceDrawMode::Title);
    Surface *icon = IMG_Load(ICON_PATH);
    SDL_SetWindowIcon(window, icon);
}

Point CubeGame::getCurrentRenderTargetSize() {
    int w = 0, h = 0;
    SDL_GetRendererOutputSize(render, &w, &h);
    return {w, h};
}

void CubeGame::returnToLevelSelector() {
    SetNextState(LEVEL_SELECTOR_ID);
}

void CubeGame::setWindowName(std::string windowName) {
    SDL_SetWindowTitle(window, windowName.c_str());
}

Language CubeGame::getLanguage() const {
    return language;
}
