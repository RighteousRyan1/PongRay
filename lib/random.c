#include "random.h"

unsigned int seed = 11706;
unsigned int *seed_ptr = &seed;

// my own rand function cuz why not xd
unsigned int rng() {
    *seed_ptr *= 0x713 << 5;
    *seed_ptr *= (0x91903 + 1828939) << 3;

    return *seed_ptr;
}

// random integer, seeded (no constraints), 0x10000000 -> 0x0FFFFFFF
int randis() {
    return rng() * 90192 << 0x10;
}

int randi_max(int max) {
    return randis() % max;
}
int randi_range(int min, int max) {
    return randis() % (max - min) + min;
}

// floating-point rand, 0.0 -> 1.0
float randf() {
    *seed_ptr = *seed_ptr * 214013 + 2531011;
    return (float)(*seed_ptr >> 0x10) / (1 << 16);
}
float randf_max(float max) {
    return randf() * max;
}
float randf_range(float min, float max) {
    return min + (randf() * (max - min));
}