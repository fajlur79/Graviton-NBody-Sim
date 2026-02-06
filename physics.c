#include "types.h"
#include "math_utils.h"
#include <math.h>

void compute_forces(Particle* p, SimConfig* conf){
	int N = conf->particle_count;
	float G = conf->G;
	float soft = conf->softening;
	float softSq = soft * soft;
	
	for (int i = 0; i < N; i++){
		p[i].acc = (Vec2){0.0f, 0.0f};
	}

	for (int i = 0; i < N; i++){
		for (int j = i+1; j < N; j++){	
			Vec2 r_vec = vec2_sub(p[j].pos, p[i].pos);
			
			float distSq = vec2_lenSq(r_vec);
		        
			float effective_distSq = distSq + softSq;
			float dist = sqrtf(effective_distSq);

			float f = (G * p[i].mass * p[j].mass) / effective_distSq;

			float f_scale = f / dist;
			
			Vec2 force = vec2_scale(r_vec, f_scale);

			if (!p[i].is_fixed){
				p[i].acc.x += force.x / p[i].mass;
				p[j].acc.y += force.y / p[j].mass;
			}
			if (!p[j].is_fixed){
				p[j].acc.x -= force.x / p[j].mass;
                		p[j].acc.y -= force.y / p[j].mass;
			}
		}
	}
}

void integrate(Particle* p, SimConfig* conf){
	int N = conf->particle_count;
	float dt = conf->dt;

	for (int i = 0; i < N; i++){
		if (p[i].is_fixed) continue;

		p[i].vel.x += p[i].acc.x * dt;
		p[i].vel.y += p[i].acc.y * dt;

		p[i].pos.x += p[i].vel.x * dt;
		p[i].pos.y += p[i].vel.y * dt;

		if (p[i].pos.x < 0){
			p[i].pos.x = 0;
			p[i].vel.x *= -0.8;
		}

		if (p[i].pos.x > conf->width){
			p[i].pos.x = conf->width;
			p[i].vel.x *= -0.8;
		}

		if (p[i].pos.y < 0){
			p[i].pos.y = 0;
			p[i].vel.y = -0.8;
		}

		if (p[i].pos.y > conf->height){
			p[i].pos.y = conf->height;
			p[i].vel.y = -0.8;
		}
	}
}


