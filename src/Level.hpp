//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "gamebase.hpp"

class Level final : public GameState
{
public:
    Level(Game &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};
