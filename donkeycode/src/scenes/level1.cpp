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

        MultiRampSetter();

        Scene_Init = true;

    }

    // Like update() function from unity
    Mario_Movement();
    //Mario's Ground Collisions divided by Y levels (One for each ramp)
    MarioGroundCollisions(); 

    RampDrawer(Ramp_0, 14);
    RampDrawer(Ramp_1, 13);
    RampDrawer(Ramp_2, 13);
    RampDrawer(Ramp_3, 13);
    RampDrawer(Ramp_4, 13);
    RampDrawer(Ramp_5, 13);

    DrawTextureRec(marioTexture, frameRec, marioPosition, WHITE);

    FireSpawner();

    BarrelSpawner();



    // TO DO: RUN PLAYER MOVEMENT MECHANICS (Function to check inputs, draw textures, etc)


    // Like onDestroy() function from unity, run before scene change.

    if (current_scene != LEVEL1) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(marioTexture);
        UnloadTexture(Fire1.texture);
        UnloadTexture(Fire2.texture);
        Truss::UnloadSharedTexture();
        UnloadTexture(barrel1.texture);


        Scene_Init = false; // reset initialization boolean.
    }
}