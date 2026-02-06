#include "types.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>


void init_renderer(SimConfig* conf);
void render_frame(Particle* p, SimConfig* conf);
void close_renderer();
void init_galaxy(Particle* p, SimConfig* conf);
void compute_forces(Particle* p, SimConfig* conf);
void integrate(Particle* p, SimConfig* conf);

int main (){
	SimConfig conf = {
		.G = 1000.0f,
		.dt = 0.16f,
		.particle_count = 1500,
		.softening = 10.0f,
		.width = 1200.0f,
		.height = 800.0f
	};

	size_t size = conf.particle_count * sizeof(Particle);
	Particle* particles = (Particle*)malloc(size);

	if (particles == NULL){
		fprintf(stderr, "CRITICAL: Failed to allocate memory.\n");
		return 1;
	}

	init_galaxy(particles, &conf);
	init_renderer(&conf);

	while (!WindowShouldClose()){
		if (IsKeyPressed(KEY_R)){
			init_galaxy(particles, &conf);
		}

		int substeps = 4;
		float sub_dt = conf.dt / substeps;
		
		SimConfig sub_conf = conf;
		sub_conf.dt = sub_dt;

		for (int s = 0; s < substeps; s++){
			compute_forces(particles, &sub_conf);
			integrate(particles, &sub_conf);
		}

		render_frame(particles, &conf);
	}

	close_renderer();
	free(particles);

	return 0;
}
