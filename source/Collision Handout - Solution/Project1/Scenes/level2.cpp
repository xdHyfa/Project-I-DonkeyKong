
#include "Scenes/level1.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include "Entities/Fire_Spawner.h"
#include "Maps/Level2Map.h"
#include "include/resource_dir.h"	
#include "raylib.h"
#include "Core/constants.h"



void runLevel2() {

    // Like start() function from unity

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        //Mario Setup
        Mario.Setup();

        Level2RampSetter();
        Level2LadderSetter();
        Scene_Init = true;

    }
    /* UPDATE STARTS HERE */

    Mario.Movement();
   
    
    /*---GROUND COLLISIONS---*/

    //Mario's Ground Collisions divided by Y levels (One for each ramp)
    Level2RampCollisions(Mario);
    Level2LadderCollision(Mario);

    
    
    /*---TEXTURE DRAW---*/
    Level2LadderDraw();
    Level2RampDraw();

    //--Debugging tool: Map Hitboxes
    //DrawLevel2Colliders();
    
    
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    
    //--Debugging tool: Mario Origin Point (RED) and Mario Floor Collider (YELLOW)
    //DrawMarioCollider();
    
    /*---ENTITY SPAWN & MOVEMENT ROUTINES---*/
    
    //FireRoutine(Fire1);
    BarrelRoutine();

    if (IsKeyPressed(KEY_ONE)) {
        ChangeScene();
    }

    /*Like onDestroy() function from unity, run before scene change.*/

    if (GetCurrentScene() != LEVEL2) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(Mario.Texture);
        UnloadTexture(Fire1.Texture);
        UnloadTexture(Fire2.Texture);
        Truss::UnloadSharedTexture();
        UnloadTexture(barrel1.Texture);
        Ladder::UnloadSharedTexture();


        Scene_Init = false; // reset initialization boolean.
    }
}