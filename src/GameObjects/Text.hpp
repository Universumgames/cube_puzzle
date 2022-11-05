//
// Created by Tom Arlt on 05.11.22.
//

#pragma once

#include "GameObject.hpp"
#include <string>

class Text : public GameObject {
public:
    static constexpr const Array<Point,8> shadowOffsets = {
            Point { -1, -1 },
            Point { +1, -1 },
            Point { -1, +1 },
            Point { +1, +1 },
            Point { +0, +2 },
            Point { +2, +0 },
            Point { +0, -2 },
            Point { -2, +0 },
    };
public:
    Text(Game &game, SDL_Renderer *render, Point position, int maxWidth, int shadowOffset, std::string text,
         Font *font, Color color);

    Text(Game &game, SDL_Renderer *render, Point position, int maxWidth, int shadowOffset, std::string text,
         const char *fontPath, int pointSize, Color color);

    ~Text() override;

    void Init() override;

    void UnInit() override;

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void changePosition(Point position);

    void changeText(std::string text);

private:
    void reloadTexture();

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
};

