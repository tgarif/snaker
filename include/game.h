#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>

#include "raylib.h"

typedef enum {
    STATE_START,
    STATE_PLAY,
    STATE_DIFFICULTY,
    STATE_QUIT,
} GameState;

typedef enum {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD,
} Difficulty;

typedef struct {
    Vector2 position;
    Vector2 direction;
} SnakeSegment;

typedef struct {
    SnakeSegment* segments;
    int length;
    float speed;
    Vector2 directionQueue[2];
    int queueSize;
} Snake;

typedef struct {
    Vector2 position;
} Food;

void InitializeGame(Snake* snake, Food* food);
void UpdateSnake(Snake* snake);
bool CheckCollision(const Snake* snake);
bool CheckFoodCollision(const Snake* snake, const Food* food);
void SpawnFood(Food* food, const Snake* snake);
void AddNewSegment(Snake* snake);
void QueueDirection(Snake* snake, Vector2 inputDirection);
float GetSnakeSpeed(Difficulty difficulty);
GameState UpdateGameState(GameState currentState, Rectangle buttons[], Difficulty* difficulty, Vector2 mousePos, bool mousePressed);

#endif
