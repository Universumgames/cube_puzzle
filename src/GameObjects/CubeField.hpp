//
// Created by Tom Arlt on 02.11.22.
//
#pragma once
#include "GameObject.hpp"

class CubeField: public GameObject{
public:
    CubeField(Game &game, SDL_Renderer *render);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
};


class FallingStone: public CubeField{

};

class Grass: public CubeField{

};

class Ice: public CubeField{

};

class Button: public CubeField{

};

class Activatable: public CubeField{

};

class Piston: public Activatable{

};

class SlidingWall: public Activatable{

};