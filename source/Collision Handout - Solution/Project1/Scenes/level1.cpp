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
#include "Entities/objects.h"

using namespace std;

Music level1Music = { 0 };
Sound deathSound2 = { 0 };
bool  isDead = false;
Sound jumpBarrelSound = { 0 };
Sound stageClearedSound = { 0 };
float deathTimer = 0.0f;
bool  isDeathSequence = false;
bool Hitboxes_On = false;

Sound HammerSound1 = { 0 };
Interactable Hammer1, Hammer2;

void runLevel1() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        Mario.Setup();
        donkey.Setup();
        lady.Setup();

        HammerSound1 = LoadSound("Audio/Bonus.wav");

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
        donkey.Position = { 21.0f, 47.0f };
        Hammer1.SetObject(16, 90, Hammer);
        Hammer2.SetObject(165, 182, Hammer);
        
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
        Level1LadderDraw();
        Level1RampDraw();
        donkey.Draw();
        lady.Draw();
        barrelSpawner.Draw();
        Hammer1.DrawObject();
        Hammer2.DrawObject();
        Level1EntitiesRoutine();
        UpdateBonus();
        Hammer1.CheckInteraction(Mario);
        Hammer2.CheckInteraction(Mario);

        if (GetHammerTime()) {
            if (Mario.CheckHammerHitbox(Fire1) && Fire1.has_Spawned) {
                Fire1.has_Spawned = false;
                PlaySound(HammerSound1);
            }
            if (Mario.CheckHammerHitbox(Fire2) && Fire2.has_Spawned) {
                Fire2.has_Spawned = false;
                PlaySound(HammerSound1);
            }
        }

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
            if (GetHammerTime()) {
                if (Mario.CheckHammerHitbox(b)) {
                    b.has_Spawned = false;
                    PlaySound(HammerSound1);
                }
            }

            if (diffX < 16 && diffY > -20 && diffY < 0 && Mario.isJumping) {
                PlaySound(jumpBarrelSound);
                AddPoints(100);
                ShowScorePopup(Mario.Position);
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

        if (Mario.isAlive && Mario.marioVelocity.y > 7.0f) {
            StopMusicStream(level1Music);
            PlaySound(deathSound2);
            isDeathSequence = true;
            deathTimer = 0.0f;
            Mario.die();
            RemoveLife();
            ResetLevel1Entities();
          }
        if (Mario.isAlive && CheckCollisionRecs(Mario.getHitbox(), donkey.hitbox)) {
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
    else {

    // siempre dibuja
    Level1LadderDraw();
    Level1RampDraw();
    donkey.Draw();
    lady.Draw();
    barrelSpawner.Draw();
    }
    if (IsKeyPressed(KEY_H)) {
        Hitboxes_On = !Hitboxes_On;
    }

    if (Hitboxes_On) {
        DrawLevel1Colliders();
        Hammer1.DrawCollider();
        DrawMarioCollider();
    }

    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    UpdateDrawScorePopup();
    SetCooldown();
   
    Level1CheckWinCondition(Mario);
    
    if (GetCurrentScene() != LEVEL1) {
        /*UnloadTexture(Mario.Texture);
        Truss::UnloadSharedTexture();
        Ladder::UnloadSharedTexture();
        UnloadLevel1Entities();*/
        UnloadSound(HammerSound1);
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
        Scene_Init = false;
        barrelSpawner.Init();
        AddLevel();
    }
}