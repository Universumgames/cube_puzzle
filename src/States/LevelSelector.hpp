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

private:
    /// load level data list, for selection
    void loadList();

    /// draw list for selection
    void drawList();

    /// load specific level
    void loadLevel(Map<int, Map<int, Vector<WorldField::WorldFieldEnum>>> levelDataMap);

    /// play specific level
    void playLevel(const LevelData &level);
    
    /// remove every character from string that is not needed
    static void removeUnwantedChars(std::string& str);
    
    static Map<int, Map<int, Vector<WorldField::WorldFieldEnum>>> getLevelDataMap(std::string& fileString);
    
private:
    Vector<LevelData> levelData;
    bool levelsLoaded = false;
    Text *text;
};

