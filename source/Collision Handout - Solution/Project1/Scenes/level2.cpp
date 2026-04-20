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

// DK animacion level2 (idle loop)
Rectangle dk2Frames[3] = {
    {  1.0f,  2.0f, 40.0f, 32.0f },
    { 85.0f,  2.0f, 46.0f, 32.0f },
    {134.0f,  2.0f, 46.0f, 32.0f }
};
int   dk2FrameIdx = 0;
float dk2FrameTimer = 0.0f;
float dk2FrameInterval = 0.3f;
bool  dk2Forward = true;
float dk2PauseTimer = 0.0f;
float dk2PauseDuration = 1.0f;
bool  dk2IsPaused = false;

// Lady animacion level2
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

// WIN ANIMATION
bool winTriggered2 = false;

enum WinPhase2 { W2_NONE, W2_FREEZE, W2_FALL, W2_HIT, W2_TELEPORT };
WinPhase2 winPhase2 = W2_NONE;
float     winPhaseTimer = 0.0f;

float dkFallSpeed = 150.0f;
float dkFloorY = 0.0f;

Rectangle dkHitFrames[4] = {
    { 176.0f,  82.0f, 39.0f, 32.0f },
    {   2.0f, 119.0f, 39.0f, 32.0f },
    {  45.0f, 119.0f, 39.0f, 32.0f },
    {  88.0f, 119.0f, 39.0f, 32.0f },
};
int   dkHitFrameIdx = 0;
float dkHitFrameTimer = 0.0f;
float dkHitFrameInterval = 0.12f;
bool  dkHitDone = false;

extern Texture2D heartTexture;
Rectangle heartFullRec = { 0.0f, 2.0f, 16.0f, 12.0f };

// Rectangulo negro que tapa trusses del medio
Rectangle GetBlackCover() {
    return {
        16.0f,
        (float)Base_1_YPos - 8.0f,
        (float)SCREEN_WIDTH - 32.0f,
        (float)(Base_4_YPos - Base_1_YPos) + 30.0f
    };
}

static void UpdateDK2IdleAnim() {
    if (dk2IsPaused) {
        dk2PauseTimer += GetFrameTime();
        if (dk2PauseTimer >= dk2PauseDuration) { dk2PauseTimer = 0.0f; dk2IsPaused = false; }
        return;
    }
    dk2FrameTimer += GetFrameTime();
    if (dk2FrameTimer >= dk2FrameInterval) {
        dk2FrameTimer = 0.0f;
        if (dk2Forward) {
            dk2FrameIdx++;
            if (dk2FrameIdx >= 3) { dk2FrameIdx = 1; dk2Forward = false; dk2IsPaused = true; }
        }
        else {
            dk2FrameIdx--;
            if (dk2FrameIdx <= 0) { dk2FrameIdx = 0; dk2Forward = true; dk2IsPaused = true; }
        }
    }
}

static void UpdateLadyAnim() {
    if (ladyIsPaused) {
        ladyPauseTimer += GetFrameTime();
        if (ladyPauseTimer >= ladyPauseDuration) { ladyPauseTimer = 0.0f; ladyIsPaused = false; ladyFrameIdx = 0; }
        return;
    }
    ladyFrameTimer += GetFrameTime();
    if (ladyFrameTimer >= ladyFrameInterval) {
        ladyFrameTimer = 0.0f;
        ladyFrameIdx++;
        if (ladyFrameIdx >= 6) { ladyFrameIdx = 0; ladyIsPaused = true; }
    }
}

static void RunWinAnimation2() {
    winPhaseTimer += GetFrameTime();

    Rectangle cover = GetBlackCover();

    Level2LadderDraw();
    Level2RampDraw();
    Level2ButtonsDraw();
    DrawRectangleRec(cover, BLACK);

    switch (winPhase2) {

    case W2_FREEZE:
        DrawTextureRec(donkey.Texture, dk2Frames[0], donkey.Position, WHITE);
        DrawTextureRec(lady.Texture, ladyFrames[0], lady.Position, WHITE);
        if (winPhaseTimer >= 1.2f) {
            dkFloorY = (float)Base_0_YPos - 34.0f;
            winPhaseTimer = 0.0f;
            winPhase2 = W2_FALL;
        }
        break;

    case W2_FALL: {
        donkey.Position.y += dkFallSpeed * GetFrameTime();
        // Flip vertical: height negativo = boca abajo en raylib
        Rectangle srcFlipped = dk2Frames[0];
        srcFlipped.height = -abs(srcFlipped.height);
        DrawTextureRec(donkey.Texture, srcFlipped, donkey.Position, WHITE);
        DrawTextureRec(lady.Texture, ladyFrames[0], lady.Position, WHITE);
        if (donkey.Position.y >= dkFloorY) {
            donkey.Position.y = dkFloorY;
            dkHitFrameIdx = 0;
            dkHitFrameTimer = 0.0f;
            dkHitDone = false;
            winPhaseTimer = 0.0f;
            winPhase2 = W2_HIT;
        }
        break;
    }

    case W2_HIT:
        if (!dkHitDone) {
            dkHitFrameTimer += GetFrameTime();
            if (dkHitFrameTimer >= dkHitFrameInterval) {
                dkHitFrameTimer = 0.0f;
                dkHitFrameIdx++;
                if (dkHitFrameIdx >= 4) { dkHitFrameIdx = 3; dkHitDone = true; }
            }
        }
        DrawTextureRec(donkey.Texture, dkHitFrames[dkHitFrameIdx], donkey.Position, WHITE);
        DrawTextureRec(lady.Texture, ladyFrames[0], lady.Position, WHITE);
        if (dkHitDone && winPhaseTimer >= 1.0f) {
            Mario.Position = { lady.Position.x - 14.0f, lady.Position.y };
            winPhaseTimer = 0.0f;
            winPhase2 = W2_TELEPORT;
        }
        break;

    case W2_TELEPORT:
        DrawTextureRec(donkey.Texture, dkHitFrames[3], donkey.Position, WHITE);
        DrawTextureRec(lady.Texture, ladyFrames[1], lady.Position, WHITE);
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
        DrawTextureRec(heartTexture, heartFullRec,
            { lady.Position.x - 6.0f, lady.Position.y - 12.0f }, WHITE);
        // Añade ChangeScene() aqui tras un timer si quieres pasar de escena
        break;

    default: break;
    }
}

void runLevel2() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        Mario.Texture = LoadTexture("sprites/MARIO.png");
        Mario.Setup();
        donkey.Setup();
        lady.Setup();

        dk2FrameIdx = 0;
        dk2FrameTimer = 0.0f;
        dk2Forward = true;
        dk2IsPaused = false;
        dk2PauseTimer = 0.0f;

        ladyFrameIdx = 0;
        ladyFrameTimer = 0.0f;
        ladyIsPaused = false;
        ladyPauseTimer = 0.0f;

        winTriggered2 = false;
        winPhase2 = W2_NONE;
        winPhaseTimer = 0.0f;

        lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f,  (float)Base_5_YPos - 14.0f };
        donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 17.0f };

        Level2RampSetter();
        Level2LadderSetter();
        Level2ButtonSetter();

        SetStartTime2();
        Scene_Init = true;

        level2Music = LoadMusicStream("Audio/Stage-2-Springboard2.wav");
        deathMusic2 = LoadMusicStream("Audio/Dead.wav");
        level2Music.looping = true;
        PlayMusicStream(level2Music);
        isDead2 = false;
    }

    // WIN ANIMATION bloquea todo lo demas
    if (winPhase2 != W2_NONE) {
        RunWinAnimation2();
        return;
    }

    // UPDATE NORMAL
    Mario.Movement();
    UpdateMusicStream(level2Music);
    UpdateMusicStream(deathMusic2);

    UpdateDK2IdleAnim();
    UpdateLadyAnim();

    Level2RampCollisions(Mario);
    Level2LadderCollisions(Mario);
    Level2CheckButtons(Mario);

    // DRAW NORMAL
    Level2LadderDraw();
    Level2RampDraw();
    Level2ButtonsDraw();

    lady.Position = { (float)SCREEN_WIDTH / 2 - 7.0f,  (float)Base_5_YPos - 14.0f };
    donkey.Position = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)Base_5_YPos + 17.0f };

    DrawTextureRec(donkey.Texture, dk2Frames[dk2FrameIdx], donkey.Position, WHITE);
    DrawTextureRec(lady.Texture, ladyFrames[ladyFrameIdx], lady.Position, WHITE);
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

    Level2EntitiesRoutine();

    if (IsKeyPressed(KEY_ONE)) ChangeScene();

    CheckWinCondition();

    if (GetCurrentScene() != LEVEL2) {
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
        Scene_Init = false;
    }
}