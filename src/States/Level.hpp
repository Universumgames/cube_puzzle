
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

class GameStateData;

enum class ExitState;

/// Level handler, create new object for every level
class Level : public ComplexGameState {
public:
    Level(CubeGame &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;


    Rect getDrawableGameRect() override;

    Rect getDrawableUIRect() override;

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

    void returnToLevelSelector(ExitState exitState) override;

protected:
    /// internal function to change render texture sizes
    void updateTextures();

    /// internal function to render game into game frame buffer
    void internalGameRender(const u32 frame, const u32 totalMSec, const float deltaT);
    /// internal funciton to render ui into ui frame buffer
    virtual void internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT);

    void initLevel();

protected:
    Rect getGameRenderDst() override;

    Rect getUIRenderDst() override;

protected:
    LevelLoader::LoadedLevelData originalLevelData;
    //Vector<GameObject *> gameObjects;
    Player *player = nullptr;
    CubeMap *cubeMap = nullptr;
    WorldMap *worldMap = nullptr;
    Text *text = nullptr;
    LevelData levelData;
    Texture * prepareTex = nullptr;
    Point oldSize;

    Texture * gameTexture = nullptr;
    Texture* uiTexture = nullptr;
};
