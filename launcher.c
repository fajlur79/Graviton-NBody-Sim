#define _POSIX_C_SOURCE 200112L
#include "raylib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

bool GuiButton(Rectangle rect, const char* text, bool isSelected) {
    bool clicked = false;
    Vector2 mouse = GetMousePosition();
    bool isHover = CheckCollisionPointRec(mouse, rect);

    if (isHover || isSelected) DrawRectangleRec(rect, SKYBLUE);
    else DrawRectangleRec(rect, LIGHTGRAY);

    if (isSelected) DrawRectangleLinesEx(rect, 4, DARKBLUE);
    else DrawRectangleLinesEx(rect, 2, DARKGRAY);

    if (isHover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) clicked = true;

    int textWidth = MeasureText(text, 20);
    DrawText(text, rect.x + (rect.width - textWidth)/2, rect.y + (rect.height - 20)/2, 20, BLACK);
    return clicked;
}

int main() {
    InitWindow(500, 450, "Graviton Launcher (Universal)");
    SetTargetFPS(60);

    Rectangle btnInt   = { 50, 100, 400, 50 };
    Rectangle btnAmd   = { 50, 170, 400, 50 };
    Rectangle btnNv    = { 50, 240, 400, 50 };
    Rectangle btnQuit  = { 50, 330, 400, 50 };

    int selection = 0;
    int final_choice = -1;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) selection++;
        if (IsKeyPressed(KEY_UP)) selection--;
        if (selection > 3) selection = 0;
        if (selection < 0) selection = 3;
        if (IsKeyPressed(KEY_ENTER)) final_choice = selection;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("SELECT GPU MODE", 150, 40, 24, DARKBLUE);

        if (GuiButton(btnInt, "Integrated Graphics (Default)", selection == 0)) { selection = 0; final_choice = 0; }
        if (GuiButton(btnAmd, "Discrete GPU (AMD / Generic)", selection == 1)) { selection = 1; final_choice = 1; }
        if (GuiButton(btnNv,  "Discrete GPU (NVIDIA Proprietary)", selection == 2)) { selection = 2; final_choice = 2; }
        if (GuiButton(btnQuit,"Quit", selection == 3)) { final_choice = 3; }

        EndDrawing();

        if (final_choice != -1) {
            CloseWindow();
            if (final_choice == 3) return 0;

            unsetenv("DRI_PRIME");
            unsetenv("__NV_PRIME_RENDER_OFFLOAD");
            unsetenv("__GLX_VENDOR_LIBRARY_NAME");

            if (final_choice == 1 || 0 ) {
                setenv("DRI_PRIME", "1", 1);
            }
            else if (final_choice == 2) {
                setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
                setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);
            }

            char *args[] = {"./graviton", NULL};
            execvp("./graviton", args);
            return 1;
        }
    }
    CloseWindow();
    return 0;
}
