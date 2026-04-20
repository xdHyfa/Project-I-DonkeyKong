#pragma once
#include "raylib.h"
#include "Entities/entity.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Entities/Barrel.h"



void Level2LadderSetter();

void Level2LadderDraw();

void Level2LadderCollisions(Entity &entity);

void Level2RampSetter();

void Level2RampDraw();

void Level2RampCollisions(Entity& entity);

void DrawLevel2Colliders();

int Level2CheckEntityPlatform(Entity& entity);

bool Level2CheckDownZone(Entity& entity);

void Level2ButtonSetter();

void Level2CheckButtons(Entity& entity);

void Level2ButtonsDraw();

void UnloadButtonTexture();

void ResetButtons();

void CheckWinCondition();

extern int Base_5_YPos;
