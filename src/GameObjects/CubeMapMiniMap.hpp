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

    void updateMinimap(const u32 frame, const u32 totalMSec, const float deltaT);

    void updateDebugText();

    void draw3DMinimap();

private:
    enum class AnimationState{
        FINISHED = 1, TRANSITIONING
    };

    struct RenderData{
        int side;
        Rect dst;
        double angle;
    };

private:
    CubeMap* cubeMap;
    DiceData& diceData;
    DiceData oldDiceData;
    int oldSide = 2;
    AnimationState animationState = AnimationState::FINISHED;
    double animationProgress = (double) AnimationState::FINISHED;
    Vector<RenderData> sidesDst;
    Rect bgXDst, bgYDst;
    Vector<Point> points;
    const Vector<int> indices = {6, 4, 7, 5, 4, 0, 5, 1, 2, 0, 6, 4};
};