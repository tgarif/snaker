#include "input.h"

#include "constants.h"
#include "raylib.h"

Vector2 GetGridInputDirection(Vector2 currentDirection) {
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && currentDirection.y == 0) return (Vector2){0, -1};
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && currentDirection.y == 0) return (Vector2){0, 1};
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && currentDirection.x == 0) return (Vector2){-1, 0};
    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && currentDirection.x == 0) return (Vector2){1, 0};
    return (Vector2){0, 0};
}

Vector2 GetMousePositionInGame(void) {
    Vector2 mouse = GetMousePosition();

    // Calculate scaling factors
    float scaleX = (float)SCREEN_WIDTH / GetScreenWidth();
    float scaleY = (float)SCREEN_HEIGHT / GetScreenHeight();

    // Convert to game coordinates
    mouse.x *= scaleX;
    mouse.y *= scaleY;

    return mouse;
}
