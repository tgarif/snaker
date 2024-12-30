#include <stdlib.h>

#include "constants.h"
#include "game.h"
#include "input.h"
#include "raylib.h"
#include "renderer.h"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);

    InitAudioDevice();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snaker Game");

    Image icon = LoadImage("resources/icons/snake.png");
    SetWindowIcon(icon);

    // Load head textures
    Texture2D headTextures[4] = {
        LoadTexture("resources/textures/head_right.png"),
        LoadTexture("resources/textures/head_left.png"),
        LoadTexture("resources/textures/head_up.png"),
        LoadTexture("resources/textures/head_down.png")};

    // Load body textures
    Texture2D bodyTextures[6] = {
        LoadTexture("resources/textures/body_vertical.png"),
        LoadTexture("resources/textures/body_horizontal.png"),
        LoadTexture("resources/textures/body_bottomleft.png"),
        LoadTexture("resources/textures/body_bottomright.png"),
        LoadTexture("resources/textures/body_topleft.png"),
        LoadTexture("resources/textures/body_topright.png")};

    // Load tail textures
    Texture2D tailTextures[4] = {
        LoadTexture("resources/textures/tail_right.png"),
        LoadTexture("resources/textures/tail_left.png"),
        LoadTexture("resources/textures/tail_up.png"),
        LoadTexture("resources/textures/tail_down.png")};

    // Load food texture
    Texture2D foodTexture = LoadTexture("resources/textures/apple.png");

    Sound appleBiteSound = LoadSound("resources/musics/apple_bite.ogg");

    // Load music tracks
    Music menuMusic = LoadMusicStream("resources/musics/menu.wav");
    Music gameMusic = LoadMusicStream("resources/musics/game.mp3");
    menuMusic.looping = true;
    gameMusic.looping = true;

    // Define buttons
    Rectangle playButton = {SCREEN_WIDTH / 2 - 100, 200, 200, 50};
    Rectangle difficultyButton = {SCREEN_WIDTH / 2 - 100, 270, 200, 50};
    Rectangle quitButton = {SCREEN_WIDTH / 2 - 100, 340, 200, 50};

    Rectangle easyButton = {SCREEN_WIDTH / 2 - 100, 200, 200, 50};
    Rectangle mediumButton = {SCREEN_WIDTH / 2 - 100, 270, 200, 50};
    Rectangle hardButton = {SCREEN_WIDTH / 2 - 100, 340, 200, 50};
    Rectangle backButton = {SCREEN_WIDTH / 2 - 100, 410, 200, 50};

    GameState gameState = STATE_START;
    Difficulty gameDifficulty = DIFF_EASY;

    RenderTexture2D renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTargetFPS(60);

    Snake snake;
    Food food;

    double lastUpdateTime;
    double accumulatedTime;
    double currentTime;

    PlayMusicStream(menuMusic);

    while (!WindowShouldClose() && gameState != STATE_QUIT) {
        Vector2 mousePos = GetMousePositionInGame();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        UpdateMusicStream(menuMusic);
        UpdateMusicStream(gameMusic);

        BeginTextureMode(renderTexture);
        ClearBackground(BLACK);

        switch (gameState) {
            case STATE_START:
                if (!IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(gameMusic);
                    PlayMusicStream(menuMusic);
                }

                DrawText("Snaker Game", SCREEN_WIDTH / 2 - (float)MeasureText("Snaker Game", 40) / 2, 100, 40, WHITE);

                RenderButton(playButton, "Play", mousePos, mousePressed, false);
                RenderButton(difficultyButton, "Difficulties", mousePos, mousePressed, false);
                RenderButton(quitButton, "Quit", mousePos, mousePressed, false);

                // Handle Button Clicks
                if (mousePressed) {
                    if (CheckCollisionPointRec(mousePos, playButton)) {
                        InitializeGame(&snake, &food);
                        snake.speed = GetSnakeSpeed(gameDifficulty);

                        gameState = STATE_PLAY;
                        lastUpdateTime = GetTime();
                        accumulatedTime = 0;

                        StopMusicStream(menuMusic);
                        PlayMusicStream(gameMusic);
                    } else if (CheckCollisionPointRec(mousePos, difficultyButton)) {
                        gameState = STATE_DIFFICULTY;
                    } else if (CheckCollisionPointRec(mousePos, quitButton)) {
                        gameState = STATE_QUIT;
                    }
                }
                break;

            case STATE_DIFFICULTY:
                // Render Difficulty Screen
                DrawText("Select Difficulty", SCREEN_WIDTH / 2 - (float)MeasureText("Select Difficulty", 30) / 2, 100, 30, WHITE);

                RenderButton(easyButton, "Easy", mousePos, mousePressed, gameDifficulty == DIFF_EASY);
                RenderButton(mediumButton, "Medium", mousePos, mousePressed, gameDifficulty == DIFF_MEDIUM);
                RenderButton(hardButton, "Hard", mousePos, mousePressed, gameDifficulty == DIFF_HARD);
                RenderButton(backButton, "Back", mousePos, mousePressed, false);

                // Handle Button Clicks
                if (mousePressed) {
                    if (CheckCollisionPointRec(mousePos, easyButton)) {
                        gameDifficulty = DIFF_EASY;
                        gameState = STATE_START;
                    } else if (CheckCollisionPointRec(mousePos, mediumButton)) {
                        gameDifficulty = DIFF_MEDIUM;
                        gameState = STATE_START;
                    } else if (CheckCollisionPointRec(mousePos, hardButton)) {
                        gameDifficulty = DIFF_HARD;
                        gameState = STATE_START;
                    } else if (CheckCollisionPointRec(mousePos, backButton)) {
                        gameState = STATE_START;
                    }
                }
                break;

            case STATE_PLAY:
                if (!IsMusicStreamPlaying(gameMusic)) {
                    StopMusicStream(menuMusic);
                    PlayMusicStream(gameMusic);
                }

                // Handle game logic
                currentTime = GetTime();
                accumulatedTime += currentTime - lastUpdateTime;
                lastUpdateTime = currentTime;

                Vector2 inputDirection = GetGridInputDirection(snake.segments[0].direction);
                QueueDirection(&snake, inputDirection);

                if (accumulatedTime >= 1.0 / snake.speed) {
                    accumulatedTime -= 1.0 / snake.speed;

                    // Apply the next direction from the queue
                    if (snake.queueSize > 0) {
                        snake.segments[0].direction = snake.directionQueue[0];
                        for (int i = 1; i < snake.queueSize; ++i) {
                            snake.directionQueue[i - 1] = snake.directionQueue[i];
                        }
                        snake.queueSize--;
                    }

                    UpdateSnake(&snake);

                    if (CheckFoodCollision(&snake, &food)) {
                        AddNewSegment(&snake);
                        SpawnFood(&food, &snake);

                        PlaySound(appleBiteSound);
                    }

                    if (CheckCollision(&snake)) {
                        gameState = STATE_START;  // Return to start screen
                    }
                }

                RenderBackground();
                RenderSnake(&snake, headTextures, bodyTextures, tailTextures);
                RenderFood(&food, foodTexture);
                break;
            default:
                break;
        }

        EndTextureMode();

        // Draw the scaled texture
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            renderTexture.texture,
            (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT},  // Flip Y axis
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0},
            0.0f,
            WHITE);
        EndDrawing();
    }

    StopMusicStream(menuMusic);
    StopMusicStream(gameMusic);

    UnloadMusicStream(menuMusic);
    UnloadMusicStream(gameMusic);
    UnloadSound(appleBiteSound);

    // Unload textures
    for (int i = 0; i < 4; i++) UnloadTexture(headTextures[i]);
    for (int i = 0; i < 6; i++) UnloadTexture(bodyTextures[i]);
    for (int i = 0; i < 4; i++) UnloadTexture(tailTextures[i]);
    UnloadTexture(foodTexture);
    UnloadImage(icon);

    free(snake.segments);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
