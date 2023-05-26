#pragma once
#include "raylib.h"

struct particle {
    int id;
    float lifeTime;
    int velocity;
    Color color;
    bool hasBeenUpdated;
};