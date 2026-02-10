#include "types.h"
#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>

#ifndef RL_QUADS
#define RL_QUADS 0x0007
#endif

void render_frame(Particle* p, SimConfig* conf, Texture2D tex){ 
    BeginDrawing();
    ClearBackground((Color){10, 10, 15, 255});

    DrawCircleGradient((int)p[0].pos.x, (int)p[0].pos.y, p[0].radius * 3.0f,
			(Color){255,100,0,40}, (Color){0,0,0,0});
    DrawCircleGradient((int)p[0].pos.x, (int)p[0].pos.y, p[0].radius, ORANGE, GOLD);

    int N = conf->particle_count;

    rlSetTexture(tex.id); 

    rlBegin(RL_QUADS);

    for (int i = 1; i < N; i++) {
        rlCheckRenderBatchLimit(4); 

        Color c = GetColor(p[i].color);
        rlColor4ub(c.r, c.g, c.b, c.a);

        float r = p[i].radius; 
        
        if (r < 2.0f) r = 4.0f; 

        float x = p[i].pos.x;
        float y = p[i].pos.y;

        rlTexCoord2f(0.0f, 0.0f); rlVertex2f(x - r, y - r); // Top-Left
        rlTexCoord2f(0.0f, 1.0f); rlVertex2f(x - r, y + r); // Bottom-Left
        rlTexCoord2f(1.0f, 1.0f); rlVertex2f(x + r, y + r); // Bottom-Right
        rlTexCoord2f(1.0f, 0.0f); rlVertex2f(x + r, y - r); // Top-Right
    }

    rlEnd();
    
    rlSetTexture(0); 

    DrawFPS(10,10);
    DrawText("Particles: 15000", 10, 40, 20, GRAY);
    
    EndDrawing();
}

void close_renderer(){
    CloseWindow();
}
