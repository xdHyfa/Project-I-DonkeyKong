#include "Scenes/HowHigh.h"
#include "raylib.h"
#include "Scenes/scenes.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "Entities/Donkey.h"

Texture2D howHighDonko = { 0 };
float howHighTimer = 0.0f;
Sound howHighSound = { 0 };
Font howHighFont = { 0 };  // <-- añade esto

void runHowHigh() {
    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        howHighDonko = LoadTexture("Sprites/HOW HIGH CAN U GET.png");
        howHighFont = LoadFont("Fonts/donkey-kong-arcade-1981.otf");  // <-- carga la fuente
        howHighTimer = 0.0f;
        Scene_Init = true;
        howHighSound = LoadSound("Audio/How-High-Can-You-Get_-_Game-Start_.wav");
        PlaySound(howHighSound);
    }

    howHighTimer += GetFrameTime();

    if (!IsSoundPlaying(howHighSound) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        UnloadFont(howHighFont);   // <-- descarga la fuente
        UnloadSound(howHighSound);
        Scene_Init = false;
        ChangeScene();
        return;
    }

    float fontSize = 8.0f;
    float spacing = 1.0f;

    // "25 m"
    DrawTextEx(howHighFont, "25 m", { 60, 200 }, fontSize, spacing, WHITE);

    // "HOW HIGH CAN YOU GET?"
    Vector2 text2Size = MeasureTextEx(howHighFont, "HOW HIGH CAN YOU GET ?", fontSize, spacing);
    DrawTextEx(howHighFont, "HOW HIGH CAN YOU GET ?",
        { (SCREEN_WIDTH - text2Size.x) / 2, 215 }, fontSize, spacing, WHITE);

    // Dibuja DK
    DrawTexture(howHighDonko, 75, 170, WHITE);
    Rectangle dkRec = { 85.0f, 2.0f, 46.0f, 32.0f };
    DrawTextureRec(howHighDonko, dkRec, { (SCREEN_WIDTH - 46.0f) / 2, 58.0f }, WHITE);
}