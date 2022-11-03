//
// Created by Tom Arlt on 02.11.22.
//

#pragma once

#include "GameObject.hpp"
#include "../data/WorldField.hpp"
#include "../data/MoveDirections.hpp"
#include "../data/CubeData.hpp"

class CubeMap;

/// World map is viewed top down
class WorldMap final : public GameObject {
public:
    WorldMap(Game &game, SDL_Renderer *render, int xWidth, int yHeight, Vector<WorldField> map, Point cubePos);

    void setCubeMap(CubeMap *cubeMap) { this->cubeMap = cubeMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;



private:
    Vector<WorldField> field;
    int xWidth, yHeight;
    CubeMap *cubeMap;
    CubeData cubeData;
    Point cubePos;

    int getFieldIndex(int x, int y) const;

    friend class CubeMap;
};
