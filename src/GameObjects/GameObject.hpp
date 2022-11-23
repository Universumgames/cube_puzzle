#pragma once

#include "../global.hpp"
#include "../gamebase.hpp"

class CubeGame;

/// an object that can move around on the CubeField if certain criteria are met
class GameObject {
protected:
    CubeGame &game;
    SDL_Renderer *render;
    bool canActivateActivatable;
public:
    /// basic constructor
    GameObject(CubeGame &game, SDL_Renderer *render) : game(game), render(render) {}

    virtual ~GameObject() {};

    /// called on creation
    virtual void Init() {}

    /// called before destroy
    virtual void UnInit() {}

    /// handle input events
    virtual void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) = 0;

    /// physics update, etc.
    virtual void Update(const u32 frame, const u32 totalMSec, const float deltaT) = 0;

    /// render only method
    virtual void Render(const u32 frame, const u32 totalMSec, const float deltaT) = 0;

    /// render ui elements on top of normal elements, called after every render method
    virtual void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {};
};
