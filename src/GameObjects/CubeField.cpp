#include "CubeField.hpp"
#include "GameObject.hpp"
#include "../data/SpriteStorage.hpp"
#include "../data/Colors.hpp"
#include "../data/spriteDefs.hpp"

// ################################# Konstruktoren ###################################################################################

CubeField::CubeField(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects) {
    this->sideId = sideId;
    this->cubeObjects = cubeObjects;
    this->x = x;
    this->y = y;
    for (auto cubeObject: cubeObjects) {
        cubeObject->setCubeFieldRef(this);
    }
}

EmptyField::EmptyField(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects)
        : CubeField(sideId, x, y, cubeObjects) {}

Grass::Grass(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects)
        : EmptyField(sideId, x, y, cubeObjects) {}

Static::Static(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects)
        : CubeField(sideId, x, y, cubeObjects) {}

Wall_1::Wall_1(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects)
        : Static(sideId, x, y, cubeObjects) {}

Wall_2::Wall_2(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects)
    : Static(sideId, x, y, cubeObjects) {}
    
Interactable::Interactable(int sideId, int x, int y, const Vector<CubeObject *>& cubeObjects)
    : CubeField(sideId, x, y, cubeObjects) {}

PressurePlate::PressurePlate(int sideId, int x, int y, int id, bool activated, const Vector<CubeObject *>& cubeObjects)
    : Interactable(sideId, x, y, cubeObjects) {
    this->id = id;
    this->isActivated = activated;
}

// ################################# Alle Render-Methoden ############################################################################

void CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec, const float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
    drawSpriteBorder(game.isDebug(), render, dst);
    Point locationNew = {location.x + 10, location.y + 10};
    Point sizeNew = {size.x - 20, size.y - 20};
    for (CubeObject* cubeObject : this->cubeObjects) {
        cubeObject->Render(game, render, sizeNew, locationNew, frame, totalMSec, deltaT);
        locationNew = {locationNew.x + 10, locationNew.y + 10};
        sizeNew = {sizeNew.x - 20, sizeNew.y - 20};
    }
}

void EmptyField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec, const float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void Grass::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, SPRITE_GRASS_INDEX,
               {location.x, location.y, size.x, size.y}, (int) diceData->getDiceSideRotation(sideId));
}

void Wall_1::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, SPRITE_WALL1_INDEX, {location.x, location.y, size.x, size.y}, (int)diceData->getDiceSideRotation(sideId));
}

void Wall_2::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, SPRITE_WALL2_INDEX, {location.x, location.y, size.x, size.y}, (int)diceData->getDiceSideRotation(sideId));
}

void PressurePlate::Render(CubeGame& game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, SPRITE_PRESSURE_PLATE_INDEX, {location.x, location.y, size.x, size.y}/*, (int)diceData->getDiceSideRotation(sideId)*/);
}

// ################################# HandleEvent und Update-Methoden #################################################################

void CubeField::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    for (auto cubeObject : this->cubeObjects) {
        cubeObject->Update(game, frame, totalMSec, deltaT);
    }
}

// ################################# Setter & Getter #################################################################################

void CubeField::setSideId(int sideID) {
    this->sideId = sideID;
    for (auto cubeObject : this->cubeObjects) {
        cubeObject->setSideId(this->sideId);
    }
}

void CubeField::setDiceData(DiceData* dice_data) {
    this->diceData = dice_data;
    for (auto cubeObject : this->cubeObjects) {
        cubeObject->setDiceData(this->diceData);
    }
}

void CubeField::setCubeMapSideRef(CubeMapSide* cube_map_side) {
    this->cubeMapSideRef = cube_map_side;
}

void CubeField::setGravityDirection(MovementDirection dir) {
    for (auto cubeObject : this->cubeObjects) {
        if (cubeObject->getType() == CubeObject::ObjectType::typeStone) {
            dynamic_cast<Stone*>(cubeObject)->setFallingDirection(dir);
        }
    }
}

CubeMapSide* CubeField::getCubeMapSideRef() {
    return this->cubeMapSideRef;
}

Point CubeField::getCoordinates() {
    return {.x = this->x, .y = this->y};
}

int CubeField::getX() const {
    return this->x;
}

int CubeField::getY() const {
    return this->y;
}

Magnet* CubeField::getMagnetIfPresent() {
    for (auto cubeObj : this->cubeObjects) {
        if (cubeObj->getType() == CubeObject::ObjectType::typeMagnet) {
            return dynamic_cast<Magnet *>(cubeObj);
        }
    }
    return nullptr;
}

bool PressurePlate::getIsActivated() const {
    return this->isActivated;
}

int PressurePlate::getId() const {
    return this->id;
}

// ################################# canPlayerEnter-Methoden #########################################################################

bool EmptyField::canPlayerEnter() {
    for (CubeObject* cubeObject : this->cubeObjects) {
        if (!cubeObject->canPlayerEnter()) {
            return false;
        }
    }
    return true;
}

bool Static::canPlayerEnter() {
    return false;
}

bool PressurePlate::canPlayerEnter() {
    for (CubeObject* cubeObject : this->cubeObjects) {
        if (!cubeObject->canPlayerEnter()) {
            return false;
        }
    }
    return true;
}

// ################################# canObjectEnter-Methoden #########################################################################

bool EmptyField::canObjectEnter(CubeObject *cubeObject) {
    CubeMapSide* cubeMapSide = this->cubeMapSideRef;
    CubeMap* cubeMap = cubeMapSide->getCubeMapRef();
    Point currentPlayerPos = cubeMap->getCurrentPlayerPos();
    if (currentPlayerPos.x == this->x && currentPlayerPos.y == this->y && cubeObject->getType() != CubeObject::ObjectType::typeMagnet) {
        return false;
    }
    for (auto anyCubeObject : this->cubeObjects) {
        if (!anyCubeObject->canAnotherObjectEnter()) {
            return false;
        }
    }
    return true;
}

bool Static::canObjectEnter(CubeObject *cubeObject) {
    return false;
}

bool Grass::canObjectEnter(CubeObject *cubeObject) {
    return false;
}

bool PressurePlate::canObjectEnter(CubeObject *cubeObject) {
    CubeMapSide* cubeMapSide = this->cubeMapSideRef;
    CubeMap* cubeMap = cubeMapSide->getCubeMapRef();
    Point currentPlayerPos = cubeMap->getCurrentPlayerPos();
    if (currentPlayerPos.x == this->x && currentPlayerPos.y == this->y && cubeObject->getType() != CubeObject::ObjectType::typeMagnet) {
        return false;
    }
    for (auto anyCubeObject : this->cubeObjects) {
        if (!anyCubeObject->canAnotherObjectEnter()) {
            return false;
        }
    }
    return cubeObject->canEnterPressurePlate();
}

// ################################# sonstige Methoden ###############################################################################

bool CubeField::isLevelFinishedIfEntered() {
    for (CubeObject* cubeObject : this->cubeObjects) {
        if (cubeObject->isLevelFinishedIfEntered()) {
            return true;
        }
    }
    return false;
}

bool CubeField::isPressurePlate() {
    return false;
}

bool PressurePlate::isPressurePlate() {
    return true;
}

void CubeField::addObject(CubeObject* cubeObject) {
    this->cubeObjects.push_back(cubeObject);
    if (this->isPressurePlate() && cubeObject->canActivatePressurePlate()) {
        dynamic_cast<PressurePlate*>(this)->activate();
    }
}

bool CubeField::removeObject(CubeObject* cubeObject) {
    for (int i = 0; i < cubeObjects.size(); i++) {
        if (cubeObject->getType() == this->cubeObjects[i]->getType()) {
            this->cubeObjects.erase(this->cubeObjects.begin() + i);
            if (this->isPressurePlate() && cubeObject->canActivatePressurePlate()) {
                dynamic_cast<PressurePlate*>(this)->deactivate();
            }
            return true;
        }
    }
    return false;
}


int CubeField::enter() {
    return -1;
}

/// returns the id of the PressurePlate. The Sliders activated by this PressurePlate have the same id.
int PressurePlate::enter() {
    if (!this->canPlayerEnter()) {
        return -1;
    }
    if (!this->isActivated) {
        this->isActivated = true;
        this->activateAllSlidersWithSameId();
        return this->id;
    }
    return -1;
}

int CubeField::leave() {
    return -1;
}

int PressurePlate::leave() {
    if (!this->isActivated) {
        return -1;
    }
    this->isActivated = false;
    this->deactivateAllSlidersWithSameId();
    return this->id;
}

int PressurePlate::activate() {
    if (!this->isActivated) {
        this->isActivated = true;
        this->activateAllSlidersWithSameId();
        return this->id;
    }
    return -1;
}

int PressurePlate::deactivate() {
    if (this->isActivated) {
        this->isActivated = false;
        this->deactivateAllSlidersWithSameId();
        return this->id;
    }
    return -1;
}

void PressurePlate::activateAllSlidersWithSameId() {
    CubeMap* cubeMap = this->cubeMapSideRef->getCubeMapRef();
    Vector<CubeMapSide*> allCubeMapSides = *cubeMap->getAllCubeMapSides();
    for (auto anyCubeMapSide : allCubeMapSides) {
        for (auto cubeField : anyCubeMapSide->cubeFields) {
            for (auto cubeObject : cubeField->cubeObjects) {
                if (cubeObject->isSlider()) {
                    auto *slider = dynamic_cast<Slider*>(cubeObject);
                    if (slider->getId() == this->id) {
                        slider->activate();
                    }
                }
            }
        }
    }
}

void PressurePlate::deactivateAllSlidersWithSameId() {
    CubeMap* cubeMap = this->cubeMapSideRef->getCubeMapRef();
    Vector<CubeMapSide*> allCubeMapSides = *cubeMap->getAllCubeMapSides();
    for (auto anyCubeMapSide : allCubeMapSides) {
        for (auto cubeField : anyCubeMapSide->cubeFields) {
            for (auto cubeObject : cubeField->cubeObjects) {
                if (cubeObject->isSlider()) {
                    auto *slider = dynamic_cast<Slider*>(cubeObject);
                    if (slider->getId() == this->id) {
                        slider->deactivate();
                    }
                }
            }
        }
    }
}