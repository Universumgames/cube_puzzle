#pragma once

#include <filesystem>
#include <fstream>
#include "../gamebase.hpp"
#include "../CubeGame.hpp"
#include "../data/LevelData.hpp"
#include "../GameObjects/Text.hpp"
#include "ComplexGameState.hpp"
#include "../global.hpp"

/// Level Selector GameState, initializes level selecting and loading data
class LevelSelector final : public ComplexGameState {
public:
    LevelSelector(CubeGame &game, Renderer *render);

    void Events(const u32 frame, const u32 totalMSec, const float deltaT) override;

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

    /// load level data list, for selection
    void loadList();

    /// draw list for selection
    void drawList();

    /// play specific level
    void playLevel(const LevelData &level);

    void playLevel(int levelID);

    void playNextLevel(int allStatesID);

    void prepareLevelListItems();

    void prepareLevelListItemTexture(LevelData& leveldata, Rect drawableRect);
    
private:
    Point oldSize;
    int selectorIndex = 0;
    int columns = 2;
    int rows = 6;
    Vector<LevelData> levelData;
    bool levelsLoaded = false;
    Text *text;
    Text* headline;
};

