#include "CubeField.hpp"
#include "GameObject.hpp"

void
CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                  const float deltaT) {
    SDL_SetRenderDrawColor(render, 200, 200, 0, 255);

    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
}

CubeField::TYPE getType(char firstChar){
    return (CubeField::TYPE) firstChar;
}

CubeField *CubeField::decode(std::string data) {
    TYPE type = getType(data[0]);
    data.pop_back();
    // TODO implement switch cases
    switch(type){
        case TYPE::EMPTY:
            return EmptyField::decode(data);
        case TYPE::STATIC:
            break;
        case TYPE::GRAVITY:
            return GravityObject::decode(data);
        case TYPE::INTERACTABLE:
            break;
        case TYPE::ACTIVATABLE:
            return Activatable::decode(data);
    }
    return nullptr;
}

Activatable *Activatable::decode(std::string data) {
    char c = data [0];
    data.pop_back();
    switch(c){
        case 'p':
            return Piston::decode(data);
        case 's':
            return SlidingWall::decode(data);
        default:
            return nullptr;
    }
    return nullptr;
}
