#pragma once
#include <cmath>

static double LerpDegrees(double start, double end, double amount) {
    double difference = abs(end - start);
    if (difference > 180) {
        if (end > start) {
            start += 360;
        } else {
            end += 360;
        }
    }

    // Interpolate it.
    double value = (start + ((end - start) * amount));

    // Wrap it..
    int rangeZero = 360;

    if (value >= 0 && value <= 360)
        return value;

    return ((int) value % rangeZero);
}