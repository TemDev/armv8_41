#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <raylib.h>
#include <assert.h>

// constants and data types used throughout the project

#define BOUNDS_X 800
#define BOUNDS_Y 330
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define RADIUS 50
#define GRAVITY 10

typedef enum {LEFT, RIGHT, JUMP, WAIT} move_type;

typedef struct {
    bool set;
    float duration; 
} move;

typedef struct {
  float tempC;
  int waterLevel;
  bool lightOff;
} data;

typedef struct{
    data data;
    int count;
    int actual_colour[4];
} environment;


typedef struct __player{
 float health;
 Vector2 velocity;
 Vector2 position;
 Vector2 size;
 bool right;
 move m;
 Texture2D texture;
 } Player;
#endif
