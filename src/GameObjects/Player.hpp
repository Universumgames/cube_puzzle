#pragma once

#include "GameObject.hpp"
#include "../data/MoveDirections.hpp"

class Level;

class CubeMap;

/// Helper class to handle player movement INPUT and rendering player, movement handled in CubeMap
class Player : public GameObject {
public:
    Player(CubeGame &game, SDL_Renderer *render);

    /// set cube map pointer, has to be set before first run
    void setCubeMap(CubeMap *cubeMap);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    /// move player, relative to screen
    bool move(PlayerMoveDirection direction);

private:
    /// stored int value is row in player sprite
    enum class AnimationState{
        IDLE = 0,
        LEFT = 0,
        RIGHT = 0
    };

    int getAnimationIndex(const u32 totalMSec);

private:
    CubeMap *cubeMap;
    Rect nextDraw;
    AnimationState currentState;
};
