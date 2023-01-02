#pragma once
#include "GameObject.hpp"
#include "TouchObject.hpp"

class UIButton: public TouchObject {
public:
    UIButton(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render, Point location, Point size, Texture* renderTexture, int touchPadding = 10);
    UIButton(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render, Point location, Point size, Color color, int touchPadding = 10);

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void setTextureSettings(int angle, Rect src = {0,0,0,0});

    bool getEnabled();
    void setEnabled(bool enabled);

public:
    Rect getRenderRect();


    void setTexture(Texture* texture);
    void setColor(Color color);


private:
protected:
    void updateRects() override;
private:
    Rect renderRect;


    Texture* renderTexture = nullptr;
    int textureAngle = 0;
    Rect textureSrc = {0,0,0,0};
    bool textureSrcNull = true;
    Color color;
    bool useTexture;
    bool enabled = true;
};

