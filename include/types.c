#include "raylib.h"
#include "types.h"

void Ball_RandomVelocity(float range, float minmag, Ball* ball) {
    ball->vel = (Vector2){ .x = randf_range(-range, range), .y = randf_range(-range, range) / 2 };

    if (fabsf(ball->vel.x) < minmag) ball->vel.x = minmag;
}