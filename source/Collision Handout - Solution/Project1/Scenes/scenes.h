#pragma once

typedef enum Scene { INTRO, TITLE, LEVEL1, LEVEL2 } Scene;
extern bool Scene_Init;

void ChangeScene();

Scene GetCurrentScene();