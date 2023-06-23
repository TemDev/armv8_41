#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <raylib.h>
#include <assert.h>

#define BOUNDS_X 800
#define BOUNDS_Y 330
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define RADIUS 50
#define GRAVITY 10

#define PLAYER_HEIGHT 98
#define PLAYER_WIDTH 80


#define NUMFRUITS 10
#define SPAWNFRUITS 70
#define SIZEFRUIT 40
#define HPFRUIT 10

typedef enum {LEFT, RIGHT, JUMP, WAIT} move_type;

typedef struct {
    bool set;
    float duration; 
} move;

typedef struct {
    bool isVisible;
    int growTime;
    Vector2 position;
} fruit;

typedef struct {
  float tempC;
  int waterLevel;
  bool lightOff;
} data;

typedef struct{
    data data;
    int count;
    int actual_colour[4];
    fruit fs[NUMFRUITS];
    Texture2D background;
    int sun_x;
    int sun_y;
    int moon_x;
    int moon_y;
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



// makes player
void makePlayer(Player*, int, int, int, Texture2D);

void updatePosition(Player*, environment*);

void updateHealth(Player *, environment *);
#endif
