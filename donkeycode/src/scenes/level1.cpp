#include "scenes/level1.h"
#include "entities/Player.h"
#include "scenes/scenes.h"
#include "entities/Fire_Spawner.h"
#include "entities/Ground.h"
#include "resource_dir.h"	
#include "raylib.h"
void runLevel1(){
    
    // Like start() function from unity
    
    if (!Scene_Init) { 
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        marioTexture = LoadTexture("sprites/MARIO.png");
        Setup();
        RampSetter(Ramp_0, 16, 0, true, 7);
        Scene_Init = true;

    }

    // Like update() function from unity
    Mario_Movement();
    RampCollision(Ramp_0, 14, 7);
    DrawTextureRec(marioTexture, frameRec, marioPosition, WHITE);
    RampDrawer(Ramp_0, 16);
    
    if (IsKeyPressed(KEY_K)&& !Fire1.has_Spawned) SpawnFire();
    if (Fire1.has_Spawned) {
        Fire1.MoveFire();
        DrawTextureRec(Fire1.texture, Fire1.FireSprite, Fire1.FirePosition , WHITE);
    }


    // TO DO: RUN PLAYER MOVEMENT MECHANICS (Function to check inputs, draw textures, etc)


    // Like onDestroy() function from unity, run before scene change.
    
    if (current_scene != LEVEL1) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(marioTexture);
        UnloadTexture(Fire1.texture);
        UnloadTexture(Fire2.texture);
        

        Scene_Init = false; // reset initialization boolean.
    }
}
