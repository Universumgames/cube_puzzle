#pragma once

#include "GameObject.hpp"
#include "../data/WorldField.hpp"
#include "../data/MoveDirections.hpp"
#include "../data/DiceData.hpp"
#include "Text.hpp"

class CubeMap;

/// World map is viewed top down
class WorldMap final : public GameObject {
public:
    WorldMap(CubeGame &game, SDL_Renderer *render, int xWidth, int yHeight, Vector<WorldField> map, Point cubePos);

    /// set cube map pointer, has to be called before first run
    void setCubeMap(CubeMap *cubeMap) { this->cubeMap = cubeMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

private:
    Vector<WorldField> field;
    int xWidth, yHeight;
    CubeMap *cubeMap;
    Point cubePos;
    Text *mapCoordinates;

    int getFieldIndex(int x, int y) const;

    friend class CubeMap;
};
