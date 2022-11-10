//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "../gamebase.hpp"
#include "../CubeGame.hpp"
#include "../data/LevelData.hpp"
#include "../GameObjects/Text.hpp"
#include "ComplexGameState.hpp"

/// Level Selector GameState, initializes level selecting and loading data
class LevelSelector final : public ComplexGameState
{
public:
    LevelSelector(CubeGame &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Init() override;

    void UnInit() override;
private:
    /// load level data list, for selection
    void loadList();
    /// draw list for selection
    void drawList();
    /// load specific level,
    void loadLevel(const std::string &path);

    /// i: index of level (loading order)
    void playLevel(const LevelData& level);
private:
    Vector<LevelData> levelData;
    bool levelsLoaded = false;
    Text* text;
};

