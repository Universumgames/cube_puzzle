#include "CubeField.hpp"
#include "GameObject.hpp"
#include "../data/SpriteStorage.hpp"
#include "../data/Colors.hpp"
#include "../data/spriteDefs.hpp"
#include "../recthelper.hpp"

// ################################# Konstruktoren ###################################################################################

CubeField::CubeField(int sideId, int x, int y, const Vector<CubeObject *> &cubeObjects) {
    this->sideId = sideId;
    this->cubeObjects = cubeObjects;
    this->x = x;
    this->y = y;
    for (auto cubeObject: cubeObjects) {
        cubeObject->setCubeFieldRef(this);
    }
}

EmptyField::EmptyField(int sideId, int x, int y, const Vector<CubeObject *> &cubeObjects)
        : CubeField(sideId, x, y, cubeObjects) {}

ObjectBarrier::ObjectBarrier(int sideId, int x, int y, const Vector<CubeObject *> &cubeObjects)
        : EmptyField(sideId, x, y, cubeObjects) {}

ArrowField::ArrowField(int sideId, int x, int y, MovementDirection arrowDirection,
                       const Vector<CubeObject *> &cubeObjects)
        : EmptyField(sideId, x, y, cubeObjects) {
    this->arrowDirection = arrowDirection;
}

Static::Static(int sideId, int x, int y, const Vector<CubeObject *> &cubeObjects)
        : CubeField(sideId, x, y, cubeObjects) {}

Wall::Wall(int sideId, int x, int y, const Vector<CubeObject *> &cubeObjects)
        : Static(sideId, x, y, cubeObjects) {}

Interactable::Interactable(int sideId, int x, int y, const Vector<CubeObject *> &cubeObjects)
        : CubeField(sideId, x, y, cubeObjects) {}

PressurePlate::PressurePlate(int sideId, int x, int y, int id, bool activated, const Vector<CubeObject *> &cubeObjects)
        : Interactable(sideId, x, y, cubeObjects) {
    this->id = id;
    this->isActivated = activated;
}

// ################################# Alle Render-Methoden ############################################################################

void
CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                  const float deltaT) {
    Rect dst = {location.x, location.y, size.x, size.y};
    drawSpriteBorder(game.isDebug(), render, dst);
    Point locationNew = {location.x + 10, location.y + 10};
    Point sizeNew = {size.x - 20, size.y - 20};
    for (CubeObject *cubeObject: this->cubeObjects) {
        cubeObject->Render(game, render, sizeNew, locationNew, frame, totalMSec, deltaT);
        locationNew = {locationNew.x + 10, locationNew.y + 10};
        sizeNew = {sizeNew.x - 20, sizeNew.y - 20};
    }
}

void
EmptyField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                   const float deltaT) {
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void ObjectBarrier::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                           float deltaT) {
    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, SPRITE_OBJECTBLOCKER_INDEX,
               {location.x, location.y, size.x, size.y});
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void ArrowField::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                        float deltaT) {
    int angle = 0;
    auto flip = SDL_FLIP_NONE;
    switch (arrowDirection) {
        case MovementDirection::moveToBigX:
            angle = 0;
            break;
        case MovementDirection::moveToSmallX:
            angle = 180;
            break;
        case MovementDirection::moveToBigY:
            angle = 90;
            break;
        case MovementDirection::moveToSmallY:
            angle = 270;
            break;
        case MovementDirection::none:
            break;
    }

    angle += (int) diceData->getDiceSideRotation(sideId);
    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderCopyEx(render, game.getSpriteStorage()->arrowStraight, NULL, &dst, angle, NULL, flip);
}

void
Wall::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec, float deltaT) {
    Point locationToCheck = {};
    switch (diceData->getDiceSideRotation(sideId)) {
        case DiceSideRotation::UP:
            locationToCheck = {x, y + 1};
            break;
        case DiceSideRotation::DOWN:
            locationToCheck = {x, y - 1};
            break;
        case DiceSideRotation::LEFT:
            locationToCheck = {x - 1, y};
            break;
        case DiceSideRotation::RIGHT:
            locationToCheck = {x + 1, y};
            break;
    }
    bool isWall1 = dynamic_cast<Wall *>(cubeMapSideRef->getField(locationToCheck.x, locationToCheck.y)) == nullptr;

    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, isWall1 ? SPRITE_WALL1_INDEX : SPRITE_WALL2_INDEX,
               {location.x, location.y, size.x, size.y});
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

void PressurePlate::Render(CubeGame &game, Renderer *render, Point size, Point location, u32 frame, u32 totalMSec,
                           float deltaT) {
    drawSprite(game.getSpriteStorage()->cubeFieldSpriteSheet, render, SPRITE_PRESSURE_PLATE_INDEX + Point{0, id},
               {location.x, location.y, size.x, size.y});
    CubeField::Render(game, render, size, location, frame, totalMSec, deltaT);
}

// ################################# HandleEvent und Update-Methoden #################################################################

void CubeField::Update(CubeGame &game, u32 frame, u32 totalMSec, float deltaT) {
    for (auto cubeObject: this->cubeObjects) {
        cubeObject->Update(game, frame, totalMSec, deltaT);
    }
}

// ################################# Setter & Getter #################################################################################

void CubeField::setSideId(int sideID) {
    this->sideId = sideID;
    for (auto cubeObject: this->cubeObjects) {
        cubeObject->setSideId(this->sideId);
    }
}

void CubeField::setDiceData(DiceData *dice_data) {
    this->diceData = dice_data;
    for (auto cubeObject: this->cubeObjects) {
        cubeObject->setDiceData(this->diceData);
    }
}

void CubeField::setCubeMapSideRef(CubeMapSide *cube_map_side) {
    this->cubeMapSideRef = cube_map_side;
}

void CubeField::setGravityDirection(MovementDirection dir) {
    for (auto cubeObject: this->cubeObjects) {
        if (cubeObject->getType() == CubeObject::ObjectType::typeStone) {
            dynamic_cast<Stone *>(cubeObject)->setFallingDirection(dir);
        }
    }
}

CubeMapSide *CubeField::getCubeMapSideRef() {
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

Magnet *CubeField::getMagnetIfPresent() {
    for (auto cubeObj: this->cubeObjects) {
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
    for (CubeObject *cubeObject: this->cubeObjects) {
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
    for (CubeObject *cubeObject: this->cubeObjects) {
        if (!cubeObject->canPlayerEnter()) {
            return false;
        }
    }
    return true;
}

// ################################# canObjectEnter-Methoden #########################################################################

bool EmptyField::canObjectEnter(CubeObject *cubeObject) {
    CubeMapSide *cubeMapSide = this->cubeMapSideRef;
    CubeMap *cubeMap = cubeMapSide->getCubeMapRef();
    Point currentPlayerPos = cubeMap->getCurrentPlayerPos();
    if (currentPlayerPos.x == this->x && currentPlayerPos.y == this->y &&
        cubeObject->getType() != CubeObject::ObjectType::typeMagnet) {
        return false;
    }
    for (auto anyCubeObject: this->cubeObjects) {
        if (!anyCubeObject->canAnotherObjectEnter()) {
            return false;
        }
    }
    return true;
}

bool Static::canObjectEnter(CubeObject *cubeObject) {
    return false;
}

bool ObjectBarrier::canObjectEnter(CubeObject *cubeObject) {
    return false;
}

bool PressurePlate::canObjectEnter(CubeObject *cubeObject) {
    CubeMapSide *cubeMapSide = this->cubeMapSideRef;
    CubeMap *cubeMap = cubeMapSide->getCubeMapRef();
    Point currentPlayerPos = cubeMap->getCurrentPlayerPos();
    if (currentPlayerPos.x == this->x && currentPlayerPos.y == this->y &&
        cubeObject->getType() != CubeObject::ObjectType::typeMagnet) {
        return false;
    }
    for (auto anyCubeObject: this->cubeObjects) {
        if (!anyCubeObject->canAnotherObjectEnter()) {
            return false;
        }
    }
    return cubeObject->canEnterPressurePlate();
}

// ################################# sonstige Methoden ###############################################################################

bool CubeField::isLevelFinishedIfEntered() {
    for (CubeObject *cubeObject: this->cubeObjects) {
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

void CubeField::addObject(CubeObject *cubeObject) {
    this->cubeObjects.push_back(cubeObject);
    if (this->isPressurePlate() && cubeObject->canActivatePressurePlate()) {
        dynamic_cast<PressurePlate *>(this)->activate();
    }
}

bool CubeField::removeObject(CubeObject *cubeObject) {
    for (int i = 0; i < cubeObjects.size(); i++) {
        if (cubeObject->getType() == this->cubeObjects[i]->getType()) {
            this->cubeObjects.erase(this->cubeObjects.begin() + i);
            if (this->isPressurePlate() && cubeObject->canActivatePressurePlate()) {
                dynamic_cast<PressurePlate *>(this)->deactivate();
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

void CubeField::HandleEvent(CubeGame &game, u32 frame, u32 totalMSec, float deltaT, Event event) {
    for (CubeObject *field: cubeObjects) {
        field->HandleEvent(game, BASIC_GO_DATA_PASSTHROUGH, event);
    }
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
    if (this->isActivated)
        return -1;
    this->isActivated = true;
    this->activateAllSlidersWithSameId();
    PressurePlate::getAudioEnter()->playOnce();
    return this->id;
}

int PressurePlate::deactivate() {
    if (!this->isActivated)
        return -1;
    this->isActivated = false;
    this->deactivateAllSlidersWithSameId();
    PressurePlate::getAudioEnter()->playOnce();
    return this->id;
}

void PressurePlate::activateAllSlidersWithSameId() {
    CubeMap *cubeMap = this->cubeMapSideRef->getCubeMapRef();
    Vector<CubeMapSide *> allCubeMapSides = *cubeMap->getAllCubeMapSides();
    for (auto anyCubeMapSide: allCubeMapSides) {
        for (auto cubeField: anyCubeMapSide->cubeFields) {
            for (auto cubeObject: cubeField->cubeObjects) {
                if (!cubeObject->isSlider()) continue;
                auto *slider = dynamic_cast<Slider *>(cubeObject);
                if (slider->getId() == this->id) {
                    slider->activate();
                }
            }
        }
    }
}

void PressurePlate::deactivateAllSlidersWithSameId() {
    CubeMap *cubeMap = this->cubeMapSideRef->getCubeMapRef();
    Vector<CubeMapSide *> allCubeMapSides = *cubeMap->getAllCubeMapSides();
    for (auto anyCubeMapSide: allCubeMapSides) {
        for (auto cubeField: anyCubeMapSide->cubeFields) {
            for (auto cubeObject: cubeField->cubeObjects) {
                if (!cubeObject->isSlider()) continue;
                auto *slider = dynamic_cast<Slider *>(cubeObject);
                if (slider->getId() == this->id) {
                    slider->deactivate();
                }
            }
        }
    }
}

AudioPlayer *enterPressurePlateSound = nullptr;

AudioPlayer *PressurePlate::getAudioEnter() {
    if (enterPressurePlateSound == nullptr) {
        enterPressurePlateSound = new AudioPlayer(MUSIC_PRESSURE_PLATE_PRESSED);
    }
    return enterPressurePlateSound;
}
