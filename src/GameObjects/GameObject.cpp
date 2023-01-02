#include "GameObject.hpp"
#include "../gamebase.hpp"
#include "../data/Colors.hpp"
#include "../CubeGame.hpp"

void GameObject::drawSpriteBorderSimple(Rect dst) {
    if(!game.isDebug()) return;
    SDL_SetRenderDrawColor(render, spriteBorderColor.r, spriteBorderColor.g, spriteBorderColor.b, spriteBorderColor.a);
    SDL_RenderDrawRect(render, &dst);
}