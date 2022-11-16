#pragma once
#include "global.hpp"

using Matrix2x2 = Rect;
using Vector2D = Point;

/**
 * matrix is defined like\n
 * 1 3 \n
 * 2 4 \n
 * with { 1, 2, 3, 4 }
 */
constexpr Vector2D operator*(const Matrix2x2 lhs, const Vector2D rhs) { return Vector2D{lhs.x * rhs.x + lhs.w * rhs.y, lhs.y * rhs.x + lhs.h * rhs.y}; }