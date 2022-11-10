//
// Created by Tom Arlt on 02.11.22.
//

#pragma once
#include "../global.hpp"

class CubeGame;

class GameObject {
protected:
    CubeGame& game;
    SDL_Renderer* render;
public:
    /// basic constructor
    GameObject(CubeGame& game, SDL_Renderer* render);
    virtual ~GameObject(){};

    /// called on creation
    virtual void Init() {}
    /// called before destroy
    virtual void UnInit() {}

    /// handle input events
    virtual void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) = 0;
    /// physics update, etc.
    virtual void Update( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
    /// render only method
    virtual void Render( const u32 frame, const u32 totalMSec, const float deltaT ) = 0;
    /// render ui elements on top of normal elements, called after every render method
    virtual void RenderUI( const u32 frame, const u32 totalMSec, const float deltaT ){};
};
