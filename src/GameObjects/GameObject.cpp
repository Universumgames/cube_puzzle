//
// Created by Tom Arlt on 02.11.22.
//

#include "GameObject.hpp"
#include "../gamebase.hpp"

GameObject::GameObject(CubeGame &game, SDL_Renderer *render) : game(game),
                                                           render(render) {

}
