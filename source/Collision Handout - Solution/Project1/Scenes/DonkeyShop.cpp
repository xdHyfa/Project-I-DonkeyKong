#include "raylib.h"
#include "DonkeyShop.h"
#include "include/resource_dir.h"
#include "core/constants.h"
#include "scenes.h"
#include <iostream>
using namespace std;

Music DonkeyShop;
bool ChangeThingy = false;
int selection = 4; 
Font Shop_Font;
Vector2 TitlePos = { 40 , 50};
Vector2 Option1Pos = {30, 90};
Vector2 Option2Pos = { 120, 90 };
Vector2 Option3Pos = { 30, 130 };
Vector2 Option4Pos = { 120, 130 };
Vector2 ExitPos = { 80, 160 };

// 0 --> option 1, 1 --> option 2, 2 --> option 3, 3 --> option 4, 4 --> exit 


void runDonkeyShop(){
    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        // DEBUG
       DonkeyShop = LoadMusicStream("Audio/DonkeyShop.wav");
       Shop_Font = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
       DonkeyShop.looping = true;
       PlayMusicStream(DonkeyShop);
        Scene_Init = true;
    }
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
            
            break;
        case 1:
            
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        case 4:
            ChangeThingy = true;
            break;
        }
    }
    UpdateMusicStream(DonkeyShop);
    DrawTextEx(Shop_Font,"DONKEY SHOP", TitlePos, 10, 0.5f,  WHITE);
    if (ChangeThingy) {
        ChangeScene();
        UnloadMusicStream(DonkeyShop);

    }
}
