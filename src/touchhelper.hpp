#pragma once
#include "global.hpp"

extern std::map<SDL_FingerID, SDL_TouchFingerEvent> lastTouchEvents;

constexpr Point uvToPixel(Point screenSize, FPoint uvLocation){
    return {(int) (screenSize.x * uvLocation.x), (int) (screenSize.y * uvLocation.y)};
}

inline bool registerTouchEvent(Event event){
    if(event.type == SDL_FINGERUP || event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION){
        lastTouchEvents[event.tfinger.fingerId] = event.tfinger;
        return true;
    }
    return false;
}

inline void clearTouchEvents(){
    //cout << lastTouchEvents.size() << endl;
    //lastTouchEvents.clear();
    auto iter = lastTouchEvents.begin();
    for(; iter != lastTouchEvents.end();){
        if(iter->second.type == SDL_FINGERUP){
            iter = lastTouchEvents.erase(iter);
        }else ++iter;
    }
}

inline void simulateKeyPress(SDL_KeyboardEvent keyboard){
    SDL_Event sdlevent = {};
    sdlevent.type = SDL_KEYDOWN;
    sdlevent.key = keyboard;
    SDL_PushEvent(&sdlevent);
}

inline void simulateKeyPress(SDL_Scancode scancode){
    SDL_Event sdlevent = {};
    sdlevent.type = SDL_KEYDOWN;
    sdlevent.key.keysym.scancode = scancode;
    SDL_PushEvent(&sdlevent);
}

inline void simulateKeyRelease(SDL_Scancode scancode){
    SDL_Event sdlevent = {};
    sdlevent.type = SDL_KEYUP;
    sdlevent.key.keysym.scancode = scancode;
    SDL_PushEvent(&sdlevent);
}
