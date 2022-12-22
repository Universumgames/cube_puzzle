#pragma once

#include "GameObject.hpp"
#include "../data/WorldField.hpp"
#include "../data/MoveDirections.hpp"
#include "../data/DiceData.hpp"
#include "Text.hpp"

class CubeMap;

/// World map is viewed top down
class WorldMap final : public GameObject {
public:
    WorldMap(CubeGame &game, ComplexGameState* gameState, SDL_Renderer *render, Point size, Vector<WorldField::WorldFieldEnum> map, Point cubePos);

    /// set cube map pointer, has to be called before first run
    void setCubeMap(CubeMap *cubeMap) { this->cubeMap = cubeMap; }

    void HandleEvent(const u32 frame, const u32 totalMSec, const float deltaT, Event event) override;

    void Update(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void Render(const u32 frame, const u32 totalMSec, const float deltaT) override;

    void RenderUI(const u32 frame, const u32 totalMSec, const float deltaT) override;

private:
    Vector<WorldField::WorldFieldEnum> field;
    Point size;
    CubeMap *cubeMap;
    Point cubePos;
    Text *mapCoordinates;

    int getFieldIndex(int x, int y) const;

    void drawMinimap();

    void fixCubePosOutBounds();

    friend class CubeMap;
};

inline void getWorldFieldTextureSource(WorldField::WorldFieldEnum field, SpriteStorage *spriteStorage, Texture **texture, Rect *src) {
    switch (field) {
        case WorldField::WorldFieldEnum::DEFAULT:
            *src = {0, 0, 16, 16};
            break;
        case WorldField::WorldFieldEnum::LAVA:
            break;
        case WorldField::WorldFieldEnum::ICE:
            *src = {16, 16, 16, 16};
            break;
    }
}

inline void drawWorldField(Renderer *render, SpriteStorage *spriteStorage, WorldField::WorldFieldEnum field, Rect dst) {
    Rect src = {};
    Texture *tex;
    getWorldFieldTextureSource(field, spriteStorage, &tex, &src);
    SDL_RenderCopy(render, tex, &src, &dst);
}