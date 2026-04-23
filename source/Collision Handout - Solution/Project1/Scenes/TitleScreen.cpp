//#include "raylib.h"
//#include "TitleScreen.h"
//#include "include/resource_dir.h"
//#include "Core/constants.h"
//#include "scenes.h"
//#include "Core/UI.h"
//
//
//Texture DonkoTitleBlue;
//Texture DonkoTitleRed;
//Texture Donko;
//Rectangle rectitle = { 0, 0, 180, 88 };
//Rectangle recdonko = { 85, 2, 46, 32 };
//Font groupName = { 0 };
//Font year = { 0 };
//
//Sound titleSound = { 0 };
//
//float Titletick = 0.0f;
//bool Title_swap = false;
//float ticks = 0.0f;
//bool sub_swap = false;
//
//void PlayAnimation() {
//    Titletick += GetFrameTime();
//
//    if (Titletick >= 0.01f && !Title_swap) {
//        rectitle.y += 92;
//        Title_swap = true;
//        Titletick = 0;
//    }
//    if (Titletick >= 0.01f && Title_swap) {
//        rectitle.y -= 92;
//        Title_swap = false;
//        Titletick = 0;
//    }
//
//    ticks += GetFrameTime();
//
//    if (ticks >= 0.01f && !sub_swap) {
//        groupSize.y += 92;
//        sub_swap = true;
//        ticks = 0;
//    }
//    if (ticks >= 0.01f && sub_swap) {
//        groupName.y -= 92;
//        sub_swap = false;
//        ticks = 0;
//    }
//}
//
//void runMainMenu() {
//
//    if (!Scene_Init) {
//        SearchAndSetResourceDir("resources");
//        DonkoTitleBlue = LoadTexture("SPRITES/Main menu.png");
//        DonkoTitleRed = LoadTexture("SPRITES/Main menu.png");
//        Donko = LoadTexture("SPRITES/Donko 2-0.png");
//        titleSound = LoadSound("Audio/MusicScreen1.wav"); 
//        groupName = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
//        year = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
//        PlaySound(titleSound);
//        Scene_Init = true;
//    }
//
//    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
//        StopSound(titleSound);
//        UnloadFont(groupName);
//        UnloadFont(year);
//        UnloadSound(titleSound);
//        ChangeScene();
//        return;
//    }
//
//    DrawTextureRec(DonkoTitleBlue, rectitle, { 22.0f, 64.0f }, WHITE);
//    DrawTextureRec(DonkoTitleRed, rectitle, { 22.0f, 64.0f }, WHITE);
//    DrawTextureRec(Donko, recdonko, { 88.0f, 168.0f }, WHITE);
//
//    float fontSize = 8.0f;
//    float spacing = 0.8f;
//    Vector2 yearsize = MeasureTextEx(year, "2026", fontSize, spacing);
//    DrawTextEx(year, "2026",
//        { (SCREEN_WIDTH - yearsize.x) / 2, 215 }, fontSize, spacing, YELLOW);
//
//    Vector2 groupSize = MeasureTextEx(groupName, "Slopsquad", fontSize, spacing);
//    DrawTextEx(groupName, "Slopsquad",
//        { (SCREEN_WIDTH - groupSize.x) / 2, 225 }, fontSize, spacing, YELLOW);
//
//    Vector2 yearsize = MeasureTextEx(year, "2026", fontSize, spacing);
//    DrawTextEx(year, "2026",
//        { (SCREEN_WIDTH - yearsize.x) / 2, 215 }, fontSize, spacing, BLUE);
//
//    Vector2 groupSize = MeasureTextEx(groupName, "Slopsquad", fontSize, spacing);
//    DrawTextEx(groupName, "Slopsquad",
//        { (SCREEN_WIDTH - groupSize.x) / 2, 225 }, fontSize, spacing, BLUE);
//    
//    PrintUI();
//
//    PlayAnimation();
//}

#include "raylib.h"
#include "TitleScreen.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "scenes.h"
#include "Core/UI.h"

Texture DonkoTitleBlue;
Texture DonkoTitleRed;
Texture Donko;

Rectangle rectitle = { 0, 0, 180, 88 };
Rectangle recdonko = { 85, 2, 46, 32 };

Font groupName = { 0 };
Font year = { 0 };
Sound titleSound = { 0 };

float Titletick = 0.0f;
bool  Title_swap = false;
float ticks = 0.0f;
bool  sub_swap = false;
Color textColor = YELLOW;

void PlayAnimation() {
    // Animación sprite título
    Titletick += GetFrameTime();
    if (Titletick >= 0.001f && !Title_swap) {
        rectitle.y += 92;
        Title_swap = true;
        Titletick = 0;
    }
    else if (Titletick >= 0.01f && Title_swap) {
        rectitle.y -= 92;
        Title_swap = false;
        Titletick = 0;
    }

    // Alternar color cada 0.5 segundos
    ticks += GetFrameTime();
    if (ticks >= 0.01f) {
        textColor = sub_swap ? YELLOW : BLUE;
        sub_swap = !sub_swap;
        ticks = 0;
    }
}

void runMainMenu() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        DonkoTitleBlue = LoadTexture("SPRITES/Main menu.png");
        DonkoTitleRed = LoadTexture("SPRITES/Main menu Red.png");
        Donko = LoadTexture("SPRITES/Donko 2-0.png");
        titleSound = LoadSound("Audio/MusicScreen1.wav");
        groupName = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
        year = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
        PlaySound(titleSound);
        Scene_Init = true;
    }

    PlayAnimation();

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        StopSound(titleSound);
        UnloadTexture(DonkoTitleBlue);
        UnloadTexture(DonkoTitleRed);
        UnloadTexture(Donko);
        UnloadFont(groupName);
        UnloadFont(year);
        UnloadSound(titleSound);
        ChangeScene();
        return;
    }

    DrawTextureRec(DonkoTitleBlue, rectitle, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(DonkoTitleRed, rectitle, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(Donko, recdonko, { 88.0f, 168.0f }, WHITE);

    float fontSize = 8.0f;
    float spacing = 0.8f;

    Vector2 yearSize = MeasureTextEx(year, "2026", fontSize, spacing);
    DrawTextEx(year, "2026",
        { (SCREEN_WIDTH - yearSize.x) / 2, 215 }, fontSize, spacing, textColor);

    Vector2 groupSize = MeasureTextEx(groupName, "Slopsquad", fontSize, spacing);
    DrawTextEx(groupName, "Slopsquad",
        { (SCREEN_WIDTH - groupSize.x) / 2, 225 }, fontSize, spacing, textColor);

    PrintUI();
}