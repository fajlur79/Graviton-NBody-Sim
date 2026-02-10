#include "types.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

float rand_float(float min, float max){
	return min + (max-min)* ((float)rand() / RAND_MAX);
}

void init_random(Particle* p, SimConfig* conf){
	for (int i = 0; i < conf -> particle_count; i++){
		p[i].pos.x = rand_float(0, conf->width);
		p[i].pos.y = rand_float(0, conf->height);
		p[i].vel.x = rand_float(-10, 10);
		p[i].vel.y = rand_float(-10, 10);
		p[i].acc = (Vec2){0,0};
		p[i].mass = rand_float(1.0f, 5.0f);
		p[i].color = 0xFFFFFFFF;
		p[i].is_fixed = false;
	}
}

void init_galaxy(Particle* p, SimConfig* conf){
	float cx = conf->width / 2.0f;
	float cy = conf->height / 2.0f;

	p[0].pos.x = cx;
	p[0].pos.y = cy;
	p[0].vel = (Vec2){0,0};
	p[0].mass = 50000.0f;
	p[0].radius = 50.0f;
	p[0].color = 0xFFA500FF;
	p[0].is_fixed = true;

	for (int i = 1; i < conf->particle_count; i++){
		float min_dist = 100.0f;
		float max_dist = (conf->height / 2.0f) -10.0f;

		if (max_dist <= min_dist) max_dist = min_dist + 50.0f;
		float dist = rand_float(min_dist, max_dist);

		float angle = rand_float(0.0f, 2.0f * M_PI);

		p[i].pos.x = cx + cosf(angle) * dist;
		p[i].pos.y = cy + sinf(angle) * dist;

		float velocity = sqrtf((conf->G * p[0].mass) / dist);

		p[i].vel.x = -sinf(angle) * velocity;
		p[i].vel.y = cosf(angle) * velocity;

		p[i].acc = (Vec2){0,0};
		p[i].mass = rand_float(0.5f, 2.0f);
		//p[i].radius = (p[i].mass < 1.0f) ? 1.0f : 2.0f;
		p[i].radius = rand_float(2.0f, 6.0f);

		if (p[i].mass > 1.5f) p[i].color = 0x8888FFFF;
		else p[i].color = 0xFF8888FF;

		p[i].is_fixed = false;
	}
}



