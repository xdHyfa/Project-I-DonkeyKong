#include "Headers/level1.h"
#include "Headers/Mario_Controller.h"
#include "Headers/scenes.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "raylib.h"
void runLevel1(){
    
    // Like start() function from unity
    
    if (!Scene_Init) { 
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");

        marioTexture = LoadTexture("MARIO.png");
        Setup();
        Scene_Init = true;
    }

    // Like update() function from unity
    Mario_Movement();

    BeginDrawing();

    ClearBackground(BLACK);
    
    DrawTextureRec(marioTexture, frameRec, marioPosition, WHITE);

    EndDrawing();

    // TO DO: RUN PLAYER MOVEMENT MECHANICS (Function to check inputs, draw textures, etc)


    // Like onDestroy() function from unity, run before scene change.
    if (current_scene != LEVEL1) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(marioTexture);

        Scene_Init = false; // reset initialization boolean.
    }
}
