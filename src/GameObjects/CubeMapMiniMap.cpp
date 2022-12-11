#include "CubeMapMiniMap.hpp"
#include "../data/Colors.hpp"
#include "CubeMap.hpp"
#include "../recthelper.hpp"
#include "../data/bezier.hpp"
#include "../mathhelper.hpp"

void CubeMapMiniMap::HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        //updateMinimap(BASIC_GO_DATA_PASSTHROUGH);
    }
}

void CubeMapMiniMap::Update(const u32 frame, const u32 totalMSec, const float deltaT) {
    if (oldDiceData != diceData || oldSide != cubeMap->currentSideId) {
        if (animationState == AnimationState::FINISHED || cubeMap->currentSideId != currentAnimationSideId) {
            animationState = AnimationState::TRANSITIONING;
            animationProgress = 0;
            currentAnimationSideId = cubeMap->currentSideId;
        }
        updateDebugText();
    }
    //updateMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

void CubeMapMiniMap::Render(const u32 frame, const u32 totalMSec, const float deltaT) {
    //drawMinimap(BASIC_GO_DATA_PASSTHROUGH);
}

CubeMapMiniMap::CubeMapMiniMap(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render, CubeMap *cubeMap) : GameObject(game, gameState, render),
                                                                                         diceData(cubeMap->diceData) {
    this->cubeMap = cubeMap;
    this->oldSide = cubeMap->currentSideId;
    this->currentAnimationSideId = cubeMap->currentSideId;
    //updateMinimap(0, 0, 0);
    updateDebugText();
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

    Point wSize = game.getWindowSize();
    int size = max(wSize.x, wSize.y) / 13;
    draw3DMinimap(BASIC_GO_DATA_PASSTHROUGH, {wSize.x - size - 10, 10, size, size});
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

Vector<Vertex> toVertex(Vector<FPoint> points, int tl, int tr, int bl, int br, DiceSideRotation rotation) {
    Vector<FPoint> texCoords;
    switch (rotation) {
        case DiceSideRotation::UP:
            texCoords = {
                    {0, 0},
                    {1, 0},
                    {1, 1},
                    {0, 0},
                    {0, 1},
                    {1, 1}
            };
            break;
        case DiceSideRotation::DOWN:
            texCoords = {
                    {1, 1},
                    {0, 1},
                    {0, 0},
                    {1, 1},
                    {1, 0},
                    {0, 0}
            };
            break;
        case DiceSideRotation::LEFT:
            texCoords = {
                    {0, 1},
                    {0, 0},
                    {1, 0},
                    {0, 1},
                    {1, 1},
                    {1, 0}
            };
            break;
        case DiceSideRotation::RIGHT:
            texCoords = {
                    {1, 0},
                    {1, 1},
                    {0, 1},
                    {1, 0},
                    {0, 0},
                    {0, 1}
            };
            break;
    }
    return {
            // top right triangle
            {.position = (points[tl]), .color = white, .tex_coord = texCoords[0]},
            {.position = (points[tr]), .color = white, .tex_coord = texCoords[1]},
            {.position = (points[br]), .color = white, .tex_coord = texCoords[2]},
            // bottom left triangle
            {.position = (points[tl]), .color = white, .tex_coord = texCoords[3]},
            {.position = (points[bl]), .color = white, .tex_coord = texCoords[4]},
            {.position = (points[br]), .color = white, .tex_coord = texCoords[5]},
    };
}

int sideIndexToSide(DiceData diceData, int side, int index) {
    int c = side, n = 0, w = 0, e = 0, s = 0, b = 0;
    diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);

    switch (index) {
        case 0:
            return c;
        case 1:
            return e;
        case 2:
            return n;
        default:
            return 0;
    }
    return 0;
}

void CubeMapMiniMap::draw3DMinimap(const u32 frame, const u32 totalMSec, const float deltaT, Rect drawableRect) {
    FPoint dir = {-1.5, 1};
    float scale = abs((float) drawableRect.w / dir.x / 3.0f); // dir.x * scale = drawableRect.w
    FPoint scaledDir = dir * scale;
    // starting point top left
    FPoint startPoint = {(float) (drawableRect.x + drawableRect.w), (float) (drawableRect.y)};
    float size = (float) drawableRect.h - ((startPoint + scaledDir).y - (float) drawableRect.y);
    FPoint dirX = {size, 0};
    FPoint dirY = {0, size};

    points.clear();
    for (int z = 0; z < 2; z++) {
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                FPoint p = startPoint + scaledDir * z - dirX * x + dirY * y;
                points.push_back(p);
            }
        }
    }

    auto nRot = DiceSideRotation::UP;
    auto sRot = DiceSideRotation::UP;
    {
        int c = cubeMap->currentSideId, n = 0, w = 0, e = 0, s = 0, b = 0;
        diceData.get2DRepresentation(c, &n, &w, &e, &s, &b);
        sAndNRotation(w, c, e, b, n, s, &nRot, &sRot);
    }

    for (int sideIndex = 0; sideIndex < 3; sideIndex++) {
        int startIndex = sideIndex * 4;
        int actualSide = sideIndexToSide(diceData, cubeMap->currentSideId, sideIndex);
        bool isCurrent = actualSide == cubeMap->currentSideId;
        DiceSideRotation rotation = diceData.getDiceSideRotation(actualSide);
        if (sideIndex == 2) rotation = nRot;
        if(isCurrent) rotation = DiceSideRotation::UP;
        Vector<Vertex> vertices = toVertex(points, indices[startIndex], indices[startIndex + 1],
                                           indices[startIndex + 2], indices[startIndex + 3], rotation);

        //Texture* texToUse = isCurrent ? cubeMap->getCurrentSide()->rawSideTexture : game.getSpriteStorage()->sideSprites[actualSide - 1];
        Texture * texToUse = cubeMap->sides[actualSide - 1]->rawSideTexture;

        SDL_RenderGeometry(render, texToUse, vertices.data(),
                           vertices.size(),
                           nullptr, 0);
    }

    // draw cross overlay over cube
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    FPoint p1 = points[0]; //startPoint;
    FPoint p2 = points[4]; //startPoint + scaledDir;
    FPoint p3 = points[5]; //p2 - FPoint{size, 0};
    FPoint p4 = points[6]; //p2 + FPoint{0, size};
    SDL_RenderDrawLine(render, p2.x, p2.y, p1.x, p1.y);
    SDL_RenderDrawLine(render, p2.x, p2.y, p3.x, p3.y);
    SDL_RenderDrawLine(render, p2.x, p2.y, p4.x, p4.y);

    // test bezier
    /*Point a = {0, game.getWindowSize().y}, b = {game.getWindowSize().x / 2, 0}, c = game.getWindowSize();
    b.y = (int) ((0.5 + sin(frame / 20.0)) * 100.0);
    for (double i = 0; i < 1; i += 0.001) {
        Point bezierP = bezierPoint(a, b, c, i);
        Rect dst = {bezierP.x, bezierP.y, 2, 2};
        SDL_SetRenderDrawColor(render, 255 * i, 255 * i, 255 - 255 * i, 255);
        SDL_RenderFillRect(render, &dst);
    }*/

    Rect dst = drawableRect;
    int angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int alpha = - pow(animationProgress * 19 - 3, 2) + 250;

    if (animationState == AnimationState::TRANSITIONING) {
        switch (cubeMap->lastNormalizedMove) {
            case PlayerMoveDirection::UP:
                angle = 230;
                flip = SDL_FLIP_VERTICAL;
                dst.y += drawableRect.h / 2;
                dst.x -= drawableRect.w / 5;
                break;
            case PlayerMoveDirection::DOWN:
                angle = 100;
                flip = SDL_FLIP_NONE;
                dst.y -= drawableRect.h / 4;
                dst.x -= drawableRect.w / 5;
                break;
            case PlayerMoveDirection::LEFT:
                flip = SDL_FLIP_HORIZONTAL;
                dst.x += drawableRect.w / 5;
                dst.y += drawableRect.h / 4;
                break;
            case PlayerMoveDirection::RIGHT:
                angle = 30;
                dst.x -= drawableRect.w / 2;
                dst.y += drawableRect.h / 6;
                break;
        }
        SDL_SetTextureAlphaMod(game.getSpriteStorage()->arrowSemiCircle, fixBetween(alpha, 0, 255));
        SDL_RenderCopyEx(render, game.getSpriteStorage()->arrowSemiCircle, nullptr, &dst, angle, nullptr, flip);
    }

    if (animationState == AnimationState::TRANSITIONING) {
        animationProgress += deltaT*2;
    }


    if (animationProgress >= (double) AnimationState::FINISHED) {
        animationProgress = 0;
        animationState = AnimationState::FINISHED;
        oldDiceData = DiceData(diceData);
        oldSide = cubeMap->currentSideId;
    }

    drawSpriteBorder(drawableRect);
}

