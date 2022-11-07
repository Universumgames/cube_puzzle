//
// Created by Tom Arlt on 02.11.22.
//

#include "Player.hpp"
#include "../States/Level.hpp"
#include "CubeMap.hpp"

Player::Player(CubeGame &game, SDL_Renderer *render) : GameObject(game, render) {}

void Player::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void Player::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void Player::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    Point windowSize = game.getWindowSize();
    drawSprite(game.getSpriteStorage()->temp, render, {0, 1}, {0, 0, min(windowSize.x, windowSize.y) / 5, min(windowSize.x, windowSize.y) / 5});
}

void Player::setCubeMap(CubeMap *cubeMap) {
    this->cubeMap = cubeMap;
}

bool Player::move(PlayerMoveDirection direction) {
    return cubeMap->movePlayer(direction);
}
