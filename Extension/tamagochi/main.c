/*******************************************************************************************
*
*   raylib [core] example - Keyboard input
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>
#include "player.h"
#include "sensorsData.h"
#define COLOUR_STEPS 150

/*#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            120 // Change this back to 330 if compiling for a platform that supports GLSL3.3
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif*/


/*bool inBounds(Vector2 object){
    return object.x < BOUNDS_X && object.x > -BOUNDS_X
     && object.y < BOUNDS_Y && object.y > -BOUNDS_Y;
    
}*/
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
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

void DrawEverything(Player* p, int *actual_colour) {
    DrawBackGround(p, actual_colour);
    DrawAttributes(p);
    DrawPlayer(p);
}

void updateKeys(Player* p, bool* moved){
    if (IsKeyDown(KEY_RIGHT)) {
            p -> velocity.x = 3.0f;
        } else if (IsKeyDown(KEY_LEFT)) {
            p -> velocity.x = -3.0f;
        } else if (IsKeyDown(KEY_UP)) {
            p -> velocity.y = -3.0f;
        } else if (IsKeyDown(KEY_DOWN)) {
            p -> velocity.y = 3.0f;
        }
        else {
            *moved = false;
        }
}

void updateEnvironment(Player* p) {

}

void updateConditions(Player* p,bool *moved) {
    updateEnvironment(p);
    updateKeys(p, moved);
    p -> health -= -0.05f;
}    
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    int colour1[] = {102, 191, 255, 255};
    int colour2[] = {80, 80, 80, 255};
    int actual_colour[] = {102, 191, 255, 255};
    bool light_off = true;
    int count = 0;
    data *dataSensors = fetchData();
    //light_off = dataSensors->lightOff;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    Player character;
   

    SetTargetFPS(60); 
    
                  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   // Load image in CPU memory (RAM)

    // Draw one image over the other with a scaling of 1.5f
    //ImageCrop(&character, (Rectangle){ 0, 0, 100, 100}); // Crop resulting image
    

    // Draw on the image with a few image draw methods
    // ImageDrawPixel(&character, 10, 10, RAYWHITE);
    // ImageDrawCircleLines(&character, 10, 10, 5, RAYWHITE);
    // ImageDrawRectangle(&character, 5, 20, 10, 10, RAYWHITE);
    Texture2D texture = getTexture("images/maincharacter/smile.png");//LoadTextureFromImage(character);//getTexture("images/maincharacter/smile.png");
    
    makePlayer(&character, 500, 100, 100, texture);
    //UnloadImage(character); 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
      
        // Background Colour updates
        if (light_off && (count < COLOUR_STEPS)) {
	    count++;
        } else if (count > 0) {
	    count--;
        }
        actual_colour[0] =  colour1[0] + (colour2[0] - colour1[0]) * count / COLOUR_STEPS;
	actual_colour[1] =  colour1[1] + (colour2[1] - colour1[1]) * count / COLOUR_STEPS;
	actual_colour[2] =  colour1[2] + (colour2[2] - colour1[2]) * count / COLOUR_STEPS;
	actual_colour[3] =  colour1[3] + (colour2[3] - colour1[3]) * count / COLOUR_STEPS;
	// 
        bool moved = true;
        updateConditions(&character, &moved);
        
        if (moved) {
            character.health -= 0.1;
        }
        
        updatePosition(&character);
        // if (moved) {
        //     printf("X:%.4f Y:%.4f \n", ballPosition.x, ballPosition.y);
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
	DrawEverything(&character, &actual_colour[0]);

            
            //DrawCircleV(character.position, RADIUS, MAROON);
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
