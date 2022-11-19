#include "CubeGame.hpp"
#include "States/LevelSelector.hpp"
#include "States/Level.hpp"
#include "States/LevelSelector.hpp"

#define loadPNGTexture(variable, render, path) { \
                                        Surface* temp = IMG_Load(path); \
                                        variable =  SDL_CreateTextureFromSurface(render, temp); \
                                        SDL_FreeSurface(temp);         \
                                        }

#define loadFont(variable, path, size) { \
                                        variable = TTF_OpenFont(path, size);         \
                                        }

CubeGame::CubeGame() : Game("CubeGame") {
    loadSprites();
    // Level selctor loads all levels and adds them procedually to the states
    allStates = {new LevelSelector(*this, render)};
    SetNextState(0);

    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    setWindowIcon();
    //audio.playBackground();
}

bool CubeGame::HandleEvent(const Event event) {
    // Global Keybindings, will be checked before the ones in GameState
    switch (event.type) {
        case SDL_KEYDOWN: {
            const Keysym &what_key = event.key.keysym;
            if (what_key.scancode == SDL_SCANCODE_0) {
                SetNextState(0);
                return true;
            }
            if (what_key.scancode == SDL_SCANCODE_F3) {
                debugView = !debugView;
                return true;
            }
            if (what_key.scancode == SDL_SCANCODE_M) {
                if (audio.playing) audio.pauseMusic();
                else audio.playBackground();
            }
            break;
        }

        default:
            break;
    }
    return Game::HandleEvent(event);
}

SpriteStorage *CubeGame::getSpriteStorage() {
    return &spriteStorage;
}

void CubeGame::loadSprites() {
    loadPNGTexture(spriteStorage.temp, render, "./asset/graphic/AllTestTileWater.png")
    loadFont(spriteStorage.basicFont, ROBOTO_FONT_FILEPATH, 30)
    loadFont(spriteStorage.debugFont, ROBOTO_FONT_LIGHT_FILEPATH, 18)
    loadPNGTexture(spriteStorage.playerSpriteSheet, render, PLAYER_SPRITE_SHEET_PATH)
    loadPNGTexture(spriteStorage.arrowSemiCircle, render, ARROW_SEMICIRCLE_PATH)
    loadPNGTexture(spriteStorage.arrowStraight, render, ARROW_STRAIGHT_PATH)

    for (int i = 1; i <= 6; i++) {
        Texture *tmp;
        loadPNGTexture(tmp, render, CUBE_SIDE_BACKGROUND_PATH(i));
        spriteStorage.sideSprites.push_back(tmp);
    }
}

bool CubeGame::isDebug() {
    return debugView;
}

void CubeGame::setWindowIcon() {
    SetPerfDrawMode(PerformanceDrawMode::Title);
    Surface *icon = IMG_Load(ICON_PATH);
    SDL_SetWindowIcon(window, icon);
}
