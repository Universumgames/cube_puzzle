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
    CubeMap(Game &game, SDL_Renderer *render, const Vector<CubeMapSide> &sides, int startSide = 1,
            Point playerPos = {0, 0});

    void SetWorldMap(WorldMap *worldMap) { this->worldMap = worldMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

public:
    bool movePlayer(PlayerMoveDirection dir);

private:
    void rollCube(DiceRollDirection rollDirection);

    void drawMinimap();

    void drawMap();

    /// get side of dice (1-6)
    CubeMapSide *getSide(int i);

    CubeField* getField(int side, int x, int y);
    CubeField* getField(int side, Point p);

private:
    WorldMap *worldMap = nullptr;
    Vector<CubeMapSide *> sides;
    int currentSideId = 1;
    Point playerPos;

    friend class WorldMap;
};

class CubeMapSide {
public:
    CubeMapSide(Vector<CubeField *> &side, int width, int height) : width(width), height(height) { this->side = side; }
    CubeMapSide(Vector<CubeField *> &side, Point size) : width(size.x), height(size.y) { this->side = side; }

    Vector<CubeField *> side;
    int width, height;

    CubeField *getField(int x, int y) { return side[getIndex(x, y)]; }

    [[nodiscard]] int getIndex(int x, int y) const { return x * width + y; }

    void HandleEvent(Game& game, const u32 frame, const u32 totalMSec, const float deltaT, Event event);

    void Update(Game& game, const u32 frame, const u32 totalMSec, const float deltaT);

    void Render(Game& game, Renderer* render, const u32 frame, const u32 totalMSec, const float deltaT);
};