#include "types.h"
#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include "raymath.h"

void render_frame(Particle* p, SimConfig* conf, Texture2D tex, Camera3D* cam, float alpha){ 
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(*cam);
    rlSetClipPlanes(0.1, 10000.0);
    
    Vector3 sunPos = Vector3Lerp(
        (Vector3){p[0].px_prev, p[0].py_prev, p[0].pz_prev},
        (Vector3){p[0].x, p[0].y, p[0].z},
        alpha
    );

    DrawSphere(sunPos, p[0].radius, ORANGE);

    int N = conf->particle_count;
    
    rlSetBlendMode(BLEND_ALPHA);

    for (int i = 1; i < N; i++) {
        Vector3 prev = {p[i].px_prev, p[i].py_prev, p[i].pz_prev};
        Vector3 curr = {p[i].x, p[i].y, p[i].z};
        Vector3 center = Vector3Lerp(prev, curr, alpha);

        Color c = GetColor(p[i].color);
        
        float size = p[i].radius * 3.0f;
        
        DrawBillboard(*cam, tex, center, size, c);
    }
    
    rlSetBlendMode(BLEND_ALPHA);
    
    EndMode3D();

    DrawFPS(GetScreenWidth() - 100,10);
    
    char buffer[64];
    sprintf(buffer, "Particles: %d", N);
    DrawText(buffer, 10, 10, 20, GREEN);
    
    DrawText("PRESS R TO RESET Galaxy",10, 40,  20, SKYBLUE);
    DrawText("PRESS Q OR ESC TO QUIT",10, 70, 20, GRAY);

    EndDrawing();
}

void close_renderer(){
    CloseWindow();
}
