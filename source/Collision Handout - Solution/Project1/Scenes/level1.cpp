#include "Scenes/level1.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include "Entities/Level1Entities.h"
#include "Entities/Barrel.h"
#include "Maps/Level1Map.h"
#include "include/resource_dir.h"	
#include "raylib.h"
#include "Core/constants.h"
#include "Entities/EntityCollision.hpp"
#include "Entities/Barrel_Spawner.h"
#include <iostream>
#include "Entities/Donkey.h"
#include "Entities/Lady.h"
#include "Core/UI.h"

using namespace std;

Music level1Music = { 0 };
Sound deathSound2 = { 0 };
bool  isDead = false;
Sound jumpBarrelSound = { 0 };
Sound stageClearedSound = { 0 };
float deathTimer = 0.0f;
bool  isDeathSequence = false;

void runLevel1() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        Mario.Setup();
        donkey.Setup();
        lady.Setup();
        Level1RampSetter();
        Level1LadderSetter();
        barrelSpawner.Init();
        SetStartTime();
        Scene_Init = true;
        level1Music = LoadMusicStream("Audio/Stage-1-Bricks3.wav");
        deathSound2 = LoadSound("Audio/Dead.wav");
        level1Music.looping = true;
        PlayMusicStream(level1Music);
        isDead = false;
        deathTimer = 0.0f;
        isDeathSequence = false;
        jumpBarrelSound = LoadSound("Audio/Bonus.wav");
        stageClearedSound = LoadSound("Audio/Stage-Cleared-1.wav");
        winTriggered = false;
    }

    if (isDeathSequence) {
        deathTimer += GetFrameTime();

        Mario.Movement();
        donkey.Update();
        donkey.Draw();
        lady.Update();
        lady.Draw();

        if (deathTimer >= 5.0f) {
            UnloadTexture(Mario.Texture);
            Truss::UnloadSharedTexture();
            Ladder::UnloadSharedTexture();
            UnloadLevel1Entities();
            barrelSpawner.Shutdown();
            barrelSpawner.Reset();
            UnloadMusicStream(level1Music);
            UnloadSound(deathSound2);
            ResetLevel1Entities();
            Mario.isAlive = true;
            Scene_Init = false;
            isDeathSequence = false;
            CheckLives();
            ResetBonus();
            barrelSpawner.Init();
             return;
        }

        Level1LadderDraw();
        Level1RampDraw();
        barrelSpawner.Draw();
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
        return;
    }

    /* UPDATE STARTS HERE */



    if (!winTriggered) {
        Mario.Movement();
        donkey.Update();
        lady.Update();
        UpdateMusicStream(level1Music);
        Level1RampCollisions(Mario);
        Level1LadderCollisions(Mario);
        barrelSpawner.Update();
        Level1EntitiesRoutine();
        UpdateBonus();

        for (Barrel& b : barrelSpawner.barrels) {
            if (!b.has_Spawned || !Mario.isAlive) continue;

            float diffX = abs(Mario.Position.x - b.Position.x);
            float diffY = Mario.Position.y - b.Position.y;

            if (EntityCollision(Mario, b)) {
                StopMusicStream(level1Music);
                PlaySound(deathSound2);
                isDeathSequence = true;
                deathTimer = 0.0f;
                Mario.die();
                RemoveLife();
                ResetLevel1Entities();
                break;
            }

            if (diffX < 16 && diffY > -20 && diffY < 0 && Mario.isJumping) {
                PlaySound(jumpBarrelSound);
                AddPoints(100);
            }
        }

        if (Mario.isAlive && (Fire1.has_Spawned && EntityCollision(Mario, Fire1) || Fire2.has_Spawned && EntityCollision(Mario, Fire2))) {
            StopMusicStream(level1Music);
            PlaySound(deathSound2);
            isDeathSequence = true;
            deathTimer = 0.0f;
            Mario.die();
            RemoveLife();
            ResetLevel1Entities();
        }

        if (IsKeyPressed(KEY_TWO)) ChangeScene();
    }

    // siempre dibuja
    Level1LadderDraw();
    Level1RampDraw();
    donkey.Draw();
    lady.Draw();
    barrelSpawner.Draw();
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    SetCooldown();
   
    Level1CheckWinCondition(Mario);

    if (GetCurrentScene() != LEVEL1) {
        UnloadTexture(Mario.Texture);
        Truss::UnloadSharedTexture();
        Ladder::UnloadSharedTexture();
        UnloadLevel1Entities();
        barrelSpawner.Shutdown();
        barrelSpawner.Reset();
        UnloadMusicStream(level1Music);
        UnloadSound(deathSound2);
        UnloadSound(jumpBarrelSound);
        donkey.Unload();
        lady.Unload();
        UnloadSound(stageClearedSound);
        ResetLevel1Entities();
        AddBonus();
        AddLevel();
        Scene_Init = false;
        barrelSpawner.Init();
    }
}