#include "raylib.h"
#include "types.h"
#include "useful.h"
#include <stdio.h>

void RectanglePrint(Rectangle r) {
    printf("{ x: %f, y: %f, w: %f, h: %f }\n", r.x, r.y, r.width, r.height);
}
void Vector2Print(Vector2 v) {
    printf("{ x: %f, y: %f }\n", v.x, v.y);
}

void BallPrint(Ball* ball) {
    printf("pos={ %f, %f }, vel={ %f, %f }, sz=%f\n", ball->pos.x, ball->pos.y, ball->vel.x, ball->vel.y, ball->size);
}
void PaddlePrint(Paddle* paddle) {
    printf("pos={ %f, %f }, sz=%f, spd=%f\n", paddle->pos.x, paddle->pos.y, paddle->size, paddle->speed);
}