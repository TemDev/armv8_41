#include <raylib.h>
#include "player.h"
#include "sensorsData.h"

#define COLOUR_STEPS 150





int colour1[] = {102, 191, 255, 255};
int colour2[] = {80, 80, 80, 255};



Texture2D getTexture(char *path) {
    // path is for the image
    Image temp = LoadImage(path);
    Texture2D texture = LoadTextureFromImage(temp);
    //UnloadImage(temp);
    return texture;
}

void DrawAttributes(Player* p) {
    DrawRectangle(0,0, p -> health, 10, GREEN);
}

void DrawBackGround(Player* p, int *actual_colour) {
    ClearBackground((Color) {actual_colour[0], actual_colour[1], actual_colour[2], actual_colour[3]});
    
    DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

}

void DrawPlayer(Player* p){
    DrawTexture(p -> texture, p ->position.x, p -> position.y, WHITE);
}

void DrawEverything(Player* p, environment *env) {
    DrawBackGround(p, env -> actual_colour);
    DrawAttributes(p);
    DrawPlayer(p);
}

void updateKeys(Player* p, bool* moved){
    
}

void updateEnvironment(Player* p, environment* env) {
    // updates the data
    FILE *sensorFile = fopen("sensorReadings.txt", "r");
    fetchData(&(env -> data), sensorFile);
    fclose(sensorFile);


    if (env ->data.lightOff && (env -> count < COLOUR_STEPS) ){
        env -> count++;
    } else if (env -> count > 0) {
	    env -> count--;
    }

    env -> actual_colour[0] =  colour1[0] + (colour2[0] - colour1[0]) * (env -> count)/ COLOUR_STEPS;
	env -> actual_colour[1] =  colour1[1] + (colour2[1] - colour1[1]) * (env -> count)/ COLOUR_STEPS;
	env -> actual_colour[2] =  colour1[2] + (colour2[2] - colour1[2]) *( env -> count) / COLOUR_STEPS;
	env -> actual_colour[3] =  colour1[3] + (colour2[3] - colour1[3]) * (env -> count )/ COLOUR_STEPS;
	
    
}

void updateEverything(Player* p, environment * env) {
    bool moved = true;
    updateEnvironment(p, env); 
    updateHealth(p, env);
    updateKeys(p, &moved);    
    updatePosition(p);
}    

int main(void) {
    
    

    
    
   
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - keyboard input");
    SetTargetFPS(60); 



    Player character;
    environment env;
    Texture2D texture = getTexture("images/maincharacter/smile.png");
    makePlayer(&character, 500, 100, 100, texture);
    env.count = 0;

    while (!WindowShouldClose()) {



        updateEverything(&character, &env);
        
        
        BeginDrawing();
        
	    DrawEverything(&character, &env);

            
        EndDrawing();
    }

  
    CloseWindow();
    return 0;
}