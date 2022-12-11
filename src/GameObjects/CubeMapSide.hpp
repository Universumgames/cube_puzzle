#pragma once

#include "../data/DiceData.hpp"
#include "CubeField.hpp"
#include "Text.hpp"
#include "CubeMap.hpp"

class CubeMap;
class CubeField;
class CubeObject;

class CubeMapSide {
private:
    DiceData *diceData;
    CubeMap *cubeMapRef;
    MovementDirection currentGravityDirection;

public:
    Vector<CubeField *> cubeFields;
    int width, height, sideID;
    Text *overlay = nullptr;

    Texture* rawSideTexture = nullptr;
    Point oldDrawableRect = {};
    DiceSideRotation oldRotation;
    
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
    void setGravityDirection(MovementDirection dir);
    CubeMap* getCubeMapRef();
    CubeField *getField(int x, int y);
    CubeField *getField(Point pos);
    [[nodiscard]] int getIndex(int x, int y) const;
    [[nodiscard]] Point getFieldSize(Rect drawableRect) const;
    [[nodiscard]] Vector<Magnet*> getAllMagnetsSurroundingPlayer(int playerPosX, int playerPosY);
    
    [[nodiscard]] bool canObjectEnterFieldAt(CubeObject* cubeObject, int x, int y);
    [[nodiscard]] Point cubePositionToScreenPosition(Point cubePos) const;
    [[nodiscard]] Point screenPositionToCubePosition(Point screenPos) const;
    void setAllSlidersInMotion();
};