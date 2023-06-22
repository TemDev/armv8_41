#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct {
  float tempC;
  int waterLevel;
  bool lightOff;
} data;

typedef struct {
    bool set;
    float duration; 
} move;

typedef struct __player{
 float health;
 Vector2 velocity;
 Vector2 position;
 Vector2 size;
 bool right;
 move m;
 Texture2D texture;
 } Player;

typedef struct{
    data data;
    int count;
    int actual_colour[4];
    int sun_x;
    int sun_y;
    int moon_x;
    int moon_y;
} environment;

// makes player
void makePlayer(Player*, int, int, int, Texture2D);

void updatePosition(Player*);

void updateHealth(Player *, environment *);
#endif
