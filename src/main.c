#include <stdio.h>

#include "raylib.h"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    size_t factor = 80;

    InitWindow(factor * 16, factor * 9, "Meow Editor");

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Ayoooo!", 300, 300, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
