#include "raylib.h"
#include "DonkeyShop.h"
#include "include/resource_dir.h"
#include "core/constants.h"
#include "scenes.h"
#include <iostream>
using namespace std;

Music DonkeyShop;
bool ChangeThingy = false;

void runDonkeyShop(){
    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        // DEBUG
       DonkeyShop = LoadMusicStream("Audio/DonkeyShop.wav");
       DonkeyShop.looping = true;
       PlayMusicStream(DonkeyShop);
        Scene_Init = true;
    }
    UpdateMusicStream(DonkeyShop);
    DrawText("DONKEY SHOP", 20, 50, 20, WHITE);
    if (ChangeThingy) {
        ChangeScene();
        UnloadMusicStream(DonkeyShop);

    }
}
