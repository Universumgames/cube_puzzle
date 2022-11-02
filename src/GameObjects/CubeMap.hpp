//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "GameObject.hpp"
#include "CubeField.hpp"

class WorldMap;

class CubeMapSide;

class CubeMap final : public GameObject {
public:
    CubeMap(Game &game, SDL_Renderer *render, const Vector<CubeMapSide>& sides);

    void SetWorldMap(WorldMap *worldMap) { this->worldMap = worldMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

private:
    void drawMinimap();

    void drawMap();

    CubeMapSide *getSide(int i);

private:
    WorldMap *worldMap = nullptr;
    Vector<CubeMapSide *> sides;

    friend class WorldMap;
};

class CubeMapSide {
public:
    CubeMapSide(const Vector<CubeField> &side, int width, int height) : side(side), width(width), height(height) {}

    Vector<CubeField> side;
    int width, height;

    int getIndex(int x, int y) const { return x * width + y; }
};