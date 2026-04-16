#include "raylib.h"
#include "InitialScreen.h"
#include "include/resource_dir.h"
#include "core/constants.h"
#include "scenes.h"

//DEFINE TEXTURES

Texture LogoGrupo;
Texture Names;

//DEFINE SCALES 

Rectangle LOGO = { -30, 10, 400, 400 };
Rectangle sourceRec = { 0, 0, 457, 546 };
Rectangle NameRec = {
    70.0f,   // X: pegado a la izquierda
    140.0f,  // Y: ajústalo para que baje más (sube este número si sigue tapando)
    80.0f,  // Ancho: el original para que no se deforme
    90.0f   // Alto: el original
};

Vector2 origin = { 0.0f, 0.0f };

void runInitialScreen() {

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        SearchAndSetResourceDir("resources");
        LogoGrupo = LoadTexture("SPRITES/LOGOGRUPO.png");
        Names = LoadTexture("SPRITES/FULL NAMES.png");

        Scene_Init = true;


    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        Scene_Init = false;
        current_scene = TITLE;

    }
    DrawTextureRec(LogoGrupo, LOGO, { 22.0f, 64.0f }, WHITE);

    DrawTexturePro(Names, sourceRec, NameRec, origin, 0.0f, WHITE);
    
}