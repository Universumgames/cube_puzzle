//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "../gamebase.hpp"
#include "../GameObjects/GameObject.hpp"
#include "../data/LevelData.hpp"

class Player;
class CubeMap;
class WorldMap;

class Level final : public GameState
{
public:
    Level(Game &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    /// load level from file into memory, called by LevelSelector
    LevelData load(const std::string &path, size_t id);

    /// init world when gamestate comes active
    void Init() override;

    void UnInit() override;

private:
    Vector<GameObject *> gameObjects;
    Player* player;
    CubeMap* cubeMap;
    WorldMap* worldMap;
};
