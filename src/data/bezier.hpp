#pragma once

#include "../global.hpp"
#include "../recthelper.hpp"

// https://javascript.info/bezier-curve#maths

// quadratic bezier curve
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

static Point bezierPoint(Point a, Point b, Point c, Point d, double t) {
    double e = 1 - t;
    double e2 = e * e;
    double e3 = e2 * e;
    double t2 = t * t;
    double t3 = t2 * t;
    return a * e3 + b * 3 * e2 * t + c * 3 * e * t2 + d * t3;
}