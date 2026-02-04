#ifndef TYPES_H

#include <math.h>

#include "raylib.h"
#include "../lib/random.h"

typedef struct Ball Ball;
typedef struct Paddle Paddle;

typedef struct Ball {
    Vector2 pos;
    Vector2 vel;

    Color color;
    float size;

    // existent just cuz
    // also cuz i need an excuse to use callbacks
    struct {
        void (*on_paddle_coll)(Ball* ball, Paddle* paddle);
        void (*on_wall_coll)(void);
    } callbacks;
} Ball;

typedef struct Paddle {
    Vector2 pos;
    float size;
    float speed;
} Paddle;

void Ball_RandomVelocity(float, float, Ball*);

#endif