// Main file of the project, operates on the UI side of the project

#include "main.h"
#include "sensorsData.h"
#include <math.h>
#include <raylib.h>
#define COLOUR_STEPS 300
#define COLOUR_STEPS2 90
#include "fruits.h"
#include "musicBackground.h"

#include "weather.h"

#define FPS 60
#define SCALING_FACTOR 4
#define GRASS_SCALING_FACTOR 13.5
#define ARRAY_FINISH -1
#define PLAYER_HEALTH 3000

int colour1[] = {102, 191, 255, 255};
int colour2[] = {80, 80, 80, 255};

// Makes a texture from a path to a file
Texture2D getTexture(char *path) {
    // path is for the image
    Image temp = LoadImage(path);
    Texture2D texture = LoadTextureFromImage(temp);
    // UnloadImage(temp);
    return texture;
}

// Draws the healthbar
void DrawAttributes(Player *p) {
    // draws a health bar
    int hbar_width = (p->health > 0) ? (p->health / (1.0 * PLAYER_HEALTH) * SCREEN_WIDTH / 2) : 0;
    Color color = (hbar_width < 350) ? YELLOW : GREEN;
    DrawRectangle(0, 0, hbar_width, 10, color);
}

// Draws a background
void DrawBackGround(Player *p, int *actual_colour, float *buffer, float time, environment *env) {
    Color c = (Color){actual_colour[0], actual_colour[1], actual_colour[2], actual_colour[3]};
    ClearBackground(c);
    // This is for audio
    getMusicBackground(time, buffer, c);

    DrawFruits(&(env->fs));
    // DrawText("e", 10, 10, 20, DARKGRAY);
}

// Draws the player on the screen using DrawTexture
void DrawPlayer(Player *p) { DrawTexture(p->texture, p->position.x, p->position.y, WHITE); }

// Draws everything on screen
void DrawEverything(Player *p, environment *env, float *buffer, float time) {
    DrawBackGround(p, env->actual_colour, buffer, time, env);
    DrawAttributes(p);
    DrawPlayer(p);
}

// Does nothing
void updateKeys(Player *p, bool *moved) {}

// Updates the envronment including the colour of background and the state of sun and moon
void updateEnvironment(Player *p, environment *env) {
    // updates the data from sensors
    FILE *sensorFile = fopen("sensorReadings.txt", "r");
    fetchData(&(env->data), sensorFile);
    fclose(sensorFile);
    // env->data.lightOff = true;
    // environment light off
    if (env->data.lightOff && (env->count < COLOUR_STEPS)) {
        env->count++;
    } else if (!env->data.lightOff && env->count > 0) {
        env->count--;
    }
    env->sun_y = ((float)SCREEN_HEIGHT * 2) * ((float)env->count / (float)COLOUR_STEPS);
    env->sun_x = ((float)SCREEN_WIDTH / 2) +
                 sqrt(pow(SCREEN_HEIGHT, 2) - pow(env->sun_y - SCREEN_HEIGHT, 2));
    env->moon_y =
        ((float)SCREEN_HEIGHT * 2) * ((float)(COLOUR_STEPS - env->count) / (float)COLOUR_STEPS);
    env->moon_x = ((float)SCREEN_WIDTH / 2) -
                  sqrt(pow(SCREEN_HEIGHT, 2) - pow(env->moon_y - SCREEN_HEIGHT, 2));
    env->actual_colour[0] = colour1[0] + (colour2[0] - colour1[0]) * (env->count) / COLOUR_STEPS;
    env->actual_colour[1] = colour1[1] + (colour2[1] - colour1[1]) * (env->count) / COLOUR_STEPS;
    env->actual_colour[2] = colour1[2] + (colour2[2] - colour1[2]) * (env->count) / COLOUR_STEPS;
    env->actual_colour[3] = colour1[3] + (colour2[3] - colour1[3]) * (env->count) / COLOUR_STEPS;
}

// Updates everything on screen
void updateEverything(Player *p, environment *env) {
    bool moved = true;
    updateEnvironment(p, env);
    updateKeys(p, &moved);
    updateFruits(&(env->fs), env);
    updatePosition(p, env);
    updateHealth(p, env);
}

// Ending screen, when game is over
void endingDraw(void) {
    float time = 0;
    while (time < 4) {
        time += GetFrameTime();
        BeginDrawing();
        ClearBackground(BLUE);
        DrawText("GAME OVER", 100, SCREEN_HEIGHT / 2, 30, GRAY);
        EndDrawing();
    }
}

// Main function that handles everything
int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Digital Pet");
    SetTargetFPS(60);

    Player character;
    environment env;
    // Snow
    Image snowImage = LoadImage("images/snow.png");
    ImageResizeNN(&snowImage, snowImage.width * SCALING_FACTOR, snowImage.height * SCALING_FACTOR);
    Texture2D snow = LoadTextureFromImage(snowImage);
    // Snow Cloud
    Image snowCloudImage = LoadImage("images/snow_clouds.png");
    ImageResizeNN(&snowCloudImage, snowCloudImage.width * SCALING_FACTOR,
                  snowCloudImage.height * SCALING_FACTOR);
    Texture2D snowCloud = LoadTextureFromImage(snowCloudImage);
    // Rain
    Image rainImage = LoadImage("images/rain.png");
    ImageResizeNN(&rainImage, rainImage.width * SCALING_FACTOR, rainImage.height * SCALING_FACTOR);
    Texture2D rain = LoadTextureFromImage(rainImage);
    // Rain Cloud
    Image rainCloudImage = LoadImage("images/rain_clouds.png");
    ImageResizeNN(&rainCloudImage, rainCloudImage.width * SCALING_FACTOR,
                  rainCloudImage.height * SCALING_FACTOR);
    Texture2D rainCloud = LoadTextureFromImage(rainCloudImage);
    // Raspberry
    Image raspberryImage = LoadImage("image/Raspberry.png");
    Texture2D raspberry = LoadTextureFromImage(raspberryImage);
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
    ImageResizeNN(&flamingAngerImage, flamingAngerImage.width * SCALING_FACTOR,
                  flamingAngerImage.height * SCALING_FACTOR);
    Texture2D flamingAnger = LoadTextureFromImage(flamingAngerImage);
    // Frozen state
    Image frozenImage = LoadImage("images/frozen.png");
    ImageResizeNN(&frozenImage, frozenImage.width * SCALING_FACTOR,
                  frozenImage.height * SCALING_FACTOR);
    Texture2D frozen = LoadTextureFromImage(frozenImage);
    // Angry state
    Image angryImage = LoadImage("images/angry.png");
    ImageResizeNN(&angryImage, angryImage.width * SCALING_FACTOR,
                  angryImage.height * SCALING_FACTOR);
    Texture2D angry = LoadTextureFromImage(angryImage);
    // HappyHey state
    Image happyHeyImage = LoadImage("images/happyHey.png");
    ImageResizeNN(&happyHeyImage, happyHeyImage.width * SCALING_FACTOR,
                  happyHeyImage.height * SCALING_FACTOR);
    Texture2D happyHey = LoadTextureFromImage(happyHeyImage);
    // Anxious state
    Image anxiousImage = LoadImage("images/anxious.png");
    ImageResizeNN(&anxiousImage, anxiousImage.width * SCALING_FACTOR,
                  anxiousImage.height * SCALING_FACTOR);
    Texture2D anxious = LoadTextureFromImage(anxiousImage);
    // Normal state
    Image normalImage = LoadImage("images/normal.png");
    ImageResizeNN(&normalImage, normalImage.width * SCALING_FACTOR,
                  normalImage.height * SCALING_FACTOR);
    Texture2D normal = LoadTextureFromImage(normalImage);
    // Grass image handling
    Image grassImage = LoadImage("images/output-onlinepngtools.png");
    ImageResizeNN(&grassImage, grassImage.width * GRASS_SCALING_FACTOR,
                  grassImage.height * GRASS_SCALING_FACTOR);
    Texture2D grass = LoadTextureFromImage(grassImage);

    // processes the music
    char music[] = "music/DENYA.wav";
    float *buffer;
    buffer = readData(music);
    // starts the music
    InitAudioDevice();
    Music song = LoadMusicStream(music);

    makePlayer(&character, PLAYER_HEALTH, PLAYER_HEIGHT, PLAYER_WIDTH, normal);
    env.count = 0;
    initFruits(&env.fs);
    int happyHeyCount = 0;
    int cloudProgress = 0;
    int rainingP = 0;
    int rainingP2 = COLOUR_STEPS2 / 2;
    Texture2D rainOrSnowCloud;
    Texture2D rainOrSnow;
    float final_x_offset[] = {((float)SCREEN_WIDTH / 2 + 100.), ((float)SCREEN_WIDTH / 2 + 120.),
                              ((float)SCREEN_WIDTH / 2),        ((float)SCREEN_WIDTH / 2 - 100.),
                              ((float)SCREEN_WIDTH / 2 - 150.), ((float)SCREEN_WIDTH / 2 - 200.),
                              ((float)SCREEN_WIDTH / 2 - 300.), ((float)SCREEN_WIDTH / 2 + 200.),
                              ((float)SCREEN_WIDTH / 2 + 250.), ((float)SCREEN_WIDTH / 2 + 320.),
                              ((float)SCREEN_WIDTH / 2 + 380.), ARRAY_FINISH};
    int x_offset = 100;
    int y_offset[] = {0, 25, 30, 40, 30, 20, 10, 35, 0, 20, 40, ARRAY_FINISH};
    float raining_Ps[] = {0,
                          COLOUR_STEPS2 / 5,
                          COLOUR_STEPS2 / 5 * 2,
                          COLOUR_STEPS2 / 5 * 3,
                          COLOUR_STEPS2 / 5 * 4,
                          ARRAY_FINISH};

    PlayMusicStream(song);

    float frameTime = 0;
    bool alive = false;
    while (!WindowShouldClose() || alive) {
        updateEverything(&character, &env);
        UpdateMusicStream(song);
        alive = character.health > 0;
        if (!alive) {
            break;
        }
        if (IsKeyPressed(KEY_P)) {
            env.musicOn = !env.musicOn;

            if (!env.musicOn)
                PauseMusicStream(song);
            else
                ResumeMusicStream(song);
        }
        BeginDrawing();

        // gets the timeframe
        frameTime += GetFrameTime(); // raylib function

        float time = (GetMusicTimePlayed(song) > 0) ? GetMusicTimePlayed(song) : 0;
        DrawEverything(&character, &env, buffer, time); // change to the time of the actual song

        DrawTexture(sun, env.sun_x, env.sun_y, WHITE);
        DrawTexture(moon, env.moon_x, env.moon_y, WHITE);

        if ((env.data.waterLevel > 1) && (env.data.tempC > 25)) {
            if (cloudProgress < COLOUR_STEPS) {
                cloudProgress++;
            }
            rainOrSnowCloud = rainCloud;
            rainOrSnow = rain;
        } else if ((env.data.waterLevel > 1) && (env.data.tempC <= 25)) {
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
                DrawTexture(rainOrSnow,
                            -x_offset +
                                final_x_offset[j] * ((float)cloudProgress / (float)COLOUR_STEPS),
                            y_offset[j] + rainImage.height / 3 +
                                SCREEN_HEIGHT * (raining_Ps[i] / (float)COLOUR_STEPS2),
                            WHITE);
            }
        }

        for (int i = 0; final_x_offset[i] != ARRAY_FINISH; i++) {
            DrawTexture(rainOrSnowCloud,
                        -x_offset +
                            final_x_offset[i] * ((float)cloudProgress / (float)COLOUR_STEPS),
                        y_offset[i], WHITE);
        }

        //  DrawTexture(grass, 0, BOUNDS_Y, WHITE);
        // DrawTexture(grass, 400, BOUNDS_Y, WHITE);

        DrawTexture(grass, 0, BOUNDS_Y, WHITE);
        DrawTexture(grass, 400, BOUNDS_Y, WHITE);

        EndDrawing();
    }

    if (!alive) {
        endingDraw();
    }
    free(buffer);
    free(env.fs);
    UnloadMusicStream(song); // Unload music stream buffers from RAM

    CloseAudioDevice();

    CloseWindow();
    return 0;
}
