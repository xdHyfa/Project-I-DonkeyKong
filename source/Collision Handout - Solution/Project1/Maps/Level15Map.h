#pragma once
#include "raylib.h"
#include "Entities/entity.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"

void Level15LadderSetter();

void Level15LadderDraw();

void Level15LadderCollisions(Entity& entity);

void Level15RampSetter();

void Level15RampDraw();

void Level15RampCollisions(Entity& entity);

void DrawLevel15Colliders();

int Level15CheckEntityPlatform(Entity& entity);

bool Level15CheckDownZone(Entity& entity);

void Level15CheckWinCondition(Entity& entity);

extern bool win15Triggered;

extern Sound stageCleared15Sound;

extern Music level15Music;
