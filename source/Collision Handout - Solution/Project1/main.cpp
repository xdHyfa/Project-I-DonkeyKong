#include <raylib.h>
#include <iostream>

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    InitAudioDevice();

   
    SetTargetFPS(60);               
  
    while (!WindowShouldClose())   
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

       

        EndDrawing();
       
    }

    
    CloseAudioDevice();
    CloseWindow();        
   

    return 0;
}
