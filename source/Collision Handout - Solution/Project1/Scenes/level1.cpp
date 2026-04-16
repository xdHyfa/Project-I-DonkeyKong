#include "Scenes/level1.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include "Entities/Fire_Spawner.h"
#include "Entities/Barrel.h"
#include "Maps/Level1Map.h"
#include "include/resource_dir.h"	
#include "raylib.h"
#include "Core/constants.h"
#include "Entities/EntityCollision.hpp"



void runLevel1() {

    // Like start() function from unity

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        //Mario Setup
        Mario.Setup();

        Level1RampSetter();
        Level1LadderSetter();
        Scene_Init = true;

    }
    /* UPDATE STARTS HERE */

    Mario.Movement();

   
    
    /*---GROUND COLLISIONS---*/

    //Mario's Ground Collisions divided by Y levels (One for each ramp)
    Level1RampCollisions(Mario);
    Level1LadderCollision(Mario);

    
    
    /*---TEXTURE DRAW---*/
    Level1LadderDraw();
    Level1RampDraw();

    //--Debugging tool: Map Hitboxes
    //DrawLevel1Colliders();
    
    
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    
    //--Debugging tool: Mario Origin Point (RED) and Mario Floor Collider (YELLOW)
    //DrawMarioCollider();
    
    /*---ENTITY SPAWN & MOVEMENT ROUTINES---*/
    
    FireRoutine();
    BarrelRoutine();

    Level1CheckWinCondition(Mario);

    if (barrel1.has_Spawned && EntityCollision(Mario, barrel1))
    {
        Mario.die();
    }

    if (IsKeyPressed(KEY_TWO)) {
        ChangeScene();
    }
    /*Like onDestroy() function from unity, run before scene change.*/

    if (GetCurrentScene() != LEVEL1) {

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