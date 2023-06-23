#include <stdlib.h>
#include "player.h"
#include "fruits.h"

#define NUM_MOVES 10

#define DURATION 20

#define COOLDOWNRAIN 90
#define COOLDOWNSNOW 20
#define COOLDOWNHOT 120
#define COOLDOWNCOLD 50

#define DAMAGERAIN 0.4
#define DAMAGESNOW 1
#define DAMAGEHOT 0.3
#define DAMAGECOLD 0.5

#define HALF_LIFE 350
#define SCALING_FACTOR 4
typedef enum{COLD, HOT, RAIN, SNOW} whet;


static move_type moves[10];
static int turns = 10;
static float cooldown = 10;
static whet prev = HOT;
static bool setcooldown = false;
static Texture2D angry;
static Texture2D angryFlaming;
static Texture2D frozen;
static Texture2D anxious;
static Texture2D regular;
static Texture2D walkingLeft [6];
static Texture2D walkingRight [6];

Texture2D getWalkingTexture(int index, bool mirror) {
    char path [42];
    sprintf(path, "images/maincharacter/walking/walking%d.png", index);

    Image temp = LoadImage(path);
    ImageResizeNN(&temp, temp.width * 4, temp.height * 4);
    if (mirror) {
        ImageFlipHorizontal(&temp);
    }
    Texture2D texture = LoadTextureFromImage(temp);
    return texture;
}

static whet getWhet(environment* env) {
    if (isHot(env)) {
        if (isRain(env)){
            return RAIN;
        } else {
            return HOT;
        }
    } else {
        if (isRain(env)){
            return SNOW;
        } else {
            return COLD;
        }
    }
}

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
    walkingRight[0] = getWalkingTexture(1, true);
    walkingRight[1] =getWalkingTexture(2, true);
    walkingRight[2] =getWalkingTexture(3, true);
    walkingRight[3] =getWalkingTexture(4, true);
    walkingRight[4] =getWalkingTexture(5, true);
    walkingRight[5] = getWalkingTexture(6, true);
    walkingLeft[0] = getWalkingTexture(1, false);
    walkingLeft[1] =  getWalkingTexture(2, false);
    walkingLeft[2] =  getWalkingTexture(3, false);
    walkingLeft[3] = getWalkingTexture(4, false);
    walkingLeft[4] = getWalkingTexture(5, false);
    walkingLeft[5] = getWalkingTexture(6, false);
    regular = tex;

    Image flamingAngerImage = LoadImage("images/flamingHellAnger.png");
    ImageResizeNN(&flamingAngerImage, flamingAngerImage.width * SCALING_FACTOR, flamingAngerImage.height * SCALING_FACTOR);
    angryFlaming = LoadTextureFromImage(flamingAngerImage);
    // Frozen state
    Image frozenImage = LoadImage("images/frozen.png");
    ImageResizeNN(&frozenImage, frozenImage.width * SCALING_FACTOR, frozenImage.height * SCALING_FACTOR);
    frozen = LoadTextureFromImage(frozenImage);
    // Angry state
    Image angryImage = LoadImage("images/angry.png");
    ImageResizeNN(&angryImage, angryImage.width * SCALING_FACTOR, angryImage.height * SCALING_FACTOR);
    angry = LoadTextureFromImage(angryImage);
    // Anxious state
    Image anxiousImage = LoadImage("images/anxious.png");
    ImageResizeNN(&anxiousImage, anxiousImage.width * SCALING_FACTOR, anxiousImage.height * SCALING_FACTOR);
    anxious = LoadTextureFromImage(anxiousImage);
}
static int max(int x, int y){
    if(x > y){
        return x;
    }else{
        return y;
    }
}
static int min(int x, int y){
    if(x < y){
        return x;
    }else{
        return y;
    }
}
void updateHealth(Player *p, environment *env) {
    int px = p -> position.x;
    int py = p -> position.y;
    // updates the thing based on the environment
    for (int i = 0; i < NUMFRUITS; i++) { 
        // detects a collision
        if (env -> fs[i].isVisible) {
            int fx = env -> fs[i].position.x;
            int fy = env -> fs[i].position.y;
            int intx = min(px + p ->size.x, fx + SIZEFRUIT) - max(fx,px);
            int inty = min(py + p ->size.y, fy + SIZEFRUIT) - max(fy,py);
            
            if (intx > 0 && inty > 0 ) {
                p -> health += HPFRUIT;
                initFruit(&(env -> fs[i]), env);
                break;
            }
        }
    }


    // checks which cooldown applies now
    
    
    bool rain = isHot(env);
    bool hot = isHot(env);
    if (cooldown < 0) {
        if (hot && !rain) {
            p ->health -= DAMAGEHOT;
        } else if (hot && rain) {
            p ->health -= DAMAGERAIN;
        } else if (rain) {
            p ->health -= DAMAGESNOW;
        } else {
            p ->health -= DAMAGECOLD;
        }
    } else {
    	cooldown--;
    }

    if (turns < NUM_MOVES) {  
    if (moves[turns] == WAIT) {
        
    } else {
        p -> health -= ((env -> data.lightOff)? 0.15: 0.3);
    }
    }
}

static void updateVelocity(Player * p, environment* env){
    // if the current move is set than continue executing that move
    if (p -> m.set) {
        // duration is less or equal than 0 than move is over
        // does the movement
        //printf("current move :%d", moves[turns]);
        if (moves[turns] == JUMP) {
            if (p -> velocity.y >= GRAVITY) {
                p -> m.set = false;
            }
        } else {
		// movement at night the pet is slower
		int mov = (env -> data.lightOff)? 1: 2;
            if (moves[turns] == RIGHT) {
                p -> velocity.x += mov;
                int val = (p -> m.duration);
                p -> texture = walkingRight[val % 6];
            } else if (moves[turns] == LEFT) {
                p -> velocity.x -= mov;
                int val = (p -> m.duration);
                p -> texture = walkingLeft[val % 6];
            } else {// this is when pet is standing
	         if (cooldown < 0) {
                    switch (getWhet(env)) {
                    case HOT:
                        p -> texture = angryFlaming;
                        break;
                    case RAIN:
                        p -> texture = angry;
                        break;
                    case SNOW:
                        p -> texture = frozen;
                        break;
                    case COLD:
                        p -> texture = anxious;
                        break;
                    default:
                        p -> texture = angry;
                        break;
                    }
                } else if (p -> health < HALF_LIFE){
                    p -> texture = anxious;

                } else {

                    p -> texture = regular;
                }

}

            if (p -> m.duration <= 0) {
                p -> m.set = false;
                       } else {
                p -> m.duration -=0.3;
                

            }          
        }
    } else if (turns < NUM_MOVES) {
        // set the new move if there are still left
        if (moves[turns] == RIGHT) {
            p -> m.duration = DURATION + (rand() % DURATION);
        } else if (moves[turns] == LEFT) {
            p -> m.duration = DURATION + (rand() % DURATION);
        } else if (moves[turns] == JUMP) {
            p -> velocity.y = - GRAVITY;
        } else if (moves[turns] == WAIT) {
            p -> m.duration =DURATION+ (rand() % DURATION);
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

void updatePosition(Player* p, environment* env) {
    // updates velocity accordingly
    updateVelocity(p,env);

    p -> position.x  += p -> velocity.x;
    p -> position.y  += p -> velocity.y;

    if (p->position.x > BOUNDS_X - p -> size.x) {
        p -> position.x = BOUNDS_X - p -> size.x;
        p -> m.duration = 0;
    } else if (p->position.x < 0) {
        p -> position.x = 0;
        p -> m.duration = 0;
    }
    if (p -> position.y > BOUNDS_Y - p -> size.y) {
        p -> position.y = BOUNDS_Y - p -> size.y;
    } else if (p->position.y < 0) {
        p -> position.y = 0;
    }
    
    p -> velocity.x = 0;
    if (p -> velocity.y < GRAVITY) {
        
        p -> velocity.y += 0.5;

    } else {
        p -> velocity.y = GRAVITY;
    }

    // updates the cooldown and sets a new one
    if (!setcooldown || prev != getWhet(env)) {
        if (isHot(env)) {
            if (isRain(env)){
                
                cooldown = COOLDOWNRAIN;

            } else {
                
                cooldown = COOLDOWNHOT;
	    }

        } else if (isRain(env)) {
            cooldown = COOLDOWNSNOW;
        } else {
            cooldown = COOLDOWNCOLD;
        }

        setcooldown = true;

    }

    prev = getWhet(env);
}
    
