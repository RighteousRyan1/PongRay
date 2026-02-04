/*

Very boring pong implementation to see if I can do things in raylib just fine!
Basically everyone's litmus test for testing new APIs, I feel.
Also a custom random implementation. Because I love reinventing wheels pointlessly.
More (and greater) to come.

Made by RighteousRyan

*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"
#include "include/useful.h"
#include "include/types.h"

#include "lib/random.h"

// game objects
static Paddle* left;
static Paddle* right;
static Ball* ball;

// useful stuff
static int vp_current;
static int vp_width;
static int vp_height;

static Rectangle wind_bounds;

static int paddle_size = 50;
#define pad_color RED
static int paddle_offset_x = 20;

static KeyboardKey lpad_up   = KEY_W;
static KeyboardKey lpad_down = KEY_S;

static KeyboardKey rpad_up   = KEY_UP;
static KeyboardKey rpad_down = KEY_DOWN;

static float pad_move_speed = 500.0f;

static int scorel;
static int scorer;

static float ball_vel_rand_range = 250.0f;
static float ball_minmag = 200.0f;

static float ball_refl_gain = 1.15f;
// when a ball hits a paddle, the relfection is not very predictable in the original pong
static float ball_y_vel_rand_range = 300.0f;
static float ball_size_default = 5.0f;
static float ball_size = 5.0f;

Sound* hitl_sfx;
Sound* hitr_sfx;

static int hit_count;

// prevent scaling spam from the ball
// this is horrible, but hey it works :)
// will employ better practices with bigger projects
#define SCALING_COOLDOWN_COUNT 2
static float scaling_cooldown[SCALING_COOLDOWN_COUNT];

Sound* score_sfx;

void on_paddle_coll(Ball* ball, Paddle* paddle) {
    ball->vel.x *= -ball_refl_gain;
    ball->vel.y += randf_range(-ball_y_vel_rand_range, ball_y_vel_rand_range);
    float size_prev = ball_size;
    ball_size *= randf_range(1.0f, 2.0f);
        
    if (hit_count % 2 == 0) PlaySound(*hitl_sfx);
    else PlaySound(*hitr_sfx);
    hit_count++;
        
    // prevents cheeky scoring when ball scales up
    if (paddle == left) {
        ball->pos.x += (ball_size - size_prev);
        scaling_cooldown[0] = 1;
    }
    else {
        ball->pos.x -= (ball_size - size_prev);
        scaling_cooldown[1] = 1;
    }
}
void on_wall_coll() {
    ball->vel.y *= -ball_refl_gain;
}

bool init() {
    seed = (int)GetTime();

    size_t pd_sz = sizeof(Paddle);
    left  = malloc(pd_sz);
    right = malloc(pd_sz);
    ball  = malloc(sizeof(Ball));

    // return false for any failed allocs
    if (!left || !right || !ball) {
        printf("Catastrophic failure.\nleft=%p, right=%p, ball=%p", (void*)&left, (void*)&right, (void*)&ball);
        return false;
    }
    left->pos.x = paddle_offset_x;
    right->pos.x = wind_bounds.width - paddle_offset_x;

    left->size = paddle_size;
    right->size = paddle_size;

    ball->color = RED;
    ball->size = ball_size_default;

    Ball_RandomVelocity(ball_vel_rand_range, ball_minmag, ball);

    Vector2 center = { .x = wind_bounds.width / 2, .y = wind_bounds.height / 2 };
    ball->pos = center;

    hitl_sfx = malloc(sizeof(Sound));
    *hitl_sfx = LoadSound("snd/hitl.ogg");

    hitr_sfx = malloc(sizeof(Sound));
    *hitr_sfx = LoadSound("snd/hitr.ogg");

    score_sfx = malloc(sizeof(Sound));
    *score_sfx = LoadSound("snd/score.ogg");

    ball->callbacks.on_paddle_coll = on_paddle_coll;
    ball->callbacks.on_wall_coll = on_wall_coll;

    BallPrint(ball);


    
    return true;
}

void input_logic(float dt) {

    bool lup = IsKeyDown(lpad_up);
    bool ldown = IsKeyDown(lpad_down);

    if (lup) {
        left->pos.y -= pad_move_speed * dt;
    }
    if (ldown) {
        left->pos.y += pad_move_speed * dt;
    }

    bool rup = IsKeyDown(rpad_up);
    bool rdown = IsKeyDown(rpad_down);

    if (rup) {
        right->pos.y -= pad_move_speed * dt;
    }
    if (rdown) {
        right->pos.y += pad_move_speed * dt;
    }

    // ensure paddle constraints
    if (left->pos.y < 0) left->pos.y = 0;
    if (left->pos.y + left->size > wind_bounds.height) left->pos.y = wind_bounds.height - left->size;

    if (right->pos.y < 0) right->pos.y = 0;
    if (right->pos.y + right->size > wind_bounds.height) right->pos.y = wind_bounds.height - right->size;
}

void score(bool left) {
    if (left) scorel++;
    else scorer++;

    Vector2 center = { .x = wind_bounds.width / 2, .y = wind_bounds.height / 2 };
    ball->pos = center;
    ball_size = ball_size_default;

    PlaySound(*score_sfx);

    Ball_RandomVelocity(ball_vel_rand_range, ball_minmag, ball);
}
void coll_logic() {
    bool check = false;
    bool ball_left = ball->pos.x < wind_bounds.width / 2;
    if (ball_left && scaling_cooldown[0] <= 0) {
        // left paddle coll
        check = CheckCollisionCircleLine(ball->pos, ball->size, left->pos, (Vector2){ .x = left->pos.x, .y = left->pos.y + left->size });

        if (check)
            ball->callbacks.on_paddle_coll(ball, left);
    }
    else if (scaling_cooldown[1] <= 0) {
        // right paddle coll
        check = CheckCollisionCircleLine(ball->pos, ball->size, right->pos, (Vector2){ .x = right->pos.x, .y = right->pos.y + right->size });

        if (check)
            ball->callbacks.on_paddle_coll(ball, right);
    }

    // paddle coll
    if (ball->pos.y - ball->size < 0 || ball->pos.y + ball->size > wind_bounds.height) {
        ball->callbacks.on_wall_coll();
    }

    // scoring
    // right scores
    if (ball->pos.x - ball->size < 0) {
        score(false);
    }
    // left scores
    else if (ball->pos.x + ball->size > wind_bounds.width) {
        score(true);
    }
}
void logic(float dt) {
    input_logic(dt);
    coll_logic();

    if (IsWindowResized()) {
        Vector2 wndpos = GetWindowPosition();
        wind_bounds.x = wndpos.x;
        wind_bounds.y = wndpos.y;
        wind_bounds.width = GetScreenWidth();
        wind_bounds.height = GetScreenHeight();
        right->pos.x = wind_bounds.width - paddle_offset_x;
    }

    for (int i = 0; i < SCALING_COOLDOWN_COUNT; i++) {
        if (scaling_cooldown[i] > 0)
            scaling_cooldown[i] -= dt;
    }

    ball->size = ball_size;
    ball->pos = Vector2Add(ball->pos, Vector2Scale(ball->vel, dt));
}

void render(float dt) {
    BeginDrawing();

    ClearBackground(DARKGRAY);

    char* disp = TextFormat("%i : %i", scorel, scorer);
    int fntsz = 40;
    float measx = MeasureText(disp, fntsz);
    float centerx = (wind_bounds.width / 2) - (measx / 2);
    DrawText(disp, centerx, 20, fntsz, RAYWHITE);

    // draw ball
    // printf("{ %f, %f }, %f\n", ball->pos.x, ball->pos.y, ball->size);
    DrawCircle(ball->pos.x, ball->pos.y, ball->size, ball->color);

    // draw paddles
    // printf("%f, %f, %f, %f\n", wind_bounds.width, right->x_offset, right->y, right->size);
    DrawLine(left->pos.x, left->pos.y, left->pos.x, left->pos.y + left->size, pad_color);
    DrawLine(right->pos.x, right->pos.y, right->pos.x, right->pos.y + right->size, pad_color);

    EndDrawing();
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TRANSPARENT);
    InitWindow(100, 100, "Raylib Pong");
    InitAudioDevice();

    vp_current = GetCurrentMonitor();
    vp_width = GetMonitorWidth(vp_current);
    vp_height = GetMonitorHeight(vp_current);
    wind_bounds = (Rectangle){ 
        .x = 0,
        .y = 0,
        .width = 1000,
        .height = 800
    };
    wind_bounds.x = vp_width / 2 - wind_bounds.width / 2;
    wind_bounds.y = vp_height / 2 - wind_bounds.height / 2;
    
    SetWindowPosition(wind_bounds.x, wind_bounds.y);
    SetWindowSize(wind_bounds.width, wind_bounds.height);

    SetTargetFPS(240);

    init();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        logic(dt);
        render(dt);
    }

    CloseWindow(); 

    return 0;
}