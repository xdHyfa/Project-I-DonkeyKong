#include "raylib.h"
#include "InitialScreen.h"
#include "resource_dir.h"
#include "core/constants.h"
#include "scenes.h"


Texture LogoGrupo;
Texture Names;
Rectangle LOGO = { 0, 0, 180, 88 };
Rectangle NAMES = { 85, 2, 46, 32 };


void runInitialScreen() {

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        LogoGrupo = LoadTexture("SPRITES/LOGOGRUPO.png");
        //Names = LoadTexture("SPRITES/MISSING PNG");
        Scene_Init = true;

    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        Scene_Init = false;
        current_scene = TITLE;

    }
    DrawTextureRec(LogoGrupo, LOGO, { 22.0f, 64.0f }, WHITE);
}