#include "types.h"
#include <stdlib.h>
#include <math.h>
#include "raymath.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

float rand_float(float min, float max){
    return min + (max-min)* ((float)rand() / RAND_MAX);
}

void init_galaxy(Particle* p, SimConfig* conf){
    Vector3 center = {0, 0, 0};

    p[0].x = center.x; p[0].y = center.y; p[0].z = center.z;
    p[0].pad1 = 0; 

    p[0].px_prev = p[0].x; p[0].py_prev = p[0].y; p[0].pz_prev = p[0].z;
    p[0].pad4 = 0;

    p[0].vx = 0; p[0].vy = 0; p[0].vz = 0; p[0].pad2 = 0;
    p[0].ax = 0; p[0].ay = 0; p[0].az = 0; p[0].pad3 = 0;

    p[0].mass = 50000.0f;
    p[0].radius = 40.0f;
    p[0].color = 0xFFA500FF;
    p[0].is_fixed = 1; 

    for (int i = 1; i < conf->particle_count; i++){
        
        float dist = rand_float(150.0f, conf->bounds_size * 0.8f);
        float angle = rand_float(0.0f, 2.0f * M_PI);
        float height = rand_float(-50.0f, 50.0f);

        p[i].x = center.x + cosf(angle) * dist;
        p[i].y = center.y + sinf(angle) * dist;
        p[i].z = center.z + height;
        p[i].pad1 = 0;

        p[i].px_prev = p[i].x;
        p[i].py_prev = p[i].y;
        p[i].pz_prev = p[i].z;
        p[i].pad4 = 0;

        float velocity = sqrtf((conf->G * p[0].mass) / dist);
        p[i].vx = -sinf(angle) * velocity;
        p[i].vy = cosf(angle) * velocity;
        p[i].vz = 0;
        p[i].pad2 = 0;

        p[i].ax = 0; p[i].ay = 0; p[i].az = 0;
        p[i].pad3 = 0;

        p[i].mass = rand_float(0.5f, 2.0f);
        p[i].radius = rand_float(2.0f, 6.0f);
        p[i].color = (i % 2 == 0) ? 0x8888FFFF: 0xFF8888FF;
        p[i].is_fixed = 0; 
    }
}
