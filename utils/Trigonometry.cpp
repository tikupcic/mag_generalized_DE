//
// Created by timi on 7.8.2017.
//

#include <math.h>
#include "Trigonometry.h"

#define RAD_TO_ANGLE 3.14159265 / 180

float sin(float angle) {
    return sinf((float) (angle * RAD_TO_ANGLE));
}

float cos(float angle) {
    return cosf((float) (angle * RAD_TO_ANGLE));
}