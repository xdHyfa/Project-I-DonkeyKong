#include "Scenes/Cutscene.h"
#include "Scenes/scenes.h"
#include "Entities/Donkey.h"
#include "Maps/Level1Map.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "raylib.h"
#include <iostream>
using namespace std;

// ---- FASES ----
enum CutscenePhase { PHASE_CLIMB, PHASE_WALK, PHASE_ROAR, PHASE_END };
CutscenePhase cutPhase = PHASE_CLIMB;

// ---- DK ----
Texture2D cutDKTexture = { 0 };
Vector2   cutDKPos = { 0.0f, 0.0f };

// Frames subiendo con Lady
Rectangle climbFrames[4] = {
    {  3.0f, 78.0f, 38.0f, 36.0f },
    { 44.0f, 82.0f, 43.0f, 32.0f },
    { 90.0f, 78.0f, 38.0f, 36.0f },
    {131.0f, 72.0f, 43.0f, 32.0f }
};
// Frame caminando a la izquierda
Rectangle walkFrame = { 1.0f, 2.0f, 39.0f, 31.0f };
// Frame boca abierta (idle DK) - usa el que tienes
Rectangle roarFrame = { 3.0f, 2.0f, 38.0f, 32.0f }; // ajusta si tienes uno específico

int   cutFrameIndex = 0;
float cutFrameTimer = 0.0f;
float cutFrameInterval = 0.15f;

// ---- ESCALERAS ----
Texture2D cutStairsTexture = { 0 };
Rectangle stairFull = { 3.0f, 16.0f, 10.0f, 15.0f }; // escalera completa
Rectangle stairShort = { 4.0f,  0.0f, 10.0f,  3.0f }; // tramo corto al desaparecer

// X de la escalera central (alineada con Level1Ladders[8])
// Dos escaleras juntas
float stairX1 = 0.0f; // se calcula en Init
float stairX2 = 0.0f;
float stairStartY = 0.0f;
// Cuántas secciones de escalera hay visibles (de abajo a arriba)
// DK empieza abajo y sube, las escaleras desaparecen según sube
int   totalStairSections = 14; // ajusta según altura
int   visibleStairs = 14; // va bajando según DK sube

// ---- PLATAFORMAS INCLINADAS ----
Texture2D cutTrussTexture = { 0 };
Rectangle trussFlat = { 0.0f, 8.0f, 16.0f, 8.0f }; // plataforma recta
Rectangle trussIncline = { 0.0f, 0.0f, 16.0f, 8.0f }; // plataforma inclinada (ajusta)

// Cuántas rampas están inclinadas ya (0 = ninguna, 6 = todas)
int tiltedRamps = 0;
float tiltTimer = 0.0f;
float tiltInterval = 0.5f; // cada cuánto se inclina una

// ---- TIMERS ----
float cutTimer = 0.0f;

// ---- POSICIONES DE RAMPAS (copiadas de Level1Map) ----
float rampYPositions[6];

void CutsceneInit() {
    SearchAndSetResourceDir("resources");
    cutDKTexture = LoadTexture("Sprites/donko 2-0.png");
    cutStairsTexture = LoadTexture("Sprites/Stairs.png");
    cutTrussTexture = LoadTexture("Sprites/TRUSS.png");
    cout << "DK texture id: " << cutDKTexture.id << endl;
    cout << "DK pos: " << cutDKPos.x << ", " << cutDKPos.y << endl;
    

    // Y de cada rampa (mismo orden que Level1Map)
    rampYPositions[0] = SCREEN_HEIGHT - 16.0f - 1;
    rampYPositions[1] = SCREEN_HEIGHT - 16.0f - 43;
    rampYPositions[2] = SCREEN_HEIGHT - 16.0f - 62;
    rampYPositions[3] = SCREEN_HEIGHT - 16.0f - 110;
    rampYPositions[4] = SCREEN_HEIGHT - 16.0f - 130;
    rampYPositions[5] = SCREEN_HEIGHT - 16.0f - 169;

    stairStartY = rampYPositions[0];

    // DK empieza abajo del todo
    cutDKPos = { (float)SCREEN_WIDTH / 2 - 19.0f, (float)SCREEN_HEIGHT - 80.0f };

    // X escaleras: alineadas con Level1Ladders[8]
    // Level1Ladders[8] está en Ramp_6[2], calculamos X aproximada
    stairX1 = SCREEN_WIDTH / 2.0f - 10.0f;
    stairX2 = stairX1 + 20.0f;

    cutPhase = PHASE_CLIMB;
    cutFrameIndex = 0;
    cutFrameTimer = 0.0f;
    cutTimer = 0.0f;
    visibleStairs = totalStairSections;
    tiltedRamps = 0;
}

void DrawCutsceneStairs() {
    for (int i = 0; i < totalStairSections; i++) {
        float y = stairStartY - (float)(i + 1) * 15.0f;

        // derecha siempre visible
        DrawTextureRec(cutStairsTexture, stairFull, { stairX2, y }, WHITE);

        // izquierda desaparece de abajo a arriba
        if (i >= totalStairSections - visibleStairs)
            DrawTextureRec(cutStairsTexture, stairFull, { stairX1, y }, WHITE);
    }
}

void DrawCutscenePlatforms() {
    // Ramp 0 - 14 truss, empieza en x=0
    for (int i = 0; i < 14; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { i * 16.0f, rampYPositions[0] }, WHITE);

    // Ramp 1 - 13 truss, empieza en x=0
    for (int i = 0; i < 13; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { i * 16.0f, rampYPositions[1] }, WHITE);

    // Ramp 2 - 13 truss, empieza en x=16
    for (int i = 0; i < 13; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { 16.0f + i * 16.0f, rampYPositions[2] }, WHITE);

    // Ramp 3 - 13 truss, empieza en x=0
    for (int i = 0; i < 13; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { i * 16.0f, rampYPositions[3] }, WHITE);

    // Ramp 4 - 13 truss, empieza en x=16
    for (int i = 0; i < 13; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { 16.0f + i * 16.0f, rampYPositions[4] }, WHITE);

    // Ramp 5 - 13 truss, empieza en x=0
    for (int i = 0; i < 13; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { i * 16.0f, rampYPositions[5] }, WHITE);

    // Ramp 6 - 3 truss en el centro
    float ramp6Y = SCREEN_HEIGHT - 16.0f - 200;
    for (int i = 0; i < 3; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { SCREEN_WIDTH / 3.0f + (i + 1) * 16.0f, ramp6Y }, WHITE);
}

void runCutscene() {

    if (!Scene_Init) {
        CutsceneInit();
        Scene_Init = true;
    }

    cutFrameTimer += GetFrameTime();
    cutTimer += GetFrameTime();

    // ---- DIBUJAR PLATAFORMAS ----
    DrawCutscenePlatforms();

    // ---- DIBUJAR ESCALERAS ----
    DrawCutsceneStairs();

    // ---- FASE 1: SUBIR ----
    if (cutPhase == PHASE_CLIMB) {
        // Animar frames
        if (cutFrameTimer >= cutFrameInterval) {
            cutFrameTimer = 0.0f;
            cutFrameIndex = (cutFrameIndex + 1) % 4;
        }

        // Mover DK hacia arriba
        cutDKPos.y -= 0.5f; // velocidad de subida, ajusta

        // Escaleras desaparecen según sube
        float climbProgress = (stairStartY - cutDKPos.y) / stairStartY;
        visibleStairs = totalStairSections - (int)(climbProgress * totalStairSections);
        if (visibleStairs < 0) visibleStairs = 0;

        // DK llega arriba: cambia a PHASE_WALK
        if (cutDKPos.y <= rampYPositions[5] - 31.0f) {
            cutPhase = PHASE_WALK;
            cutTimer = 0.0f;
            cutDKPos.x = stairX2; // empieza desde la escalera central
        }

        DrawTextureRec(cutDKTexture, climbFrames[cutFrameIndex], cutDKPos, WHITE);
    }

    // ---- FASE 2: CAMINAR A LA IZQUIERDA ----
    else if (cutPhase == PHASE_WALK) {
        // DK se mueve a la izquierda dando saltos
        if (cutFrameTimer >= cutFrameInterval) {
            cutFrameTimer = 0.0f;
            cutDKPos.x -= 2.0f; // velocidad de caminar, ajusta
        }

        // Plataformas se inclinan según camina
        if (cutTimer >= tiltInterval) {
            cutTimer = 0.0f;
            if (tiltedRamps < 6) tiltedRamps++;
        }

        // DK llega a su posición final (donde tira barriles)
        if (cutDKPos.x <= donkey.Position.x) {
            cutPhase = PHASE_ROAR;
            cutTimer = 0.0f;
        }

        DrawTextureRec(cutDKTexture, walkFrame, cutDKPos, WHITE);
    }

    // ---- FASE 3: ROAR ----
    else if (cutPhase == PHASE_ROAR) {
        // Alterna entre roarFrame y idle cada 0.3s durante 2s
        bool showRoar = ((int)(cutTimer / 0.3f)) % 2 == 0;
        Rectangle& frame = showRoar ? roarFrame : walkFrame;
        DrawTextureRec(cutDKTexture, frame, cutDKPos, WHITE);

        if (cutTimer >= 2.0f) {
            cutPhase = PHASE_END;
            cutTimer = 0.0f;
        }
    }

    // ---- FASE 4: PASAR A HOWHIGH ----
    else if (cutPhase == PHASE_END) {
        if (cutTimer >= 0.5f) {
            UnloadTexture(cutDKTexture);
            UnloadTexture(cutStairsTexture);
            UnloadTexture(cutTrussTexture);
            Scene_Init = false;
            ChangeScene();
        }
    }
}