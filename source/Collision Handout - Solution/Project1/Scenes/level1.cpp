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
#include "Entities/Star.h"

using namespace std;

Music level1Music = { 0 };
Sound deathSound2 = { 0 };
bool  isDead = false;
Sound jumpBarrelSound = { 0 };
Sound stageClearedSound = { 0 };
float deathTimer = 0.0f;
bool  isDeathSequence = false;
bool Hitboxes_On = false;
Music Hammer_Music = { 0 };
bool isHammerPlaying = false;
bool winSoundPlayed = false;
Sound HammerSound1 = { 0 };
Interactable Hammer1, Hammer2, Star1;

void runLevel1() {



    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        Luigi.Texture = LoadTexture("sprites/luigi.png");
        Mario.Setup();
        Mario.PlayerNum = 1;
        Luigi.Setup();
        Luigi.PlayerNum = 2;
        Luigi.Position = { 96, SCREEN_HEIGHT - (float)Luigi.SpriteSize - 17 };
        donkey.Setup();
        lady.Setup();
        StopHammerTime();
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
        winSoundPlayed = false;
        donkey.Position = { 21.0f, 47.0f };
        donkey.hitbox = { 21.0f, 47.0f, 46.0f, 32.0f }; //RESET DE LA HITBOX?
        Hammer1.SetObject(16, 90, Hammer);
        Hammer2.SetObject(165, 182, Hammer);
        Star1.SetObject(16, 150, StarItem);
        Hammer_Music = LoadMusicStream("Audio/Hammer-Time_.wav");
        Hammer_Music.looping = true;

    }

    if (GetPause()) {
        Level1LadderDraw();
        Level1RampDraw();
        donkey.Draw();
        lady.Draw();
        barrelSpawner.Draw();
        Hammer1.DrawObject();
        Hammer2.DrawObject();
        Star1.DrawObject();
        DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
        if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
        PlayEntityDeath();
        return;
    }




    if (isDeathSequence) {
        deathTimer += GetFrameTime();
        if (!Mario.isAlive) Mario.DeathSequence();
        if (!Luigi.isAlive) Luigi.DeathSequence();


        if (deathTimer >= 5.0f) {
            bool bothDead = !Mario.isAlive && !Luigi.isAlive;
            if (GetTwoPlayers() && !bothDead) ResumeMusicStream(level1Music);
            if (!Mario.isAlive && !GetTwoPlayers() || !Mario.isAlive && !Luigi.isAlive) {
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
                Luigi.isAlive = true;
                Scene_Init = false;
                isDeathSequence = false;
                Star1.ResetObject();
                StopStarTime(1);
                StopStarTime(2);
                RemoveLife();
                CheckLives();
                ResetBonus();
                barrelSpawner.Init();
                return;
            }

            // One player still alive � resume
            isDeathSequence = false;
            deathTimer = 0.0f;
        }

        donkey.Update();
        donkey.Draw();
        lady.Update();
        lady.Draw();
        Level1LadderDraw();
        Level1RampDraw();
        barrelSpawner.Draw();
        DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
        if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
        return;
    }

    /* UPDATE STARTS HERE */

    if (GetIsKilling()) {
        Level1LadderDraw();
        Level1RampDraw();
        donkey.Draw();
        lady.Draw();
        barrelSpawner.Draw();
        Hammer1.DrawObject();
        Hammer2.DrawObject();
        Star1.DrawObject();
        DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
        if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
        PlayEntityDeath();
        UpdateMusicStream(Hammer_Music);
        return;
    }

    if (Mario.isAlive) Level1CheckWinCondition(Mario);
    if (GetTwoPlayers() && Luigi.isAlive) Level1CheckWinCondition(Luigi);

    if (!winTriggered) {
        CheckTwoPlayers();
        Mario.Movement();
        if (GetTwoPlayers()) Luigi.Movement();
        donkey.Update();
        lady.Update();
        if (!GetHammerTime()) UpdateMusicStream(level1Music);
        Level1RampCollisions(Mario);
        if (GetTwoPlayers()) Level1RampCollisions(Luigi);
        Level1LadderCollisions(Mario);
        if (GetTwoPlayers()) Level1LadderCollisions(Luigi);
        barrelSpawner.Update();
        Level1LadderDraw();
        Level1RampDraw();
        donkey.Draw();
        lady.Draw();
        barrelSpawner.Draw();
        Hammer1.DrawObject();
        Hammer2.DrawObject();
        Star1.DrawObject();
        Level1EntitiesRoutine();
        UpdateBonus();
        UpdateStar(Mario);
        if (GetTwoPlayers()) UpdateStar(Luigi);
        Hammer1.CheckInteraction(Mario, 1);
        Hammer2.CheckInteraction(Mario, 1);
        if (GetTwoPlayers()) Hammer1.CheckInteraction(Luigi, 2);
        if (GetTwoPlayers()) Hammer2.CheckInteraction(Luigi, 2);
        Star1.CheckInteraction(Mario, 1);
        if (GetTwoPlayers()) Star1.CheckInteraction(Luigi, 2);

        if (GetHammerTime()) {
            if (!isHammerPlaying) PlayMusicStream(Hammer_Music), isHammerPlaying = true;
            else {
                UpdateMusicStream(Hammer_Music);
            }
            if (Mario.CheckHammerHitbox(Fire1) && Fire1.has_Spawned) {
                AddPoints(300);
                ShowScorePopup(Mario.Position, 300);
                Fire1.has_Spawned = false;
                PlaySound(HammerSound1);
                StartEntityDeath(Fire1);
                return;
            }
            if (Mario.CheckHammerHitbox(Fire2) && Fire2.has_Spawned) {
                Fire2.has_Spawned = false;
                PlaySound(HammerSound1);
                AddPoints(300);
                ShowScorePopup(Mario.Position, 300);
                StartEntityDeath(Fire2);
                return;
            }
            if (GetTwoPlayers() && Luigi.CheckHammerHitbox(Fire1) && Fire1.has_Spawned) {
                AddPoints(300);
                ShowScorePopup(Luigi.Position, 300);
                Fire1.has_Spawned = false;
                PlaySound(HammerSound1);
                StartEntityDeath(Fire1);
                return;
            }
            if (GetTwoPlayers() && Luigi.CheckHammerHitbox(Fire2) && Fire2.has_Spawned) {
                Fire2.has_Spawned = false;
                PlaySound(HammerSound1);
                AddPoints(300);
                ShowScorePopup(Luigi.Position, 300);
                StartEntityDeath(Fire2);
                return;
            }
        }
        else {
            isHammerPlaying = false;
        }

        for (Barrel& b : barrelSpawner.barrels) {
            if (!b.has_Spawned || !Mario.isAlive && !GetTwoPlayers() || (!Mario.isAlive && !Luigi.isAlive && GetTwoPlayers())) continue;

            float diffX = abs(Mario.Position.x - b.Position.x);
            float diffY = Mario.Position.y - b.Position.y;

            float Luigi_diffX = abs(Luigi.Position.x - b.Position.x);
            float Luigi_diffY = Luigi.Position.y - b.Position.y;

            if (Mario.isAlive && EntityCollision(Mario, b)) {
                if (Mario.StarActive) {
                    b.has_Spawned = false;
                    AddPoints(300);
                    ShowScorePopup(Mario.Position, 300);
                    PlaySound(HammerSound1);
                    StartEntityDeath(b);
                    return;
                }
                StopHammerTime();
                isHammerPlaying = false;
                StopMusicStream(Hammer_Music);
                PauseMusicStream(level1Music);
                PlaySound(deathSound2);
                isDeathSequence = true;
                deathTimer = 0.0f;
                Mario.die();
            }
            if (GetTwoPlayers() && Luigi.isAlive && EntityCollision(Luigi, b)) {
                if (Luigi.StarActive) {
                    b.has_Spawned = false;
                    AddPoints(300);
                    ShowScorePopup(Luigi.Position, 300);
                    PlaySound(HammerSound1);
                    StartEntityDeath(b);
                    return;
                }
                StopHammerTime();
                isHammerPlaying = false;
                StopMusicStream(Hammer_Music);
                PauseMusicStream(level1Music);
                PlaySound(deathSound2);
                isDeathSequence = true;
                deathTimer = 0.0f;
                Luigi.die();
            }
            if (isDeathSequence) { ResetLevel1Entities(); break; }
            if (GetHammerTime()) {
                if (Mario.CheckHammerHitbox(b)) {
                    b.has_Spawned = false;
                    PlaySound(HammerSound1);
                    AddPoints(300);
                    ShowScorePopup(Mario.Position, 300);
                    StartEntityDeath(b);
                    return;
                }
                if (GetTwoPlayers() && Luigi.CheckHammerHitbox(b)) {
                    b.has_Spawned = false;
                    PlaySound(HammerSound1);
                    AddPoints(300);
                    ShowScorePopup(Luigi.Position, 300);
                    StartEntityDeath(b);
                    return;
                }
            }

            if (diffX < 16 && diffY > -20 && diffY < 0 && Mario.isJumping) {
                PlaySound(jumpBarrelSound);
                AddPoints(100);
                ShowScorePopup(Mario.Position, 100);
            }
            if (GetTwoPlayers() && Luigi_diffX < 16 && Luigi_diffY > -20 && Luigi_diffY < 0 && Luigi.isJumping) {
                PlaySound(jumpBarrelSound);
                AddPoints(100);
                ShowScorePopup(Luigi.Position, 100);
            }
        }

        if (Mario.isAlive && (Fire1.has_Spawned && EntityCollision(Mario, Fire1) || Fire2.has_Spawned && EntityCollision(Mario, Fire2))) {
            if (Mario.StarActive) {
                if (Fire1.has_Spawned && EntityCollision(Mario, Fire1)) { Fire1.has_Spawned = false; AddPoints(300); ShowScorePopup(Mario.Position, 300); PlaySound(HammerSound1); StartEntityDeath(Fire1); return; }
                if (Fire2.has_Spawned && EntityCollision(Mario, Fire2)) { Fire2.has_Spawned = false; AddPoints(300); ShowScorePopup(Mario.Position, 300); PlaySound(HammerSound1); StartEntityDeath(Fire2); return; }
            }
            else {
                StopHammerTime();
                isHammerPlaying = false;
                StopMusicStream(Hammer_Music);
                PauseMusicStream(level1Music);
                PlaySound(deathSound2);
                isDeathSequence = true;
                deathTimer = 0.0f;
                Mario.die();
                ResetLevel1Entities();
            }
        }
        if (GetTwoPlayers() && Luigi.isAlive && (Fire1.has_Spawned && EntityCollision(Luigi, Fire1) || Fire2.has_Spawned && EntityCollision(Luigi, Fire2))) {
            if (Luigi.StarActive) {
                if (Fire1.has_Spawned && EntityCollision(Luigi, Fire1)) { Fire1.has_Spawned = false; AddPoints(300); ShowScorePopup(Luigi.Position, 300); PlaySound(HammerSound1); StartEntityDeath(Fire1); return; }
                if (Fire2.has_Spawned && EntityCollision(Luigi, Fire2)) { Fire2.has_Spawned = false; AddPoints(300); ShowScorePopup(Luigi.Position, 300); PlaySound(HammerSound1); StartEntityDeath(Fire2); return; }
            }
            else {
                StopHammerTime();
                isHammerPlaying = false;
                StopMusicStream(Hammer_Music);
                PauseMusicStream(level1Music);
                PlaySound(deathSound2);
                isDeathSequence = true;
                deathTimer = 0.0f;
                Luigi.die();
                ResetLevel1Entities();
            }
        }

        if (Mario.isAlive && Mario.marioVelocity.y > 7.0f && !Mario.StarActive) {
            StopHammerTime();
            isHammerPlaying = false;
            StopMusicStream(Hammer_Music);
            PauseMusicStream(level1Music);
            PlaySound(deathSound2);
            isDeathSequence = true;
            deathTimer = 0.0f;
            Mario.die();
            ResetLevel1Entities();
        }
        if (Mario.isAlive && CheckCollisionRecs(Mario.getHitbox(), donkey.hitbox)) {
            StopHammerTime();
            isHammerPlaying = false;
            StopMusicStream(Hammer_Music);
            PauseMusicStream(level1Music);
            PlaySound(deathSound2);
            isDeathSequence = true;
            deathTimer = 0.0f;
            Mario.die();
            ResetLevel1Entities();
        }

        if (Luigi.isAlive && Luigi.marioVelocity.y > 7.0f && !Luigi.StarActive)
        {
            StopHammerTime();
            isHammerPlaying = false;
            StopMusicStream(Hammer_Music);
            PauseMusicStream(level1Music);
            PlaySound(deathSound2);
            isDeathSequence = true;
            deathTimer = 0.0f;
            Luigi.die();
            ResetLevel1Entities();
        }

        if (GetTwoPlayers() && Luigi.isAlive && CheckCollisionRecs(Luigi.getHitbox(), donkey.hitbox)) {
            StopHammerTime();
            isHammerPlaying = false;
            StopMusicStream(Hammer_Music);
            PauseMusicStream(level1Music);
            PlaySound(deathSound2);
            isDeathSequence = true;
            deathTimer = 0.0f;
            Luigi.die();
            ResetLevel1Entities();
        }


        if (IsKeyPressed(KEY_TWO)) ChangeScene();
    }
    else {
        // Win: dibujar y cambiar escena directamente sin reproducir sonido
        Level1LadderDraw();
        Level1RampDraw();
        donkey.Draw();
        lady.Draw();
        barrelSpawner.Draw();
        ChangeScene();
    }
    if (IsKeyPressed(KEY_H)) {
        Hitboxes_On = !Hitboxes_On;
    }

    if (Hitboxes_On) {
        DrawLevel1Colliders();
        Hammer1.DrawCollider();
        DrawMarioCollider();
    }

    DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, Mario.GetStarTint());
    if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, Luigi.GetStarTint());
    UpdateDrawScorePopup();
    SetCooldown();


    if (GetCurrentScene() != LEVEL1) {
        /*UnloadTexture(Mario.Texture);
        Truss::UnloadSharedTexture();
        Ladder::UnloadSharedTexture();
        UnloadLevel1Entities();*/
        Mario.isAlive = true;
        Luigi.isAlive = true;
        UnloadSound(HammerSound1);
        isHammerPlaying = false;
        UnloadMusicStream(Hammer_Music);
        barrelSpawner.Shutdown();
        barrelSpawner.Reset();
        UnloadMusicStream(level1Music);
        UnloadSound(deathSound2);
        UnloadSound(jumpBarrelSound);
        donkey.Unload();
        lady.Unload();
        UnloadSound(stageClearedSound);
        ResetLevel1Entities();
        Star1.ResetObject();
        StopStarTime(1);
        StopStarTime(2);
        AddBonus();
        Scene_Init = false;
        barrelSpawner.Init();
        AddLevel();
    }
}

bool CheckLevel1Players() {
    if (isDeathSequence) return false;
    if (Mario.isAlive || Luigi.isAlive) {
        return true;
    }
    else return false;
}