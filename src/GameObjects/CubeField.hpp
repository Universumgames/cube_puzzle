//
// Created by Tom Arlt on 02.11.22.
//
#pragma once

#include "GameObject.hpp"
#include "../CubeGame.hpp"

class CubeField {
public:
    CubeField() = default;

    virtual void HandleEvent(Game &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) = 0;

    virtual void Update(Game &game, const u32 frame, const u32 totalMSec, const float deltaT) = 0;

    virtual void Render(Game &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                        const float deltaT) {
        SDL_SetRenderDrawColor(render, 200, 200, 0, 255);

        Rect dst = {location.x, location.y, size.x, size.y};
        SDL_RenderFillRect(render, &dst);
    };

    virtual bool isPlayerMovableTo() = 0;

    virtual bool isObjectMovableTo() = 0;
};

class EmptyField : public CubeField {
public:
    bool isPlayerMovableTo() override {
        return true;
    }

    bool isObjectMovableTo() override {
        return true;
    }

    void HandleEvent(Game &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {

    }

    void Update(Game &game, const u32 frame, const u32 totalMSec, const float deltaT) override {

    }

    void Render(Game &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                const float deltaT) override {
        //CubeField::Render(game, render, size, location, BASIC_GO_DATA_PASSTHROUGH);
        Rect src = {0,0,16,16};
        Rect dst = {location.x, location.y, size.x, size.y};
        SDL_RenderCopy(render, game.getSpriteStorage()->temp, &src, &dst);
    }
};

class FallingStone : public CubeField {
public:
    bool isPlayerMovableTo() override { return false; }

    bool isObjectMovableTo() override { return false; }

};

class Grass : public CubeField {
public:
    bool isPlayerMovableTo() override { return false; }

    bool isObjectMovableTo() override { return false; }
};

class Ice : public CubeField {
public:
    bool isPlayerMovableTo() override { return false; }

    bool isObjectMovableTo() override { return false; }
};

class Button : public CubeField {
public:
    bool isPlayerMovableTo() override { return true; }

    bool isObjectMovableTo() override { return true; }
};

class Activatable : public CubeField {
public:
    bool isPlayerMovableTo() override { return false; }

    bool isObjectMovableTo() override { return false; }
};

class Piston : public Activatable {

};

class SlidingWall : public Activatable {

};