#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "game.h"
#include "input.h"
#include "raylib.h"
#include "renderer.h"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snaker Game");

    // Load head textures
    Texture2D headTextures[4] = {
        LoadTexture("resources/head_right.png"),
        LoadTexture("resources/head_left.png"),
        LoadTexture("resources/head_up.png"),
        LoadTexture("resources/head_down.png")};

    // Load body textures
    Texture2D bodyTextures[6] = {
        LoadTexture("resources/body_vertical.png"),
        LoadTexture("resources/body_horizontal.png"),
        LoadTexture("resources/body_bottomleft.png"),
        LoadTexture("resources/body_bottomright.png"),
        LoadTexture("resources/body_topleft.png"),
        LoadTexture("resources/body_topright.png")};

    // Load tail textures
    Texture2D tailTextures[4] = {
        LoadTexture("resources/tail_right.png"),
        LoadTexture("resources/tail_left.png"),
        LoadTexture("resources/tail_up.png"),
        LoadTexture("resources/tail_down.png")};

    // Load food texture
    Texture2D foodTexture = LoadTexture("resources/apple.png");

    SetTargetFPS(60);

    Snake snake;
    Food food;
    InitializeGame(&snake, &food);

    double lastUpdateTime = GetTime();
    double accumulatedTime = 0;

    while (!WindowShouldClose()) {
        double currentTime = GetTime();
        accumulatedTime += currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;

        // Queue input direction
        Vector2 inputDirection = GetGridInputDirection(snake.segments[0].direction);
        QueueDirection(&snake, inputDirection);

        if (accumulatedTime >= 1.0 / snake.speed) {
            accumulatedTime -= 1.0 / snake.speed;

            // Apply the next direction from the queue, if available
            if (snake.queueSize > 0) {
                snake.segments[0].direction = snake.directionQueue[0];

                // Shift the queue
                for (int i = 1; i < snake.queueSize; ++i) {
                    snake.directionQueue[i - 1] = snake.directionQueue[i];
                }
                snake.queueSize--;  // Reduce the queue size
            }

            // Update the snake
            UpdateSnake(&snake);

            // Check for food collision
            if (CheckFoodCollision(&snake, &food)) {
                AddNewSegment(&snake);
                SpawnFood(&food, &snake);
            }

            // Check for collisions
            if (CheckCollision(&snake)) {
                break;  // Game over
            }
        }

        // Render the game
        BeginDrawing();
        RenderBackground();
        RenderSnake(&snake, headTextures, bodyTextures, tailTextures);
        RenderFood(&food, foodTexture);
        EndDrawing();
    }

    // Unload textures
    for (int i = 0; i < 4; i++) UnloadTexture(headTextures[i]);
    for (int i = 0; i < 6; i++) UnloadTexture(bodyTextures[i]);
    for (int i = 0; i < 4; i++) UnloadTexture(tailTextures[i]);
    UnloadTexture(foodTexture);

    free(snake.segments);
    CloseWindow();

    return 0;
}
