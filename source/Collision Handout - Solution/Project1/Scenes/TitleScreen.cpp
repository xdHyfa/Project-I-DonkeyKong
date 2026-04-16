#include "raylib.h"
#include "TitleScreen.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "scenes.h"


Texture DonkoTitleBlue;
Texture DonkoTitleRed;
Texture Donko;
Rectangle rectitle = { 0, 0, 180, 88 };
Rectangle recdonko = { 85, 2, 46, 32 };


float Titletick = 0.0f;
bool Title_swap = false;

void PlayAnimation() {
    Titletick += GetFrameTime();

    if (Titletick >= 0.01f && !Title_swap) {
        rectitle.y += 92;
        Title_swap = true;
        Titletick = 0;
    }
    if (Titletick >= 0.01f && Title_swap) {
        rectitle.y -= 92;
        Title_swap = false;
        Titletick = 0;
    }
}

void runMainMenu() {

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        DonkoTitleBlue = LoadTexture("SPRITES/Main menu.png");
        DonkoTitleRed = LoadTexture("SPRITES/Main menu.png");
        Donko = LoadTexture("SPRITES/Donko 2-0.png");
        Scene_Init = true;

    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        ChangeScene();

    }
    DrawTextureRec(DonkoTitleBlue, rectitle, {22.0f, 64.0f}, WHITE);
    DrawTextureRec(DonkoTitleRed, rectitle, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(Donko, recdonko, { 88.0f, 168.0f }, WHITE);
    
    PlayAnimation();
}