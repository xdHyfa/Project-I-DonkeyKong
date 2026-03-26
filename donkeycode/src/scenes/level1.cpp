#include "scenes/level1.h"
#include "entities/Player.h"
#include "scenes/scenes.h"
#include "entities/Fire_Spawner.h"
#include "entities/Barrel.h"
#include "entities/Ground.h"
#include "resource_dir.h"	
#include "raylib.h"
#include "core/constants.h"
void runLevel1() {

    // Like start() function from unity

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        marioTexture = LoadTexture("sprites/MARIO.png");
        //Mario Setup
        Setup();

        Truss::LoadSharedTexture();

        Ramp_0_YPos = (SCREEN_HEIGHT - TrussHeight) - 1;
        RampSetter(Ramp_0, 14, true, true, 7, Ramp_0_YPos);

        Ramp_1_YPos = (SCREEN_HEIGHT - TrussHeight) - 43;
        RampSetter(Ramp_1, 13, false, false, 0, Ramp_1_YPos);

        Ramp_2_YPos = (SCREEN_HEIGHT - TrussHeight) - 62;
        RampSetter(Ramp_2, 13, false, true, 0, Ramp_2_YPos);

        Ramp_3_YPos = (SCREEN_HEIGHT - TrussHeight) - 110;
        RampSetter(Ramp_3, 13, false, false, 0, Ramp_3_YPos);

        Ramp_4_YPos = (SCREEN_HEIGHT - TrussHeight) - 130;
        RampSetter(Ramp_4, 13, false, true, 0, Ramp_4_YPos); 

        Ramp_5_YPos = (SCREEN_HEIGHT - TrussHeight) - 168;
        RampSetter(Ramp_5, 13, false, false, 9, Ramp_5_YPos);

        Scene_Init = true;

    }

    // Like update() function from unity
    Mario_Movement();
    //Mario's Ground Collisions divided by Y levels (One for each ramp)

    DrawColliders();

    if (CheckCollisionPointRec(marioFloorCollider, Ramp_0_Zone)) {
        RampCollision(Ramp_0, 14, marioFloorCollider, marioPosition, 16, true);
    }
    else if (CheckCollisionPointRec(marioFloorCollider, Ramp_1_Zone)) {
        RampCollision(Ramp_1, 13, marioFloorCollider, marioPosition, 16, true);
    }
    else if (CheckCollisionPointRec(marioFloorCollider, Ramp_2_Zone)){
        RampCollision(Ramp_2, 13, marioFloorCollider, marioPosition, 16, true);
    }
    else if (CheckCollisionPointRec(marioFloorCollider, Ramp_3_Zone)) {
        RampCollision(Ramp_3, 13, marioFloorCollider, marioPosition, 16, true);
    }
    else if (CheckCollisionPointRec(marioFloorCollider, Ramp_4_Zone)) {
        RampCollision(Ramp_4, 13, marioFloorCollider, marioPosition, 16, true);
    }
    else if (CheckCollisionPointRec(marioFloorCollider, Ramp_5_Zone)) {
        RampCollision(Ramp_5, 13, marioFloorCollider, marioPosition, 16, true);
    }
    else {
        marioPosition.y++;
        //Gravity Here
    }

    RampDrawer(Ramp_0, 14);
    RampDrawer(Ramp_1, 13);
    RampDrawer(Ramp_2, 13);
    RampDrawer(Ramp_3, 13);
    RampDrawer(Ramp_4, 13);
    RampDrawer(Ramp_5, 13);

    DrawTextureRec(marioTexture, frameRec, marioPosition, WHITE);

    if (IsKeyPressed(KEY_K)&& !Fire1.has_Spawned) SpawnFire();
    //if (IsKeyPressed(KEY_J)) SpawnBarrel();
    if (Fire1.has_Spawned) {
        Fire1.MoveFire();
        RampCollision(Ramp_0, 14, Fire1.FireFloorCollider, Fire1.FirePosition, 16, false);
        DrawTextureRec(Fire1.texture, Fire1.FireSprite, Fire1.FirePosition , WHITE);
    }
    //if (barrel1.has_Spawned) {
    //    barrel1.MoveBarrel();
    //    RampCollision(Ramp_0, 14, barrel1.BarrelFloorCollider, barrel1.BarrelPosition, 13, false);
    //    DrawTextureRec(barrel1.texture, barrel1.BarrelSprite, barrel1.BarrelPosition, WHITE);
    //}


    // TO DO: RUN PLAYER MOVEMENT MECHANICS (Function to check inputs, draw textures, etc)


    // Like onDestroy() function from unity, run before scene change.
    
    if (current_scene != LEVEL1) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(marioTexture);
        UnloadTexture(Fire1.texture);
        UnloadTexture(Fire2.texture);

        //UnloadTexture(barrel1.texture);
        

        Scene_Init = false; // reset initialization boolean.
    }
}
