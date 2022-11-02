//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "../gamebase.hpp"

class LevelSelector final : public GameState
{
public:
    LevelSelector(Game &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void loadList();
    void drawList();
    void loadLevel(const std::string &path);
};
