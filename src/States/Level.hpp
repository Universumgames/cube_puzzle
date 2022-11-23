
#pragma once

#include "../gamebase.hpp"
#include "../GameObjects/GameObject.hpp"
#include "../data/LevelData.hpp"
#include "../GameObjects/Text.hpp"
#include "ComplexGameState.hpp"
#include "../data/LevelLoader.hpp"

class Player;

class CubeMap;

class WorldMap;

class CubeGame;

/// Level handler, create new object for every level
class Level final : public ComplexGameState {
public:
    Level(CubeGame &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;



    /**
     *  load level from file into memory, called by LevelSelector
     * @param path path to file to load from
     * @param arrayIndex the arrayIndex the level should have (index in allstates vector)
     * @return loaded leveldata
     */

    LevelData load(const LevelLoader::LoadedLevelData& data, size_t arrayIndex);

    /// init world when gamestate comes active
    void Init() override;

    void UnInit() override;


    /// alternative to load(...) function, if this level should be generated from template data
    LevelData loadTemplateLevel(size_t id);

private:
    void updateTextures();

    /// internal function to render game into game frame buffer
    void internalGameRender(const u32 frame, const u32 totalMSec, const float deltaT);
    /// internal funciton to render ui into ui frame buffer
    void internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT);

private:
    Vector<GameObject *> gameObjects;
    Player *player;
    CubeMap *cubeMap;
    WorldMap *worldMap;
    Text *text;
    LevelData levelData;
    Texture * prepareTex;
    Point oldSize;

    Texture * gameTexture;
    Texture* uiTexture;
};
