#include "Scenes/level15.h"
#include "Entities/Player.h"
#include "Entities/Level15Entities.h"
#include "Scenes/scenes.h"
#include "Maps/Level15Map.h"
#include "include/resource_dir.h"
#include "raylib.h"
#include "Core/constants.h"
#include "Entities/EntityCollision.hpp"
#include "Entities/Lady.h"
#include "Core/UI.h"
#include <iostream>
using namespace std;

Music level15Music = { 0 };
Sound deathSound15 = { 0 };
Music Hammer15Music = { 0 };
Sound stageCleared15Sound = { 0 };
float deathTimer15 = 0.0f;
bool isDeathSequence15 = false;
bool Hitboxes15_On = false;
bool win15SoundPlayed = false;

void runLevel15() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        Luigi.Texture = LoadTexture("sprites/luigi.png");
        Mario.Setup();
        Mario.PlayerNum = 1;
        Mario.Position = { 80, SCREEN_HEIGHT - (float)Mario.SpriteSize - 17 };
        Luigi.Setup();
        Luigi.PlayerNum = 2;
        Luigi.Position = { 96, SCREEN_HEIGHT - (float)Luigi.SpriteSize - 17 };
        StopHammerTime();

        Level15RampSetter();
        Level15LadderSetter();
        SetStartTime15();
        Scene_Init = true;

        if (!Option4True) level15Music = LoadMusicStream("Audio/Stage-1-Bricks3.wav");
        else level15Music = LoadMusicStream("Audio/DK RAP.wav");
        level15Music.looping = true;
        PlayMusicStream(level15Music);

        deathSound15 = LoadSound("Audio/Dead.wav");
        stageCleared15Sound = LoadSound("Audio/Stage-Cleared-1.wav");
        Hammer15Music = LoadMusicStream("Audio/Hammer-Time_.wav");
        Hammer15Music.looping = true;
        if (Option2True) PlayMusicStream(Hammer15Music);
        win15Triggered = false;
        win15SoundPlayed = false;
        deathTimer15 = 0.0f;
        isDeathSequence15 = false;
        CheckTwoPlayers();
        if (Option2True) {
            StartHammerTime(1);
            if (GetTwoPlayers()) { StartHammerTime(2); }
        }
    }

    if (GetPause()) {
        Level15LadderDraw();
        Level15RampDraw();
        Level15TrussOverLadderDraw();
        DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
        if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
        PlayEntityDeath();
        return;
    }

    if (isDeathSequence15) {
        deathTimer15 += GetFrameTime();
        if (!Mario.isAlive) Mario.DeathSequence(), StopHammerTime();
        if (!Luigi.isAlive) Luigi.DeathSequence(), StopHammerTime();

        if (deathTimer15 >= 5.0f) {
            bool bothDead = !Mario.isAlive && !Luigi.isAlive;
            if (GetTwoPlayers() && !bothDead) ResumeMusicStream(level15Music);
            if (!Mario.isAlive && !GetTwoPlayers() || bothDead) {
                UnloadTexture(Mario.Texture);
                Truss::UnloadSharedTexture();
                Ladder::UnloadSharedTexture();
                UnloadLevel15Entities();
                UnloadMusicStream(level15Music);
                UnloadMusicStream(Hammer15Music);
                UnloadSound(deathSound15);
                UnloadSound(stageCleared15Sound);
                ResetLevel15Entities();
                Mario.isAlive = true;
                Luigi.isAlive = true;
                isDeathSequence15 = false;
                deathTimer15 = 0.0f;
                Scene_Init = false;
                RemoveLife();
                CheckLives();
                ResetBonus();
                return;
            }
            isDeathSequence15 = false;
            deathTimer15 = 0.0f;
        }

        Level15LadderDraw();
        Level15RampDraw();
        Level15TrussOverLadderDraw();
        DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
        if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
        return;
    }

    if (GetIsKilling()) {
        Level15LadderDraw();
        Level15RampDraw();
        Level15TrussOverLadderDraw();
        DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
        if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
        PlayEntityDeath();
        return;
    }


    if (!win15Triggered) {
        Mario.Movement();
        if (GetTwoPlayers()) Luigi.Movement();

        if (!GetHammerTime()) UpdateMusicStream(level15Music);
        else UpdateMusicStream(Hammer15Music);

        Level15RampCollisions(Mario);
        if (GetTwoPlayers()) Level15RampCollisions(Luigi);
        Level15LadderCollisions(Mario);
        if (GetTwoPlayers()) Level15LadderCollisions(Luigi);

        Level15LadderDraw();
        Level15RampDraw();
        Level15TrussOverLadderDraw();

        Level15EntitiesRoutine();
        if (GetHammerTime()) {
            if (CheckHammerKills(Mario)) return;
            if (GetTwoPlayers() && CheckHammerKills(Luigi)) return;
        }
        UpdateBonus();

        if (WinStarCollected15()) {
            StopMusicStream(level15Music);
            win15Triggered = true;
        }

        if (GoombaKilledPlayer(1)) {
            PauseMusicStream(level15Music);
            PlaySound(deathSound15);
            isDeathSequence15 = true;
            deathTimer15 = 0.0f;
            if (Mario.isAlive) Mario.die();
        }
        if (GetTwoPlayers() && GoombaKilledPlayer(2)) {
            PauseMusicStream(level15Music);
            PlaySound(deathSound15);
            isDeathSequence15 = true;
            deathTimer15 = 0.0f;
            if (Luigi.isAlive) Luigi.die();
        }

        if (BillBalaKilledPlayer(1)) {
            PauseMusicStream(level15Music);
            PlaySound(deathSound15);
            isDeathSequence15 = true;
            deathTimer15 = 0.0f;
            if (Mario.isAlive) Mario.die();
        }
        if (GetTwoPlayers() && BillBalaKilledPlayer(2)) {
            PauseMusicStream(level15Music);
            PlaySound(deathSound15);
            isDeathSequence15 = true;
            deathTimer15 = 0.0f;
            if (Luigi.isAlive) Luigi.die();
        }

        if (Mario.isAlive && Mario.marioVelocity.y > 7.0f) {
            PauseMusicStream(level15Music);
            PlaySound(deathSound15);
            isDeathSequence15 = true;
            deathTimer15 = 0.0f;
            Mario.die();
        }
        if (GetTwoPlayers() && Luigi.isAlive && Luigi.marioVelocity.y > 7.0f) {
            PauseMusicStream(level15Music);
            PlaySound(deathSound15);
            isDeathSequence15 = true;
            deathTimer15 = 0.0f;
            Luigi.die();
        }

        if (Option1True && IsKeyPressed(KEY_TWO)) ChangeScene();
    }
    else {
        Level15LadderDraw();
        Level15RampDraw();
        Level15TrussOverLadderDraw();
        if (!win15SoundPlayed) {
            StopMusicStream(level15Music);
            win15SoundPlayed = true;
        }
        if (win15SoundPlayed) {
            ChangeScene();
        }
    }

    if (Option1True && IsKeyPressed(KEY_H)) {
        Hitboxes15_On = !Hitboxes15_On;
    }
    if (Hitboxes15_On) {
        DrawLevel15Colliders();
        DrawMarioCollider();
    }

    DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, WHITE);
    if (GetTwoPlayers()) DrawTextureRec(Luigi.Texture, Luigi.frameRec, Luigi.Position, WHITE);
    UpdateDrawScorePopup();
    SetCooldown();

    if (GetCurrentScene() != LEVEL15) {
        Mario.isAlive = true;
        Luigi.isAlive = true;
        UnloadMusicStream(level15Music);
        UnloadMusicStream(Hammer15Music);
        UnloadSound(deathSound15);
        UnloadSound(stageCleared15Sound);
        Truss::UnloadSharedTexture();
        Ladder::UnloadSharedTexture();
        UnloadLevel15Entities();
        ResetLevel15Entities();
        AddBonus();
        Scene_Init = false;
        AddLevel();
    }
}