#include "raylib.h"
#include "InitialScreen.h"
#include "include/resource_dir.h"
#include "core/constants.h"
#include "scenes.h"

//DEFINE TEXTURES

Texture LogoGrupo;
Texture Names1;
Texture Names2;
Texture Names3;
Texture Names4;
Texture TutorName;
Texture UNI;
Texture PROJE;

//DEFINE SCALES 

Rectangle LOGO = { 0, 0, 180, 88 };
Rectangle NAME1 = { 85, 20, 46, 32 };
Rectangle NAME2 = { 105, 20, 46, 32 };
Rectangle NAME3 = { 125, 20, 46, 32 };
Rectangle NAME4 = { 145, 20, 46, 32 };
Rectangle TutorN = { 175, 20, 46, 32 };
Rectangle UNIV = { 205, 110, 46, 32 };
Rectangle PROJ = { 230, 110, 46, 32 };






void runInitialScreen() {

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        LogoGrupo = LoadTexture("SPRITES/LOGOGRUPO.png");
        Names1 = LoadTexture("SPRITES/Name L.png");
        Names2 = LoadTexture("SPRITES/Name M.png");
        Names3 = LoadTexture("SPRITES/Name N.png");
        Names4 = LoadTexture("SPRITES/Name D.png");
        TutorName = LoadTexture("SPRITES/Name Tutor.png");
        UNI = LoadTexture("SPRITES/UPC - CITM");
        PROJE = LoadTexture("SPRITES/PROJECT 1");

        //Names = LoadTexture("SPRITES/MISSING PNG");
        Scene_Init = true;

    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        Scene_Init = false;
        current_scene = TITLE;

    }
    DrawTextureRec(LogoGrupo, LOGO, { 22.0f, 64.0f }, WHITE);

    DrawTextureRec(Names1, NAME1, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(Names2, NAME2, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(Names3, NAME3, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(Names4, NAME4, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(TutorName, TutorN, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(UNI, UNIV, { 22.0f, 64.0f }, WHITE);
    DrawTextureRec(PROJE, PROJ, { 22.0f, 64.0f }, WHITE);
}