#pragma once
#include "raylib.h"
#include "Entities/entity.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"

// -----------------------------------------------------------------------
//  Platform Y positions (used by Level3Entities.cpp for spawn heights)
// -----------------------------------------------------------------------
extern int L3_Y0;   // ground floor
extern int L3_Y1;   // P1L / P1R
extern int L3_Y2;   // P2 central
extern int L3_Y3;   // P3L / P3R
extern int L3_Y4;   // P4 upper-right  (Star here)
extern int L3_Y5;   // Top-right (Lady)
extern int L3_Y7;   // Upper-left (DK)

// -----------------------------------------------------------------------
//  Collision zones
// -----------------------------------------------------------------------
extern Rectangle L3_Zone0;
extern Rectangle L3_Zone1L;
extern Rectangle L3_Zone1R;
extern Rectangle L3_Zone2;
extern Rectangle L3_Zone3L;
extern Rectangle L3_Zone3R;
extern Rectangle L3_Zone4;
extern Rectangle L3_Zone5;
extern Rectangle L3_Zone7;

// -----------------------------------------------------------------------
//  Win state & Star hitbox
// -----------------------------------------------------------------------
extern bool      level3WinTriggered;
extern Rectangle L3_StarHitbox;

// -----------------------------------------------------------------------
//  Public API
// -----------------------------------------------------------------------
void Level3RampSetter();
void Level3RampDraw();
void Level3RampCollisions(Entity& entity);

void Level3LadderSetter();
void Level3LadderDraw();
void Level3LadderCollisions(Entity& entity);

void Level3CheckWinCondition(Entity& entity);
int  Level3CheckEntityPlatform(Entity& entity);
bool Level3CheckDownZone(Entity& entity);

void DrawLevel3Colliders();   // call each frame for debug overlay