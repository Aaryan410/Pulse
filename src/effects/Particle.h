#pragma once
#include "Color.h"

struct Particle {
    float x;
    float y;

    float velocityX;
    float velocityY;

    float lifetime;
    float maxLifetime;

    int size;

    Color startColor;
    Color endColor;
};