#include "raylib.h"
#include "InitialScreen.h"
#include "resource_dir.h"
#include "core/constants.h"
#include "scenes.h"




void runInitialScreen() {

    if (!Scene_Init) {
        // TO DO: RUN INITIAL SETTINGS (SET START POSITION, LOAD STUFF...)
        

    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        Scene_Init = false;
        current_scene = TITLE;

    }
  
    
   
}