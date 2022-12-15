#pragma once

#include <filesystem>
#include <fstream>
#include "../gamebase.hpp"
#include "../CubeGame.hpp"
#include "../data/LevelData.hpp"
#include "../GameObjects/Text.hpp"
#include "ComplexGameState.hpp"
#include "../global.hpp"

class TitleScreen;

/// Level Selector GameState, initializes level selecting and loading data
class LevelSelector final : public ComplexGameState {
public:
    LevelSelector(CubeGame &game, Renderer *render);

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Init() override;

    void UnInit() override;

    Rect getDrawableGameRect() override;

    Rect getDrawableUIRect() override;

private:
protected:
    Rect getGameRenderDst() override;

    Rect getUIRenderDst() override;

private:
    void levelsInit();

    /// load level data list, for selection
    void loadLevels();

    void loadTutorialLevels();

    /// draw list for selection
    void drawDebugList();

    /// play specific level
    void playLevel(const LevelData &level);

    void playLevel(int levelID);

    void playNextLevel(int allStatesID);

    int getLevelIDByState(int stateIndex);

    void prepareLevelListItems();

    void prepareLevelListItemTexture(LevelData& leveldata, Point drawableRect);

    Point getLevelSelectSize();

    Rect getDrawableUISpace();

    Rect getListSpace();

    Rect getSideBarSpace(int topPadding);
    
private:
    Point oldSize;
    int selectorIndex = 0;
    Vector<LevelData> levelData;
    Vector<TutorialLevelData> tutLevelData;
    bool levelsLoaded = false;
    Text *debugText;
    Text* sideBarText;
    Text* headline;

    bool loadingNext;

    friend class TitleScreen;
    friend class CubeGame;
};

