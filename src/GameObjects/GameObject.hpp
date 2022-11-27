#pragma once

#include "../global.hpp"
#include "../gamebase.hpp"

class CubeGame;
class ComplexGameState;

class GameObject {
protected:
    CubeGame &game;
    SDL_Renderer *render;
    ComplexGameState* gameState;
public:
    /// basic constructor
    GameObject(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render) : game(game), render(render), gameState(gameState) {}

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

    void drawSpriteBorder(Rect dst);
};
