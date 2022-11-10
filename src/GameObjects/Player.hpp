//
// Created by Tom Arlt on 02.11.22.
//

#pragma once
#include "GameObject.hpp"
#include "../data/MoveDirections.hpp"

class Level;
class CubeMap;

/// Helper class to handle player movement INPUT and rendering player, movement handled in CubeMap
class Player: public GameObject{
public:
    Player(CubeGame &game, SDL_Renderer *render);

    /// set cube map pointer, has to be set before first run
    void setCubeMap(CubeMap* cubeMap);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    /// move player, relative to screen
    bool move(PlayerMoveDirection direction);

private:
    CubeMap* cubeMap;
    Rect nextDraw;
};
