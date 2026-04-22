#include "Entities/Lady.h"
#include "raylib.h"

Lady lady;

void Lady::Setup()
{
    Texture = LoadTexture("sprites/LADY.png");
    helpTexture = LoadTexture("sprites/HELP.png");
    frameRec = { 1.0f, 1.0f, 14.0f, 22.0f };
    animTimer = 0.0f;
    isAnimating = false;
    showHelp = false;
    Position = { 91.0f, 26.0f };
}

void Lady::Update()
{
    animTimer += GetFrameTime();

    if (!isAnimating)
    {
        frameRec = { 1.0f, 1.0f, 14.0f, 22.0f };
        showHelp = false;

        if (animTimer >= animInterval) // cada 3 segundos arranca la ráfaga
        {
            animTimer = 0.0f;
            isAnimating = true;
            showHelp = true;
        }
    }
    else
    {
        // ráfaga: alterna rápido entre frame base y frame animación
        int blinks = (int)(animTimer / 0.1f); // cambia cada 0.1 segundos
        if (blinks % 2 == 0)
            frameRec = { 17.0f, 1.0f, 14.0f, 22.0f };
        else
            frameRec = { 1.0f, 1.0f, 14.0f, 22.0f };

        if (animTimer >= animDuration) // duración total de la ráfaga
        {
            animTimer = 0.0f;
            isAnimating = false;
            frameRec = { 1.0f, 1.0f, 14.0f, 22.0f };
            showHelp = false;
        }
    }
}

void Lady::Draw()
{
    DrawTextureRec(Texture, frameRec, Position, WHITE);

    if (showHelp)
    {
        // HELP aparece arriba a la derecha de Lady
        Rectangle helpRec = { 0.0f, 0.0f, 23.0f, 8.0f };
        Vector2 helpPos = { Position.x + 10.0f, Position.y - 10.0f };
        DrawTextureRec(helpTexture, helpRec, helpPos, WHITE);
    }
}

void Lady::Unload()
{
    UnloadTexture(Texture);
    UnloadTexture(helpTexture);
}