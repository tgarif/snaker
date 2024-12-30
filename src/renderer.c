#include "renderer.h"

#include "constants.h"
#include "raylib.h"

void RenderSnake(const Snake *snake, Texture2D headTextures[4], Texture2D bodyTextures[6], Texture2D tailTextures[4]) {
    // Determine textures based on direction
    for (int i = 0; i < snake->length; ++i) {
        Texture2D texture;

        // Head rendering
        if (i == 0) {
            if (snake->segments[i].direction.x == 1)
                texture = headTextures[0];  // Right
            else if (snake->segments[i].direction.x == -1)
                texture = headTextures[1];  // Left
            else if (snake->segments[i].direction.y == -1)
                texture = headTextures[2];  // Up
            else
                texture = headTextures[3];  // Down
        }
        // Tail rendering
        else if (i == snake->length - 1) {
            Vector2 tailDirection = (Vector2){
                snake->segments[i].position.x - snake->segments[i - 1].position.x,
                snake->segments[i].position.y - snake->segments[i - 1].position.y};

            if (tailDirection.x > 0)
                texture = tailTextures[0];  // Right
            else if (tailDirection.x < 0)
                texture = tailTextures[1];  // Left
            else if (tailDirection.y > 0)
                texture = tailTextures[3];  // Up
            else
                texture = tailTextures[2];  // Down
        }
        // Body
        else {
            Vector2 prev = snake->segments[i - 1].position;
            Vector2 next = snake->segments[i + 1].position;

            // Vertical body
            if (prev.x == next.x) {
                texture = bodyTextures[0];  // Vertical
            }
            // Horizontal body
            else if (prev.y == next.y) {
                texture = bodyTextures[1];  // Horizontal
            }
            // Corners
            else if ((prev.x < snake->segments[i].position.x && next.y < snake->segments[i].position.y) ||
                     (next.x < snake->segments[i].position.x && prev.y < snake->segments[i].position.y)) {
                texture = bodyTextures[4];  // Top-left
            } else if ((prev.x > snake->segments[i].position.x && next.y < snake->segments[i].position.y) ||
                       (next.x > snake->segments[i].position.x && prev.y < snake->segments[i].position.y)) {
                texture = bodyTextures[5];  // Top-right
            } else if ((prev.x < snake->segments[i].position.x && next.y > snake->segments[i].position.y) ||
                       (next.x < snake->segments[i].position.x && prev.y > snake->segments[i].position.y)) {
                texture = bodyTextures[2];  // Bottom-left
            } else {
                texture = bodyTextures[3];  // Bottom-right
            }
        }
        DrawTexture(texture, snake->segments[i].position.x, snake->segments[i].position.y, WHITE);
    }
}

void RenderFood(const Food *food, Texture2D foodTexture) {
    DrawTexture(foodTexture, food->position.x, food->position.y, WHITE);
}

void RenderBackground(void) {
    ClearBackground(BLACK);
    for (int x = 0; x < SCREEN_WIDTH; x += SEGMENT_SIZE) {
        for (int y = 0; y < SCREEN_HEIGHT; y += SEGMENT_SIZE) {
            DrawRectangleLines(x, y, SEGMENT_SIZE, SEGMENT_SIZE, DARKGRAY);
        }
    }
}
