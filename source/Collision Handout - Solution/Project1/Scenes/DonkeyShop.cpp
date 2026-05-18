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
Vector2 Option1Pos = {30, 90};
Vector2 Option2Pos = { 120, 90 };
Vector2 Option3Pos = { 30, 130 };
Vector2 Option4Pos = { 120, 130 };
Vector2 ExitPos = { 95, 220 };

Texture2D DonkoDance;
Rectangle recDance = { 85, 2, 46, 32 };
float DanceTimer = 0.0f;

bool Customize1 = false;
bool Customize2 = false;
bool Customize3 = false;
bool Customize4 = false;

// 0 --> option 1, 1 --> option 2, 2 --> option 3, 3 --> option 4, 4 --> exit 

float hue = 0.0f;

void runDonkeyShop(){
    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        // DEBUG
       DonkeyShop = LoadMusicStream("Audio/DonkeyShop.wav");
       Shop_Font = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
       DonkeyShop.looping = true;
       ButtonSound = LoadSound("Audio/Button.wav");
       DonkoDance = LoadTexture("SPRITES/Donko 2-0.png");

       PlayMusicStream(DonkeyShop);
        Scene_Init = true;
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
        DrawTextEx(Shop_Font, "Option 1", Option1Pos, 10, 0.5f, YELLOW);
        DrawTextEx(Shop_Font, "Option 2", Option2Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 3", Option3Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 4", Option4Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10, 0.5f, WHITE);
        break;
    case 1:
        DrawTextEx(Shop_Font, "Option 1", Option1Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 2", Option2Pos, 10, 0.5f, YELLOW);
        DrawTextEx(Shop_Font, "Option 3", Option3Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 4", Option4Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10, 0.5f, WHITE);
        break;
    case 2:
        DrawTextEx(Shop_Font, "Option 1", Option1Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 2", Option2Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 3", Option3Pos, 10, 0.5f, YELLOW);
        DrawTextEx(Shop_Font, "Option 4", Option4Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10, 0.5f, WHITE);
        break;
    case 3:
        DrawTextEx(Shop_Font, "Option 1", Option1Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 2", Option2Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 3", Option3Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 4", Option4Pos, 10, 0.5f, YELLOW);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10, 0.5f, WHITE);
        break;
    case 4:
        DrawTextEx(Shop_Font, "Option 1", Option1Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 2", Option2Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 3", Option3Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Option 4", Option4Pos, 10, 0.5f, WHITE);
        DrawTextEx(Shop_Font, "Exit", ExitPos, 10, 0.5f, YELLOW);
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
            PlaySound(ButtonSound);
            Customize1 = !Customize1;
            break;
        case 1:
            PlaySound(ButtonSound);
            Customize2 = !Customize2;
            break;
        case 2:
            PlaySound(ButtonSound);
            Customize3 = !Customize3;
            break;
        case 3:
            PlaySound(ButtonSound);
            Customize4 = !Customize4;
            break;
        case 4:
            PlaySound(ButtonSound);
            ChangeThingy = true;
            break;
        }
    }
    UpdateMusicStream(DonkeyShop);
    DrawTextEx(Shop_Font,"DONKEY SHOP", TitlePos, 10, 0.5f, rainbow);
    if (ChangeThingy) {
        ChangeScene();
        UnloadSound(ButtonSound);
        UnloadMusicStream(DonkeyShop);

    }
}
