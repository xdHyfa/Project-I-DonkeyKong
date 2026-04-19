#include "Entities/Donkey.h"
#include "Entities/Barrel.h"
#include "raylib.h"
#include "Core/constants.h"

Donkey donkey;

// Frames en orden: lanzamiento
// frame 0 -> ultima imagen fila 1  (41x32) x = 2*42, y = 0
// frame 1 -> primera imagen fila 1 (41x32) x = 0,    y = 0
// frame 2 -> segunda imagen fila 1 (42x32) x = 42,   y = 0

float frameWidths[3] = { 38.0f, 42.0f, 42.0f };
float frameOffsetsX[3] = { 182.0f, 3.0f, 48.0f };
float frameOffsetsY[3] = { 2.0f, 41.0f, 41.0f };

void Donkey::Setup()
{
    Texture = LoadTexture("Sprites/donko 2-0.png");
    frameRec = { 3.0f, 2.0f, 38.0f, 32.0f }; 
    isThrowing = false;
    frameIndex = 0;
    frameTimer = 0.0f;
    barrelTexture = LoadTexture("sprites/commonbarrel.png");
    stairsTexture = LoadTexture("sprites/Stairs.png");
    oilTexture = LoadTexture("sprites/oil.png");

}

void Donkey::PlayThrowAnim()
{
    isThrowing = true;
    frameIndex = 0;
    frameTimer = 0.0f;
}

void Donkey::Update()
{
    if (isIdle)
    {
        idleTimer += GetFrameTime();
        frameRec = { 3.0f, 2.0f, 38.0f, 32.0f }; // frame idle
        if (idleTimer >= idleInterval)
        {
            idleTimer = 0.0f;
            isIdle = false;
            isThrowing = true;
            frameIndex = 0;
        }
        return;
    }

    if (!isThrowing) return;

    frameTimer += GetFrameTime();
    if (frameTimer >= frameInterval)
    {
        frameTimer = 0.0f;
        frameIndex++;

        if (frameIndex >= 3)
        {
            
            frameIndex = 0;
            isThrowing = false;
            isIdle = true; // vuelve al idle
            frameRec = { 3.0f, 2.0f, 38.0f, 32.0f };
        }
        else
        {
            frameRec.x = frameOffsetsX[frameIndex];
            frameRec.y = frameOffsetsY[frameIndex];
            frameRec.width = frameWidths[frameIndex];
        }
        if (frameIndex == 2) // último frame -> spawnea ya
        {
            spawnBarrel = true;
        }
    }
}

void Donkey::Draw()
{
    DrawTextureRec(Texture, frameRec, Position, WHITE);
    Rectangle barrelFrame = { 107.0f, 0.0f, 10.0f, 14.0f }; // ajusta w y h si hace falta

    DrawTextureRec(barrelTexture, barrelFrame, { 1.0f,  65.0f }, WHITE); // abajo izq
    DrawTextureRec(barrelTexture, barrelFrame, { 10.0f, 65.0f }, WHITE); // abajo der
    DrawTextureRec(barrelTexture, barrelFrame, { 1.0f,  51.0f }, WHITE); // arriba izq
    DrawTextureRec(barrelTexture, barrelFrame, { 10.0f, 51.0f }, WHITE); // arriba der
    if (isThrowing && frameIndex == 1)
    {
        Rectangle barrelOverlay = { 3.0f, 3.0f, 18.0f, 10.0f }; // ajusta w y h
        Vector2 barrelPos = { Position.x + 10.0f, Position.y + 5.0f }; // ajusta posición
        DrawTextureRec(barrelTexture, barrelOverlay, barrelPos, WHITE);
    }

    DrawTextureRec(Texture, frameRec, Position, WHITE);

    Rectangle stairRec = { 3.0f, 16.0f, 10.0f, 15.0f };
    
    // Escalera 1
    DrawTextureRec(stairsTexture, stairRec, { 64.0f, 64.0f }, WHITE);
    DrawTextureRec(stairsTexture, stairRec, { 64.0f, 49.0f }, WHITE);
    DrawTextureRec(stairsTexture, stairRec, { 64.0f, 34.0f }, WHITE);
    DrawTextureRec(stairsTexture, stairRec, { 64.0f, 22.0f }, WHITE);

    // Escalera 2
    DrawTextureRec(stairsTexture, stairRec, { 80.0f, 64.0f }, WHITE);
    DrawTextureRec(stairsTexture, stairRec, { 80.0f, 49.0f }, WHITE);
    DrawTextureRec(stairsTexture, stairRec, { 80.0f, 34.0f }, WHITE);
    DrawTextureRec(stairsTexture, stairRec, { 80.0f, 22.0f }, WHITE);

    Rectangle oilRec = { 0.0f, 0.0f, 16.0f, 16.0f };
    DrawTextureRec(oilTexture, oilRec, { 22.0f, 231.0f }, WHITE);

}

void Donkey::Unload()
{
    UnloadTexture(stairsTexture);
    UnloadTexture(Texture);
    UnloadTexture(oilTexture);
    UnloadTexture(barrelTexture);
}