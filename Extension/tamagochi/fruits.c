#include <stdlib.h>
#include <raylib.h>
#include "fruits.h"
static Texture2D rastex;
void initFruits(fruit* fs) {
    rastex = LoadTexture("images/Raspberry.png");
    for (int i = 0; i < NUMFRUITS; i++) {
        fs[i].position.x = (rand() % SCREEN_WIDTH - 50) + 10; // decrease the value by 50 to make sure the image does not go out of bounds
        fs[i].position.y = 0;
        fs[i].isVisible = false;
        fs[i].growTime = rand() % SPAWNFRUITS + SPAWNFRUITS;
    }
}
void initFruit(fruit* fs) {
        fs -> position.x = (rand() % SCREEN_WIDTH - 50) + 10; // decrease the value by 50 to make sure the image does not go out of bounds
        fs->position.y = 0;
        fs->isVisible = false;
        fs->growTime = rand() % SPAWNFRUITS + SPAWNFRUITS;
}

void updateFruits(fruit* fs) {
    for (int i = 0; i < NUMFRUITS; i++) {
        if (fs[i].isVisible) {
            if (fs[i].position.y < BOUNDS_Y - SIZEFRUIT) {
                fs[i].position.y +=1;
            } else {
                fs[i].position.y = BOUNDS_Y - SIZEFRUIT;
            }
        } else {
            fs[i].growTime--;
        }
        
        if (fs[i].growTime <= 0) { 
            fs[i].isVisible = true;
            fs[i].growTime = 0;
        }

    }
}


void DrawFruits(fruit* fs) {
    for (int i = 0; i < NUMFRUITS; i++) {
        if (fs[i].isVisible) {
        DrawTexture(rastex, fs[i].position.x,  fs[i].position.y, WHITE);
        }
    }
}
