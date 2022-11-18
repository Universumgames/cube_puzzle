#pragma once

#include "../global.hpp"
#include "../recthelper.hpp"

static Point bezierPoint(Point a, Point b, Point c, double t) {
    // return (b-a) * (1*(1-t)) + (c - b) * 2 * t;
    /*double d = 1-t;
    double d2 = d*d;
    return a * d2 + b *2*d + c *(t*t);*/
    // calculate the bezier point at time t
    //double x = (1 - t) * (1 - t) * a.x + 2 * (1 - t) * t * b.x + t * t * c.x;
    //double y = (1 - t) * (1 - t) * a.y + 2 * (1 - t) * t * b.y + t * t * c.y;
    //return Point{(int)x, (int)y};
    return a * (1 - t) * (1 - t) + b * 2 * (1 - t) * t + c * t * t;
}