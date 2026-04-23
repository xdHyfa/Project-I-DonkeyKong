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

// Nueva fase WPHASE_IMPACT entre FALL y REUNION
enum WinPhase2 { WPHASE_SHAKE, WPHASE_FALL, WPHASE_IMPACT, WPHASE_REUNION };
static WinPhase2 winPhase2 = WPHASE_SHAKE;

static float winTimer2 = 0.0f;
static float shakeTimer = 0.0f;
static int   shakeCount = 0;
static int   shakeFrameIdx = 0;
static float dkFallY = 0.0f;
static bool  heartVisible = false;
static float heartTimer = 0.0f;

// Impact animation state
static int   impactFrameIdx = 0;
static float impactFrameTimer = 0.0f;
static float impactTotalTimer = 0.0f;
static const float IMPACT_FRAME_INTERVAL = 0.5f;  // 0.5s por frame
static const float IMPACT_TOTAL_DURATION = 4.0f;  // duración total ~4 segundos
static float dkImpactStopY = 0.0f; // Y donde DK se detiene (Base_0_YPos - 32)

static Texture2D dkTexture = { 0 };
static Texture2D ladyTexture = { 0 };
static Texture2D heartTexture = { 0 };

// DK sprites - sacudida
static Rectangle dkShakeFrames[2] = {
    { 85.0f,  2.0f, 46.0f, 32.0f },
    { 134.0f, 2.0f, 46.0f, 32.0f }
};

// DK sprite - caida boca abajo
static Rectangle dkFallFrame = { 176.0f, 82.0f, 40.0f, 32.0f };

// DK sprites - impacto contra el suelo (3 frames, fila inferior)
static Rectangle dkImpactFrames[3] = {
    {   2.0f, 119.0f, 40.0f, 32.0f },
    {  45.0f, 119.0f, 40.0f, 32.0f },
    {  89.0f, 119.0f, 40.0f, 32.0f }
};

// Lady sprite
static Rectangle ladyFrameRec = { 1.0f, 1.0f, 14.0f, 22.0f };

// Corazon sprite
static Rectangle heartRec = { 0.0f, 2.0f, 15.0f, 13.0f };

// Mario pose reunion
static Rectangle marioReunionFrame = { 2.0f, 0.0f, 12.0f, 16.0f };

static Vector2 savedDKPos = { 0.0f, 0.0f };
static Vector2 reunionMarioPos = { 0.0f, 0.0f };
static Vector2 reunionLadyPos = { 0.0f, 0.0f };

// Dibuja un sprite flipeado horizontalmente usando DrawTexturePro
static void DrawTextureRecFlippedH(Texture2D tex, Rectangle src, Vector2 pos, Color tint)
{
    // Negamos el width del source para que raylib lo espeje
    Rectangle srcFlipped = { src.x, src.y, -src.width, src.height };
    Rectangle dest = { pos.x + src.width, pos.y, src.width, src.height };
    DrawTexturePro(tex, srcFlipped, dest, { 0.0f, 0.0f }, 0.0f, tint);
}

void WinCutscene2Init() {
    SearchAndSetResourceDir("resources");

    dkTexture = LoadTexture("Sprites/donko 2-0.png");
    ladyTexture = LoadTexture("sprites/LADY.png");
    heartTexture = LoadTexture("sprites/HEARTS.png");

    savedDKPos = donkey.Position;
    dkFallY = donkey.Position.y;

    // Y donde DK se frena: encima del truss base (Base_0)
    // Base_0_YPos = (SCREEN_HEIGHT - TrussHeight) - 1  =>  SCREEN_HEIGHT - 17
    // DK sprite height = 32, lo paramos justo encima del truss
    dkImpactStopY = (float)(SCREEN_HEIGHT - 16 - 1) - 24.0f;

    // Lady no se mueve, guardamos su posicion actual
    reunionLadyPos = lady.Position;
    // Mario aparece a la DERECHA de Lady, hardcodeado en su misma Y
    reunionMarioPos = { reunionLadyPos.x + 14.0f + 2.0f, reunionLadyPos.y };

    winPhase2 = WPHASE_SHAKE;
    winTimer2 = 0.0f;
    shakeTimer = 0.0f;
    shakeCount = 0;
    shakeFrameIdx = 0;
    heartVisible = false;
    heartTimer = 0.0f;
    impactFrameIdx = 0;
    impactFrameTimer = 0.0f;
    impactTotalTimer = 0.0f;
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

    // ---- FASE 2: DK CAE BOCA ABAJO, SE FRENA EN EL TRUSS ----
    else if (winPhase2 == WPHASE_FALL) {

        dkFallY += 90.0f * GetFrameTime();

        // Frenar al llegar al truss base
        if (dkFallY >= dkImpactStopY) {
            dkFallY = dkImpactStopY;
            donkey.Position.y = dkFallY;
            impactFrameIdx = 0;
            impactFrameTimer = 0.0f;
            winPhase2 = WPHASE_IMPACT;
            winTimer2 = 0.0f;
        }
        else {
            donkey.Position.y = dkFallY;
        }

        DrawTextureRec(dkTexture, dkFallFrame, donkey.Position, WHITE);
        DrawTextureRec(ladyTexture, ladyFrameRec, lady.Position, WHITE);
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    }

    // ---- FASE 2b: ANIMACION DE IMPACTO DE DK (loop ~4s, 0.5s por frame) ----
    else if (winPhase2 == WPHASE_IMPACT) {

        impactFrameTimer += GetFrameTime();
        impactTotalTimer += GetFrameTime();

        if (impactFrameTimer >= IMPACT_FRAME_INTERVAL) {
            impactFrameTimer = 0.0f;
            impactFrameIdx = (impactFrameIdx + 1) % 3; // loop entre 0,1,2
        }

        if (impactTotalTimer >= IMPACT_TOTAL_DURATION) {
            // Tiempo total agotado -> pasar a reunion, DK desaparece
            heartVisible = false;
            heartTimer = 0.0f;
            winPhase2 = WPHASE_REUNION;
            winTimer2 = 0.0f;
        }
        else {
            DrawTextureRec(dkTexture, dkImpactFrames[impactFrameIdx], donkey.Position, WHITE);
        }

        DrawTextureRec(ladyTexture, ladyFrameRec, lady.Position, WHITE);
        DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);
    }

    // ---- FASE 3: REUNION, CORAZON, Y PASA ----
    else if (winPhase2 == WPHASE_REUNION) {

        heartTimer += GetFrameTime();

        // Lady en su sitio
        DrawTextureRec(ladyTexture, ladyFrameRec, reunionLadyPos, WHITE);

        // Mario flipeado horizontalmente (miraba al lado incorrecto),
        // posicionado a la derecha de Lady
        DrawTextureRecFlippedH(Mario.Texture, marioReunionFrame, reunionMarioPos, WHITE);

        // Corazon aparece tras 2 segundos, centrado entre ambos
        if (heartTimer >= 2.0f) {
            heartVisible = true;
        }

        if (heartVisible) {
            // Centrado entre el centro de Lady (14px ancho) y el centro de Mario (12px ancho)
            float ladyCenterX = reunionLadyPos.x + 7.0f;
            float marioCenterX = reunionMarioPos.x + 6.0f;
            Vector2 heartPos = {
                (ladyCenterX + marioCenterX) / 2.0f - 7.5f,
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