#include "CubeMapSide.hpp"

#include <utility>

// ################################# Konstruktoren ###################################################################################

CubeMapSide::CubeMapSide(Vector<CubeField *> cubeFields, int width, int height, int sideID) : width(width), height(height), sideID(sideID) {
    this->cubeFields = std::move(cubeFields);
    for (CubeField* cubeField : this->cubeFields) {
        cubeField->setSideId(sideID);
        cubeField->setCubeMapSideRef(this);
        cubeField->setGravityDirection(MovementDirection::moveToBigY);
    }
    this->currentGravityDirection = MovementDirection::moveToBigY;
}

CubeMapSide::CubeMapSide(Vector<CubeField *> cubeFields, Point size, int sideID)
    : CubeMapSide(std::move(cubeFields), size.x, size.y, sideID) {}

// ################################# Alle Render-Methoden ############################################################################

void CubeMapSide::Render(CubeGame &game, ComplexGameState* gameState, Renderer *render, const u32 frame, const u32 totalMSec, const float deltaT, Rect drawableRect) {
    int x = 0, y = 0;
    Point size = getFieldSize(drawableRect);
    Point offset = {drawableRect.x, drawableRect.y};
    DiceSideRotation rotation = diceData->getDiceSideRotation(sideID);
    int dimm = sin(frame / 120.0) * 20;
    SDL_SetTextureColorMod(game.getSpriteStorage()->sideSprites[sideID - 1], 230 + dimm,230 + dimm,230 + dimm);
    drawSide(game.getSpriteStorage()->sideSprites[sideID - 1], render, drawableRect, rotation);
    SDL_SetTextureColorMod(game.getSpriteStorage()->sideSprites[sideID - 1], 255,255,255);
    
    if (overlay == nullptr) {
        overlay = new Text(game, gameState, render, 400, "", game.getSpriteStorage()->debugFont, {0, 0});
    }
    for (auto *field: cubeFields) {
        Point pos = cubePositionToScreenPosition({x, y});
        field->Render(game, render, size, {size.x * pos.x + offset.x, size.y * pos.y + offset.y},
                      BASIC_GO_DATA_PASSTHROUGH);
        overlay->changePosition({size.x * pos.x + offset.x, size.y * pos.y + offset.y});
        overlay->changeText(std::to_string(x) + "," + std::to_string(y));
        overlay->RenderUI(BASIC_GO_DATA_PASSTHROUGH);
        x++;
        if (x >= width) {
            y++;
            x = 0;
        }
    }
    renderGridOverlay(game, render, BASIC_GO_DATA_PASSTHROUGH, drawableRect);
}

void CubeMapSide::renderGridOverlay(CubeGame &game, Renderer *render, const u32 frame, const u32 totalMSec, const float deltaT, Rect drawableRect) {
    Point size = getFieldSize(drawableRect);
    Point offset = {drawableRect.x, drawableRect.y};
    if (game.isDebug()) {
        // grid lines
        double lineWidth = max(max(size.x, size.y) / 40.0, 2.0);
        for (int x = 1; x < width; x++) {
            Rect dst = {(int) (offset.x + size.x * x - lineWidth / 2), offset.y, (int) lineWidth, drawableRect.h};
            SDL_SetRenderDrawColor(render, 0, 0, 0, 100);
            SDL_RenderFillRect(render, &dst);
        }
        for (int y = 1; y < height; y++) {
            Rect dst = {offset.x, (int) (offset.y + size.y * y - lineWidth / 2), drawableRect.w, (int) lineWidth};
            SDL_SetRenderDrawColor(render, 0, 0, 0, 100);
            SDL_RenderFillRect(render, &dst);
        }
        
        // colored rectangle
        auto sideOrientation = this->diceData->getDiceSideRotation(sideID);
        Rect dst = {0, 0, 0, 0};
        switch (sideOrientation) {
            case DiceSideRotation::UP:
                dst = {(int) (offset.x + (drawableRect.w / 2.0) - 25), offset.y, 50, 50};
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
                break;
            case DiceSideRotation::DOWN:
                dst = {(int) (offset.x + (drawableRect.w / 2.0) - 25), offset.y + drawableRect.h - 50, 50, 50};
                SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
                break;
            case DiceSideRotation::LEFT:
                dst = {(int) offset.x, (int) (offset.y + (drawableRect.h / 2.0) - 25), 50, 50};
                SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
                break;
            case DiceSideRotation::RIGHT:
                dst = {offset.x + drawableRect.w - 25, (int) (offset.y + (drawableRect.h / 2.0) - 25), 50, 50};
                SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
                break;
        }
        SDL_RenderFillRect(render, &dst);
    }
}

void CubeMapSide::renderCubeFields(CubeGame &game, Renderer *render, const u32 frame, const u32 totalMSec, const float deltaT, Rect drawableRect) {
    for (int w = 0; w < this->width; w++) {
        for (int h = 0; h < this->height; h++) {
            //getDrawPosition({w, h}, 0.9);
            int index = getIndex(w, h);
            //cubeFields[index]->Render(game, render, size, location, frame, totalMSec, deltaT);
        }
    }
    //CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT
}

// ################################# HandleEvent und Update-Methoden #################################################################

void CubeMapSide::HandleEvent(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT, Event event) {
    for (auto *field: cubeFields) {
        field->HandleEvent(game, BASIC_GO_DATA_PASSTHROUGH, event);
    }
}

void CubeMapSide::Update(CubeGame &game, const u32 frame, const u32 totalMSec, const float deltaT) {
    for (auto *field: cubeFields) {
        field->Update(game, BASIC_GO_DATA_PASSTHROUGH);
    }
    if (overlay != nullptr)
        overlay->setEnabled(game.isDebug());
}

// ################################# Setter & Getter #################################################################################

void CubeMapSide::setCubeMapRef(CubeMap *cube_map) {
    this->cubeMapRef = cube_map;
}

void CubeMapSide::setDiceData(DiceData* dice_data) {
    this->diceData = dice_data;
    for (CubeField* cubeField : this->cubeFields) {
        cubeField->setDiceData(this->diceData);
    }
}

void CubeMapSide::setGravityDirection(MovementDirection dir) {
    this->currentGravityDirection = dir;
    for (auto cubeField : this->cubeFields) {
        cubeField->setGravityDirection(dir);
    }
}

CubeMap* CubeMapSide::getCubeMapRef() {
    return this->cubeMapRef;
}

CubeField* CubeMapSide::getField(int x, int y) {
    if(getIndex(x,y) < 0 || getIndex(x,y) >= cubeFields.size()) return nullptr;
    return this->cubeFields[getIndex(x, y)];
}

CubeField* CubeMapSide::getField(Point pos) {
    return getField(pos.x, pos.y);
}

int CubeMapSide::getIndex(int x, int y) const {
    if(x < 0 || y < 0) return -1;
    return y * width + x;
}

/// Gibt die Anzahl der Pixel zurück, die ein einzelnes Feld breit und hoch ist.
Point CubeMapSide::getFieldSize(Rect drawableRect) const {
    int w = min(drawableRect.w, drawableRect.h) / max(width, height);
    return {w, w};
}

Vector<Magnet*> CubeMapSide::getAllMagnetsSurroundingPlayer(int playerPosX, int playerPosY) {
    Vector<Magnet*> listMagnets;
    Vector<CubeField*> listCubeFields;
    if (playerPosX + 1 < this->width - 1) {
        listCubeFields.push_back(getField(playerPosX + 1, playerPosY));
    }
    if (playerPosX > 0) {
        listCubeFields.push_back(getField(playerPosX - 1, playerPosY));
    }
    if (playerPosY + 1< this->height - 1) {
        listCubeFields.push_back(getField(playerPosX, playerPosY + 1));
    }
    if (playerPosY > 0) {
        listCubeFields.push_back(getField(playerPosX, playerPosY - 1));
    }
    for (auto anyCubeField : listCubeFields) {
        Magnet* magnet = anyCubeField->getMagnetIfPresent();
        if (magnet != nullptr) {
            if (magnet->getIsGrabbed()) {
                return {};
            }
            listMagnets.push_back(magnet);
        }
    }
    return listMagnets;
}

// ################################# sonstige Methoden ###############################################################################

bool CubeMapSide::canObjectEnterFieldAt(CubeObject* cubeObject, int x, int y) {
    for (auto cubeField : this->cubeFields) {
        if (x == cubeField->getX() && y == cubeField->getY()) {
            return cubeField->canObjectEnter(cubeObject);
        }
    }
    return false;
}

/// convert the coordinates to the correct coordinates according to current rotation
Point CubeMapSide::cubePositionToScreenPosition(Point cubePos) const {
    DiceSideRotation faceDirection = this->diceData->getDiceSideRotation(sideID);
    Point res = {};
    switch (faceDirection) {
        case DiceSideRotation::UP:
            return cubePos;
        case DiceSideRotation::DOWN:
            return {this->width - cubePos.x - 1, this->height - cubePos.y - 1};
        case DiceSideRotation::LEFT:
            return {cubePos.y, this->width - cubePos.x - 1};
        case DiceSideRotation::RIGHT:
            return {this->height - cubePos.y - 1, cubePos.x};
    }
    return res;
}

Point CubeMapSide::screenPositionToCubePosition(Point screenPos) const {
    Point p = {};
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            p = {x,y};
            Point s = cubePositionToScreenPosition(p);
            if( s.x == screenPos.x && s.y == screenPos.y) break;
        }
    }
    return p;
}

void CubeMapSide::setAllSlidersInMotion() {
    Vector<CubeMapSide*>* allCubeMapSides;
    Vector<int> allActivatedPressurePlates;
    Vector<int> allDeactivatedPressurePlates;
    Vector<Slider*> allSliders;
    
    allCubeMapSides = this->cubeMapRef->getAllCubeMapSides();
    for (auto anyCubeMapSide : *allCubeMapSides) {
        for (auto cubeField : anyCubeMapSide->cubeFields) {
            if (cubeField->isPressurePlate()) {
                auto* pressurePlate = dynamic_cast<PressurePlate*>(cubeField);
                if (pressurePlate->getIsActivated()) {
                    allActivatedPressurePlates.push_back(pressurePlate->getId());
                } else {
                    allDeactivatedPressurePlates.push_back(pressurePlate->getId());
                }
            }
            for (auto cubeObject : cubeField->cubeObjects) {
                if (cubeObject->isSlider()) {
                    allSliders.push_back(dynamic_cast<Slider*>(cubeObject));
                }
            }
        }
    }
}