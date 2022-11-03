//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "GameObject.hpp"
#include "CubeField.hpp"
#include "../data/MoveDirections.hpp"

class WorldMap;

class Player;

class CubeMapSide;

class CubeMap final : public GameObject {
public:
    CubeMap(Game &game, SDL_Renderer *render, const Vector<CubeMapSide> &sides);

    void SetWorldMap(WorldMap *worldMap) { this->worldMap = worldMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

public:
    bool movePlayer(PlayerMoveDirection dir);

private:
    void rollCube(CubeRollDirection rollDirection);

    void drawMinimap();

    void drawMap();

    /// get side of dice (1-6)
    CubeMapSide *getSide(int i);

private:
    WorldMap *worldMap = nullptr;
    Vector<CubeMapSide *> sides;
    int currentSideId = 1;

    friend class WorldMap;
};

class CubeMapSide {
public:
    CubeMapSide(const Vector<CubeField> &side, int width, int height) : side(side), width(width), height(height) {}

    Vector<CubeField> side;
    int width, height;

    int getIndex(int x, int y) const { return x * width + y; }
};