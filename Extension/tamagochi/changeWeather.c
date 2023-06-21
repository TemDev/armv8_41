#include "raylib.h"

void sunUp(weatherObject *sun) {
    if (sun->position.x > sunUpPos - error && sun->position.x < sunUpPos + error) {
        sun->position.x += valueLessThanError;
    }
}