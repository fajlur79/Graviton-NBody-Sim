#include "types.h"
#include "raylib.h"
#include <stdio.h>


void render_frame(Particle* p, SimConfig* conf){
	BeginDrawing();

	ClearBackground((Color){10, 10, 15, 255});

	int N = conf->particle_count;

	for (int i = 0; i < N; i++) {
		Color c = GetColor(p[i].color);
		
		DrawCircleV((Vector2){p[i].pos.x, p[i].pos.y},
				p[i].radius,
				c
			   );
	}

	DrawFPS(10,10);
	DrawText("Press R to Reset", 10, 40, 20, RAYWHITE);
	DrawText("Press Q or ESC to Quit", 10, 70, 20, BLUE );
	
	char count_text[32];
	sprintf(count_text, "Particles: %d", N);
	DrawText(count_text, 10, 100, 20, GRAY);

	EndDrawing();
}

void close_renderer(){
	CloseWindow();
}

