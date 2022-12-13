#pragma once

#include "GameObject.hpp"
#include "../data/MoveDirections.hpp"
#include "../AudioHandler.hpp"
#include "CubeObject.hpp"

class CubeMap;

/// Helper class to handle player movement INPUT and rendering player, movement handled in CubeMap
class Player : public GameObject {
public:
    Player(CubeGame& game, ComplexGameState *gameState, SDL_Renderer *render);
    
    /// set cube map pointer, has to be set before first run
    void setCubeMap(CubeMap *cubeMap);
    
    void HandleEvent(u32 frame, u32 totalMSec, float deltaT, Event event) override;
    
    void Update(u32 frame, u32 totalMSec, float deltaT) override;
    
    void Render(u32 frame, u32 totalMSec, float deltaT) override;
    
    /// move player, relative to screen
    bool move(PlayerMoveDirection direction);
    
    void registerGrabbedMagnet(Magnet *magnet);
    
    void deleteGrabbedMagnet(Magnet *magnet);
    
    void deleteAllGrabbedMagnets();

private:
    /// stored int value is row in player sprite
    enum class AnimationState : int {
        IDLE = 0,
        LEFT = 2,
        RIGHT = 1,
        UP = 0,
        DOWN = 0
    };
    
    int getAnimationIndex(u32 totalMSec);

private:
    Vector<Magnet *> listGrabbedMagnets = Vector<Magnet *>();
    CubeMap *cubeMap;
    Rect nextDraw;
    AnimationState currentState = AnimationState::IDLE;
    double lastMovementCountdown = 0;
    AudioPlayer *moveCancelledAudio = nullptr;
};
