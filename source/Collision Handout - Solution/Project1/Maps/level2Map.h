#pragma once
#include "raylib.h"
#include "Entities/entity.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Entities/Barrel.h"



void Level2LadderSetter();

void Level2LadderDraw();

void Level2LadderCollision(Entity &entity);

void Level2RampSetter();

void Level2RampDraw();

void Level2RampCollisions(Entity& entity);

void DrawLevel2Colliders();
