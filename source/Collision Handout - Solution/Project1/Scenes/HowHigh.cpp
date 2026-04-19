#include "Scenes/HowHigh.h"
#include "raylib.h"
#include "Scenes/scenes.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "Entities/Donkey.h"

Texture2D howHighDonko = { 0 };
float howHighTimer = 0.0f;
Sound howHighSound = { 0 };

void runHowHigh() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        howHighDonko = LoadTexture("Sprites/HOW HIGH CAN U GET.png");
        howHighTimer = 0.0f;
        Scene_Init = true;
        howHighSound = LoadSound("Audio/How-High-Can-You-Get_-_Game-Start_.wav");
        PlaySound(howHighSound);
    }

    howHighTimer += GetFrameTime();

    // Auto avanza después de audio
    if (!IsSoundPlaying(howHighSound) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        UnloadSound(howHighSound);
        Scene_Init = false;
        ChangeScene();
        return;
    }
    // Dibuja "25 m" y "HOW HIGH CAN YOU GET?"
    DrawText("25 m", 60, 200, 8, WHITE);

    int text2Width = MeasureText("HOW HIGH CAN YOU GET ?", 6);
    DrawText("HOW HIGH CAN YOU GET ?", (SCREEN_WIDTH - text2Width) / 2, 215, 6, WHITE);

    // Dibuja DK en el centro
    Rectangle dkRec = { 85.0f, 2.0f, 46.0f, 32.0f }; // mismo que en TitleScreen
    DrawTexture(howHighDonko, 75, 170, WHITE);

    DrawTextureRec(howHighDonko, dkRec, { (SCREEN_WIDTH - 46.0f) / 2, 58.0f }, WHITE);
}