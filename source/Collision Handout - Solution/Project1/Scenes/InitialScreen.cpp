#include "raylib.h"
#include "InitialScreen.h"
#include "include/resource_dir.h"
#include "core/constants.h"
#include "scenes.h"
#include <iostream>


//DEFINE TEXTURES

Texture LogoGrupo;
Texture Names;

//DEFINE SCALES 

Rectangle LOGO = { -30, 10, 400, 400 };
Rectangle sourceRec = { 0, 0, 560, 720 };
Rectangle NameRec = {
    70.0f,   // X: pegado a la izquierda
    140.0f,  // Y: ajústalo para que baje más (sube este número si sigue tapando)
    80.0f,  // Ancho: el original para que no se deforme
    100.0f   // Alto: el original
};

Vector2 origin = { 0.0f, 20.0f };
Sound DonkeyMotif = { 0 };


void runInitialScreen() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        DonkeyMotif = LoadSound("Audio/DonkeyMotif.wav");

        // DEBUG
        if (DonkeyMotif.frameCount == 0) {
            std::cout << "SOUND FAILED TO LOAD - frameCount is 0" << std::endl;
        }
        else {
            std::cout << "Sound loaded OK - frameCount: " << DonkeyMotif.frameCount << std::endl;
        }

        LogoGrupo = LoadTexture("SPRITES/LOGOGRUPO.png");
        Names = LoadTexture("Sprites/nombre2.png");
        Scene_Init = true;
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        ChangeScene();
        if (!IsSoundPlaying(DonkeyMotif)) {
            PlaySound(DonkeyMotif);
        }

    }
    DrawTextureRec(LogoGrupo, LOGO, { 22.0f, 34.0f }, WHITE);

    DrawTexturePro(Names, sourceRec, NameRec, origin, 0.0f, WHITE);

   
}