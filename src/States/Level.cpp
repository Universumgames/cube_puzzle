#include "Level.hpp"

#include <utility>
#include "../GameObjects/Player.hpp"
#include "../GameObjects/CubeMap.hpp"
#include "../GameObjects/WorldMap.hpp"
#include "../data/EmptyData.hpp"
#include "../recthelper.hpp"
#include "../CubeGame.hpp"

#define iterateGameObjectsVector(method) for(auto gameobject:gameObjects){gameobject->method;}

#define iterateGameObjectsNoPlayer(method){ \
                                    worldMap->method; \
                                    cubeMap->method;\
                                    text->method;   \
                                    }

#define iterateGameObjects(method) { \
                                    iterateGameObjectsNoPlayer(method); \
                                    player->method; \
                                    }

void Level::Events(const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_PumpEvents();

    Event event;
    while (SDL_PollEvent(&event)) {
        if (game.HandleEvent(event))
            continue;
        iterateGameObjects(HandleEvent(BASIC_GO_DATA_PASSTHROUGH, event));
    }
}

u32 lastMSec = 0;
u32 lastFrame = 0;

void Level::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    //iterateGameObjectsVector(Update(BASIC_GO_DATA_PASSTHROUGH));
    text->setEnabled(cubeGame.isDebug());
    if (frame % 50 == 0) {
        u32 deltaSec = totalMSec - lastMSec;
        u32 deltaFrame = frame - lastFrame;
        double fps = (double) deltaFrame / ((double) deltaSec / 1000);
        text->changeText(levelData.name + " " + std::to_string(round(fps * 100) / 100).substr(0, 5) + "fps");
        lastMSec = totalMSec;
        lastFrame = frame;
    }
}

void Level::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    // render to textures first
    updateTextures();
    internalGameRender(BASIC_GO_DATA_PASSTHROUGH);
    internalUIRender(BASIC_GO_DATA_PASSTHROUGH);

    // render separate textures into framebuffer
    SDL_SetRenderTarget(render, prepareTex);
    SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
    SDL_RenderClear(render);
    Rect gameTextureDstRect = getGameRenderDst();
    SDL_RenderCopyEx(render, gameTexture, NULL, &gameTextureDstRect, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(render, uiTexture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);

    // render buffered frame
    SDL_SetRenderTarget(render, NULL);
    SDL_RenderClear(render);
    SDL_RenderCopyEx(render, prepareTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(render);

}

Level::Level(CubeGame &game, Renderer *render) : ComplexGameState(game, render) {
    this->text = new Text(game, this, render, 500, "test level", cubeGame.getSpriteStorage()->debugFont, {10, 10}, 1,
                          white);
}

void Level::Init() {
    initLevel();

    GameState::Init();
    //gameObjects.push_back(worldMap);
    //gameObjects.push_back(cubeMap);
    //gameObjects.push_back(player);
    //gameObjects.push_back(text);
    iterateGameObjects(Init())
    game.SetPerfDrawMode(Game::PerformanceDrawMode::None);
    cubeGame.setWindowName("Level " + std::to_string(levelData.id) + " " + levelData.name);
    oldSize = {};
    updateTextures();
}

void Level::UnInit() {
    GameState::UnInit();
    //iterateGameObjectsVector(UnInit())
}

LevelData Level::loadTemplateLevel(size_t id) {
    originalLevelData = {.path = "/dev/zero", .name = "template level", .id = 1, .sides = emptyCubeMapSides, .worldSize = emptyWorldFieldSize, .worldField = emptyWorldField, .cubePos = {
            0, 0}, .playerPos = {0, 0}, .cubeSide = 2};
    levelData = {.path = "", .id = 1, .allStatesIndex = id, .name = "template level"};
    return levelData;
}

LevelData Level::load(const LevelLoader::LoadedLevelData &data, size_t arrayIndex) {
    originalLevelData = data;
    levelData = {.path=data.path, .id=data.id, .allStatesIndex = arrayIndex, .name = data.name};
    return levelData;
}

void Level::updateTextures() {
    if (oldSize != game.getWindowSize()) {
        if (prepareTex != nullptr) {
            SDL_DestroyTexture(prepareTex);
        }
        if (gameTexture != nullptr) {
            SDL_DestroyTexture(gameTexture);
        }
        if (uiTexture != nullptr) {
            SDL_DestroyTexture(uiTexture);
        }
        oldSize = game.getWindowSize();
        prepareTex = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, game.getWindowSize().x, game.getWindowSize().y);
        Rect gameRect = getGameRenderDst();
        gameTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, gameRect.w, gameRect.h);
        uiTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_TARGET, game.getWindowSize().x, game.getWindowSize().y);
    }
}

void Level::internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (uiTexture == NULL) return;
    SDL_SetRenderTarget(render, uiTexture);
    SDL_SetTextureBlendMode(uiTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    iterateGameObjects(RenderUI(BASIC_GO_DATA_PASSTHROUGH))
}

void Level::internalGameRender(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (gameTexture == NULL) return;
    SDL_SetRenderTarget(render, gameTexture);
    SDL_SetTextureBlendMode(gameTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    iterateGameObjectsNoPlayer(Render(BASIC_GO_DATA_PASSTHROUGH))
}

#define GAMERECT_USE_FULL_WINDOW true

Rect Level::getDrawableGameRect() {
#if GAMERECT_USE_FULL_WINDOW
    Point targetSize = cubeGame.getWindowSize();
    Point center = targetSize / 2;
    int s = min(targetSize.x, targetSize.y);
    Point totalSize = {s, s};
    Point offset = center - (totalSize / 2);
    return {offset.x, offset.y, totalSize.x, totalSize.y};
#else
    Point targetSize = cubeGame.getCurrentRenderTargetSize();
    int s = min(targetSize.x, targetSize.y);
    return {0,0, s, s};
#endif
}

Rect Level::getDrawableUIRect() {
    Point targetSize = cubeGame.getWindowSize();
    Point center = targetSize / 2;
    Point totalSize = {targetSize.x, targetSize.y};
    Point offset = center - (totalSize / 2);
    return {offset.x, offset.y, totalSize.x, totalSize.y};
}

Rect Level::getGameRenderDst() {
#if GAMERECT_USE_FULL_WINDOW
    Point p = game.getWindowSize();
    return {0, 0, p.x, p.y};
#else
    Point targetSize = cubeGame.getWindowSize();
    Point center = targetSize / 2;
    Rect src = getDrawableGameRect();
    Point totalSize = {src.w, src.h};
    Point offset = center - (totalSize / 2);
    return {offset.x, offset.y, totalSize.x, totalSize.y};
#endif
}

Rect Level::getUIRenderDst() {
    Point targetSize = cubeGame.getWindowSize();
    int s = min(targetSize.x, targetSize.y);
    Point center = targetSize / 2;
    Point totalSize = {s, s};
    Point offset = center - (totalSize / 2);
    return {offset.x, offset.y, s, s};
}

void Level::returnToLevelSelector(ExitState exitState) {
    cubeGame.interGameStateData = {.sourceStateID = (int) levelData.allStatesIndex, .exitState = exitState};
    cubeGame.returnToLevelSelector();
}

void Level::initLevel() {
    // resetting level and init objects
    auto data = originalLevelData;
    delete worldMap;
    delete cubeMap;
    delete player;
    //gameObjects = Vector<GameObject *>();
    worldMap = new WorldMap(cubeGame, this, render, data.worldSize, data.worldField, data.cubePos);
    cubeMap = new CubeMap(cubeGame, this, render, data.sides, data.cubeSide, data.playerPos);
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(cubeGame, this, render);
    player->setCubeMap(cubeMap);
    cubeMap->SetPlayer(player);
}
