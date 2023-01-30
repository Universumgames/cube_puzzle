//
// Created by Tom Arlt on 02.01.23.
//

#include "TouchController.hpp"
#include "CubeGame.hpp"
#include "GameObjects/UIButton.hpp"
#include "recthelper.hpp"


void TouchController::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    bool isTouchDevice = registerTouchEvent(event);
    if (isTouchDevice) showTouchAccomodations = true; // when touch is used, enable touch buttons
}

void TouchController::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (!showTouchAccomodations) return;
    for (auto touch: touchObjects[scene]) {
        touch->Update(BASIC_GO_DATA_PASSTHROUGH);
    }
    for (auto touch: always) {
        touch->Update(BASIC_GO_DATA_PASSTHROUGH);
    }
}

void TouchController::Render(const u32 frame, const u32 totalMSec, const float deltaT) {

}

void TouchController::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (!showTouchAccomodations) return;
    for (auto touch: touchObjects[scene]) {
        touch->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    }
    for (auto touch: always) {
        touch->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
    }
}

Rect placeButtonBottomRight(Game &game, int maxX, int maxY, int x, int y, int size = -1) {
    int buttonMargin = 10;
    int buttonSize1 = size == -1 ? max(30, max(game.getWindowSize().x, game.getWindowSize().y) / 12) : size;
    Point buttonAreaStart =
            game.getWindowSize()
            //- Point{buttonMargin, buttonMargin}
            - Point{0, (buttonSize1 + buttonMargin) * maxY}
            - Point{(buttonSize1 + buttonMargin) * maxX, 0};
    return {buttonAreaStart.x + (buttonSize1 + buttonMargin) * x, buttonAreaStart.y + (buttonSize1 + buttonMargin) * y,
            buttonSize1, buttonSize1};
}

Rect placeButtonTopLeft(Game &game, int x, int y, int size = -1) {
    int buttonMargin = 10;
    int buttonSize1 = size == -1 ? max(30, max(game.getWindowSize().x, game.getWindowSize().y) / 12) : size;
    Point buttonAreaStart = {buttonMargin, buttonMargin};
    return {buttonAreaStart.x + (buttonSize1 + buttonMargin) * x, buttonAreaStart.y + (buttonSize1 + buttonMargin) * y,
            buttonSize1, buttonSize1};
}

Rect placeButtonBottomLeft(Game &game, int maxY, int x, int y, int size = -1) {
    int buttonMargin = 10;
    int buttonSize1 = size == -1 ? max(30, max(game.getWindowSize().x, game.getWindowSize().y) / 12) : size;
    Point buttonAreaStart = {buttonMargin, game.getWindowSize().y - (buttonMargin + buttonSize1) * maxY};
    return {buttonAreaStart.x + (buttonSize1 + buttonMargin) * x, buttonAreaStart.y + (buttonSize1 + buttonMargin) * y,
            buttonSize1, buttonSize1};
}


bool magnetHoldingState = false;

TouchController::TouchController(CubeGame &game, ComplexGameState *gameState, SDL_Renderer *render) : GameObject(game,
                                                                                                                 gameState,
                                                                                                                 render) {
    // titlescreen
    {
        Vector<TouchObject *> titlescreenObjects = Vector<TouchObject *>();
        auto o = new TouchObject(game, gameState, render, {0, 0}, {0, 0});
        o->setUpdateLambda([](CubeGame &game, TouchObject *touch) {
            touch->setLocation({0, 0});
            touch->setSize(game.getWindowSize());
        });
        o->setPressedLambda([](CubeGame &game, TouchObject *touch) {
            game.returnToLevelSelector();
            //simulateKeyPress(SDL_SCANCODE_RETURN);
        });
        titlescreenObjects.push_back(o);
        touchObjects[TouchScene::RUDIMENTARY] = titlescreenObjects;
    }

    // level selector
    {
        UIButton *tutorialButton;
        UIButton *upButton;
        UIButton *downButton;
        UIButton *enterButton;

        tutorialButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchTutorial);
        upButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchArrow);
        downButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchArrow);
        enterButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchEnter);

        tutorialButton->setTextureSettings(0, {0,0,32,32});
        tutorialButton->setUpdateLambda([](CubeGame &game, TouchObject *tutorialButton) {
            Rect r = placeButtonBottomRight(game, 3, 2, 0, 1);
            tutorialButton->setSize({r.w, r.h});
            tutorialButton->setLocation({r.x, r.y});
        });
        tutorialButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_T);
        });
        upButton->setTextureSettings(-90);
        upButton->setUpdateLambda([](CubeGame &game, TouchObject *upButton) {
            Rect r = placeButtonBottomRight(game, 3, 2, 1, 0);
            upButton->setSize({r.w, r.h});
            upButton->setLocation({r.x, r.y});
        });
        upButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_UP);
        });
        downButton->setTextureSettings(90);
        downButton->setUpdateLambda([](CubeGame &game, TouchObject *downButton) {
            Rect r = placeButtonBottomRight(game, 3, 2, 1, 1);
            downButton->setSize({r.w, r.h});
            downButton->setLocation({r.x, r.y});
        });
        downButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_DOWN);
        });
        enterButton->setUpdateLambda([](CubeGame &game, TouchObject *enterButton) {
            Rect r = placeButtonBottomRight(game, 3, 2, 2, 1);
            enterButton->setSize({r.w, r.h});
            enterButton->setLocation({r.x, r.y});
        });
        enterButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_RETURN);
        });
        Vector<TouchObject *> selectorTouchObjects = {
                tutorialButton, upButton, downButton, enterButton
        };
        touchObjects[TouchScene::SELECT] = selectorTouchObjects;
    }

    // level
    {
        UIButton *upButton;
        UIButton *downButton;
        UIButton *leftButton;
        UIButton *rightButton;
        UIButton *escapeButton;
        UIButton *holdMagnetButton;

        upButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchArrow);
        downButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchArrow);
        leftButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchArrow);
        rightButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchArrow);
        escapeButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchExit);
        holdMagnetButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchGrab);

        upButton->setTextureSettings(-90);
        upButton->setUpdateLambda([](CubeGame &game, TouchObject *touch) {
            Rect r = placeButtonBottomRight(game, 3, 3, 1, 0);
            touch->setSize({r.w, r.h});
            touch->setLocation({r.x, r.y});
        });
        upButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_W);
        });
        downButton->setTextureSettings(90);
        downButton->setUpdateLambda([](CubeGame &game, TouchObject *touch) {
            Rect r = placeButtonBottomRight(game, 3, 3, 1, 2);
            touch->setSize({r.w, r.h});
            touch->setLocation({r.x, r.y});
        });
        downButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_S);
        });
        leftButton->setTextureSettings(180);
        leftButton->setUpdateLambda([](CubeGame &game, TouchObject *touch) {
            Rect r = placeButtonBottomRight(game, 3, 3, 0, 1);
            touch->setSize({r.w, r.h});
            touch->setLocation({r.x, r.y});
        });
        leftButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_A);
        });
        rightButton->setTextureSettings(0);
        rightButton->setUpdateLambda([](CubeGame &game, TouchObject *touch) {
            Rect r = placeButtonBottomRight(game, 3, 3, 2, 1);
            touch->setSize({r.w, r.h});
            touch->setLocation({r.x, r.y});
        });
        rightButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_D);
        });
        escapeButton->setUpdateLambda([](CubeGame &game, TouchObject *escapeButton) {
            Rect r = placeButtonTopLeft(game, 0, 1, max(30, max(game.getWindowSize().x, game.getWindowSize().y) / 20));
            escapeButton->setSize({r.w, r.h});
            escapeButton->setLocation({r.x, r.y});
        });
        escapeButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_ESCAPE);
        });

        holdMagnetButton->setUpdateLambda([](CubeGame &game, TouchObject *touch) {
            Rect r = placeButtonBottomLeft(game, 2, 1, 0);
            touch->setSize({r.w, r.h});
            touch->setLocation({r.x, r.y});
        });
        holdMagnetButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            magnetHoldingState = !magnetHoldingState;
            if (magnetHoldingState)
                simulateKeyPress(SDL_SCANCODE_RSHIFT);
            else simulateKeyRelease(SDL_SCANCODE_RSHIFT);

            UIButton *lockButton = (UIButton *) touch;
            //lockButton->setColor(magnetHoldingState ? red : green);
        });

        Vector<TouchObject *> moveTouchObjects = {
                upButton, downButton, leftButton, rightButton, escapeButton, holdMagnetButton
        };
        touchObjects[TouchScene::MOVE] = moveTouchObjects;
    }

// always visible
    {
        UIButton *musicButton;
        UIButton *touchDisable;

        musicButton = new UIButton(game, gameState, render, {0, 0}, {0, 0}, game.getSpriteStorage()->touchMusic);
        touchDisable = new UIButton(game, gameState, render, {0, 0}, {0, 0}, violet);

        musicButton->setTextureSettings(0, {0,0,16,16});
        musicButton->setUpdateLambda([](CubeGame &game, TouchObject *musicButton) {
            Rect r = placeButtonTopLeft(game, 0, 0, max(30, max(game.getWindowSize().x, game.getWindowSize().y) / 20));
            musicButton->setSize({r.w, r.h});
            musicButton->setLocation({r.x, r.y});
            ((UIButton *) musicButton)->setTextureSettings(0, game.audioHandler->getAudioEnabled() ? Rect{0,0,16,16}: Rect{16,0,16,16});
        });
        musicButton->setPressedLambda([](CubeGame &, TouchObject *touch) {
            simulateKeyPress(SDL_SCANCODE_M);
        });
        touchDisable->setUpdateLambda([this](CubeGame &game, TouchObject *touchDisable) {
            Rect r = placeButtonTopLeft(game, 1, 0, max(30, max(game.getWindowSize().x, game.getWindowSize().y) / 20));
            touchDisable->setSize({r.w, r.h});
            touchDisable->setLocation({r.x, r.y});
            ((UIButton *) touchDisable)->setColor(this->showTouchAccomodations ? green : red);
        });
        touchDisable->setPressedLambda([this](CubeGame &, TouchObject *touch) {
            this->showTouchAccomodations = !this->showTouchAccomodations;
        });

        always = {
                musicButton//, touchDisable
        };
    }
}

void TouchController::setScene(TouchController::TouchScene scene) {
    this->scene = scene;
}
