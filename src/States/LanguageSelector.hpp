#pragma once
#include "../gamebase.hpp"
#include "../CubeGame.hpp"
#include "../data/LevelData.hpp"
#include "../GameObjects/Text.hpp"
#include "ComplexGameState.hpp"
#include "../global.hpp"
#include "../GameObjects/UIButton.hpp"

class LanguageSelector final : public ComplexGameState {
public:
    LanguageSelector(CubeGame &cubeGame, Renderer *render);

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Init() override;
    Rect getDrawableGameRect() override;
    Rect getDrawableUIRect() override;

private:
    bool skip;

    Rect getDrawRectForLanguage(Language lang);
    Texture* getTextureForLang(Language lang);

    void continueGame();

    std::map<Language, UIButton*> languages;

    UIButton* en = nullptr;
    UIButton* de = nullptr;
protected:
    Rect getGameRenderDst() override;
    Rect getUIRenderDst() override;
};
