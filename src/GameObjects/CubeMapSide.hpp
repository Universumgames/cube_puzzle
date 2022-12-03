#pragma once

#include "../data/DiceData.hpp"
#include "CubeField.hpp"
#include "Text.hpp"
#include "CubeMap.hpp"

class CubeMap;
class CubeField;

class CubeMapSide {
private:
    DiceData *diceData;
    CubeMap *cubeMap;

public:
    Vector<CubeField *> cubeFields;
    int width, height, sideID;
    Text *overlay = nullptr;
    
    CubeMapSide() = delete;
    CubeMapSide(Vector<CubeField *> cubeFields, int width, int height, int sideID);
    CubeMapSide(Vector<CubeField *> cubeFields, Point size, int sideID);
    
    void Render(CubeGame &game, ComplexGameState *gameState, Renderer *render, u32 frame, u32 totalMSec, float deltaT, Rect drawableRect);
    void renderGridOverlay(CubeGame &game, Renderer *render, u32 frame, u32 totalMSec, float deltaT, Rect drawableRect);
    void renderCubeFields(CubeGame &game, Renderer *render, u32 frame, u32 totalMSec, float deltaT, Rect drawableRect);
    
    void HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event);
    void Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT);
    
    void setCubeMapRef(CubeMap *cube_map);
    void setDiceData(DiceData* dice_data);
    CubeMap* getCubeMapRef();
    CubeField *getField(int x, int y);
    CubeField *getField(Point pos);
    [[nodiscard]] int getIndex(int x, int y) const;
    [[nodiscard]] Point getFieldSize(Rect drawableRect) const;
    
    [[nodiscard]] Point cubePositionToScreenPosition(Point cubePos) const;
    [[nodiscard]] Point screenPositionToCubePosition(Point screenPos) const;
    void setAllSlidersInMotion();
};