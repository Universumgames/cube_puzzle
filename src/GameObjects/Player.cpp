#include "Player.hpp"
#include "../States/Level.hpp"
#include "CubeMap.hpp"
#include "../data/spriteDefs.hpp"
#include <iostream>

#define PLAYER_MOVEMENT_COUNTDOWN_MILLIS 0 // set 250 for actual gameplay

Player::Player(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render)
        : GameObject(game, gameState, render) {
    moveCancelledAudio = new AudioPlayer(MUSIC_HIT_WALL_PATH);
}

void Player::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (lastMovementCountdown > 0 || !cubeMap->canPlayerMove()) {
        return;
    }
    if (event.type == SDL_KEYUP
    && (event.key.keysym.scancode != SDL_SCANCODE_LSHIFT)
    && (event.key.keysym.scancode != SDL_SCANCODE_RSHIFT)) {
        return;
    }
    const Keysym &what_key = event.key.keysym;
    bool moved = true;
    if (what_key.scancode == SDL_SCANCODE_UP) {
        moved = move(PlayerMoveDirection::UP);
        currentState = AnimationState::UP;
    } else if (what_key.scancode == SDL_SCANCODE_DOWN) {
        moved = move(PlayerMoveDirection::DOWN);
        currentState = AnimationState::DOWN;
    } else if (what_key.scancode == SDL_SCANCODE_RIGHT) {
        moved = move(PlayerMoveDirection::RIGHT);
        currentState = AnimationState::RIGHT;
    } else if (what_key.scancode == SDL_SCANCODE_LEFT) {
        moved = move(PlayerMoveDirection::LEFT);
        currentState = AnimationState::LEFT;
    } else {
        currentState = AnimationState::IDLE;
    }
    if (what_key.scancode == SDL_SCANCODE_LSHIFT || what_key.scancode == SDL_SCANCODE_RSHIFT) {
        if (event.type == SDL_KEYDOWN) {
            std::cout << "registered Shift down!" << std::endl;
            this->listGrabbedMagnets = this->cubeMap->getAllNeighboringMagnets();
            std::cout << "size of magnet-list: " << this->listGrabbedMagnets.size() << std::endl;
            for (auto magnet : this->listGrabbedMagnets) {
                magnet->setIsGrabbed(true);
                std::cout << "one Magnet registered!" << std::endl;
            }
        } else {
            this->deleteAllGrabbedMagnets();
            std::cout << "deleted all magnets..." << std::endl;
        }
    }
    if (!moved && moveCancelledAudio != nullptr) {
        moveCancelledAudio->playOnce();
    }
    if (moved) {
        for (auto magnet : this->listGrabbedMagnets) {
            if (!magnet->getIsGrabbed()) {
                this->deleteGrabbedMagnet(magnet);
            }
        }
    }
}

void Player::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (cubeMap == nullptr) {
        return;
    }
    nextDraw = cubeMap->getPlayerDrawPosition();
    lastMovementCountdown = max(0.0, lastMovementCountdown) - deltaT;
}

void Player::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (cubeMap == nullptr) {
        return;
    }
    if (cubeMap->isAnimating()) {
        return;
    }
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
    lastMovementCountdown = PLAYER_MOVEMENT_COUNTDOWN_MILLIS / 1000.0;
    return cubeMap->movePlayer(direction, this->listGrabbedMagnets);
}

void Player::registerGrabbedMagnet(Magnet* magnet) {
    this->listGrabbedMagnets.push_back(magnet);
}

void Player::deleteGrabbedMagnet(Magnet* magnet) {
    for (int i = 0; i < listGrabbedMagnets.size(); i++) {
        Magnet* anyMagnet = listGrabbedMagnets[i];
        if (anyMagnet == magnet) {
            listGrabbedMagnets.erase(listGrabbedMagnets.begin() + i);
        }
    }
}

void Player::deleteAllGrabbedMagnets() {
    this->listGrabbedMagnets.erase(listGrabbedMagnets.begin(), listGrabbedMagnets.end());
}

int Player::getAnimationIndex(const u32 totalMSec) {
    int scaled = totalMSec / 1000 * 2;
    switch (currentState) {
        case AnimationState::IDLE:
            return scaled % SPRITE_PLAYER_IDLE_WIDTH;
        case AnimationState::LEFT:
            return scaled % SPRITE_PLAYER_WALK_WIDTH;
        case AnimationState::RIGHT:
            return scaled % SPRITE_PLAYER_WALK_WIDTH;
        default:
            return 0;
    }
}
