#include "touchhelper.hpp"

std::map<SDL_FingerID, SDL_TouchFingerEvent> lastTouchEvents = std::map<SDL_FingerID, SDL_TouchFingerEvent>();