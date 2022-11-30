#pragma once

#include "GameObject.hpp"
#include "../data/MoveDirections.hpp"

class CubeMap;

/// Helper class to handle player movement INPUT and rendering player, movement handled in CubeMap
class Player : public GameObject {
public:
    Player(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render);

    /// set cube map pointer, has to be set before first run
    void setCubeMap(CubeMap *cubeMap);

    void HandleEvent(u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(u32 frame, u32 totalMSec, float deltaT) override;
    void Render(u32 frame, u32 totalMSec, float deltaT) override;

    /// move player, relative to screen
    bool move(PlayerMoveDirection direction);

private:
    /// stored int value is row in player sprite
    enum class AnimationState{
        IDLE = 0,
        LEFT = 0,
        RIGHT = 0
    };

    int getAnimationIndex(u32 totalMSec);

private:
    CubeMap *cubeMap;
    Rect nextDraw;
    AnimationState currentState;
    double lastMovementCountdown = 0;
};
