#pragma once

#include <utility>
#include "GameObject.hpp"
#include "CubeField.hpp"
#include "../data/MoveDirections.hpp"
#include "../data/DiceData.hpp"
#include "Text.hpp"
#include "CubeMapSide.hpp"

class WorldMap;
class Player;
class CubeMapSide;
class CubeMapMiniMap;
class CubeField;
class Magnet;

/// Logic class for handling player movement on cube and calling cubeField methods
class CubeMap final : public GameObject {
    friend class WorldMap;
    friend class CubeMapMiniMap;
    
public:
    CubeMap() = delete;
    /// map initializer
    CubeMap(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render, const Vector<CubeMapSide *> &sides,
            int startSide = 2, Point playerPos = {0, 0});

    void Render(u32 frame, u32 totalMSec, float deltaT) override;
    void RenderUI(u32 frame, u32 totalMSec, float deltaT) override;
    
    void HandleEvent(u32 frame, u32 totalMSec, float deltaT, Event event) override;
    void Update(u32 frame, u32 totalMSec, float deltaT) override;
    void Init() override;
    
    /// set world map pointer, has to be set before run
    void SetWorldMap(WorldMap *worldMap);
    void setIsAnimating(bool isAnimating);
    [[nodiscard]] Rect getPlayerDrawPosition();
    [[nodiscard]] CubeMapSide *getCurrentSide();
    [[nodiscard]] Vector<CubeMapSide*>* getAllCubeMapSides();
    [[nodiscard]] Point getCurrentPlayerPos() const;
    [[nodiscard]] Vector<Magnet*> getAllNeighboringMagnets();
    
    [[nodiscard]] bool isAnimating() const;
    
    // the following 2 methods are implemented in CubeMap_playerMovement:
    /// returns true if move was successful, false otherwise
    [[nodiscard]] bool canPlayerMove() const;
    bool movePlayer(PlayerMoveDirection dir, const Vector<Magnet*>& listGrabbedMagnets);
    
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
    bool isSideTransitionAnimationInProgress = false;
    bool isObjectAnimationInProgress = false;
    PlayerMoveDirection lastNormalizedMove;
    
    /// get cubeFields of dice (1-6)
    CubeMapSide *getSide(int i);
    CubeField *getField(int side, int x, int y);
    CubeField *getField(int side, Point p);
    Rect getDrawableRect();
    
    void moveCubeInWorld(DiceRollDirection rollDirection);
    void drawMap(u32 frame, u32 totalMSec, float deltaT);
    void saveCurrentFrame();
    
    // the following 4 methods are implemented in CubeMap_playerMovement:
    PlayerMoveDirection screenDirectionToDirectionOnCubeSide(PlayerMoveDirection direction);
    bool rotateCubeIfNecessary(Point &newPlayerPos, PlayerMoveDirection moveDirection);
    [[nodiscard]] bool checkCubeSideTransition(int sideAId, int sideBId, int oldSideId) const;
    void doLevelFinishedLogic();
};