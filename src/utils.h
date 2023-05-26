#pragma once
#include "raylib.h"
#include <cmath>

struct IntVector2 {
    int x, y;
};

void clamp(int& value, int min, int max);

Vector2 normalize(Vector2 vector);