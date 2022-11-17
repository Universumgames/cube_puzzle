#include "CubeMapMiniMap.hpp"
#include "../data/Colors.hpp"
#include "CubeMap.hpp"
#include "../recthelper.hpp"

void CubeMapMiniMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        //updateMinimap(BASIC_GO_DATA_PASSTHROUGH);
    }
}

void CubeMapMiniMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (oldDiceData != diceData || oldSide != cubeMap->currentSideId) {
        if (animationState == AnimationState::FINISHED) {
            animationState = AnimationState::TRANSITIONING;
            animationProgress = 0;
        }
        updateDebugText();
    }
    updateMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMapMiniMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

CubeMapMiniMap::CubeMapMiniMap(CubeGame &game, SDL_Renderer *render, CubeMap *cubeMap) : GameObject(game, render),
                                                                                         diceData(cubeMap->diceData) {
    this->cubeMap = cubeMap;
    this->oldSide = cubeMap->currentSideId;
    updateMinimap(0, 0, 0);
    updateDebugText();
}

void sAndNRotation(int w, int c, int e, int b, int n, int s, DiceSideRotation *nRot, DiceSideRotation *sRot) {
    if (nRot == nullptr || sRot == nullptr) return;
    auto anchorN = DiceData::getAnchorSideOfTopLeftCorner(n);
    auto anchorS = DiceData::getAnchorSideOfTopLeftCorner(s);
    if (anchorN == w) *nRot = DiceSideRotation::LEFT;
    else if (anchorN == c) *nRot = DiceSideRotation::DOWN;
    else if (anchorN == e) *nRot = DiceSideRotation::RIGHT;
    else if (anchorN == b) *nRot = DiceSideRotation::UP;

    if (anchorS == w) *sRot = DiceSideRotation::LEFT;
    else if (anchorS == c) *sRot = DiceSideRotation::UP;
    else if (anchorS == e) *sRot = DiceSideRotation::RIGHT;
    else if (anchorS == b) *sRot = DiceSideRotation::DOWN;
}

void CubeMapMiniMap::drawMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    cubeMap->minimapText->RenderUI(BASIC_GO_DATA_PASSTHROUGH);

    Color borderColor = black;

    // border of y-axis of cross
    drawColoredFilledRect(render, borderColor, bgYDst);
    // border of x-axis of cross
    drawColoredFilledRect(render, borderColor, bgXDst);

    for (auto side: sidesDst) {
        drawSide(game.getSpriteStorage()->sideSprites[side.side - 1], render, side.dst, side.angle);
    }
}

void CubeMapMiniMap::RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) {
    drawMinimap(BASIC_GO_DATA_PASSTHROUGH);

    //SDL_Vertex *vertices = (SDL_Vertex *) calloc(4, sizeof(SDL_Vertex));
    //int* indices = (int*) calloc(6, sizeof(int));
    SDL_Vertex vertices[4] = {{.position = {0, 0}, .color = white, .tex_coord = {0, 0}},
                              {.position={100, 0}, .color = white, .tex_coord = {1,0}},
                              {.position = {0, 100}, .color = white, .tex_coord = {0, 1}},
                              {.position= {100, 100}, .color = white, .tex_coord = {1,1}}};
    const std::vector< SDL_Vertex > verts = { { SDL_FPoint{ 400, 150 }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
                                { SDL_FPoint{ 200, 450 }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
                                { SDL_FPoint{ 600, 450 }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, }};
    int indices[6] = {0, 2,3, 0, 1, 3};
    int err = SDL_RenderGeometry(render, game.getSpriteStorage()->sideSprites[0], vertices, 4, indices, 6);
    SDL_RenderGeometry( render, nullptr, verts.data(), verts.size(), nullptr, 0 );
}

double LerpDegrees(double start, double end, double amount) {
    double difference = abs(end - start);
    if (difference > 180) {
        if (end > start) {
            start += 360;
        } else {
            end += 360;
        }
    }

    // Interpolate it.
    double value = (start + ((end - start) * amount));

    // Wrap it..
    int rangeZero = 360;

    if (value >= 0 && value <= 360)
        return value;

    return ((int) value % rangeZero);
}

void CubeMapMiniMap::updateMinimap(const u32 frame, const u32 totalMSec, const float deltaT) {
    PlayerMoveDirection lastMoveDir = cubeMap->lastNormalizedMove;
    int c = oldSide, n = 0, w = 0, e = 0, s = 0, b = 0;
    int relativeToWindow = max(game.getWindowSize().x, game.getWindowSize().y) / 20.0;
    int sideSize = max(relativeToWindow, 32);
    int sideDistance = 10;
    auto cloneData = DiceData(oldDiceData);
    oldDiceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    Point offset = {game.getWindowSize().x - (sideSize * 3 + sideDistance), sideDistance};
    int borderWidth = 1;

    auto nRot = DiceSideRotation::UP;
    auto sRot = DiceSideRotation::UP;
    sAndNRotation(w, c, e, b, n, s, &nRot, &sRot);

    bgYDst = {offset.x + sideSize - borderWidth, offset.y - borderWidth, sideSize + borderWidth * 2,
              sideSize * 4 + borderWidth * 2};
    bgXDst = {offset.x - borderWidth, offset.y + sideSize - borderWidth, sideSize * 3 + borderWidth * 2,
              sideSize + borderWidth * 2};

    /*
     * struct RenderData{
        int side;
        Rect dst;
        DiceSideRotation rot;
    };
     */
    int transitionOffset = sideSize * animationProgress;
    Point rotatedTransitionOffset = {};
    Point xAxisOffset = {}, yAxisOffset = {};
    RenderData add = {};
    switch (lastMoveDir) {
        case PlayerMoveDirection::UP: // move y axis down
            rotatedTransitionOffset = {0, transitionOffset};
            yAxisOffset = rotatedTransitionOffset;
            add = {.side = b, .dst = {offset.x + sideSize + yAxisOffset.x, offset.y + yAxisOffset.y - sideSize,
                                      sideSize,
                                      sideSize}, .angle = (double) cloneData.getDiceSideRotation(b)};
            break;
        case PlayerMoveDirection::DOWN: // move y axis up
            rotatedTransitionOffset = {0, -transitionOffset};
            yAxisOffset = rotatedTransitionOffset;
            add = {.side = n, .dst = {offset.x + sideSize + yAxisOffset.x, offset.y + sideSize * 4 + yAxisOffset.y,
                                      sideSize,
                                      sideSize}, .angle = (double) cloneData.getDiceSideRotation(b)};
            break;
        case PlayerMoveDirection::LEFT: // move x axis right
            rotatedTransitionOffset = {transitionOffset, 0};
            xAxisOffset = rotatedTransitionOffset;
            add = {.side = b, .dst = {offset.x + xAxisOffset.x - sideSize, offset.y + sideSize + xAxisOffset.y,
                                      sideSize,
                                      sideSize}, .angle = (double) cloneData.getDiceSideRotation(b)};
            break;
        case PlayerMoveDirection::RIGHT: // move x axis left
            rotatedTransitionOffset = {-transitionOffset, 0};
            xAxisOffset = rotatedTransitionOffset;
            add = {.side = b, .dst = {offset.x + sideSize * 3 + xAxisOffset.x, offset.y + sideSize + xAxisOffset.y,
                                      sideSize,
                                      sideSize}, .angle = (double) cloneData.getDiceSideRotation(b)};
            break;
    }

    double wLerpAngle = LerpDegrees((double) cloneData.getDiceSideRotation(w),
                                    (double) diceData.getDiceSideRotation(w),
                                    animationProgress);
    double eLerpAngle = LerpDegrees(
            (double) cloneData.getDiceSideRotation(e),
            (double) diceData.getDiceSideRotation(e),
            animationProgress);
    sidesDst = {
            {.side = n, .dst = {offset.x + sideSize + yAxisOffset.x, offset.y + yAxisOffset.y, sideSize,
                                sideSize}, .angle = (double) nRot},
            {.side = w, .dst = {offset.x + xAxisOffset.x, offset.y + sideSize + xAxisOffset.y, sideSize,
                                sideSize}, .angle = wLerpAngle},
            {.side = c, .dst = {offset.x + sideSize + xAxisOffset.x + yAxisOffset.x,
                                offset.y + sideSize + xAxisOffset.y + yAxisOffset.y, sideSize,
                                sideSize}, .angle = (double) cloneData.getDiceSideRotation(c)},
            {.side = e, .dst = {offset.x + sideSize * 2 + xAxisOffset.x, offset.y + sideSize + xAxisOffset.y, sideSize,
                                sideSize}, .angle = eLerpAngle},
            {.side = s, .dst = {offset.x + sideSize + yAxisOffset.x, offset.y + sideSize * 2 + yAxisOffset.y, sideSize,
                                sideSize}, .angle = (double) sRot},
            {.side = b, .dst = {offset.x + sideSize + yAxisOffset.x, offset.y + sideSize * 3 + yAxisOffset.y, sideSize,
                                sideSize}, .angle = (double) cloneData.getDiceSideRotation(b)}
    };

    if (animationState == AnimationState::TRANSITIONING) {
        animationProgress += deltaT;
        sidesDst.push_back(add);
    }


    if (animationProgress >= (double) AnimationState::FINISHED) {
        animationProgress = 0;
        animationState = AnimationState::FINISHED;
        oldDiceData = DiceData(diceData);
        oldSide = cubeMap->currentSideId;
    }
}

void CubeMapMiniMap::updateDebugText() {
    int n = 0, w = 0, c = 0, e = 0, s = 0, b = 0;
    c = cubeMap->currentSideId;
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
    std::string text = "number cube minimap: \n   " + std::to_string(n) + "   \n" +
                       std::to_string(w) + " " + std::to_string(c) + " " + std::to_string(e) + "\n" +
                       "   " + std::to_string(s) + "   \n" +
                       "   " + std::to_string(b) + "   ";
    cubeMap->minimapText->changeText(text);
}

