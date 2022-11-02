#pragma once

#include "gamebase.hpp"

class CubeGame final : public Game
{
public:
    CubeGame();

    bool HandleEvent(const Event event) override;
};
