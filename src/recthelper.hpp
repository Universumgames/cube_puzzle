#pragma once

#include "global.hpp"

constexpr const Rect *EntireRect = nullptr;

constexpr Point operator "" _up(unsigned long long n) { return Point{0, -(int) n}; }

constexpr Point operator "" _down(unsigned long long n) { return Point{0, (int) n}; }

constexpr Point operator "" _left(unsigned long long n) { return Point{-(int) n, 0}; }

constexpr Point operator "" _right(unsigned long long n) { return Point{(int) n, 0}; }

constexpr Point operator+(const Point lhs, const Point rhs) { return Point{lhs.x + rhs.x, lhs.y + rhs.y}; }

constexpr Point operator-(const Point lhs, const Point rhs) { return Point{lhs.x - rhs.x, lhs.y - rhs.y}; }

constexpr Point operator/(const Point lhs, const int rhs) { return Point{lhs.x / rhs, lhs.y / rhs}; }

constexpr Point operator*(const Point lhs, const int rhs) { return Point{lhs.x * rhs, lhs.y * rhs}; }

constexpr Point operator*(const Point lhs, const double rhs) { return Point{(int) (lhs.x * rhs), (int) (lhs.y * rhs)}; }

constexpr Point operator%(const Point lhs, const int rhs) { return Point{lhs.x % rhs, lhs.y % rhs}; }

constexpr Point &operator+=(Point &lhs, const Point rhs) {
    lhs = lhs + rhs;
    return lhs;
}

constexpr Point &operator-=(Point &lhs, const Point rhs) {
    lhs = lhs - rhs;
    return lhs;
}

constexpr bool operator==(const Point &lhs, const Point rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

constexpr bool operator!=(const Point &lhs, const Point rhs) {
    return !(lhs == rhs);
}

constexpr Rect operator+(const Point lhs, const Rect rhs) { return Rect{lhs.x + rhs.x, lhs.y + rhs.y, rhs.w, rhs.h}; }

constexpr Rect operator+(const Rect lhs, const Point rhs) { return Rect{lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h}; }

constexpr Rect operator-(const Rect lhs, const Point rhs) { return Rect{lhs.x - rhs.x, lhs.y - rhs.y, lhs.w, lhs.h}; }

constexpr Rect &operator+=(Rect &lhs, const Point rhs) {
    lhs = lhs + rhs;
    return lhs;
}

constexpr Rect &operator-=(Rect &lhs, const Point rhs) {
    lhs = lhs - rhs;
    return lhs;
}

constexpr Rect operator+(const Rect lhs, const Rect rhs) {
    return Rect{lhs.x + rhs.x, lhs.y + rhs.y, lhs.w + rhs.w, lhs.h + rhs.h};
}

constexpr Rect operator-(const Rect lhs, const Rect rhs) {
    return Rect{lhs.x - rhs.x, lhs.y - rhs.y, lhs.w - rhs.w, lhs.h - rhs.h};
}

constexpr Rect &operator+=(Rect &lhs, const Rect rhs) {
    lhs = lhs + rhs;
    return lhs;
}

constexpr Rect &operator-=(Rect &lhs, const Rect rhs) {
    lhs = lhs - rhs;
    return lhs;
}

constexpr Rect addPadding(Rect r, int padding) {
    return {r.x + padding, r.y + padding, r.w - 2 * padding, r.h - 2 * padding};
}

constexpr Rect addPadding(Rect r, int top, int bottom, int left, int right) {
    return {r.x + left, r.y + top, r.w - left - right, r.h - top - bottom};
}

constexpr Rect scale(Rect r, double scale) {
    return {r.x, r.y, (int) (r.w * scale), (int) (r.h * scale)};
}

constexpr Rect centerIn(Rect toCenter, Rect context) {
    Point center = Point{context.x, context.y} + (Point{context.w, context.h} / 2);
    Point offset = center - (Point{toCenter.w, toCenter.h} / 2);
    return {offset.x, offset.y, toCenter.w, toCenter.h};
}

constexpr Rect centerInHorizontal(Rect toCenter, Rect context) {
    Point center = Point{context.x, context.y} + (Point{context.w, context.h} / 2);
    Point offset = center - (Point{toCenter.w, toCenter.h} / 2);
    return {offset.x, toCenter.y, toCenter.w, toCenter.h};
}

constexpr Rect centerInVertical(Rect toCenter, Rect context) {
    Point center = Point{context.x, context.y} + (Point{context.w, context.h} / 2);
    Point offset = center - (Point{toCenter.w, toCenter.h} / 2);
    return {toCenter.x, offset.y, toCenter.w, toCenter.h};
}

constexpr FPoint operator+(const FPoint lhs, const FPoint rhs) { return FPoint{lhs.x + rhs.x, lhs.y + rhs.y}; }

constexpr FPoint operator-(const FPoint lhs, const FPoint rhs) { return FPoint{lhs.x - rhs.x, lhs.y - rhs.y}; }

constexpr FPoint operator/(const FPoint lhs, const int rhs) { return FPoint{lhs.x / rhs, lhs.y / rhs}; }

constexpr FPoint operator*(const FPoint lhs, const int rhs) { return FPoint{lhs.x * rhs, lhs.y * rhs}; }

constexpr FPoint operator*(const FPoint lhs, const double rhs) {
    return FPoint{(float) (lhs.x * rhs), (float) (lhs.y * rhs)};
}
