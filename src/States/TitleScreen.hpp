#include "ComplexGameState.hpp"
#include "../GameObjects/Text.hpp"

class LevelSelector;

class TitleScreen: public ComplexGameState {
public:
    TitleScreen(CubeGame& game, Renderer *renderer);
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;
    void Init() override;
    Rect getDrawableGameRect() override;
    Rect getDrawableUIRect() override;
protected:
    Rect getGameRenderDst() override;
    Rect getUIRenderDst() override;
private:
    LevelSelector* levelSelector;
    Text* versionInfo;


    friend class CubeGame;
};