#pragma once
#include "global.hpp"
#include "GameObjects/TouchObject.hpp"
#include "touchhelper.hpp"

class TouchController: public GameObject {
public:
    TouchController(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render);
    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;
    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;
    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;
    enum class TouchScene{
        RUDIMENTARY,
        SELECT,
        MOVE
    };

    void setScene(TouchScene scene);

    bool showTouchAccomodations = false;

private:
    TouchScene scene = TouchScene::RUDIMENTARY;
    std::map<TouchScene, Vector<TouchObject*>> touchObjects;
    Vector<TouchObject*>always;
};
