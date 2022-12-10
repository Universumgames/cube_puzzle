//
// Created by Tom Arlt on 07.12.22.
//

#include "TutorialLevel.hpp"

#include <utility>
#include "../CubeGame.hpp"

TutorialLevelData TutorialLevel::load(const LevelLoader::TutLoadedLevelData &data, size_t arrayIndex) {
    TutorialLevelData level = Level::load(data, arrayIndex);
    this->levelData = level;
    level.sidebarText = data.sideBarText;
    sidebarText->changeText(data.sideBarText);
    return level;
}

TutorialLevel::TutorialLevel(CubeGame& game, Renderer *render) :
        Level(game, render) {
    sidebarText = new Text(game, this, render, 500, "", game.getSpriteStorage()->basicFont, {});
}

void TutorialLevel::Init() {
    Level::Init();
    gameObjects.push_back(sidebarText);

    //sidebarText->Init();
}

void TutorialLevel::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    Level::Update(BASIC_GO_DATA_PASSTHROUGH);
}

void TutorialLevel::internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect gameRect = Level::getDrawableGameRect();
    Rect uiRect = Level::getDrawableUIRect();
    int width = max(200, uiRect.w - (gameRect.x + gameRect.w));
    sidebarText->setMaxWidth(width);
    Point textSize = sidebarText->getTextSize();
    int startXOffset = min(gameRect.x + gameRect.w + 20, uiRect .x + uiRect.w - width + 20);
    Rect textLoc = {startXOffset,0, textSize.x, textSize.y};
    Rect renderTextLoc = centerInVertical(textLoc, uiRect);
    sidebarText->changePosition({renderTextLoc.x, renderTextLoc.y});
    //sidebarText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    Level::internalUIRender(BASIC_GO_DATA_PASSTHROUGH);
}
