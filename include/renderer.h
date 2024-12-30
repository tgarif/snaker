#ifndef RENDERER_H_
#define RENDERER_H_

#include "game.h"
#include "raylib.h"

void RenderSnake(const Snake* snake, Texture2D headTextures[4], Texture2D bodyTextures[6], Texture2D tailTexture[4]);
void RenderFood(const Food* food, Texture2D foodTexture);
void RenderBackground();

#endif
