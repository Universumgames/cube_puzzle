//
// Created by Tom Arlt on 02.11.22.
//

#pragma once
#include "GameObject.hpp"
#include "../data/MoveDirections.hpp"

class Level;
class CubeMap;

class Player: public GameObject{
public:
    Player(CubeGame &game, SDL_Renderer *render);

    void setCubeMap(CubeMap* cubeMap);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    bool move(PlayerMoveDirection direction);

private:
    Level* currentLevel;
    CubeMap* cubeMap;
};
