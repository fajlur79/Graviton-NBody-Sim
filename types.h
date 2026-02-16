#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "raylib.h"


typedef struct {
	float x, y, z ; float pad1;
	float vx, vy, vz; float pad2;
	float ax, ay, az ; float pad3;

	float px_prev, py_prev, pz_prev; float pad4;

	float mass;
	float radius;
	uint32_t color;
	uint32_t is_fixed;
} Particle;

typedef struct {
	float G;
	float dt;
	int particle_count;

	float softening;
	float bounds_size;
	float theta;
} SimConfig;

#endif 

