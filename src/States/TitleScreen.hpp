#include "TutorialLevel.hpp"
#include "../GameObjects/Text.hpp"

class LevelSelector;

class TitleScreen: public TutorialLevel {
public:
    TitleScreen(CubeGame& game, Renderer *renderer);
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;
    void Init() override;
protected:
    void internalUIRender(const u32 frame, const u32 totalMSec, const float deltaT) override;
private:
    LevelSelector* levelSelector;
    Text* versionInfo;
    Text* remoteLevelLoading;
    std::ifstream solution;
    u32 lastMovementTotalMSec = 0;

    TouchObject* startButton;

    friend class CubeGame;
};