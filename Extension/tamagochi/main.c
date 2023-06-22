#include <raylib.h>
#include <math.h>
#include "player.h"
#include "sensorsData.h"
#include "main.h"
#define COLOUR_STEPS 300
#define COLOUR_STEPS2 90
#include "musicBackground.h"
#include "fruits.c"
#define FPS 60
#define SCALING_FACTOR 4
#define GRASS_SCALING_FACTOR 13.5
#define ARRAY_FINISH -1



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
    //env->data.lightOff = true;
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
    // Snow
    Image snowImage = LoadImage("images/snow.png");
    ImageResizeNN(&snowImage, snowImage.width * SCALING_FACTOR, snowImage.height * SCALING_FACTOR);
    Texture2D snow = LoadTextureFromImage(snowImage);
    // Snow Cloud
    Image snowCloudImage = LoadImage("images/snow_clouds.png");
    ImageResizeNN(&snowCloudImage, snowCloudImage.width * SCALING_FACTOR, snowCloudImage.height * SCALING_FACTOR);
    Texture2D snowCloud = LoadTextureFromImage(snowCloudImage);
    // Rain
    Image rainImage = LoadImage("images/rain.png");
    ImageResizeNN(&rainImage, rainImage.width * SCALING_FACTOR, rainImage.height * SCALING_FACTOR);
    Texture2D rain = LoadTextureFromImage(rainImage);
    // Rain Cloud
    Image rainCloudImage = LoadImage("images/rain_clouds.png");
    ImageResizeNN(&rainCloudImage, rainCloudImage.width * SCALING_FACTOR, rainCloudImage.height * SCALING_FACTOR);
    Texture2D rainCloud = LoadTextureFromImage(rainCloudImage);
    // Raspberry
    Image raspberryImage = LoadImage("image/Raspberry.png");
    Texture2D raspberry = LoadTextureFromImage(raspberryImage);
    // Apple
    Image appleImage = LoadImage("images/Apple.png");
    Texture2D apple = LoadTextureFromImage(appleImage);
    // Pear
    Image pearImage = LoadImage("images/Pear.png");
    Texture2D pear = LoadTextureFromImage(pearImage);
    // Moon
    Image moonImage = LoadImage("images/Moon.png");
    ImageResizeNN(&moonImage, moonImage.width * SCALING_FACTOR, moonImage.height * SCALING_FACTOR);
    Texture2D moon = LoadTextureFromImage(moonImage);
    // Sun
    Image sunImage = LoadImage("images/Sun.png");
    ImageResizeNN(&sunImage, sunImage.width * SCALING_FACTOR, sunImage.height * SCALING_FACTOR);
    Texture2D sun = LoadTextureFromImage(sunImage);
    // Flaming Hell Anger  state
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
    int cloudProgress = 0;
    int rainingP = 0;
    int rainingP2 = COLOUR_STEPS2 / 2;
    Texture2D rainOrSnowCloud;
    Texture2D rainOrSnow;
    float final_x_offset[] = {((float)SCREEN_WIDTH / 2 + 100.), ((float)SCREEN_WIDTH / 2 + 120.), ((float)SCREEN_WIDTH / 2), ((float)SCREEN_WIDTH / 2 - 100.),  ((float)SCREEN_WIDTH / 2 - 150.), ((float)SCREEN_WIDTH / 2 - 200.), ((float)SCREEN_WIDTH / 2 - 300.), ((float)SCREEN_WIDTH / 2 + 200.), ((float)SCREEN_WIDTH / 2 + 250.), ((float)SCREEN_WIDTH / 2 + 320.), ((float)SCREEN_WIDTH / 2 + 380.), ARRAY_FINISH};
    int x_offset = 100;
    int y_offset[] = {0, 25, 30, 40, 30, 20, 10, 35, 0 ,20, 40, ARRAY_FINISH};
    float raining_Ps[] = {0, COLOUR_STEPS2 / 5, COLOUR_STEPS2 / 5 * 2, COLOUR_STEPS2 / 5 * 3, COLOUR_STEPS2 / 5 * 4, ARRAY_FINISH};
    
    while (!WindowShouldClose()) {
        // env.data.tempC = 35;
        if (happyHeyCount < FPS * 2) {

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
        } else if (character.health > 250) {
	  character.texture = normal;
	} else {
	  character.texture = anxious;
	}
	if (env.data.tempC <= 5) {
	  character.texture = frozen;
	} else if (env.data.tempC >= 30) {
	  character.texture = flamingAnger;
	}
	  

        updateEverything(&character, &env);
        DrawTexture(sun, env.sun_x, env.sun_y, WHITE);
	DrawTexture(moon, env.moon_x, env.moon_y, WHITE);

	// env.data.waterLevel = 5;
	// env.data.tempC = 4;
	if ((env.data.waterLevel > 1) && (env.data.tempC > 5)) {
	  if (cloudProgress < COLOUR_STEPS) {
	    cloudProgress++;
	  }
	  rainOrSnowCloud = rainCloud;
	  rainOrSnow = rain;
	} else if ((env.data.waterLevel > 1) && (env.data.tempC <= 5)) {
	  if (cloudProgress < COLOUR_STEPS) {
	    cloudProgress++;
	  }
	  rainOrSnowCloud = snowCloud;
	  rainOrSnow = snow;
	} else if ((env.data.waterLevel < 1) && (cloudProgress > 0)) {
	  cloudProgress--;
	}
	for (int i = 0; raining_Ps[i] != ARRAY_FINISH; i++) {
	  if (raining_Ps[i] == COLOUR_STEPS2) {
	    raining_Ps[i] = 0;
	  } else {
	    raining_Ps[i]++;
	  }
	}
	
	for (int i = 0; raining_Ps[i] != ARRAY_FINISH; i++) {
	  for (int j = 0; final_x_offset[j] != ARRAY_FINISH; j++) {
	    DrawTexture(rainOrSnow, -x_offset + final_x_offset[j] * ((float)cloudProgress / (float)COLOUR_STEPS), y_offset[j] + rainImage.height / 3 + SCREEN_HEIGHT * (raining_Ps[i] / (float)COLOUR_STEPS2), WHITE);
	  }
	}
	
	for (int i = 0; final_x_offset[i] != ARRAY_FINISH; i++) {
	  DrawTexture(rainOrSnowCloud, -x_offset + final_x_offset[i] * ((float)cloudProgress / (float)COLOUR_STEPS), y_offset[i], WHITE);
	}
	
        DrawTexture(grass, 0, BOUNDS_Y, WHITE);
	DrawTexture(grass, 400, BOUNDS_Y, WHITE);
        
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
