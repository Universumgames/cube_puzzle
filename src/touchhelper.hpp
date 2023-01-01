#pragma once
#include "global.hpp"

constexpr Point uvToPixel(Point screenSize, FPoint uvLocation){
    return {(int) (screenSize.x * uvLocation.x), (int) (screenSize.y * uvLocation.y)};
}
