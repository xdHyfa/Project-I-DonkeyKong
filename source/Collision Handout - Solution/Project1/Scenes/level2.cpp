#include "Scenes/level2.h"
#include "Entities/Player.h"
#include "Entities/Level2Entities.h"
#include "Scenes/scenes.h"
#include "Maps/Level2Map.h"
#include "include/resource_dir.h"	
#include "raylib.h"
#include "Core/constants.h"

Music level2Music = { 0 };
Music deathMusic2 = { 0 };
bool  isDead2 = false;

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
        Level2ButtonSetter();

        SetStartTime2();
        Scene_Init = true;

        level2Music = LoadMusicStream("Audio/Stage-2-Springboard2.wav");      //MUSICA
        deathMusic2 = LoadMusicStream("Audio/Dead.wav");
        level2Music.looping = true;
        PlayMusicStream(level2Music);
        isDead2 = false;



    }
    /* UPDATE STARTS HERE */
    

    Mario.Movement();
    UpdateMusicStream(level2Music);
    UpdateMusicStream(deathMusic2);
    
    /*---GROUND COLLISIONS---*/

    //Mario's Ground Collisions divided by Y levels (One for each ramp)
    Level2RampCollisions(Mario);
    Level2LadderCollisions(Mario);
    Level2CheckButtons(Mario);

    
    
    /*---TEXTURE DRAW---*/
    Level2LadderDraw();
    Level2RampDraw();
    Level2ButtonsDraw();
    //--Debugging tool: Map Hitboxes
    //DrawLevel2Colliders();
    
    
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    
    //--Debugging tool: Mario Origin Point (RED) and Mario Floor Collider (YELLOW)
    //DrawMarioCollider();
    
    /*---ENTITY SPAWN & MOVEMENT ROUTINES---*/
    
    Level2EntitiesRoutine();

    if (IsKeyPressed(KEY_ONE)) {
        ChangeScene();
    }

    CheckWinCondition();
    /*Like onDestroy() function from unity, run before scene change.*/

    if (GetCurrentScene() != LEVEL2) {

        //TO DO: UNLOAD STUFF.
        UnloadTexture(Mario.Texture);
        UnloadLevel2Entities();
        Truss::UnloadSharedTexture();
        Ladder::UnloadSharedTexture();
        UnloadButtonTexture();
        ResetLevel2Entities();
        ResetButtons();
        UnloadMusicStream(level2Music);
        UnloadMusicStream(deathMusic2);

        Scene_Init = false; // reset initialization boolean.
    }
}