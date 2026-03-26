#include "scenes/level1.h"
#include "entities/Player.h"
#include "scenes/scenes.h"
#include "entities/Fire_Spawner.h"
#include "entities/Barrel.h"
#include "entities/Ground.h"
#include "resource_dir.h"	
#include "raylib.h"
#include "core/constants.h"
void runLevel1(){
    
    // Like start() function from unity
    
    if (!Scene_Init) { 
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        marioTexture = LoadTexture("sprites/MARIO.png");
        Setup();
        Truss:: LoadSharedTexture();
        RampSetter(Ramp_0, 14, true , true, 7, SCREEN_HEIGHT-32);
        RampSetter(Ramp_1, 13, false , false, 0, 225-48);
        RampSetter(Ramp_2, 13, false, true, 0, 225-(80));
        RampSetter(Ramp_3, 13, false , false, 0, 225 - (128));
        Scene_Init = true;

    }

    // Like update() function from unity
    Mario_Movement();
    RampCollision(Ramp_0, 14, marioFloorCollider, marioPosition,16,true);
    DrawTextureRec(marioTexture, frameRec, marioPosition, WHITE);
    RampDrawer(Ramp_0, 14);
    RampDrawer(Ramp_1, 13);
    RampDrawer(Ramp_2, 13);
    RampDrawer(Ramp_3, 13);
    if (IsKeyPressed(KEY_K)&& !Fire1.has_Spawned) SpawnFire();
    if (IsKeyPressed(KEY_J)) SpawnBarrel();
    if (Fire1.has_Spawned) {
        Fire1.MoveFire();
        RampCollision(Ramp_0, 14, Fire1.FireFloorCollider, Fire1.FirePosition, 16, false);
        DrawTextureRec(Fire1.texture, Fire1.FireSprite, Fire1.FirePosition , WHITE);
    }
    if (barrel1.has_Spawned) {
        barrel1.MoveBarrel();
        RampCollision(Ramp_0, 14, barrel1.BarrelFloorCollider, barrel1.BarrelPosition, 13, false);
        DrawTextureRec(barrel1.texture, barrel1.BarrelSprite, barrel1.BarrelPosition, WHITE);
    }


    // TO DO: RUN PLAYER MOVEMENT MECHANICS (Function to check inputs, draw textures, etc)


    // Like onDestroy() function from unity, run before scene change.
    
    if (current_scene != LEVEL1) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(marioTexture);
        UnloadTexture(Fire1.texture);
        UnloadTexture(Fire2.texture);

        UnloadTexture(barrel1.texture);
        

        Scene_Init = false; // reset initialization boolean.
    }
}
