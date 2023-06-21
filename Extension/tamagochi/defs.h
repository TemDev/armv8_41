#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <raylib.h>

#define BOUNDS_X 800
#define BOUNDS_Y 400
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define RADIUS 50


typedef struct __player{
 float health;
 Vector2 velocity;
 Vector2 position;
 Vector2 size;
 bool right;
 bool jump;
 Texture2D texture;
 } Player;
#endif
