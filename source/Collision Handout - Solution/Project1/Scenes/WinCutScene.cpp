#include "Scenes/WinCutscene.h"
#include "Scenes/scenes.h"
#include "Entities/Donkey.h"
#include "Entities/Lady.h"
#include "Entities/Player.h"
#include "Maps/Level1Map.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "raylib.h"
#include <cmath>
#include <iostream>
using namespace std;

enum WinPhase { WIN_HEART, WIN_JUMP, WIN_CLIMB, WIN_KIDNAP, WIN_END };
WinPhase winPhase = WIN_HEART;

// ---- TEXTURAS PROPIAS DE LA CUTSCENE ----
Texture2D heartTexture = { 0 };
Texture2D winDKTexture = { 0 };
Texture2D winStairsTexture = { 0 };

// ---- AUDIO ----
Sound winSound = { 0 };          // <-- NEW: streaming .wav track

// ---- CORAZON ----
Rectangle heartFull = { 0.0f,  2.0f, 16.0f, 12.0f };
Rectangle heartBroken = { 16.0f, 2.0f, 16.0f, 12.0f };
Vector2   heartPos = { 0.0f,  0.0f };
bool      showBroken = false;

// ---- DK FRAMES SIN LADY ----
Rectangle emptyFrames[4] = {
    {  93.0f, 37.0f, 33.0f, 36.0f },
    { 129.0f, 37.0f, 33.0f, 36.0f },
    { 165.0f, 37.0f, 33.0f, 36.0f },
    { 206.0f, 41.0f, 43.0f, 32.0f }
};

// ---- DK FRAMES CON LADY ----
Rectangle winClimbFrames[4] = {
    {  3.0f,  78.0f, 38.0f, 36.0f },
    { 44.0f,  82.0f, 43.0f, 32.0f },
    { 90.0f,  78.0f, 38.0f, 36.0f },
    { 131.0f, 82.0f, 43.0f, 32.0f }
};

// ---- DK POSICION Y ANIMACION ----
Vector2 winDKPos = { 0.0f, 0.0f };
int     winFrameIdx = 0;
float   winFrameTimer = 0.0f;
float   winFrameInterval = 0.15f;
bool    hasLady = false;

// ---- ESCALERAS ----
Rectangle winStairRec = { 3.0f, 16.0f, 10.0f, 15.0f };

// ---- POSICIONES ----
float winRamp5Y = 0.0f;
float winRamp6Y = 0.0f;
float ladyWinY = 0.0f;

// ---- TIMERS ----
float winTimer = 0.0f;


void WinCutsceneInit() {
    SearchAndSetResourceDir("resources");

    // Textures
    heartTexture = LoadTexture("sprites/HEARTS.png");
    winDKTexture = LoadTexture("Sprites/donko 2-0.png");
    winStairsTexture = LoadTexture("sprites/Stairs.png");

    // ---- AUDIO ----                                          // <-- NEW
    winSound = LoadSound("Audio/Stage-Cleared-1.wav");
    PlaySound(winSound);                                // start playing immediately

    // Mario mira a la izquierda
    frameRec.width = -abs(frameRec.width);

    // Posiciones de referencia
    winRamp5Y = SCREEN_HEIGHT - 16.0f - 169;
    winRamp6Y = SCREEN_HEIGHT - 16.0f - 200;

    // DK empieza donde Level1 lo dejó
    winDKPos = donkey.Position;
    ladyWinY = lady.Position.y;

    heartPos = {
        (Mario.Position.x + lady.Position.x) / 2.0f + 2.0f,
        lady.Position.y - 10.0f
    };

    winTriggered = false;
    winPhase = WIN_HEART;
    winFrameIdx = 0;
    winFrameTimer = 0.0f;
    winTimer = 0.0f;
    hasLady = false;
    showBroken = false;
}

void DrawWinStairs() {
    DrawTextureRec(winStairsTexture, winStairRec, { 64.0f, 56.0f }, WHITE);
    DrawTextureRec(winStairsTexture, winStairRec, { 64.0f, 41.0f }, WHITE);
    DrawTextureRec(winStairsTexture, winStairRec, { 64.0f, 26.0f }, WHITE);
    DrawTextureRec(winStairsTexture, winStairRec, { 64.0f, 14.0f }, WHITE);

    DrawTextureRec(winStairsTexture, winStairRec, { 80.0f, 56.0f }, WHITE);
    DrawTextureRec(winStairsTexture, winStairRec, { 80.0f, 41.0f }, WHITE);
    DrawTextureRec(winStairsTexture, winStairRec, { 80.0f, 26.0f }, WHITE);
    DrawTextureRec(winStairsTexture, winStairRec, { 80.0f, 14.0f }, WHITE);
}

void runWinCutscene() {

    // Init: solo la primera vez
    if (!Scene_Init) {
        WinCutsceneInit();
        Scene_Init = true;
    }

    // Guardia de fin
    if (winPhase == WIN_END && winTimer >= 1.0f) return;

    // Timers
    winTimer += GetFrameTime();
    winFrameTimer += GetFrameTime();

    
    // ---- DRAW BASE ----
    Level1RampDraw();
    Level1LadderDraw();
    DrawWinStairs();

    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

    if (!hasLady)
        DrawTextureRec(lady.Texture, { 1.0f, 1.0f, 14.0f, 22.0f }, lady.Position, WHITE);

    // ================================================================
    // FASE 0: CORAZON
    // ================================================================
    if (winPhase == WIN_HEART) {
        Rectangle& hRec = showBroken ? heartBroken : heartFull;
        DrawTextureRec(heartTexture, hRec, heartPos, WHITE);
        DrawTextureRec(winDKTexture, { 3.0f, 2.0f, 38.0f, 32.0f }, winDKPos, WHITE);

        if (winTimer >= 2.0f) {
            winPhase = WIN_JUMP;
            winTimer = 0.0f;
        }
    }

    // ================================================================
    // FASE 1: DK SALTA
    // ================================================================
    else if (winPhase == WIN_JUMP) {
        DrawTextureRec(heartTexture, heartFull, heartPos, WHITE);

        float baseY = donkey.Position.y;
        float targetX = 64.0f;
        float jumpHeight = 10.0f;
        float jumpDuration = 0.4f;

        float t = winTimer / jumpDuration;
        if (t > 1.0f) t = 1.0f;

        winDKPos.x = donkey.Position.x + (targetX - donkey.Position.x) * t;
        winDKPos.y = baseY - sinf(t * 3.14159f) * jumpHeight;

        DrawTextureRec(winDKTexture, { 3.0f, 2.0f, 38.0f, 32.0f }, winDKPos, WHITE);

        if (winTimer >= jumpDuration) {
            winDKPos.x = targetX;
            winDKPos.y = baseY;
            winPhase = WIN_CLIMB;
            winTimer = 0.0f;
        }
    }

    // ================================================================
    // FASE 2: DK SUBE (sin Lady)
    // ================================================================
    else if (winPhase == WIN_CLIMB) {
        if (winFrameTimer >= winFrameInterval) {
            winFrameTimer = 0.0f;
            winFrameIdx = (winFrameIdx + 1) % 4;
        }

        winDKPos.y -= 1.0f;

        DrawTextureRec(heartTexture, heartFull, heartPos, WHITE);

        if (winDKPos.y <= winRamp6Y - 32.0f) {
            winPhase = WIN_KIDNAP;
            winTimer = 0.0f;
            hasLady = true;
            showBroken = true;
        }

        DrawTextureRec(winDKTexture, emptyFrames[winFrameIdx], winDKPos, WHITE);
    }

    // ================================================================
    // FASE 3: SECUESTRO
    // ================================================================
    else if (winPhase == WIN_KIDNAP) {
        DrawTextureRec(heartTexture, heartBroken, heartPos, WHITE);

        if (winFrameTimer >= winFrameInterval) {
            winFrameTimer = 0.0f;
            winFrameIdx = (winFrameIdx + 1) % 4;
        }

        winDKPos.y -= 1.0f;

        if (winDKPos.y < -40.0f) {
            winPhase = WIN_END;
            winTimer = 0.0f;
        }

        DrawTextureRec(winDKTexture, winClimbFrames[winFrameIdx], winDKPos, WHITE);
    }

    // ================================================================
    // FASE 4: FIN
    // ================================================================
    else if (winPhase == WIN_END) {
        DrawTextureRec(heartTexture, heartBroken, heartPos, WHITE);

        if (winTimer >= 1.0f) {
            // ---- AUDIO cleanup ----                          // <-- NEW
            StopSound(winSound);
            UnloadSound(winSound);
            CloseAudioDevice();

            // Textures cleanup
            UnloadTexture(heartTexture);
            UnloadTexture(winDKTexture);
            UnloadTexture(winStairsTexture);

            UnloadTexture(Mario.Texture);
            Truss::UnloadSharedTexture();
            Ladder::UnloadSharedTexture();
            donkey.Unload();
            lady.Unload();

            Scene_Init = false;
            ChangeScene();
        }
    }
}