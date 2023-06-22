
#include "player.h"

typedef struct {
    bool set;

};

// static char moves[11];
// static int turns = 0;

void makePlayer(Player* p, int health, int sizex, int sizey, Texture2D tex) {
    p -> health = health;
    p -> velocity.x = 0;
    p -> velocity.y = 0;
    p->position.x = SCREEN_WIDTH/2;
    p -> position.y = SCREEN_HEIGHT/2;
    p->right = true;
    p->jump = false;
    p-> size.x = sizex;
    p -> size.y = sizey;
    p -> texture = tex;
}

static void updateVelocity(Player * p){
   
   }

static void randomise() {
    // if (turns < 10 && true ) {
    //     if (moves[turns] == 'R') {

    //     } else {

    //     }

    // }
    
}

void updatePosition(Player* p) {
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
    p -> velocity.y = 0;
}
    
