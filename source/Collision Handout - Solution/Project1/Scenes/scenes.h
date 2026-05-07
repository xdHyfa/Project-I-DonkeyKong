#pragma once
#include "Entities/entity.h"
typedef enum Scene { INTRO, TITLE, CUTSCENE, HOWHIGH, LEVEL1, WINCUTSCENE, HOWHIGH2, LEVEL2, WINCUTSCENE2, HIGHSCORE } Scene;
extern bool Scene_Init;

void CheckTwoPlayers();

bool GetTwoPlayers();

void ChangeScene();

void ChangeScene(bool NoLives);

void StartHammerTime(int playerNum);   // 1 = Mario, 2 = Luigi
bool GetHammerTime(int playerNum);
void StopHammerTime(int playerNum);

// Legacy overloads (Player.cpp usa estos internamente via this->PlayerNum)
void StartHammerTime();
bool GetHammerTime();
void StopHammerTime();

bool GetIsKilling();

void StartEntityDeath(Entity& entity);

void PlayEntityDeath();

Scene GetCurrentScene();