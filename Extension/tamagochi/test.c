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

#include "raylib.h"
#include "stdbool.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            120 // Change this back to 330 if compiling for a platform that supports GLSL3.3
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define BOUNDS_X 400
#define BOUNDS_Y 400
#define RADIUS 50
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
    return ;
}
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

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
    Texture2D texture = getTexture("images/maincharacter/smile.png");
    //UnloadImage(character); 
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Vector2 movement = {0, 0};
        bool moved = true;
        if (IsKeyDown(KEY_RIGHT)) {
            movement.x += 3.0f;
        } else if (IsKeyDown(KEY_LEFT)) {
            movement.x -= 3.0f;
        } else if (IsKeyDown(KEY_UP)) {
            movement.y -= 3.0f;
        } else if (IsKeyDown(KEY_DOWN)) {
            movement.y += 3.0f;
        }
        else {
            moved = false;
        }
        
        if ((ballPosition.x + movement.x) < (screenWidth - RADIUS)  && (ballPosition.x + movement.x) > RADIUS
     && (ballPosition.y + movement.y) < (screenHeight - RADIUS) && (ballPosition.y + movement.y) > RADIUS) {
            ballPosition.x += movement.x;
            ballPosition.y += movement.y;
        }
        // if (moved) {
        //     printf("X:%.4f Y:%.4f \n", ballPosition.x, ballPosition.y);
        // }
        movement.x = 0;
        movement.y = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawCircleV(ballPosition, RADIUS, MAROON);
            DrawTexture(texture, ballPosition.x, ballPosition.y, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
