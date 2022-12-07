#pragma once

#include "GameObject.hpp"
#include <string>
#include "../data/Colors.hpp"

/// Helper class for rendering Text
class Text : public GameObject {
public:
    Text(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render, int maxWidth, std::string text, Font *font, Point position,
         int shadowOffset = 1, Color color = white);

    /// load font from file
    Text(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render, int maxWidth, std::string text, const char *fontPath, int pointSize,
         Point position, int shadowOffset = 1, Color color = white);

    ~Text() override;

    /// called before first run
    void Init() override;

    /// called before destroy
    void UnInit() override;

    /// handle user input, empty
    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override {}

    /// physics update, etc., emtpy
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override {}

    /// text is ui element, empty
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override {}

    /// render text
    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

    /// change position of text, call this in update method
    void changePosition(Point position);

    /// change text, will trigger new texture generation
    void changeText(std::string text);

    /// change text color
    void changeColor(Color color);

    /// getter if text is rendered
    bool isEnabled();

    /// disable text on the fly
    void setEnabled(bool active);

    void setDebug(bool debug);

    /// get size of generated texture
    Point getTextSize();

    Rect getDrawedRect() const;

    void setMaxWidth(int maxWidth);

private:
    /// regenerate texture
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
    Point blendedTextSize;
    Color color;
    Array<Point, 8> shadowOffsets;
    bool enabled = true;
    bool debug = true;
};

