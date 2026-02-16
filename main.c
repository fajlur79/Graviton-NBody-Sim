#include "types.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h> 

void render_frame(Particle* p, SimConfig* conf, Texture2D tex, Camera3D* cam, float alpha);
void init_galaxy(Particle* p, SimConfig* conf);
void compute_forces(Particle* p, SimConfig* conf);

int main() {
    InitWindow(1200, 800, "Graviton: Hybrid GPU Physics - CIRCULAR PARTICLES");
    SetTargetFPS(60); 

    Camera3D cam = { 0 };
    cam.position = (Vector3){ 0.0f, 400.0f, 400.0f };
    cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    cam.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    SimConfig conf = {
        .G = 1000.0f,
        .dt = 0.0333f,
        .particle_count = 15000,
        .softening = 10.0f,
        .bounds_size = 1500.0f,
        .theta = 0.8f
    };

    Particle* particles = (Particle*)malloc(conf.particle_count * sizeof(Particle));
    if (particles == NULL) return 1;

    init_galaxy(particles, &conf);

    int texSize = 256; 
    Image img = GenImageColor(texSize, texSize, BLANK);
    
    unsigned char *pixels = (unsigned char *)img.data;

    float centerX = texSize / 2.0f;
    float centerY = texSize / 2.0f;
    float maxRadius = (texSize / 2.0f) - 2.0f;  

    printf("Creating %dx%d circular particle texture...\n", texSize, texSize);

    for (int y = 0; y < texSize; y++) {
        for (int x = 0; x < texSize; x++) {
            float dx = (float)x - centerX;
            float dy = (float)y - centerY;
            float dist = sqrtf(dx * dx + dy * dy);
            
            int index = (y * texSize + x) * 4;

            float alpha = 0.0f;
            
            if (dist <= maxRadius) {
                float t = dist / maxRadius;
                
                alpha = 1.0f - t;
                alpha = alpha * alpha;  
                
                if (t > 0.7f) {
                    float edgeT = (t - 0.7f) / 0.3f;  
                    float smooth = edgeT * edgeT * (3.0f - 2.0f * edgeT);
                    alpha *= (1.0f - smooth);
                }
                
                if (alpha < 0.0f) alpha = 0.0f;
                if (alpha > 1.0f) alpha = 1.0f;
            }
            
            unsigned char alphaVal = (unsigned char)(alpha * 255.0f);
            
            pixels[index + 0] = 255;       // R
            pixels[index + 1] = 255;       // G
            pixels[index + 2] = 255;       // B
            pixels[index + 3] = alphaVal;  // A
        }
    }
    
    printf("Texture generation complete!\n");
    
    Texture2D tex = LoadTextureFromImage(img);
    
    SetTextureWrap(tex, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    
    printf("Texture loaded: ID=%d, Size=%dx%d\n", tex.id, tex.width, tex.height);

    int cameraMode = CAMERA_THIRD_PERSON;

    double t = 0.0;
    double accumulator = 0.0;
    double currentTime = GetTime();

while (!WindowShouldClose()) {
        
        if (IsKeyPressed(KEY_R)) {
            init_galaxy(particles, &conf); 
        }
        
        if (IsKeyPressed(KEY_Q)) {
            break;
        }
        
        if (IsKeyPressed(KEY_C)) {
            if (cameraMode == CAMERA_THIRD_PERSON) {
                cameraMode = CAMERA_FREE;
                DisableCursor();
            } else {
                cameraMode = CAMERA_THIRD_PERSON;
                EnableCursor();
            }
        }

        UpdateCamera(&cam, cameraMode);

        double newTime = GetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        if (frameTime > 0.25) frameTime = 0.25;

        accumulator += frameTime;

        while (accumulator >= conf.dt) {
            compute_forces(particles, &conf);
            t += conf.dt;
            accumulator -= conf.dt;
        }

        float alpha = (float)(accumulator / conf.dt);

        render_frame(particles, &conf, tex, &cam, alpha);
    }
        UnloadTexture(tex);
    free(particles);
    CloseWindow();

    return 0;
}
