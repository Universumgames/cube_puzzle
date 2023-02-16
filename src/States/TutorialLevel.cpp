//
// Created by Tom Arlt on 07.12.22.
//

#include "TutorialLevel.hpp"

#include <utility>
#include "../CubeGame.hpp"

TutorialLevelData TutorialLevel::load(const LevelLoader::TutLoadedLevelData &data, size_t arrayIndex) {
    TutorialLevelData level = Level::load(data, arrayIndex);
    this->levelData = level;
    this->levelData.sidebarText = data.sideBarText;
    this->sidebarText->changeText(data.sideBarText);
    loaded = true;
    return level;
}

TutorialLevel::TutorialLevel(CubeGame &game, Renderer *render) :
        Level(game, render) {
    resetSidebarText(game.getSpriteStorage()->basicFont);

}

void TutorialLevel::Init() {
    Level::Init();
    //gameObjects.push_back(sidebarText);

    sidebarText->Init();

    //sidebarText->Init();
}

void TutorialLevel::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    Level::Update(BASIC_GO_DATA_PASSTHROUGH);

    Rect gameRect = Level::getDrawableGameRect();
    Rect uiRect = Level::getDrawableUIRect();
    Rect sideRect = sidebarText->getDrawedRect();

    if (sideRect.y + sideRect.h > game.getWindowSize().y) {
        resetSidebarText(game.getSpriteStorage()->smallFont);
    } else if ((sideRect.y + sideRect.h) * 2 < game.getWindowSize().y) {
        resetSidebarText(game.getSpriteStorage()->basicFont);
    }

    sidebarText->Update(BASIC_GO_DATA_PASSTHROUGH);

    int width = max(200, uiRect.w - (gameRect.x + gameRect.w));
    sidebarText->setMaxWidth(width);
    Point textSize = sidebarText->getTextSize();
    int startXOffset = uiRect.x + uiRect.w - width - 10;
    Rect textLoc = {startXOffset, 0, textSize.x, textSize.y};
    Rect renderTextLoc = centerInVertical(textLoc, uiRect);
    sidebarText->changePosition({renderTextLoc.x, renderTextLoc.y});
}

void TutorialLevel::internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) {
    Level::internalUIRender(BASIC_GO_DATA_PASSTHROUGH);
    sidebarText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
}

void TutorialLevel::resetSidebarText(Font *font) {
    if (font == usedSidebarFont) return;
    if (sidebarText != nullptr) delete sidebarText;
    sidebarText = new Text(cubeGame, this, render, 500, loaded ? levelData.sidebarText : "", font, {});
    sidebarText->Init();
    usedSidebarFont = font;
}
