#ifndef USEFUL_H

#include "raylib.h"
#include <stdio.h>

typedef struct Ball Ball;
typedef struct Paddle Paddle;

// probably just change these to formatting instead of printing so Paddle/Ball_Print can work with these
void RectanglePrint(Rectangle);
void Vector2Print(Vector2);
void BallPrint(Ball*);
void PaddlePrint(Paddle*);

#endif