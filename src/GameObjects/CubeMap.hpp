#pragma once

#include <utility>

#include "GameObject.hpp"
#include "CubeField.hpp"
#include "../data/MoveDirections.hpp"
#include "../data/DiceData.hpp"
#include "Text.hpp"

class WorldMap;

class Player;

class CubeMapSide;

class CubeMapMiniMap;

/// Logic class for handling player movement on cube and calling cubeField methods
class CubeMap final : public GameObject {
public:
    /// map initializer
    CubeMap(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render, const Vector<CubeMapSide *> &sides,
            int startSide = 2,
            Point playerPos = {0, 0});

    /// set world map pointer, has to be set before run
    void SetWorldMap(WorldMap *worldMap) { this->worldMap = worldMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Init() override;

public:
    bool movePlayer(PlayerMoveDirection dir);

    Rect getPlayerDrawPosition();

    CubeMapSide *getCurrentSide();

    bool isAnimating() { return sideTransitionAnimating; }

private:
    void moveCubeInWorld(DiceRollDirection rollDirection);

    void drawMap(const u32 frame, const u32 totalMSec, const float deltaT);

    PlayerMoveDirection screenDirectionToDirectionOnCubeSide(PlayerMoveDirection direction);

    bool rotateCubeIfNecessary(Point &newPlayerPos, PlayerMoveDirection moveDirection);

    [[nodiscard]] bool checkCubeSideTransition(int sideAId, int sideBId, int oldSideId) const;

    /// get cubeFields of dice (1-6)
    CubeMapSide *getSide(int i);

    CubeField *getField(int side, int x, int y);

    CubeField *getField(int side, Point p);

    Rect getDrawableRect();

    void saveCurrentFrame();

    void doLevelFinishedLogic();

private:
    WorldMap *worldMap = nullptr;
    CubeMapMiniMap *miniMap;
    Vector<CubeMapSide *> sides;
    int currentSideId;
    Point playerPos{};
    DiceData diceData;
    Text *debugSideIndicator;
    Text *minimapText;
    Text *debugDiceData;

    Texture *renderTarget = nullptr;
    Texture *oldSideFrame = nullptr;
    Point oldSideFrameSize;
    double sideTransitionState = 0;
    bool sideTransitionAnimating = false;

    PlayerMoveDirection lastNormalizedMove;

    friend class WorldMap;

    friend class CubeMapMiniMap;
};

class CubeMapSide {
public:
    CubeMapSide() = default;

    CubeMapSide(Vector<CubeField *> cubeFields, int width, int height, int sideID) : width(width), height(height),
                                                                                     sideID(sideID) {
        this->cubeFields = std::move(cubeFields);
    }

    CubeMapSide(Vector<CubeField *> cubeFields, Point size, int sideID) : width(size.x), height(size.y),
                                                                          sideID(sideID) {
        this->cubeFields = std::move(cubeFields);
    }

    Vector<CubeField *> cubeFields;
    int width, height, sideID;
    Text *overlay = nullptr;

    CubeField *getField(int x, int y) { return cubeFields[getIndex(x, y)]; }

    CubeField *getField(Point pos) { return getField(pos.x, pos.y); }

    [[nodiscard]] int getIndex(int x, int y) const { return y * width + x; }

    Point getFieldSize(Rect drawableRect);

    void HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event);

    void Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT);

    void Render(CubeGame &game, ComplexGameState *gameState, Renderer *render, DiceData diceData, const u32 frame,
                const u32 totalMSec, const float deltaT, Rect drawableRect);

    void renderGridOverlay(CubeGame &game, Renderer *render, DiceData diceData, const u32 frame, const u32 totalMSec,
                           const float deltaT, Rect drawableRect);

    void renderCubeFields(CubeGame &game, Renderer *render, const u32 frame, const u32 totalMSec, const float deltaT,
                          Rect drawableRect);

    [[nodiscard]] Point cubePositionToScreenPosition(DiceData diceData, Point cubePos) const;

    Point screenPositionToCubePosition(DiceData diceData, Point screenPos) const;

};