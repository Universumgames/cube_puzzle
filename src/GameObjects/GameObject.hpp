//
// Created by Tom Arlt on 02.11.22.
//

#pragma once
#include "../global.hpp"

class Game;

class GameObject {
protected:
    Game& game;
    SDL_Renderer* render;
public:
    GameObject(Game& game, SDL_Renderer* render);
    virtual ~GameObject() = default;

    virtual void Init() {}
    virtual void UnInit() {}

    virtual void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) = 0;
    virtual void Update( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
    virtual void Render( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
    virtual void RenderUI( const u32 frame, const u32 totalMSec, const float deltaT ){};
};
