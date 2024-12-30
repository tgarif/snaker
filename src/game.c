#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "raymath.h"
#include "utils.h"

void InitializeGame(Snake *snake, Food *food) {
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->segments = malloc(snake->length * sizeof(SnakeSegment));
    if (!snake->segments) {
        perror("Failed to allocate memory for snake segments");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < snake->length; ++i) {
        snake->segments[i].position = (Vector2){
            SCREEN_WIDTH / 2 - i * SEGMENT_SIZE, SCREEN_HEIGHT / 2};
        snake->segments[i].direction = (Vector2){1, 0};  // Moving right
    }

    snake->speed = SNAKE_SPEED;
    snake->queueSize = 0;

    SpawnFood(food, snake);
}

void UpdateSnake(Snake *snake) {
    // Move each segment to the position of the segment in front of it
    for (int i = snake->length - 1; i > 0; --i) {
        snake->segments[i].position = snake->segments[i - 1].position;
    }

    // Move the head to the next grid cell
    snake->segments[0].position.x += snake->segments[0].direction.x * SEGMENT_SIZE;
    snake->segments[0].position.y += snake->segments[0].direction.y * SEGMENT_SIZE;
}

bool CheckCollision(const Snake *snake) {
    // Check self-collision
    for (int i = 1; i < snake->length; ++i) {
        if (snake->segments[0].position.x == snake->segments[i].position.x &&
            snake->segments[0].position.y == snake->segments[i].position.y) {
            return true;
        }
    }

    // Check wall collision
    return snake->segments[0].position.x < 0 || snake->segments[0].position.x >= SCREEN_WIDTH ||
           snake->segments[0].position.y < 0 || snake->segments[0].position.y >= SCREEN_HEIGHT;
}

bool CheckFoodCollision(const Snake *snake, const Food *food) {
    int headGridX = (int)(snake->segments[0].position.x / SEGMENT_SIZE);
    int headGridY = (int)(snake->segments[0].position.y / SEGMENT_SIZE);
    int foodGridX = (int)(food->position.x / SEGMENT_SIZE);
    int foodGridY = (int)(food->position.y / SEGMENT_SIZE);

    return headGridX == foodGridX && headGridY == foodGridY;
}

void SpawnFood(Food *food, const Snake *snake) {
    bool isValid;
    do {
        food->position.x = (int)(RandomFloat(0, SCREEN_WIDTH / SEGMENT_SIZE)) * SEGMENT_SIZE;
        food->position.y = (int)(RandomFloat(0, SCREEN_HEIGHT / SEGMENT_SIZE)) * SEGMENT_SIZE;

        isValid = true;
        for (int i = 0; i < snake->length; ++i) {
            if (snake->segments[i].position.x == food->position.x &&
                snake->segments[i].position.y == food->position.y) {
                isValid = false;
                break;
            }
        }
    } while (!isValid);
}

void AddNewSegment(Snake *snake) {
    snake->length++;
    snake->segments = realloc(snake->segments, snake->length * sizeof(SnakeSegment));
    if (!snake->segments) {
        perror("Failed to reallocate memory for snake segments");
        exit(EXIT_FAILURE);
    }

    // Add a new segment at the position of the last segment
    snake->segments[snake->length - 1].position = snake->segments[snake->length - 2].position;
    snake->segments[snake->length - 1].direction = snake->segments[snake->length - 2].direction;
}

void QueueDirection(Snake *snake, Vector2 inputDirection) {
    // Ignore neutral input
    if (inputDirection.x == 0 && inputDirection.y == 0) return;

    // Ensure the input is valid and not the reverse of the current direction
    if ((inputDirection.x != -snake->segments[0].direction.x || inputDirection.y != -snake->segments[0].direction.y)) {
        // Avoid adding the same direction as the last one in the queue
        if (snake->queueSize == 0 || !Vector2Equals(inputDirection, snake->directionQueue[snake->queueSize - 1])) {
            snake->directionQueue[snake->queueSize++] = inputDirection;
        }
    }
}

float GetSnakeSpeed(Difficulty difficulty) {
    switch (difficulty) {
        case DIFF_EASY:
            return SNAKE_SPEED;
        case DIFF_MEDIUM:
            return 15.0f;
        case DIFF_HARD:
            return 20.0f;
        default:
            return SNAKE_SPEED;
    }
}

// Update the game state based on input
GameState UpdateGameState(GameState currentState, Rectangle buttons[], Difficulty *difficulty, Vector2 mousePos, bool mousePressed) {
    if (currentState == STATE_START) {
        if (mousePressed) {
            if (CheckCollisionPointRec(mousePos, buttons[0])) {
                return STATE_PLAY;
            } else if (CheckCollisionPointRec(mousePos, buttons[1])) {
                return STATE_DIFFICULTY;
            } else if (CheckCollisionPointRec(mousePos, buttons[2])) {
                return STATE_QUIT;
            }
        }
    } else if (currentState == STATE_DIFFICULTY) {
        if (mousePressed) {
            if (CheckCollisionPointRec(mousePos, buttons[0])) {
                *difficulty = DIFF_EASY;
                return STATE_START;
            } else if (CheckCollisionPointRec(mousePos, buttons[1])) {
                *difficulty = DIFF_MEDIUM;
                return STATE_START;
            } else if (CheckCollisionPointRec(mousePos, buttons[2])) {
                *difficulty = DIFF_HARD;
                return STATE_START;
            } else if (CheckCollisionPointRec(mousePos, buttons[3])) {
                return STATE_START;
            }
        }
    }
    return currentState;
}
