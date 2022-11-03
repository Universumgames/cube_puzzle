//
// Created by Tom Arlt on 02.11.22.
//

#include "Player.hpp"
#include "../States/Level.hpp"
#include "CubeMap.hpp"

Player::Player(Game &game, SDL_Renderer *render) : GameObject(game, render) {}

void Player::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void Player::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void Player::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void Player::setCubeMap(CubeMap *cubeMap) {
    this->cubeMap = cubeMap;
}

bool Player::move(PlayerMoveDirection direction) {
    return cubeMap->movePlayer(direction);
}
