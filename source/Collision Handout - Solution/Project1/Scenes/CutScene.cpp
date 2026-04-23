#include "Scenes/Cutscene.h"
#include "Scenes/scenes.h"
#include "Entities/Donkey.h"
#include "Maps/Level1Map.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "raylib.h"
#include <iostream>
#include "Entities/Lady.h"
using namespace std;

// ---- FASES ----
enum CutscenePhase { PHASE_CLIMB, PHASE_JUMP, PHASE_JUMP_PEAK,  PHASE_WALK, PHASE_ROAR, PHASE_END };
CutscenePhase cutPhase = PHASE_CLIMB;

// ---- DK ----
Texture2D cutDKTexture = { 0 };
Vector2   cutDKPos = { 0.0f, 0.0f };

// Frames subiendo con Lady
Rectangle climbFrames[4] = {
    {  3.0f, 78.0f, 38.0f, 36.0f },
    { 44.0f, 82.0f, 43.0f, 32.0f },
    { 90.0f, 78.0f, 38.0f, 36.0f },
    {131.0f, 82.0f, 43.0f, 32.0f }
};
// Frame caminando a la izquierda
Rectangle walkFrame = { 1.0f, 2.0f, 39.0f, 31.0f };
// Frame boca abierta (idle DK) - usa el que tienes
Rectangle roarFrame = { 3.0f, 2.0f, 38.0f, 32.0f }; // ajusta si tienes uno específico
Rectangle peakLadyFrame = { 206.0f, 41.0f, 42.0f, 31.0f };


Vector2 ladyFixedPos = { 0.0f, 0.0f };
bool    ladyVisible = false;


Sound introSound = { 0 };
Sound stompSound = { 0 };
Sound cutSound = { 0 };

float walkBobY = 0.0f;  // posición Y del salto
float walkBobTimer = 0.0f;
bool  wasDown = false;  // detecta cuando toca el suelo



int   cutFrameIndex = 0;
float cutFrameTimer = 0.0f;
float cutFrameInterval = 0.15f;
int visibleStairsLeft = 0;
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

    donkey.Position = { 21.0f, 47.0f };

    cutDKTexture = LoadTexture("Sprites/donko 2-0.png");
    cutStairsTexture = LoadTexture("Sprites/Stairs.png");
    cutTrussTexture = LoadTexture("Sprites/TRUSS.png");
    lady.Setup();
    ladyVisible = false;

    introSound = LoadSound("Audio/Stage-Introduction-1.wav");
    stompSound = LoadSound("Audio/Stomp.wav");
    cutSound = LoadSound("Audio/Cutscene.wav");
    PlaySound(introSound);

    // Y de cada rampa (mismo orden que Level1Map)
    rampYPositions[0] = SCREEN_HEIGHT - 16.0f - 1;
    rampYPositions[1] = SCREEN_HEIGHT - 16.0f - 43;
    rampYPositions[2] = SCREEN_HEIGHT - 16.0f - 62;
    rampYPositions[3] = SCREEN_HEIGHT - 16.0f - 110;
    rampYPositions[4] = SCREEN_HEIGHT - 16.0f - 130;
    rampYPositions[5] = SCREEN_HEIGHT - 16.0f - 169;

    stairStartY = rampYPositions[0] + 4.0f;

    // DK empieza abajo del todo
    cutDKPos = { (float)SCREEN_WIDTH / 2 - 19.0f+ 13.0f, (float)SCREEN_HEIGHT - 46.0f };

    // X escaleras: alineadas con Level1Ladders[8]
    // Level1Ladders[8] está en Ramp_6[2], calculamos X aproximada
    stairX2 = SCREEN_WIDTH / 3.0f + 4 * 16.0f - 10.0f; // borde derecho de Ramp_6
    stairX1 = stairX2 - 20.0f;

    cutPhase = PHASE_CLIMB;
    cutFrameIndex = 0;
    cutFrameTimer = 0.0f;
    cutTimer = 0.0f;
    visibleStairs = totalStairSections;
    tiltedRamps = 0;
    walkBobTimer = 0.0f;
    wasDown = false;
    visibleStairsLeft = totalStairSections;
}

void DrawCutsceneStairs() {
    float ramp6Y = SCREEN_HEIGHT - 16.0f - 200;
    int sectionCount = 0;

    for (int i = 0; i < totalStairSections; i++) {
        float y = stairStartY - (float)(i + 1) * 15.0f;
        if (y < ramp6Y) break;
        sectionCount++;
    }

    for (int i = 0; i < sectionCount; i++) {
        float y = stairStartY - (float)(i + 1) * 15.0f;
        bool isTop = (i >= sectionCount - 2);

        // derecha
        if (i >= sectionCount - visibleStairs) {
            DrawTextureRec(cutStairsTexture, stairFull, { stairX2, y }, WHITE);
        }
        else if (isTop) {
            DrawTextureRec(cutStairsTexture, stairFull, { stairX2, y }, WHITE);
        }

        // izquierda: faltan las 2 de arriba desde el inicio
        if (i >= sectionCount - visibleStairsLeft && i < sectionCount - 2) {
            DrawTextureRec(cutStairsTexture, stairFull, { stairX1, y }, WHITE);
        }
    }
}

void DrawCutscenePlatforms() {
    auto drawRamp = [&](int rampIndex, int size, bool tiltLeft, int plane, float startY, float startX) {
        float adderY = startY;
        float adderX = startX;
        bool tilted = (rampIndex >= 6 - tiltedRamps);
        for (int i = 0; i < size; i++) {
            if (i >= plane) {
                if (!tilted) {
                    // recta: sin tilt
                }
                else {
                    // caída: aplica tilt
                    if (tiltLeft) adderY--;
                    else          adderY++;
                }
            }
            DrawTextureRec(cutTrussTexture, trussFlat, { adderX, adderY }, WHITE);
            adderX += 16.0f;
        }
    };

    drawRamp(0, 14, true, 7, rampYPositions[0], 0.0f);
    drawRamp(1, 13, false, 0, rampYPositions[1], 0.0f);
    drawRamp(2, 13, true, 0, rampYPositions[2], 16.0f);
    drawRamp(3, 13, false, 0, rampYPositions[3], 0.0f);
    drawRamp(4, 13, true, 0, rampYPositions[4], 16.0f);
    drawRamp(5, 13, false, 9, rampYPositions[5], 0.0f);

    float ramp6Y = SCREEN_HEIGHT - 16.0f - 200;
    for (int i = 0; i < 3; i++)
        DrawTextureRec(cutTrussTexture, trussFlat, { SCREEN_WIDTH / 3.0f + (i + 1) * 16.0f, ramp6Y }, WHITE);
}
void runCutscene() {

    
    if (!Scene_Init) {
        CutsceneInit();
        Scene_Init = true;
    }
    if (ladyVisible) {
        DrawTextureRec(lady.Texture, { 1.0f, 1.0f, 14.0f, 22.0f }, ladyFixedPos, WHITE);
    }

    cutFrameTimer += GetFrameTime();
    cutTimer += GetFrameTime();

    // ---- DIBUJAR PLATAFORMAS ----
    DrawCutscenePlatforms();

    // ---- DIBUJAR ESCALERAS ----
    
    DrawCutsceneStairs();

    Rectangle stairRec = { 3.0f, 16.0f, 10.0f, 15.0f };

    DrawTextureRec(cutStairsTexture, stairRec, { 64.0f, 56.0f }, WHITE);
    DrawTextureRec(cutStairsTexture, stairRec, { 64.0f, 41.0f }, WHITE);
    DrawTextureRec(cutStairsTexture, stairRec, { 64.0f, 26.0f }, WHITE);
    DrawTextureRec(cutStairsTexture, stairRec, { 64.0f, 14.0f }, WHITE);

    DrawTextureRec(cutStairsTexture, stairRec, { 80.0f, 56.0f }, WHITE);
    DrawTextureRec(cutStairsTexture, stairRec, { 80.0f, 41.0f }, WHITE);
    DrawTextureRec(cutStairsTexture, stairRec, { 80.0f, 26.0f }, WHITE);
    DrawTextureRec(cutStairsTexture, stairRec, { 80.0f, 14.0f }, WHITE);

    // ---- FASE 1: SUBIR ----
    if (cutPhase == PHASE_CLIMB) {
        // Animar frames
        if (cutFrameTimer >= cutFrameInterval) {
            cutFrameTimer = 0.0f;
            cutFrameIndex = (cutFrameIndex + 1) % 4;
        }

        // Mover DK hacia arriba
        cutDKPos.y -= 1.4f; // velocidad de subida, ajusta

        // Escaleras desaparecen según sube
        float climbProgress = (stairStartY - cutDKPos.y) / stairStartY;
        visibleStairs = totalStairSections - (int)(climbProgress * totalStairSections);
        visibleStairsLeft = visibleStairs;
        if (visibleStairs < 0) visibleStairs = 0;
        if (visibleStairsLeft < 0) visibleStairsLeft = 0;

        if (cutDKPos.y <= rampYPositions[5] - 31.0f) {
            StopSound(introSound);
            visibleStairs = 0;
            visibleStairsLeft = 0;
            cutDKPos.y = rampYPositions[5] - 31.0f;
            cutPhase = PHASE_JUMP;
            cutTimer = 0.0f;
        }

        DrawTextureRec(cutDKTexture, climbFrames[cutFrameIndex], cutDKPos, WHITE);
    }



    else if (cutPhase == PHASE_JUMP) {
        float peakTime = 0.6f;
        float jumpHeightPx = 25.0f;
        float baseY = rampYPositions[5] - 31.0f;

        // pausa antes de saltar
        if (cutTimer < 0.3f) {
            
            DrawTextureRec(cutDKTexture, climbFrames[3], cutDKPos, WHITE);
            return;
        }

        float jumpProgress = cutTimer - 0.3f; // empieza a contar desde 0.3

        if (jumpProgress < peakTime) {
            // sube
            cutDKPos.y = baseY - (jumpProgress / peakTime) * jumpHeightPx;
            DrawTextureRec(cutDKTexture, climbFrames[3], cutDKPos, WHITE);
        }
        else {
            // llegó al pico
            cutPhase = PHASE_JUMP_PEAK;
            cutTimer = 0.0f;
            cutDKPos.y = baseY - jumpHeightPx;
            DrawTextureRec(cutDKTexture, climbFrames[3], cutDKPos, WHITE);
        }
    }

    else if (cutPhase == PHASE_JUMP_PEAK) {
        float jumpHeightPx = 25.0f;
        float baseY = rampYPositions[5] - 31.0f;
        cutDKPos.y = baseY - jumpHeightPx;

        ladyFixedPos = { cutDKPos.x - 14.0f, cutDKPos.y + 3.0f };
        ladyVisible = true;

        DrawTextureRec(lady.Texture, { 1.0f, 1.0f, 14.0f, 22.0f }, ladyFixedPos, WHITE);

        // Cambia de sprite inmediatamente al aparecer Lady, sin delay
        Rectangle& peakFrame = ladyVisible ? peakLadyFrame : climbFrames[3];
        DrawTextureRec(cutDKTexture, peakFrame, cutDKPos, WHITE);

        // Reducido de 1.0f a 0.4f para acortar la pausa
        if (cutTimer >= 0.4f) {
            float fallProgress = (cutTimer - 0.4f) / 0.3f;
            cutDKPos.y = (baseY - jumpHeightPx) + fallProgress * jumpHeightPx;

            if (cutDKPos.y >= baseY) {
                cutDKPos.y = baseY;
                cutPhase = PHASE_WALK;
                cutTimer = 0.0f;
            }
        }
    }



    else if (cutPhase == PHASE_WALK) {
        float baseY = rampYPositions[5] - 31.0f;
        float bobHeight = 8.0f;
        float bobCycle = 0.7f;
        float totalDistance = stairX2 - donkey.Position.x;
        float stepDistance = totalDistance / 6.0f;

        walkBobTimer += GetFrameTime();
        float t = fmod(walkBobTimer, bobCycle) / bobCycle;

        // X se mueve continuamente
        cutDKPos.x -= (stepDistance / bobCycle) * GetFrameTime();

        if (t < 0.5f) {
            cutDKPos.y = baseY;
            if (!wasDown) {
                wasDown = true;
                PlaySound(stompSound);
                if (tiltedRamps < 6) tiltedRamps++;
            }
        }
        else {
            wasDown = false;
            float jumpT = (t - 0.5f) / 0.5f;
            cutDKPos.y = baseY - sin(jumpT * 3.14f) * bobHeight;
        }

        if (cutDKPos.x <= donkey.Position.x) {
            cutDKPos.x = donkey.Position.x;
            cutDKPos.y = baseY;
            cutPhase = PHASE_ROAR;
            cutTimer = 0.0f;
        }

        DrawTextureRec(cutDKTexture, walkFrame, cutDKPos, WHITE);
    }

    // ---- FASE 3: ROAR ----
    else if (cutPhase == PHASE_ROAR) {
        
        if (!IsSoundPlaying(cutSound) && !IsSoundPlaying(introSound))
            PlaySound(cutSound);
        
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
        DrawTextureRec(cutDKTexture, roarFrame, cutDKPos, WHITE); // sigue dibujando
        if (ladyVisible)
            DrawTextureRec(lady.Texture, { 1.0f, 1.0f, 14.0f, 22.0f }, ladyFixedPos, WHITE);

        if (cutTimer >= 0.5f) {
            UnloadTexture(cutDKTexture);
            UnloadTexture(cutStairsTexture);
            UnloadTexture(cutTrussTexture);
            UnloadSound(introSound);
            UnloadSound(stompSound);
            UnloadSound(cutSound);
            lady.Unload();
            Scene_Init = false;
            ChangeScene();
        }
}
}