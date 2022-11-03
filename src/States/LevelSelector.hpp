//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "../gamebase.hpp"
#include "../CubeGame.hpp"
#include "../Data/LevelData.hpp"


class LevelSelector final : public GameState
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

    CubeGame& getCubeGame(){
        return dynamic_cast<CubeGame &>(game);
    }
private:
    Vector<LevelData> levelData;
    bool levelsLoaded = false;
};

