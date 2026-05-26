#include "raylib.h"
#include "DonkeyShop.h"
#include "include/resource_dir.h"
#include "core/constants.h"
#include "scenes.h"
#include <iostream>
using namespace std;

#define CYCLE_SPEED   10.0f   // How fast the color cycles (degrees/frame). Higher = faster.
#define SATURATION    1.0f   // Color vividness: 0.0 (gray) to 1.0 (fully vivid)
#define BRIGHTNESS    1.0f   // Color brightness: 0.0 (black) to 1.0 (full brightness)

Color GetRainbowColor(float* hue)
{
    *hue += CYCLE_SPEED;
    if (*hue >= 360.0f) *hue = 0.0f;
    return ColorFromHSV(*hue, SATURATION, BRIGHTNESS);
}

Music DonkeyShop;
Sound ButtonSound;
bool ChangeThingy = false;
int selection = 4; 
Font Shop_Font;
Vector2 TitlePos = { 40 , 30};
Vector2 Option1Pos = {5, 100};
Vector2 Option2Pos = { 115, 100 };
Vector2 Option3Pos = { 5, 135 };
Vector2 Option4Pos = { 115, 135 };
char Option1Text[15] = { "GOD MODE(500K)" };
char Option2Text[16] = { "HAMMER-UP(100K)" };
char Option3Text[20] = { "2P MODE(250K)" };
char Option4Text[18] = {"CUSTOM MUSIC(25K)"};
Vector2 ExitPos = { 95, 220 };

Texture2D DonkoDance;
Rectangle recDance = { 85, 2, 46, 32 };
float DanceTimer = 0.0f;
bool SecretCode = false;
float secretTimer = 0.0f;
Sound SecretSound = { 0 };
bool SecretActivated = false;

// 0 --> option 1, 1 --> option 2, 2 --> option 3, 3 --> option 4, 4 --> exit 

float hue = 0.0f;
int PointsCount = 0.0f;
Vector2 PointsPos = { 40 , 60 };
float PointsTimer = 0.0f;
bool ChangeColor = false;
void runDonkeyShop(){
    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        // DEBUG
       DonkeyShop = LoadMusicStream("Audio/DonkeyShop.wav");
       Shop_Font = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
       DonkeyShop.looping = true;
       ButtonSound = LoadSound("Audio/Button.wav");
       DonkoDance = LoadTexture("SPRITES/Donko 2-0.png");
       PointsCount = AllTimeScore;
       SecretSound = LoadSound("Audio/DonkeyMotif.wav");
       PlayMusicStream(DonkeyShop);
        Scene_Init = true;
    }

    if (IsKeyPressed(KEY_SIX)) {
        SecretCode = true;
        cout << "Six seven start" << endl;
    }
    if (SecretCode) {
        int pressedKey = GetKeyPressed();
        if (pressedKey == KEY_SEVEN) {
            AllTimeScore = 676767;
            PlaySound(SecretSound);
            SecretActivated = true;
        }
        else if (pressedKey != 0 && pressedKey != KEY_SIX){
            SecretCode = false;
            cout << "failed six seven" << endl;
            secretTimer = 0.0f;
        }
        secretTimer += GetFrameTime();
    }
    if (SecretCode && secretTimer > 1.5f) {
        SecretCode = false;
        secretTimer = 0.0f;
        cout << "failed six seven" << endl;
    }

    Color rainbow = GetRainbowColor(&hue);
    if (DanceTimer >= 0.5f) {
        recDance.width = -recDance.width;
        DanceTimer = 0.0f;
    }
    else {
        DanceTimer += GetFrameTime();
    }
    DrawTextureRec(DonkoDance, recDance, { 88.0f, 165.0f }, WHITE);
    switch (selection) {
    case 0:
        DrawTextEx(Shop_Font, Option1Text, Option1Pos, 7.0f, 0.01f, YELLOW);
        DrawTextEx(Shop_Font, Option2Text, Option2Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option3Text, Option3Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option4Text, Option4Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10.0f, 0.1f, WHITE);
        break;
    case 1:
        DrawTextEx(Shop_Font, Option1Text, Option1Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option2Text, Option2Pos, 7.0f, 0.01f, YELLOW);
        DrawTextEx(Shop_Font, Option3Text, Option3Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option4Text, Option4Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10.0f, 0.1f, WHITE);
        break;
    case 2:
        DrawTextEx(Shop_Font, Option1Text, Option1Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option2Text, Option2Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option3Text, Option3Pos, 7.0f, 0.01f, YELLOW);
        DrawTextEx(Shop_Font, Option4Text, Option4Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10.0f, 0.1f, WHITE);
        break;
    case 3:
        DrawTextEx(Shop_Font, Option1Text, Option1Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option2Text, Option2Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option3Text, Option3Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option4Text, Option4Pos, 7.0f, 0.01f, YELLOW);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10.0f, 0.1f, WHITE);
        break;
    case 4:
        DrawTextEx(Shop_Font, Option1Text, Option1Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option2Text, Option2Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option3Text, Option3Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, Option4Text, Option4Pos, 7.0f, 0.01f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10.0f, 0.1f, YELLOW);
        break;
    }
    if (IsKeyPressed(KEY_UP)) {
        if (selection > 1) selection -= 2;
        else selection = 4;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        if (selection < 2) selection += 2;
        else selection = 4;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        if (selection % 2 != 0 && selection != 4) selection--;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        if (selection % 2 == 0 && selection != 4) selection++;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selection) {
        case 0:

            if (!Option1True && AllTimeScore >= 500000) { PlaySound(ButtonSound); Option1True = true; AllTimeScore -= 500000; SaveScores(); }
            break;
        case 1:
            if (!Option2True && AllTimeScore >= 100000) { PlaySound(ButtonSound); Option2True = true; AllTimeScore -= 100000; SaveScores(); }
            break;
        case 2:
            if (!Option3True && AllTimeScore >= 250000) { PlaySound(ButtonSound); Option3True = true; AllTimeScore -= 250000; SaveScores(); }
            break;
        case 3:
            if (!Option4True && AllTimeScore >= 25000) { PlaySound(ButtonSound); Option4True = true; AllTimeScore -= 25000; SaveScores(); }
            break;
        case 4:
            PlaySound(ButtonSound);
            ChangeThingy = true;
            break;
        }
    }
    UpdateMusicStream(DonkeyShop);
    DrawTextEx(Shop_Font,"DONKEY SHOP", TitlePos, 10, 0.5f, rainbow);
    if (PointsTimer >= 0.05f) {
        PointsTimer = 0;
        ChangeColor = !ChangeColor;
    }
    else PointsTimer += GetFrameTime();
    if (ChangeColor)  DrawTextEx(Shop_Font, TextFormat("%06d POINTS", AllTimeScore), PointsPos, 10, 0.5f, YELLOW);
    else DrawTextEx(Shop_Font, TextFormat("%06d POINTS",AllTimeScore), PointsPos, 10, 0.5f, WHITE);
    if (ChangeThingy) {
        ChangeThingy = false;
        if (SecretActivated) {
            SecretActivated = false;
            AllTimeScore = 0;
            SaveScores();
        }
        UnloadSound(ButtonSound);
        UnloadSound(SecretSound);
        UnloadMusicStream(DonkeyShop);
        ChangeScene();
        return;
    }
}
