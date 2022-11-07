#pragma once

#include "gamebase.hpp"
#include "data/SpriteStorage.hpp"

class LevelSelector;

class CubeGame final : public Game
{
public:
    CubeGame();

    bool HandleEvent(const Event event) override;

    SpriteStorage* getSpriteStorage() override;

    bool isDebug();
private:
    SpriteStorage spriteStorage;

    void loadSprites();

    bool debugView = true;

protected:
    friend class LevelSelector;
};
