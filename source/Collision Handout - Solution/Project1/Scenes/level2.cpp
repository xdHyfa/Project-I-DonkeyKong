#include "Scenes/level2.h"
#include "Entities/Player.h"
#include "Entities/Level2Entities.h"
#include "Scenes/scenes.h"
#include "Maps/Level2Map.h"
#include "include/resource_dir.h"	
#include "raylib.h"
#include "Core/constants.h"
#include "Entities/Donkey.h"
#include "Entities/Lady.h"
#include <iostream>
using namespace std;

Music level2Music = { 0 };
Music deathMusic2 = { 0 };
bool  isDead2 = false;

// DK animación level2
Rectangle dk2Frames[3] = {
    {  1.0f, 2.0f, 40.0f, 32.0f }, // frame 1 - idle
    { 85.0f, 2.0f, 46.0f, 32.0f }, // frame 2
    {134.0f, 2.0f, 46.0f, 32.0f }  // frame 3
};
int   dk2FrameIdx = 0;
float dk2FrameTimer = 0.0f;
float dk2FrameInterval = 0.3f;
bool  dk2Forward = true; // true = 1->2->3, false = 3->2->1
float dk2PauseTimer = 0.0f;
float dk2PauseDuration = 1.0f; // segundos de pausa entre ciclos
bool  dk2IsPaused = false;


Rectangle ladyFrames[6] = {
    {  1.0f, 1.0f, 14.0f, 22.0f }, 
    { 17.0f, 1.0f, 15.0f, 22.0f }, 
    {  1.0f, 1.0f, 14.0f, 22.0f }, 
    { 17.0f, 1.0f, 15.0f, 22.0f }, 
    {  1.0f, 1.0f, 14.0f, 22.0f }, 
    { 17.0f, 1.0f, 15.0f, 22.0f }, 
};
int   ladyFrameIdx = 0;
float ladyFrameTimer = 0.0f;
float ladyFrameInterval = 0.08f; // cambio rápido entre frames
float ladyPauseTimer = 0.0f;
float ladyPauseDuration = 3.0f;  // pausa larga después del ciclo
bool  ladyIsPaused = false;

void runLevel2() {

    // Like start() function from unity

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        //Mario Setup
        Mario.Setup();

        donkey.Setup();
        lady.Setup();

        dk2FrameIdx = 0;
        dk2FrameTimer = 0.0f;
        dk2Forward = true;

        // Posiciona Lady encima de Base_5
        lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f, (float)Base_5_YPos - 22.0f };

        // Posiciona DK justo debajo de Lady
        donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos - 32.0f };

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
    
    dk2FrameTimer += GetFrameTime();

    if (dk2IsPaused) {
        dk2PauseTimer += GetFrameTime();
        if (dk2PauseTimer >= dk2PauseDuration) {
            dk2PauseTimer = 0.0f;
            dk2IsPaused = false;
        }
    }
    else if (dk2FrameTimer >= dk2FrameInterval) {
        dk2FrameTimer = 0.0f; // <-- también corrige el bug de 3.0f aquí
        if (dk2Forward) {
            dk2FrameIdx++;
            if (dk2FrameIdx >= 3) {
                dk2FrameIdx = 1;
                dk2Forward = false;
                dk2IsPaused = true; // pausa al llegar al final
            }
        }
        else {
            dk2FrameIdx--;
            if (dk2FrameIdx <= 0) {
                dk2FrameIdx = 0;
                dk2Forward = true;
                dk2IsPaused = true; // pausa al volver al inicio
            }
        }
    }

    if (ladyIsPaused) {
        ladyPauseTimer += GetFrameTime();
        if (ladyPauseTimer >= ladyPauseDuration) {
            ladyPauseTimer = 0.0f;
            ladyIsPaused = false;
            ladyFrameIdx = 0; // vuelve al frame 1 al salir de pausa
        }
    }
    else {
        ladyFrameTimer += GetFrameTime();
        if (ladyFrameTimer >= ladyFrameInterval) {
            ladyFrameTimer = 0.0f;
            ladyFrameIdx++;
            if (ladyFrameIdx >= 6) {
                ladyFrameIdx = 0;
                ladyIsPaused = true; // pausa tras completar el ciclo
            }
        }
    }

    /*---GROUND COLLISIONS---*/

    //Mario's Ground Collisions divided by Y levels (One for each ramp)
    Level2RampCollisions(Mario);
    Level2LadderCollisions(Mario);
    Level2CheckButtons(Mario);

    
    
    /*---TEXTURE DRAW---*/
    Level2LadderDraw();
    Level2RampDraw();
    Level2ButtonsDraw();

    lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f,  (float)Base_5_YPos - 14.0f };
    donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 17.0f };


    DrawTextureRec(donkey.Texture, dk2Frames[dk2FrameIdx], donkey.Position, WHITE);
    DrawTextureRec(lady.Texture, ladyFrames[ladyFrameIdx], lady.Position, WHITE);
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
        donkey.Unload();
        lady.Unload();

        Scene_Init = false; // reset initialization boolean.
    }
}