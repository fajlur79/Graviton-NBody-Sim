#include "types.h"
#include "math_utils.h"
#include <math.h>
#include "barnes_hut.h"
#include <stdlib.h>


void compute_forces(Particle* p, SimConfig* conf){
	int N = conf->particle_count;
	float G = conf->G;
	float soft = conf->softening;
	float theta = 0.5f;
	
	for (int i = 0; i < N; i++){
		p[i].acc = (Vec2){0.0f, 0.0f};
	}

	float root_size = (conf->width > conf->height) ? conf->width : conf->height;
	QuadNode* root = bh_create_node(0, 0, root_size);
	

	for (int i = 0; i < N; i++){
	    if (p[i].pos.x >= 0 && p[i].pos.x <= root_size &&
	        p[i].pos.y >= 0 && p[i].pos.y <= root_size){
		bh_insert(root, &p[i]);
	    }		
	}
	bh_compute_mass_distribution(root);
	
	for (int i = 0; i < N; i++){
	    bh_calculate_force(&p[i], root, G, theta, soft);
	}
	bh_free_tree(root);
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
			p[i].vel.y *= -0.8;
		}

		if (p[i].pos.y > conf->height){
			p[i].pos.y = conf->height;
			p[i].vel.y *= -0.8;
		}
	}
}


