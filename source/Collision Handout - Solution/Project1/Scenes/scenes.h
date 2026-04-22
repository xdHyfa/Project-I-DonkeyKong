#pragma once

typedef enum Scene { INTRO, TITLE, CUTSCENE, HOWHIGH, LEVEL1, WINCUTSCENE, HOWHIGH2, LEVEL2 } Scene;
extern bool Scene_Init;

void ChangeScene();

void ChangeScene(bool NoLives);

void StartHammerTime();

bool GetHammerTime();

void StopHammerTime();

Scene GetCurrentScene();