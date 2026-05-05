#pragma once
#include "raylib.h"
#include "Entities/entity.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"

void Level3LadderSetter();
void Level3LadderDraw();
void Level3LadderCollisions(Entity& entity);

void Level3RampSetter();
void Level3RampDraw();
void Level3RampCollisions(Entity& entity);

void Level3CheckWinCondition(Entity& entity);

int Level3CheckEntityPlatform(Entity& entity);
bool Level3CheckDownZone(Entity& entity);

void DrawLevel3Colliders();

extern bool level3WinTriggered;
