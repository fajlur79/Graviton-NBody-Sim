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

int main() {
    InitWindow(800, 600, "Graviton: N-Body Simulation");
    
    int monitor = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);
    
    SetWindowSize(screenWidth, screenHeight);
    ToggleFullscreen(); 

    SimConfig conf = {
        .G = 1000.0f,
        .dt = 0.01f,
        .particle_count = 1000,
        .softening = 10.0f,
        .width = (float)screenWidth,
        .height = (float)screenHeight
    };

    SetTargetFPS(60);

    size_t size = conf.particle_count * sizeof(Particle);
    Particle* particles = (Particle*)malloc(size);

    if (particles == NULL) {
        fprintf(stderr, "CRITICAL: Failed to allocate memory.\n");
        return 1;
    }

    init_galaxy(particles, &conf);

    while (!WindowShouldClose()) {
        
        if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        if (IsKeyPressed(KEY_R)) {
            init_galaxy(particles, &conf);
        }

        int substeps = 4;
        float sub_dt = conf.dt / substeps;
        
        SimConfig sub_conf = conf;
        sub_conf.dt = sub_dt;

        for (int s = 0; s < substeps; s++) {
            compute_forces(particles, &sub_conf);
            integrate(particles, &sub_conf);
        }

        render_frame(particles, &conf);
    }

    CloseWindow();
    free(particles);

    return 0;
}
