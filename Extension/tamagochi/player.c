#include <stdlib.h>
#include "main.h"
#include "player.h"

#define NUM_MOVES 10

#define DURATION 20



static move_type moves[10];
static int turns = 10;



void makePlayer(Player* p, int health, int sizex, int sizey, Texture2D tex) {
    // initialises the player with the correct versions
    p -> health = health;
    p -> velocity.x = 0;
    p -> velocity.y = 0;
    p->position.x = SCREEN_WIDTH/2;
    p -> position.y = SCREEN_HEIGHT/2;
    p->right = true;
    p-> size.x = sizex;
    p -> size.y = sizey;
    p -> texture = tex;
    p -> m.set = false;
    p -> m.duration = 0;
}

void updateHealth(Player *p, environment * env) {
    // change this later
    if (turns < NUM_MOVES) {  
    if (moves[turns] == WAIT) {
        
    } else {
        p -> health -= 0.3; 
    }
    }
}

static void updateVelocity(Player * p){
    // if the current move is set than continue executing that move
    if (p -> m.set) {
        // duration is less or equal than 0 than move is over
        // does the movement
        
        if (moves[turns] == JUMP) {
            if (p -> velocity.y >= GRAVITY) {
                p -> m.set = false;
            }
        } else {
            if (moves[turns] == RIGHT) {
                p -> velocity.x += 2;
            } else if (moves[turns] == LEFT) {
                p -> velocity.x -= 2;
            } 

            if (p -> m.duration <= 0) {
                p -> m.set = false;
            } else {
                p->m.duration -= 0.5;      
            }          
        }
    } else if (turns < NUM_MOVES) {
        // set the new move if there are still left
        if (moves[turns] == RIGHT) {
            p -> m.duration = 30 + (rand() % DURATION);
        } else if (moves[turns] == LEFT) {
            p -> m.duration = 30 + (rand() % DURATION);
        } else if (moves[turns] == JUMP) {
            p -> velocity.y = - GRAVITY;
        } else if (moves[turns] == WAIT) {
            p -> m.duration = 30 + (rand() % DURATION);
        }
        p -> m.set = true;
        turns++;
    } else {
        // if all moves in the queue were executed 
        // regenerate the queue
        for (char i = 0; i < NUM_MOVES; i++) {
            int temp = rand() % 4;
            switch (temp) {
                case 0:
                    moves[i] = RIGHT;
                break;
                case 1:
                    moves[i] = LEFT;
                break;
                case 2:
                    moves[i] = JUMP;
                break;
                default: 
                    moves[i] = WAIT;
                break;
            }
        }
        turns = 0;
    }
}

void updatePosition(Player* p) {
    // updates velocity accordingly
    updateVelocity(p);

    p -> position.x  += p -> velocity.x;
    p -> position.y  += p -> velocity.y;
    if (p->position.x > BOUNDS_X - p->size.x) {
        p -> position.x = BOUNDS_X - p->size.x;
    }
    if (p->position.x < 0) {
        p -> position.x = 0;
    }
    if (p->position.y > BOUNDS_Y - p -> size.y) {
        p -> position.y = BOUNDS_Y - p -> size.y;
    }
    if (p->position.y < 0) {
        p -> position.y = 0;
    }
    
    p -> velocity.x = 0;
    if (p -> velocity.y < GRAVITY) {
        
        p -> velocity.y += 1;

    } else {
        p -> velocity.y = GRAVITY;
    }
}
    
