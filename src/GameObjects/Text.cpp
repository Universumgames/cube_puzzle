#include "Text.hpp"
#include <utility>
#include "../global.hpp"
#include "../recthelper.hpp"

Text::Text(CubeGame &game, SDL_Renderer *render, int maxWidth, std::string text, Font *font, Point position,
           int shadowOffset,
           Color color) : GameObject(game, render) {
    this->position = position;
    this->shadowOffset = shadowOffset;
    this->text = std::move(text);
    this->font = font;
    this->toClean = false;
    this->color = color;
    this->maxWidth = maxWidth;
}

Text::Text(CubeGame &game, SDL_Renderer *render, int maxWidth, std::string text, const char *fontPath, int pointSize,
           Point position, int shadowOffset, Color color) : GameObject(game, render) {
    this->position = position;
    this->shadowOffset = shadowOffset;
    this->text = std::move(text);
    this->font = TTF_OpenFont(fontPath, pointSize);
    this->toClean = true;
    this->color = color;
    this->maxWidth = maxWidth;
}

void Text::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (!enabled) return;
    GameObject::RenderUI(frame, totalMSec, deltaT);

    SDL_SetTextureColorMod(texture, 0, 0, 0);
    Point p = position;
    for (const Point &pd: shadowOffsets) {
        const Rect dst_rect = {p.x + pd.x, p.y + pd.y, blendedTextSize.x, blendedTextSize.y};
        SDL_RenderCopy(render, texture, EntireRect, &dst_rect);
    }

    {
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
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
        TTF_CloseFont(font);
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

void Text::reloadTexture() {
    if (!enabled) {
        return;
    }
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, maxWidth);
    texture = SDL_CreateTextureFromSurface(render, surface);
    u32 format;
    int access;
    SDL_QueryTexture(texture, &format, &access, &blendedTextSize.x, &blendedTextSize.y);
    SDL_FreeSurface(surface);
    //dst = {position.x, position.y, blendedTextSize.h, blendedTextSize.w};
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
}

void Text::changeColor(Color color) {
    this->color = color;
}

bool Text::isEnabled() {
    return enabled;
}

void Text::setEnabled(bool active) {
    auto oldEnabledState = enabled;
    this->enabled = active;
    if (this->enabled && oldEnabledState != active) reloadTexture();
}

Point Text::getTextSize() {
    return blendedTextSize;
}
