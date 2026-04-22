#include "Scenes/HowHigh.h"
#include "raylib.h"
#include "Scenes/scenes.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "Entities/Donkey.h"
#include "Core/UI.h"

Texture2D howHighDonko = { 0 };
float howHighTimer = 0.0f;
Sound howHighSound = { 0 };
Font howHighFont = { 0 };

void runHowHigh() {
    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        howHighDonko = LoadTexture("Sprites/HOW HIGH CAN U GET.png");
        howHighFont = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
        howHighTimer = 0.0f;
        Scene_Init = true;
        howHighSound = LoadSound("Audio/How-High-Can-You-Get_-_Game-Start_.wav");
        PlaySound(howHighSound);
    }

    howHighTimer += GetFrameTime();

    if (!IsSoundPlaying(howHighSound) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        UnloadFont(howHighFont);
        UnloadSound(howHighSound);
        Scene_Init = false;
        ChangeScene();
        return;
    }

    float fontSize = 8.0f;
    float spacing = 1.0f;
    int level = GetLevel();

    Rectangle dkRec = { 85.0f, 2.0f, 46.0f, 32.0f };
    float gap = 4.0f;
    float spriteH = 32.0f;

    for (int i = 0; i < level; i++) {
        float offsetY = (level - 1 - i) * (spriteH + gap);

        DrawTexture(howHighDonko, 75, 170 - (int)offsetY, WHITE);
        DrawTextureRec(howHighDonko, dkRec, { (SCREEN_WIDTH - 46.0f) / 2, 58.0f - offsetY }, WHITE);

        DrawTextEx(howHighFont, TextFormat("%d m", (level - i) * 25),
            { 45, 200.0f - offsetY }, fontSize, spacing, WHITE);
    }

    Vector2 text2Size = MeasureTextEx(howHighFont, "HOW HIGH CAN YOU GET ?", fontSize, spacing);
    DrawTextEx(howHighFont, "HOW HIGH CAN YOU GET ?",
        { (SCREEN_WIDTH - text2Size.x) / 2, 215 }, fontSize, spacing, WHITE);
}