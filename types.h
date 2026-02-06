#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	Vec2 pos;
	Vec2 vel;
	Vec2 acc;

	float mass;
	float radius;

	uint32_t color;

	bool is_fixed;
} Particle;

typedef struct {
	float G;
	float dt;
	int particle_count;

	float softening;

	float width;
	float height;
} SimConfig;

#endif 

