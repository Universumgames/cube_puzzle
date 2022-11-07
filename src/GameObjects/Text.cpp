//
// Created by Tom Arlt on 05.11.22.
//

#include "Text.hpp"

#include <utility>
#include "../recthelper.hpp"

void Text::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {

}

void Text::Update(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void Text::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void Text::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    if(!enabled) return;
    GameObject::RenderUI(frame, totalMSec, deltaT);

    SDL_SetTextureColorMod(texture, 0, 0, 0);
    Point p = position;
    for (const Point &pd: shadowOffsets) {
        const Rect dst_rect = {p.x + pd.x, p.y + pd.y, blendedTextSize.x, blendedTextSize.y};
        SDL_RenderCopy(render, texture, EntireRect, &dst_rect);
    }

    {
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        const Rect dst_rect = {p.x, p.y, blendedTextSize.x, blendedTextSize.y};
        SDL_RenderCopy(render, texture, EntireRect, &dst_rect);
    }
}

void Text::Init() {
    GameObject::Init();
    reloadTexture();
}

void Text::UnInit() {
    GameObject::UnInit();
    if (texture != nullptr) SDL_DestroyTexture(texture);
}

Text::~Text() {
    UnInit();
    if (toClean) {
        delete font;
    }
}

void Text::changePosition(Point position) {
    this->position = position;
}

void Text::changeText(std::string text) {
    if (this->text == text) return;
    this->text = text;
    reloadTexture();
}

Text::Text(CubeGame &game, SDL_Renderer *render, int maxWidth, std::string text, Font *font, Point position,
           int shadowOffset,
           Color color) : GameObject(game, render) {
    this->position = position;
    this->shadowOffset = shadowOffset;
    this->text = std::move(text);
    this->font = font;
    this->toClean = false;
    this->color = color;
}

Text::Text(CubeGame &game, SDL_Renderer *render, int maxWidth, std::string text, const char *fontPath, int pointSize,
           Point position, int shadowOffset, Color color) : GameObject(game, render) {
    this->position = position;
    this->shadowOffset = shadowOffset;
    this->text = std::move(text);
    this->font = TTF_OpenFont(fontPath, pointSize);
    this->toClean = true;
    this->color = color;
}

void Text::reloadTexture() {
    if(!enabled) return;
    if (texture != nullptr) SDL_DestroyTexture(texture);

    Surface *surf = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, maxWidth);

    texture = SDL_CreateTextureFromSurface(render, surf);

    u32 fmt;
    int access;
    SDL_QueryTexture(texture, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y);

    SDL_FreeSurface(surf);

    dst = {position.x, position.y, blendedTextSize.h, blendedTextSize.w};
    calculateShadowOffset();
}

void Text::calculateShadowOffset() {
    shadowOffsets = {
            Point{-1 * shadowOffset, -1 * shadowOffset},
            Point{+1 * shadowOffset, -1 * shadowOffset},
            Point{-1 * shadowOffset, +1 * shadowOffset},
            Point{+1 * shadowOffset, +1 * shadowOffset},
            Point{+0 * shadowOffset, +2 * shadowOffset},
            Point{+2 * shadowOffset, +0 * shadowOffset},
            Point{+0 * shadowOffset, -2 * shadowOffset},
            Point{-2 * shadowOffset, +0 * shadowOffset},
    };

    /**
     * {
        Point { -1, -1 },
                Point { +1, -1 },
                Point { -1, +1 },
                Point { +1, +1 },
                Point { +0, +2 },
                Point { +2, +0 },
                Point { +0, -2 },
                Point { -2, +0 },
    };
     */
}

void Text::changeColor(Color color) {
    this->color = color;
}

bool Text::isEnabled() {
    return enabled;
}

void Text::setEnabled(bool active) {
    auto oldEn = enabled;
    enabled = active;
    if (active && oldEn != active) reloadTexture();
}
