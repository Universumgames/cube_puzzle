//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "../gamebase.hpp"
#include "../CubeGame.hpp"
#include "../data/LevelData.hpp"
#include "../GameObjects/Text.hpp"
#include "ComplexGameState.hpp"


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
    void loadList();
    void drawList();
    void loadLevel(const std::string &path);

    /// i: index of level (loading order)
    void playLevel(const LevelData& level);
private:
    Vector<LevelData> levelData;
    bool levelsLoaded = false;
    Text* text;
};

