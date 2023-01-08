#pragma once

#include "Level.hpp"

class TutorialLevel : public Level {
public:
    TutorialLevel(CubeGame& game, Renderer *render);
    TutorialLevelData load(const LevelLoader::TutLoadedLevelData& data, size_t arrayIndex);

    void Init() override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
protected:
    void internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) override;

private:
    void resetSidebarText(Font* font);

private:
    TutorialLevelData levelData;
    bool loaded = false;
    Text* sidebarText = nullptr;
    Font* usedSidebarFont = nullptr;
};
