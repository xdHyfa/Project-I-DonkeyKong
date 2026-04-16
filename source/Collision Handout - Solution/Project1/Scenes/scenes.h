#pragma once

typedef enum Scene { INTRO, TITLE, LEVEL1, LEVEL2 } Scene;
extern Scene current_scene;
extern bool Scene_Init;