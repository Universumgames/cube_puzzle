#pragma once

#include <array>
#include <algorithm>

#include <cstdarg>
#include <cstddef>
#include <cmath>

#include <algorithm>
#include <chrono>
#include <memory>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define SDL_MAIN_HANDLED

#if defined(_WIN32) || defined(Macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)

#include <SDL_stdinc.h>

#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


#else
#include <SDL2/SDL_stdinc.h>

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif

using uint = unsigned int;
using i32 = int32_t;
using u32 = uint32_t;

using Color = SDL_Color;
using Event = SDL_Event;
using Point = SDL_Point;
using Keysym = SDL_Keysym;
using Rect = SDL_Rect;
using Surface = SDL_Surface;
using Texture = SDL_Texture;
using Renderer = SDL_Renderer;
using Window = SDL_Window;
using Font = TTF_Font;

using std::max;
using std::min;

template<class T, size_t Size>
using Array = std::array<T, Size>;
template<class T>
using Vector = std::vector<T>;
template<class K, class V>
using Map = std::map<K, V>;

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

using std::cout, std::cin, std::cerr, std::endl;

#define BASIC_GO_DATA_PASSTHROUGH frame, totalMSec, deltaT