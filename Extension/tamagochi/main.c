#include <raylib.h>
#include "player.h"
#include "sensorsData.h"
#define COLOUR_STEPS 150
#define FPS 60
#define SCALING_FACTOR 4
#define GRASS_SCALING_FACTOR 13.5



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
    //currently unimplemented

}

void updateEnvironment(Player* p, environment* env) {
    // updates the data from sensors
    FILE *sensorFile = fopen("sensorReadings.txt", "r");
    fetchData(&(env -> data), sensorFile);
    fclose(sensorFile);

    //environment light off
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
    SetTargetFPS(FPS); 



    Player character;
    environment env;
    // Flaing Hell Anger  state
    Image flamingAngerImage = LoadImage("images/flamingHellAnger.png");
    ImageResizeNN(&flamingAngerImage, flamingAngerImage.width * SCALING_FACTOR, flamingAngerImage.height * SCALING_FACTOR);
    Texture2D flamingAnger = LoadTextureFromImage(flamingAngerImage);
    // Frozen state
    Image frozenImage = LoadImage("images/frozen.png");
    ImageResizeNN(&frozenImage, frozenImage.width * SCALING_FACTOR, frozenImage.height * SCALING_FACTOR);
    Texture2D frozen = LoadTextureFromImage(frozenImage);
    // Angry state
    Image angryImage = LoadImage("images/angry.png");
    ImageResizeNN(&angryImage, angryImage.width * SCALING_FACTOR, angryImage.height * SCALING_FACTOR);
    Texture2D angry = LoadTextureFromImage(angryImage);
    // HappyHey state
    Image happyHeyImage = LoadImage("images/happyHey.png");
    ImageResizeNN(&happyHeyImage, happyHeyImage.width * SCALING_FACTOR, happyHeyImage.height * SCALING_FACTOR);
    Texture2D happyHey = LoadTextureFromImage(happyHeyImage);
    // Anxious state
    Image anxiousImage = LoadImage("images/anxious.png");
    ImageResizeNN(&anxiousImage, anxiousImage.width * SCALING_FACTOR, anxiousImage.height * SCALING_FACTOR);
    Texture2D anxious = LoadTextureFromImage(anxiousImage);
    // Normal state
    Image normalImage = LoadImage("images/normal.png");
    ImageResizeNN(&normalImage, normalImage.width * SCALING_FACTOR, normalImage.height * SCALING_FACTOR);
    Texture2D normal = LoadTextureFromImage(normalImage);
    // Grass image handling
    Image grassImage = LoadImage("images/output-onlinepngtools.png");
    ImageResizeNN(&grassImage, grassImage.width * GRASS_SCALING_FACTOR, grassImage.height * GRASS_SCALING_FACTOR);
    Texture2D grass = LoadTextureFromImage(grassImage);
    //
    //Texture2D texture = getTexture("images/maincharacter/smile.png");
    
    makePlayer(&character, 500, 100, 100, normal);
    env.count = 0;
    int happyHeyCount = 0;

    while (!WindowShouldClose()) {
        if (happyHeyCount < FPS * 2) {
	  character.texture = happyHey;
	  happyHeyCount++;
        }else if (character.health > 250) {
	  character.texture = normal;
	}else {
	  character.texture = anxious;
	}
	

        DrawTexture(grass, 0, BOUNDS_Y, WHITE);
	DrawTexture(grass, 400, BOUNDS_Y, WHITE);
        updateEverything(&character, &env);
        
        
        BeginDrawing();
        
        DrawEverything(&character, &env);

            
        EndDrawing();
    }

  
    CloseWindow();
    return 0;
}
