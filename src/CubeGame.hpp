#pragma once

#include "gamebase.hpp"

class LevelSelector;

class CubeGame final : public Game
{
public:
    CubeGame();

    bool HandleEvent(const Event event) override;

protected:
    friend class LevelSelector;
};
