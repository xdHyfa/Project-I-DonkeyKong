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
#include "Entities/EntityCollision.hpp"
#include "Core/UI.h"
#include "Entities/objects.h"
#include <iostream>
using namespace std;

Music level2Music = { 0 };
Music deathMusic2 = { 0 };
Sound deathSound3 = { 0 };
bool  isDead2 = false;
float deathTimer2 = 0.0f;
bool  isDeathSequence2 = false;

bool isHammer2Playing = false;
Music Hammer_Music2 = { 0 };
Sound HammerSound = { 0 };
Interactable Hammer3, Hammer4, Umbrella1, Purse1, Hat1;

// DK animación level2
Rectangle dk2Frames[3] = {
    {  1.0f, 2.0f, 40.0f, 32.0f }, // frame 1 - idle
    { 85.0f, 2.0f, 46.0f, 32.0f }, // frame 2
    {134.0f, 2.0f, 46.0f, 32.0f }  // frame 3
};
int   dk2FrameIdx = 0;
float dk2FrameTimer = 0.0f;
float dk2FrameInterval = 0.3f;
bool  dk2Forward = true;
float dk2PauseTimer = 0.0f;
float dk2PauseDuration = 1.0f;
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
float ladyFrameInterval = 0.08f;
float ladyPauseTimer = 0.0f;
float ladyPauseDuration = 3.0f;
bool  ladyIsPaused = false;

void runLevel2() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        Mario.Setup();
        donkey.Setup();
        lady.Setup();

        HammerSound = LoadSound("Audio/Bonus.wav");
        StopHammerTime();
        dk2FrameIdx = 0;
        dk2FrameTimer = 0.0f;
        dk2Forward = true;

        lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f, (float)Base_5_YPos - 22.0f };
        donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos - 32.0f };
       

        Level2RampSetter();
        Level2LadderSetter();
        Level2ButtonSetter();

        Hammer_Music2 = LoadMusicStream("Audio/Hammer-Time_.wav");
        Hammer_Music2.looping = true;
        SetStartTime2();
        Scene_Init = true;

        level2Music = LoadMusicStream("Audio/Stage-2-Springboard2.wav");
        deathMusic2 = LoadMusicStream("Audio/Dead.wav");
        deathSound3 = LoadSound("Audio/Dead.wav");
        level2Music.looping = true;
        PlayMusicStream(level2Music);
        isDead2 = false;
        Hammer3.SetObject(8, 132, Hammer);
        Hammer4.SetObject(106, 90, Hammer);
        Purse1.SetObject(120, 238, Purse);
        Hat1.SetObject(196, 197, Hat);
        Umbrella1.SetObject(28, 65, Umbrella);
        donkey.hitbox = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 16.0f, 45.0f, 32.0f };
    }

    /* UPDATE STARTS HERE */

    if (isDeathSequence2) {
        deathTimer2 += GetFrameTime();

        // PRIMERO unload check, ANTES de dibujar nada
        if (deathTimer2 >= 5.0f) {
            UnloadTexture(Mario.Texture);
            Truss::UnloadSharedTexture();
            Ladder::UnloadSharedTexture();
            UnloadLevel2Entities();
            UnloadButtonTexture();
            ResetLevel2Entities();
            ResetButtons();
            UnloadMusicStream(level2Music);
            UnloadMusicStream(deathMusic2);
            UnloadSound(deathSound3);
            donkey.Unload();
            lady.Unload();
            Mario.isAlive = true;
            Scene_Init = false;
            isDeathSequence2 = false;
            deathTimer2 = 0.0f;
            CheckLives();
            ResetBonus();
            return;
        }

        // Solo dibuja si NO se hizo unload
        Mario.Movement();
        donkey.Update();
        lady.Update();
        Level2LadderDraw();
        Level2RampDraw();
        Level2ButtonsDraw();
        DrawTextureRec(donkey.Texture, dk2Frames[dk2FrameIdx], donkey.Position, WHITE);
        DrawTextureRec(lady.Texture, ladyFrames[ladyFrameIdx], lady.Position, WHITE);
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
        return;
    }

    if (GetIsKilling()) {
        Level2LadderDraw();
        Level2RampDraw();
        Level2ButtonsDraw();
        lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f,  (float)Base_5_YPos - 14.0f };
        donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 17.0f };

        DrawTextureRec(donkey.Texture, dk2Frames[dk2FrameIdx], donkey.Position, WHITE);
        DrawTextureRec(lady.Texture, ladyFrames[ladyFrameIdx], lady.Position, WHITE);
        Hammer3.DrawObject();
        Hammer4.DrawObject();
        Hammer3.DrawObject();
        Hammer4.DrawObject();
        Purse1.DrawObject();
        Umbrella1.DrawObject();
        Hat1.DrawObject();
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
        PlayEntityDeath();
        UpdateMusicStream(Hammer_Music2);
        return;
    }


    Mario.Movement();
    UpdateBonus();

    Level2LadderDraw();
    Level2RampDraw();
    Level2ButtonsDraw();

    lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f,  (float)Base_5_YPos - 14.0f };
    donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 17.0f };

    DrawTextureRec(donkey.Texture, dk2Frames[dk2FrameIdx], donkey.Position, WHITE);
    DrawTextureRec(lady.Texture, ladyFrames[ladyFrameIdx], lady.Position, WHITE);
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    Hammer3.DrawObject();
    Hammer4.DrawObject();
    Purse1.DrawObject();
    Umbrella1.DrawObject();
    Hat1.DrawObject();

    if (GetHammerTime()) {
        if (!isHammer2Playing) PlayMusicStream(Hammer_Music2), isHammer2Playing = true;
        else {
            UpdateMusicStream(Hammer_Music2);
        }
    
    if (Mario.CheckHammerHitbox(Fire3) && Fire3.has_Spawned) { Fire3.has_Spawned = false, PlaySound(HammerSound), AddPoints(300), ShowScorePopup(Mario.Position, 300), StartEntityDeath(Fire3); return; }
    if (Mario.CheckHammerHitbox(Fire4) && Fire4.has_Spawned) { Fire4.has_Spawned = false, PlaySound(HammerSound), AddPoints(300), ShowScorePopup(Mario.Position, 300), StartEntityDeath(Fire4); return; }
    if (Mario.CheckHammerHitbox(Fire5) && Fire5.has_Spawned) { Fire5.has_Spawned = false, PlaySound(HammerSound), AddPoints(300), ShowScorePopup(Mario.Position, 300), StartEntityDeath(Fire5); return; }
    if (Mario.CheckHammerHitbox(Fire6) && Fire6.has_Spawned) { Fire6.has_Spawned = false, PlaySound(HammerSound), AddPoints(300), ShowScorePopup(Mario.Position, 300), StartEntityDeath(Fire6); return; }
    if (Mario.CheckHammerHitbox(Fire7) && Fire7.has_Spawned) { Fire7.has_Spawned = false, PlaySound(HammerSound), AddPoints(300), ShowScorePopup(Mario.Position, 300), StartEntityDeath(Fire7); return; }
    }
    else {
        isHammer2Playing = false;
    }
    if (!GetHammerTime()) UpdateMusicStream(level2Music);
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
        dk2FrameTimer = 0.0f;
        if (dk2Forward) {
            dk2FrameIdx++;
            if (dk2FrameIdx >= 3) {
                dk2FrameIdx = 1;
                dk2Forward = false;
                dk2IsPaused = true;
            }
        }
        else {
            dk2FrameIdx--;
            if (dk2FrameIdx <= 0) {
                dk2FrameIdx = 0;
                dk2Forward = true;
                dk2IsPaused = true;
            }
        }
    }

    if (ladyIsPaused) {
        ladyPauseTimer += GetFrameTime();
        if (ladyPauseTimer >= ladyPauseDuration) {
            ladyPauseTimer = 0.0f;
            ladyIsPaused = false;
            ladyFrameIdx = 0;
        }
    }
    else {
        ladyFrameTimer += GetFrameTime();
        if (ladyFrameTimer >= ladyFrameInterval) {
            ladyFrameTimer = 0.0f;
            ladyFrameIdx++;
            if (ladyFrameIdx >= 6) {
                ladyFrameIdx = 0;
                ladyIsPaused = true;
            }
        }
    }

    SetCooldown();
    Level2RampCollisions(Mario);
    Level2LadderCollisions(Mario);
    Level2CheckButtons(Mario);

    /*---ENTITY COLLISIONS---*/
    if (Mario.isAlive && (Fire3.has_Spawned && EntityCollision(Mario, Fire3) || Fire4.has_Spawned && EntityCollision(Mario, Fire4) || Fire5.has_Spawned && EntityCollision(Mario, Fire5) || Fire6.has_Spawned && EntityCollision(Mario, Fire6) || Fire7.has_Spawned && EntityCollision(Mario, Fire7)))
    {
        StopMusicStream(level2Music);
        PlaySound(deathSound3);
        isDeathSequence2 = true;
        deathTimer2 = 0.0f;
        Mario.die();
        RemoveLife();
        ResetLevel2Entities();
    }

    if (Mario.isAlive && CheckCollisionRecs(Mario.getHitbox(), donkey.hitbox)) {
        StopMusicStream(level2Music);
        PlaySound(deathSound3);
        isDeathSequence2 = true;
        deathTimer2 = 0.0f;
        Mario.die();
        RemoveLife();
        ResetLevel2Entities();
    }

    if (Mario.isAlive && Mario.marioVelocity.y > 7.0f) {
        StopMusicStream(level2Music);
        PlaySound(deathSound3);
        isDeathSequence2 = true;
        deathTimer2 = 0.0f;
        Mario.die();
        RemoveLife();
        ResetLevel1Entities();
        Mario.marioVelocity.y = 0;
    }

    Level2LadderDraw();
    Level2RampDraw();
    Level2ButtonsDraw();

    lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f,  (float)Base_5_YPos - 14.0f };
    donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 17.0f };

    DrawTextureRec(donkey.Texture, dk2Frames[dk2FrameIdx], donkey.Position, WHITE);
    DrawTextureRec(lady.Texture, ladyFrames[ladyFrameIdx], lady.Position, WHITE);
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

    Level2EntitiesRoutine();
    Hammer3.DrawObject();
    Hammer4.DrawObject();
    Purse1.DrawObject();
    Umbrella1.DrawObject();
    Hat1.DrawObject();

    Hammer3.CheckInteraction(Mario);
    Hammer4.CheckInteraction(Mario);
    Purse1.CheckInteraction(Mario);
    Umbrella1.CheckInteraction(Mario);
    Hat1.CheckInteraction(Mario);

    if (IsKeyPressed(KEY_ONE)) {
        ChangeScene();
    }

    CheckWinCondition();

    if (GetCurrentScene() != LEVEL2) {
        /*UnloadTexture(Mario.Texture);
        UnloadLevel2Entities();
        Truss::UnloadSharedTexture();
        Ladder::UnloadSharedTexture();
        UnloadButtonTexture();*/
        isHammer2Playing = false;
        UnloadMusicStream(Hammer_Music2);
        AddLevel();
        AddBonus();
        UnloadSound(HammerSound);
        ResetLevel2Entities();
        ResetButtons();
        UnloadMusicStream(level2Music);
        UnloadMusicStream(deathMusic2);
        UnloadSound(deathSound3);
        donkey.Unload();
        lady.Unload();
        Scene_Init = false;
        return;
    }
}