#include "Scenes/level1.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include "Entities/Fire_Spawner.h"
#include "Entities/Barrel.h"
#include "Entities/Ground.h"
#include "include/resource_dir.h"	
#include "raylib.h"
#include "Core/constants.h"

void runLevel1() {

    // Like start() function from unity

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        //Mario Setup
        Mario.Setup();

        MultiRampSetter();

        Scene_Init = true;

    }
    /* UPDATE STARTS HERE */

    Mario.Movement();
   
    
    /*---GROUND COLLISIONS---*/

    //Mario's Ground Collisions divided by Y levels (One for each ramp)
    MapCollision(Mario);
    DrawColliders();

    /*---TEXTURE DRAW---*/
    RampDrawer(Ramp_0, 14);
    RampDrawer(Ramp_1, 13);
    RampDrawer(Ramp_2, 13);
    RampDrawer(Ramp_3, 13);
    RampDrawer(Ramp_4, 13);
    RampDrawer(Ramp_5, 13);

    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

    /*---ENTITY SPAWN & MOVEMENT ROUTINES---*/
    
    FireRoutine();
    BarrelRoutine();



    /*Like onDestroy() function from unity, run before scene change.*/

    if (current_scene != LEVEL1) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(Mario.Texture);
        UnloadTexture(Fire1.Texture);
        UnloadTexture(Fire2.Texture);
        Truss::UnloadSharedTexture();
        UnloadTexture(barrel1.Texture);


        Scene_Init = false; // reset initialization boolean.
    }
}