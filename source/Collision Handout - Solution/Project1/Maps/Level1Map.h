#pragma once
#include "raylib.h"
#include "Entities/entity.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Entities/Barrel.h"

void Level1CheckWinCondition(Entity &entity);

void Level1LadderSetter();

void Level1LadderDraw();

void Level1LadderCollision(Entity &entity);

void Level1RampSetter();

void BarrelGroundCollisions(Barrel &barrel);

void Level1RampDraw();

void Level1RampCollisions(Entity& entity);

void DrawLevel1Colliders();
