//
// Created by Tom Arlt on 05.11.22.
//

#pragma once

#include "GameObject.hpp"
#include <string>
#include "../data/Colors.hpp"

class Text : public GameObject {
public:
    Text(Game &game, SDL_Renderer *render, int maxWidth, std::string text, Font *font, Point position,
         int shadowOffset = 1, Color color = white);

    Text(Game &game, SDL_Renderer *render, int maxWidth, std::string text, const char *fontPath, int pointSize,
         Point position, int shadowOffset = 1, Color color = white);

    ~Text() override;

    void Init() override;

    void UnInit() override;

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void changePosition(Point position);

    void changeText(std::string text);

    void changeColor(Color color);

    bool isEnabled();

    void setEnabled(bool active);

private:
    void reloadTexture();

    void calculateShadowOffset();

private:
    Point position;
    int maxWidth;
    Font *font = nullptr;
    bool toClean;
    int shadowOffset;
    Texture *texture = nullptr;
    std::string text;
    Rect blendedTextSize;
    Rect dst;
    Color color;
    Array<Point, 8> shadowOffsets;
    bool enabled = true;
};

