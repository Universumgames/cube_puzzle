#include "CubeField.hpp"
#include "GameObject.hpp"

void
CubeField::Render(CubeGame &game, Renderer *render, Point size, Point location, const u32 frame, const u32 totalMSec,
                  const float deltaT) {
    SDL_SetRenderDrawColor(render, 200, 200, 0, 255);

    Rect dst = {location.x, location.y, size.x, size.y};
    SDL_RenderFillRect(render, &dst);
}
