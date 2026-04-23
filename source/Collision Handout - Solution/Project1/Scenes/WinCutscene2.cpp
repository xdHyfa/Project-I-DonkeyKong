#include "Scenes/WinCutscene2.h"
#include "Scenes/scenes.h"
#include "Maps/Level2Map.h"
#include "Entities/Donkey.h"
#include "Entities/Lady.h"
#include "Entities/Player.h"
#include "Core/constants.h"
#include "include/resource_dir.h"
#include "raylib.h"
#include <cmath>

enum WinPhase2 { WPHASE_SHAKE, WPHASE_FALL, WPHASE_REUNION };
static WinPhase2 winPhase2 = WPHASE_SHAKE;

static float winTimer2 = 0.0f;
static float shakeTimer = 0.0f;
static int   shakeCount = 0;
static int   shakeFrameIdx = 0;
static float dkFallY = 0.0f;
static bool  heartVisible = false;
static float heartTimer = 0.0f;

static Texture2D dkTexture = { 0 };
static Texture2D ladyTexture = { 0 };
static Texture2D heartTexture = { 0 };

// DK sprites
static Rectangle dkShakeFrames[2] = {
    { 85.0f,  2.0f, 46.0f, 32.0f },
    { 134.0f, 2.0f, 46.0f, 32.0f }
};
static Rectangle dkFallFrame = { 176.0f, 82.0f, 40.0f, 32.0f };

// Lady sprite
static Rectangle ladyFrameRec = { 1.0f, 1.0f, 14.0f, 22.0f };

// Corazón sprite
static Rectangle heartRec = { 0.0f, 2.0f, 15.0f, 13.0f };

// Mario pose fija reunión — AJUSTA coordenadas cuando me las des
static Rectangle marioReunionFrame = { 0.0f, 0.0f, 16.0f, 22.0f }; // PLACEHOLDER

static Vector2 savedDKPos = { 0.0f, 0.0f };
static Vector2 reunionMarioPos = { 0.0f, 0.0f };
static Vector2 reunionLadyPos = { 0.0f, 0.0f };

void WinCutscene2Init() {
    SearchAndSetResourceDir("resources");

    dkTexture = LoadTexture("Sprites/donko 2-0.png");
    ladyTexture = LoadTexture("sprites/LADY.png");
    heartTexture = LoadTexture("sprites/HEARTS.png");

    savedDKPos = donkey.Position;
    dkFallY = donkey.Position.y;

    // Guardamos posición de Lady para la reunión
    reunionLadyPos = lady.Position;
    reunionMarioPos = { lady.Position.x + 18.0f, lady.Position.y };

    winPhase2 = WPHASE_SHAKE;
    winTimer2 = 0.0f;
    shakeTimer = 0.0f;
    shakeCount = 0;
    shakeFrameIdx = 0;
    heartVisible = false;
    heartTimer = 0.0f;
}

void runWinCutscene2() {

    if (!Scene_Init) {
        WinCutscene2Init();
        Scene_Init = true;
    }

    winTimer2 += GetFrameTime();
    shakeTimer += GetFrameTime();

    Level2LadderDraw();
    Level2RampDraw();

    // ---- FASE 1: DK SE SACUDE, ACELERANDO ----
    if (winPhase2 == WPHASE_SHAKE) {

        float t = (float)shakeCount / 24.0f;
        float interval = 0.18f - t * (0.18f - 0.04f);
        if (interval < 0.04f) interval = 0.04f;

        if (shakeTimer >= interval) {
            shakeTimer = 0.0f;
            shakeFrameIdx = 1 - shakeFrameIdx;
            shakeCount++;
        }

        DrawTextureRec(dkTexture, dkShakeFrames[shakeFrameIdx], donkey.Position, WHITE);
        DrawTextureRec(ladyTexture, ladyFrameRec, lady.Position, WHITE);
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

        if (shakeCount >= 24) {
            winPhase2 = WPHASE_FALL;
            winTimer2 = 0.0f;
        }
    }

    // ---- FASE 2: DK CAE BOCA ABAJO ----
    else if (winPhase2 == WPHASE_FALL) {

        dkFallY += 90.0f * GetFrameTime();
        donkey.Position.y = dkFallY;

        DrawTextureRec(dkTexture, dkFallFrame, donkey.Position, WHITE);
        DrawTextureRec(ladyTexture, ladyFrameRec, lady.Position, WHITE);
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

        if (donkey.Position.y >= (float)SCREEN_HEIGHT + 40.0f) {
            heartVisible = false;
            heartTimer = 0.0f;
            winPhase2 = WPHASE_REUNION;
            winTimer2 = 0.0f;
        }
    }

    // ---- FASE 3: REUNIÓN, CORAZÓN, Y PASA ----
    else if (winPhase2 == WPHASE_REUNION) {

        heartTimer += GetFrameTime();

        // Mario aparece en pose fija junto a Lady
        DrawTextureRec(ladyTexture, ladyFrameRec, reunionLadyPos, WHITE);
        DrawTextureRec(Mario.Texture, marioReunionFrame, reunionMarioPos, WHITE);

        // Corazón aparece tras 2 segundos
        if (heartTimer >= 2.0f) {
            heartVisible = true;
        }

        if (heartVisible) {
            Vector2 heartPos = {
                (reunionMarioPos.x + reunionLadyPos.x) / 2.0f - 7.0f,
                reunionLadyPos.y - 14.0f
            };
            DrawTextureRec(heartTexture, heartRec, heartPos, WHITE);
        }

        if (winTimer2 >= 5.0f) {
            UnloadTexture(dkTexture);
            UnloadTexture(ladyTexture);
            UnloadTexture(heartTexture);
            Scene_Init = false;
            ChangeScene();
        }
    }
}