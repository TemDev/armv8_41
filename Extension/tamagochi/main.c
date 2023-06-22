#include <raylib.h>
#include <math.h>
#include "player.h"
#include "sensorsData.h"
#include "musicBackground.h"

#include "fruits.c"
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
    // draws a health bar
    int hbar_width = (p -> health > 0)? p -> health: 0;
    Color color = (hbar_width < 350)? YELLOW: GREEN;
    DrawRectangle(0,0, p -> health, 10, color);
    // Texture2D button1 = LoadTexture("Strawberry.png"); // Load button texture
    // Texture2D button2 = LoadTexture("Apple.png"); // Load button textures
    // Texture2D button3 = LoadTexture("Pear.png"); // Load button texture

    // Define frame rectangle for drawing
    // float frameHeight1 = (float)button1.height/NUM_FRAMES;
    // Rectangle sourceRec1 = { 0, 0, (float)button1.width, frameHeight1 };

    // // Define button bounds on screen
    // Rectangle btnBounds1 = { screenWidth/4.0f - button1.width/4.0f, 
    // screenHeight/4.0f - button1.height/NUM_FRAMES/4.0f, (float)button1.width, frameHeight1 };

    // float frameHeight2 = (float)button2.height/NUM_FRAMES;
    // Rectangle sourceRec2 = { 2, 2, (float)button2.width, frameHeight2 };

    // // Define button bounds on screen
    // Rectangle btnBounds2 = { screenWidth/2.0f - button2.width/2.0f, 
    // screenHeight/2.0f - button2.height/NUM_FRAMES/2.0f, (float)button2.width, frameHeight2 };

    // float frameHeight3 = (float)button3.height/NUM_FRAMES;
    // Rectangle sourceRec3 = { 6, 6, (float)button3.width, frameHeight3 };

    // // Define button bounds on screen
    // Rectangle btnBounds3 = { screenWidth/6.0f - button3.width/6.0f,
    //  screenHeight/6.0f - button3.height/NUM_FRAMES/6.0f, (float)button3.width, frameHeight3 };
}

void DrawBackGround(Player* p, int *actual_colour, float * buffer, float time, environment* env) {
    Color c = (Color) {actual_colour[0], actual_colour[1], actual_colour[2], actual_colour[3]};
    ClearBackground(c);
    // This is for audio
    //DrawTexture(getMusicBackground(env -> background, time, buffer, c) ,0, 0, WHITE);
    DrawFruits(&(env ->fs));
    DrawText("Don't let it die", 10, 10, 20, DARKGRAY);

}

void DrawPlayer(Player* p){
    DrawTexture(p -> texture, p ->position.x, p -> position.y, WHITE);
}

void DrawEverything(Player* p, environment *env, float *buffer, float time) {
    DrawBackGround(p, env -> actual_colour, buffer, time,env);
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
    env->data.lightOff = true;
    //environment light off
    if (env -> data.lightOff && (env -> count < COLOUR_STEPS) ){
        env -> count++;
    } else if (!env->data.lightOff && env -> count > 0) {
	    env -> count--;
    }
    env -> sun_y = ((float) SCREEN_HEIGHT * 2) * ((float) env -> count / (float)COLOUR_STEPS);
    env -> sun_x = ((float) SCREEN_WIDTH / 2) + sqrt(pow(SCREEN_HEIGHT, 2)- pow(env->sun_y - SCREEN_HEIGHT, 2));
    env -> moon_y = ((float) SCREEN_HEIGHT * 2) * ((float) (COLOUR_STEPS - env -> count) / (float)COLOUR_STEPS);
    env -> moon_x = ((float) SCREEN_WIDTH / 2) - sqrt(pow(SCREEN_HEIGHT, 2)- pow(env->moon_y - SCREEN_HEIGHT, 2));
    env -> actual_colour[0] =  colour1[0] + (colour2[0] - colour1[0]) * (env -> count)/ COLOUR_STEPS;
    env -> actual_colour[1] =  colour1[1] + (colour2[1] - colour1[1]) * (env -> count)/ COLOUR_STEPS;
    env -> actual_colour[2] =  colour1[2] + (colour2[2] - colour1[2]) *( env -> count) / COLOUR_STEPS;
    env -> actual_colour[3] =  colour1[3] + (colour2[3] - colour1[3]) * (env -> count )/ COLOUR_STEPS;
	
    
}

void updateEverything(Player* p, environment * env) {
    bool moved = true;
    updateEnvironment(p, env);
    updateKeys(p, &moved);  
    updateFruits(&(env ->fs));
    updatePosition(p);
    updateHealth(p, env);
}    

int main(void) {
    
   
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Digital Pet");
    SetTargetFPS(FPS); 



    Player character;
    environment env;
    // Raspberry
    Image raspberryImage = LoadImage("image/Raspberry.png");
    Texture2D raspberry = LoadTextureFromImage(raspberryImage);
    // Apple
    Image appleImage = LoadImage("images/Apple.png");
    Texture2D apple = LoadTextureFromImage(appleImage);
    // Pear
    Image pearImage = LoadImage("images/Pear.png");
    Texture2D pear = LoadTextureFromImage(pearImage);
    // Moon state
    Image moonImage = LoadImage("images/Moon.png");
    ImageResizeNN(&moonImage, moonImage.width * SCALING_FACTOR, moonImage.height * SCALING_FACTOR);
    Texture2D moon = LoadTextureFromImage(moonImage);
    // Flaing Hell Anger  state
    Image sunImage = LoadImage("images/Sun.png");
    ImageResizeNN(&sunImage, sunImage.width * SCALING_FACTOR, sunImage.height * SCALING_FACTOR);
    Texture2D sun = LoadTextureFromImage(sunImage);
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
    
    // processes the music 
    char music [] = "music/songx.wav";
    float *buffer;
    buffer  = readData(music);
    // starts the music

    Music song = LoadMusicStream(music);

    //Texture2D texture = getTexture("images/maincharacter/smile.png");
    
    makePlayer(&character, 500, PLAYER_HEIGHT,PLAYER_WIDTH, normal);
    env.count = 0;
    env.background = malloc(sizeof(Texture2D));
    initFruits(&env.fs);
    int happyHeyCount = 0;

    PlayMusicStream(song);


    
    float frameTime = 0;

    while (!WindowShouldClose()) {


        frameTime += GetFrameTime();  // raylib function
        // if (frameTime > 10) {
        //     if (data->tempC > 26) {
        //         tooHot = true;
        //     } else if (data->tempC < 0) {
        //         tooCold = true;
        //     }
        //     if (data->waterlevel > 1) {
        //         precipitation = true;
        //     } else if (data->waterlevel > 3) {
        //         heavyPrecipitation = true;
        //     }
        //     frameTime = 0;
        // }

        if (frameTime < 2) {
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
        DrawTexture(sun, env.sun_x, env.sun_y, WHITE);
	    DrawTexture(moon, env.moon_x, env.moon_y, WHITE);
        
        BeginDrawing();
        float time = 0;//(GetMusicTimePlayed(song) > 0)? GetMusicTimePlayed(song) : 0;
        DrawEverything(&character, &env, buffer,time);// change to the time of the actual song
        DrawTexture(grass, 0, BOUNDS_Y, WHITE);
	    DrawTexture(grass, 400, BOUNDS_Y, WHITE);
            
        EndDrawing();
    }
    free(buffer);
    free(env.background);
    free(env.fs);
    UnloadMusicStream(song);   // Unload music stream buffers from RAM

    CloseAudioDevice();

  
    CloseWindow();
    return 0;
}
