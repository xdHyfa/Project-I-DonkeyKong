#include "libraries/raylib/include/raylib.h"
#include "Game.hpp"
#include "constants.hpp"



int main()
{

    InitWindow(SCALED_WIDTH, SCALED_HEIGHT, "Donkey Code");

    Camera2D camera = { 0 };
    camera.zoom = 4.0f;

    SetTargetFPS(24);    

    Game game;

    while (!WindowShouldClose())    
    {
        game.HandleInput();

        game.Update();

        game.CheckForCollisions();

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode2D(camera);

        game.Draw();


        EndDrawing();

    }

   
    CloseWindow();        
   
    return 0;
}