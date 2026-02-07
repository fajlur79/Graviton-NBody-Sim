#define _POSIX_C_SOURCE 200112L

#include "raylib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

bool GuiButton(Rectangle rect, const char* text, bool isSelected) {
    bool clicked = false;
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rect);

    if (isHover || isSelected) {
        DrawRectangleRec(rect, SKYBLUE);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            clicked = true;
        }
    } else {
        DrawRectangleRec(rect, LIGHTGRAY);
    }
    if (isSelected){
	DrawRectangleLinesEx(rect, 4, DARKBLUE);
    } else{
	DrawRectangleLinesEx(rect, 2, DARKGRAY);
    }

    int textWidth = MeasureText(text, 20);
    DrawText(text, 
             rect.x + (rect.width - textWidth) / 2, 
             rect.y + (rect.height - 20) / 2, 
             20, 
             (isSelected || isHover) ? DARKBLUE : BLACK);

    return clicked;
}

int main() {
    InitWindow(500, 400, "Graviton Launcher");
    SetTargetFPS(60);

    Rectangle btnIntegrated = { 50, 120, 400, 50 };
    Rectangle btnDiscrete   = { 50, 190, 400, 50 };
    Rectangle btnQuit       = { 50, 280, 400, 50 };

    int selection = 0;
    int final_choice = -1;

    while (!WindowShouldClose()) {

	if (IsKeyPressed(KEY_DOWN)) selection++;
	if (IsKeyPressed(KEY_UP)) selection--;

	if (selection > 2) selection = 0;
	if (selection < 0) selection = 2;

	if (IsKeyPressed(KEY_ENTER)){
		final_choice = selection;
	}

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("GRAVITON ENGINE", 130, 30, 30, DARKBLUE);
        DrawText("Select Graphics Processor:", 130, 70, 20, DARKGRAY);


        if (GuiButton(btnIntegrated, "Integrated GPU (Power Saving)", selection ==0)) {
            final_choice = 0;
	    selection = 0;
        }

        if (GuiButton(btnDiscrete, "Discrete GPU (High Performance)", selection == 1)) {
           final_choice = 1; 
	   selection = 1;
        }

        if (GuiButton(btnQuit, "Quit", selection == 2)) {
            final_choice = 2;
        }
        
        DrawText("v1.0 | Raylib | Barnes-Hut", 10, 380, 10, LIGHTGRAY);

        EndDrawing();

        if (final_choice != -1) {
            CloseWindow();
 
	    if (final_choice == 2){
		return 0;
	    }
 
            if (final_choice == 1) {
                setenv("DRI_PRIME", "1", 1);
                setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
                setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
            } else {
                unsetenv("DRI_PRIME");
            }

            char *args[] = {"./graviton", NULL};
            execvp("./graviton", args);
            
            fprintf(stderr, "Failed to launch graviton!\n");
            return 1;
        }
    }

    CloseWindow();
    return 0;
}
