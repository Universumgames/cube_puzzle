#include "Player.hpp"
#include "../States/Level.hpp"
#include "CubeMap.hpp"
#include "../data/spriteDefs.hpp"

Player::Player(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render) : GameObject(game, gameState, render) {}

void Player::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type != SDL_KEYDOWN) return;
    const Keysym &what_key = event.key.keysym;
    if (what_key.scancode == SDL_SCANCODE_UP) {
        move(PlayerMoveDirection::UP);
    } else if (what_key.scancode == SDL_SCANCODE_DOWN) {
        move(PlayerMoveDirection::DOWN);
    } else if (what_key.scancode == SDL_SCANCODE_RIGHT) {
        move(PlayerMoveDirection::RIGHT);
        currentState = AnimationState::RIGHT;
    } else if (what_key.scancode == SDL_SCANCODE_LEFT) {
        move(PlayerMoveDirection::LEFT);
        currentState = AnimationState::LEFT;
    } else {
        currentState = AnimationState::IDLE;
    }
}

void Player::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (cubeMap == nullptr) return;
    nextDraw = cubeMap->playerDrawPosition();
}

void Player::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (cubeMap == nullptr) return;
    Rect r = nextDraw;
    r.y += (int) (sin(frame / 20.0) * 2);
    drawSprite(game.getSpriteStorage()->playerSpriteSheet, render, {getAnimationIndex(totalMSec), (int) currentState},
               r);
    drawSpriteBorder(r);
}

void Player::setCubeMap(CubeMap *cubeMap) {
    this->cubeMap = cubeMap;
}

bool Player::move(PlayerMoveDirection direction) {
    return cubeMap->movePlayer(direction);
}

int Player::getAnimationIndex(const u32 totalMSec) {
    int scaled = totalMSec / 1000 * 2;
    switch (currentState) {
        case AnimationState::IDLE:
            return scaled % SPRITE_PLAYER_IDLE_WIDTH;
        default:
            return 0;
    }
}
