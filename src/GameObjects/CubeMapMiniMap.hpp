#pragma once
#include "../global.hpp"
#include "GameObject.hpp"
#include "../data/DiceData.hpp"

class CubeMap;

class CubeMapMiniMap: public GameObject {
public:
    CubeMapMiniMap(CubeGame &game, SDL_Renderer *render, CubeMap* cubeMap);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT);

    void updateMinimap();


private:
    CubeMap* cubeMap;
    DiceData& diceData;
};