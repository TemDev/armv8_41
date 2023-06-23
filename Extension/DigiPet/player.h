// Header file for player.c, contains most of the structs used in the project

#ifndef PLAYER_H
#define PLAYER_H

#include <assert.h>
#include <raylib.h>
#include <stdio.h>

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

// Type of movement
typedef enum { LEFT, RIGHT, JUMP, WAIT } move_type;

// How to move
typedef struct {
    bool set;
    float duration;
} move;

// Struct representing fruits
typedef struct {
    bool isVisible;
    int growTime;
    Vector2 position;
} fruit;

// Struct containing the sesnors data
typedef struct {
    float tempC;
    int waterLevel;
    bool lightOff;
} data;

// Struct containing values for the environment
typedef struct {
    data data;
    int count;
    int actual_colour[4];
    fruit fs[NUMFRUITS];
    Texture2D background;
    int sun_x;
    int sun_y;
    int moon_x;
    int moon_y;
    bool musicOn;
} environment;

// Struct of the player
typedef struct __player {
    float health;
    Vector2 velocity;
    Vector2 position;
    Vector2 size;
    bool right;
    move m;
    Texture2D texture;
} Player;

// Makes player
void makePlayer(Player *, int, int, int, Texture2D);

// Updates the position of the player in the environment
void updatePosition(Player *, environment *);

// Updates the health of the player in the environment
void updateHealth(Player *, environment *);
#endif
