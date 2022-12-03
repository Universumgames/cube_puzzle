#include "CubeObject.hpp"

// ################################# Alle Render-Methoden ############################################################################

void CubeObject::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game, render, dst);
}

void Stone::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls einen Stein
    SDL_SetRenderDrawColor(render, 100, 100, 255, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Slider::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls einen Slider, wobei ich noch implementieren muss, in welche Richtung der slidet,
    //  also vll erstmal noch warten mit dem Sprite, die sollten dann ja auch unterschiedlich aussehen.
    SDL_SetRenderDrawColor(render, 100, 0, 255, 60);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Magnet::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls einen "Magneten" oder wie auch immer Leo ihn benannt haben will.
    SDL_SetRenderDrawColor(render, 50, 200, 255, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Flag::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    // TODO Tom hier pls eine Flagge
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    CubeObject::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void CubeObject::drawSpriteBorder(CubeGame &game, Renderer *render, Rect dst) {
    if (!game.isDebug()) {
        return;
    }
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}