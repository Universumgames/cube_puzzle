#pragma once

#include "Level.hpp"

class TutorialLevel : public Level {
public:
    TutorialLevel(CubeGame game, Renderer *render);
    TutorialLevelData load(const LevelLoader::TutLoadedLevelData& data, size_t arrayIndex);
private:
    LevelLoader::TutLoadedLevelData levelData;
};
