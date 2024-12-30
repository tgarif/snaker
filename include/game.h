#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 direction;
} SnakeSegment;

typedef struct {
    SnakeSegment* segments;
    int length;
    float speed;
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

#endif
