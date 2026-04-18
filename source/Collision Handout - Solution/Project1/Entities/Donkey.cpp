#include "Entities/Donkey.h"
#include "raylib.h"

Donkey donkey;

// Frames en orden: lanzamiento
// frame 0 -> ultima imagen fila 1  (41x32) x = 2*42, y = 0
// frame 1 -> primera imagen fila 1 (41x32) x = 0,    y = 0
// frame 2 -> segunda imagen fila 1 (42x32) x = 42,   y = 0

float frameWidths[3] = { 41.0f, 42.0f, 42.0f };
float frameOffsetsX[3] = { 182.0f, 3.0f, 48.0f };
float frameOffsetsY[3] = { 2.0f, 41.0f, 41.0f };

void Donkey::Setup()
{
    Texture = LoadTexture("Sprites/donko 2-0.png");
    frameRec = { frameOffsetsX[0], frameOffsetsY[0], frameWidths[0], 32.0f };
    isThrowing = false;
    frameIndex = 0;
    frameTimer = 0.0f;
}

void Donkey::PlayThrowAnim()
{
    isThrowing = true;
    frameIndex = 0;
    frameTimer = 0.0f;
}

void Donkey::Update()
{
    if (!isThrowing) return;

    frameTimer += GetFrameTime();
    if (frameTimer >= frameInterval)
    {
        frameTimer = 0.0f;
        frameIndex++;

        if (frameIndex >= 3)
        {
            spawnBarrel = true; // <-- aquí, justo al terminar el último frame
            frameIndex = 0;
            isThrowing = false;
            frameRec = { 3.0f, 2.0f, 41.0f, 32.0f };
        }
        else
        {
            frameRec.x = frameOffsetsX[frameIndex];
            frameRec.y = frameOffsetsY[frameIndex];
            frameRec.width = frameWidths[frameIndex];
        }
    }
}

void Donkey::Draw()
{
    DrawTextureRec(Texture, frameRec, Position, WHITE);
}

void Donkey::Unload()
{
    UnloadTexture(Texture);
}