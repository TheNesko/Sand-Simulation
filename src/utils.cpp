#include "utils.h"

void clamp(int& value, int min, int max){
    if (value > max) value = max;
    if (value < min) value = min;
}

Vector2 normalize(Vector2 vector){
    Vector2 normalized;
    float length = sqrt(vector.x*vector.x + vector.y*vector.y);
    normalized.x = vector.x / length;
    normalized.y = vector.y / length;
    return normalized;
}