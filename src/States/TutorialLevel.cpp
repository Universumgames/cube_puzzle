//
// Created by Tom Arlt on 07.12.22.
//

#include "TutorialLevel.hpp"
#include "../CubeGame.hpp"

TutorialLevelData TutorialLevel::load(const LevelLoader::TutLoadedLevelData &data, size_t arrayIndex) {
    auto level = Level::load(data, arrayIndex);
    this->levelData = data;
    return TutorialLevelData();
}

TutorialLevel::TutorialLevel(CubeGame game, Renderer *render) : Level(game, render) {

}
