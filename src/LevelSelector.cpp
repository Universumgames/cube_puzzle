//
// Created by Tom Arlt on 02.11.22.
//

#include "LevelSelector.hpp"

void LevelSelector::Events(const u32 frame, const u32 totalMSec, const float deltaT)
{
    SDL_PumpEvents();

    Event event;
    while( SDL_PollEvent( &event ) )
    {
        if( game.HandleEvent( event ) )
            continue;

        switch( event.type )
        {
            case SDL_KEYDOWN:
            {
                const Keysym & what_key = event.key.keysym;

                if( what_key.scancode == SDL_SCANCODE_F9 )
                {
                    // crash/shutdown, since State #2 does not exist
                    game.SetNextState( 2 );
                }
                else if( what_key.scancode == SDL_SCANCODE_ESCAPE )
                {
                    game.SetNextState( 0 );
                }

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
                game.SetNextState( 1 );
                break;

            default:
                break;
        }
    }
}

void LevelSelector::Update(const u32 frame, const u32 totalMSec, const float deltaT)
{
}

void LevelSelector::Render(const u32 frame, const u32 totalMSec, const float deltaT)
{
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_Rect dst = game.getWindowSize();
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderFillRect(render, &dst);
    SDL_RenderPresent(render);
}

LevelSelector::LevelSelector(Game &game, Renderer *render) : GameState(game, render) {}
