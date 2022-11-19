#include "Level.hpp"

#include <utility>
#include "../GameObjects/Player.hpp"
#include "../GameObjects/CubeMap.hpp"
#include "../GameObjects/WorldMap.hpp"
#include "../data/EmptyData.hpp"
#include "../recthelper.hpp"
#include "../CubeGame.hpp"

#define iterateGameObjects(method) for(auto gameobject:gameObjects){gameobject->method;}

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
    iterateGameObjects(Update(BASIC_GO_DATA_PASSTHROUGH));
    text->setEnabled(cubeGame.isDebug());
    if (frame % 50 == 0) {
        u32 deltaSec = totalMSec - lastMSec;
        u32 deltaFrame = frame - lastFrame;
        double fps = (double) deltaFrame / ((double) deltaSec / 1000);
        text->changeText(levelData.name + " " + std::to_string(round(fps * 100)/100).substr(0,5) + "fps");
        lastMSec = totalMSec;
        lastFrame = frame;
    }
}

void Level::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    if(oldSize != game.getWindowSize()) {
        SDL_DestroyTexture(prepareTex);
        oldSize = game.getWindowSize();
        prepareTex = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, game.getWindowSize().x, game.getWindowSize().y);
    }
    SDL_SetRenderTarget(render, prepareTex);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
    SDL_RenderFillRect(render, EntireRect);
    iterateGameObjects(Render(BASIC_GO_DATA_PASSTHROUGH))
    iterateGameObjects(RenderUI(BASIC_GO_DATA_PASSTHROUGH))
    SDL_SetRenderTarget(render, NULL);
    SDL_RenderClear(render);
    SDL_RenderCopyEx(render, prepareTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(render);

}

Level::Level(CubeGame &game, Renderer *render) : ComplexGameState(game, render) {
    this->text = new Text(game, render, 500, "test level", cubeGame.getSpriteStorage()->debugFont, {10, 10}, 1, white);
}

void Level::Init() {
    GameState::Init();
    gameObjects.push_back(worldMap);
    gameObjects.push_back(cubeMap);
    gameObjects.push_back(player);
    gameObjects.push_back(text);
    iterateGameObjects(Init())
    game.SetPerfDrawMode(Game::PerformanceDrawMode::Title);
    prepareTex =  SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,
                     SDL_TEXTUREACCESS_TARGET, game.getWindowSize().x, game.getWindowSize().y);
    oldSize = game.getWindowSize();
}

void Level::UnInit() {
    GameState::UnInit();
    iterateGameObjects(UnInit())
}

LevelData Level::loadTemplateLevel(size_t id) {
    worldMap = new WorldMap(cubeGame, render, emptyWorldFieldSize, emptyWorldField, {0, 0});
    cubeMap = new CubeMap(cubeGame, render, emptyCubeMapSides);
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(cubeGame, render);
    player->setCubeMap(cubeMap);
    levelData = {.path = "", .id = 1, .allStatesIndex = id, .name = "template level"};
    return levelData;
}

LevelData Level::load(const LevelLoader::LoadedLevelData& data, size_t arrayIndex) {
    worldMap = new WorldMap(cubeGame, render, data.worldSize, data.worldField, data.cubePos);
    cubeMap = new CubeMap(cubeGame, render, data.sides, data.cubeSide, data.playerPos);
    worldMap->setCubeMap(cubeMap);
    cubeMap->SetWorldMap(worldMap);
    player = new Player(cubeGame, render);
    player->setCubeMap(cubeMap);
    levelData = {.path=data.path, .id=data.id, .allStatesIndex = arrayIndex, .name = data.name};
    return levelData;
}
