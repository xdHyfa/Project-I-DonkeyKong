#include "raylib.h"
#include "Maps/Level3Map.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Core/constants.h"
#include "Entities/entity.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include "Core/UI.h"
#include <iostream>
using namespace std;

#define TrussHeight 16.0f

// -----------------------------------------------------------------------
//  PLATFORM LAYOUT  (matches whiteboard sketch, bottom -> top)
//  Screen: 224 x 256 px
//
//  Floor 0 (ground)  : x=0..224   (14 trusses) – full width
//  Platform 1        : x=0..80    ( 5 trusses) – short, LEFT side only
//  Platform 2        : x=16..208  (12 trusses) – wide, slight gap on left
//  Platform 3        : x=0..112   ( 7 trusses) – left half
//  Platform 4        : x=96..224  ( 8 trusses) – right half
//  Top platform      : x=112..224 ( 7 trusses) – top-right, DK + Lady + Star
// -----------------------------------------------------------------------

int L3_Y0, L3_Y1, L3_Y2, L3_Y3, L3_Y4, L3_Y5;

Truss L3_Base0[14];
Truss L3_Base1[5];
Truss L3_Base2[12];
Truss L3_Base3[7];
Truss L3_Base4[8];
Truss L3_Base5[7];

// Collision zones – ALL ZERO (disabled) until visual pass is approved
Rectangle L3_Zone0 = { 0, 0, 0, 0 };
Rectangle L3_Zone1 = { 0, 0, 0, 0 };
Rectangle L3_Zone2 = { 0, 0, 0, 0 };
Rectangle L3_Zone3 = { 0, 0, 0, 0 };
Rectangle L3_Zone4 = { 0, 0, 0, 0 };
Rectangle L3_Zone5 = { 0, 0, 0, 0 };

// Win condition
bool level3WinTriggered = false;
float level3WinTimer = 0.0f;

// Star hitbox – DISABLED (zero size)
Rectangle L3_StarHitbox = { 0, 0, 0, 0 };

void Level3CheckWinCondition(Entity& entity) {
    // DISABLED – re-enable after layout is approved
    (void)entity;
}

// -----------------------------------------------------------------------
//  RAMP SETUP
// -----------------------------------------------------------------------
void Level3RampSetter() {
    Truss::LoadSharedTexture(2);   // blue truss style (matches Level 2)

    // Floor 0 – full width
    L3_Y0 = (int)(SCREEN_HEIGHT - TrussHeight - 1);
    BaseSetter(L3_Base0, 14, 0, L3_Y0, false);

    // Platform 1 – short left shelf (~40 px above ground)
    L3_Y1 = L3_Y0 - 40;
    BaseSetter(L3_Base1, 5, 0, L3_Y1, false);

    // Platform 2 – wide (~80 px above ground)
    L3_Y2 = L3_Y0 - 80;
    BaseSetter(L3_Base2, 12, 16, L3_Y2, false);

    // Platform 3 – left half (~120 px above ground)
    L3_Y3 = L3_Y0 - 120;
    BaseSetter(L3_Base3, 7, 0, L3_Y3, false);

    // Platform 4 – right half (~160 px above ground)
    L3_Y4 = L3_Y0 - 160;
    BaseSetter(L3_Base4, 8, 96, L3_Y4, false);

    // Top platform – top-right, DK + Lady + Star (~200 px above ground)
    L3_Y5 = L3_Y0 - 200;
    BaseSetter(L3_Base5, 7, 112, L3_Y5, false);
}

void Level3RampDraw() {
    RampDrawer(L3_Base0, 14);
    RampDrawer(L3_Base1, 5);
    RampDrawer(L3_Base2, 12);
    RampDrawer(L3_Base3, 7);
    RampDrawer(L3_Base4, 8);
    RampDrawer(L3_Base5, 7);
}

// DISABLED – zones are zero, nothing fires
void Level3RampCollisions(Entity& entity) {
    (void)entity;
}

// DISABLED
int Level3CheckEntityPlatform(Entity& entity) {
    (void)entity;
    return -1;
}

// -----------------------------------------------------------------------
//  LADDERS
//
//  Reading the sketch bottom-to-top, left-to-right:
//   L0 : x~8,   ground     -> platform 1   (far left wall)
//   L1 : x~56,  platform 1 -> platform 2   (left-center)
//   L2 : x~16,  platform 2 -> platform 3   (left edge)
//   L3 : x~96,  platform 2 -> platform 3   (center)
//   L4 : x~96,  platform 3 -> platform 4   (center – overlaps platform gap)
//   L5 : x~176, platform 3 -> platform 4   (right side)
//   L6 : x~128, platform 4 -> top          (left of top platform)
//   L7 : x~192, platform 4 -> top          (right of top platform)
// -----------------------------------------------------------------------
Ladder L3_Ladders[8];

Ladder L3_Extra0[6], L3_Extra1[6], L3_Extra2[6], L3_Extra3[6];
Ladder L3_Extra4[6], L3_Extra5[6], L3_Extra6[6], L3_Extra7[6];

Rectangle L3_DownZone[8];   // kept for API compatibility, all zero

static void SetL3FullLadder(Ladder& ladder, Ladder* extras, float x, float y) {
    ladder.setPos(x, y);
    extras[0].setPos(ladder.Position.x, ladder.Position.y + 15);
    for (int i = 1; i < 6; i++) {
        extras[i].setPos(extras[i - 1].Position.x, extras[i - 1].Position.y + 3);
    }
}

void Level3LadderSetter() {
    Ladder::LoadSharedTexture();

    for (int i = 0; i < 8; i++) L3_Ladders[i].setSprite(4, true);

    Ladder* extras[8] = {
        L3_Extra0, L3_Extra1, L3_Extra2, L3_Extra3,
        L3_Extra4, L3_Extra5, L3_Extra6, L3_Extra7
    };
    for (int g = 0; g < 8; g++)
        for (int i = 0; i < 6; i++)
            extras[g][i].setSprite(4, false);

    // L0 – far left wall, ground -> platform 1
    SetL3FullLadder(L3_Ladders[0], L3_Extra0,
        8.0f,
        (float)L3_Y1 + TrussHeight);

    // L1 – left-center, platform 1 -> platform 2
    SetL3FullLadder(L3_Ladders[1], L3_Extra1,
        56.0f,
        (float)L3_Y2 + TrussHeight);

    // L2 – left edge, platform 2 -> platform 3
    SetL3FullLadder(L3_Ladders[2], L3_Extra2,
        16.0f,
        (float)L3_Y3 + TrussHeight);

    // L3 – center, platform 2 -> platform 3
    SetL3FullLadder(L3_Ladders[3], L3_Extra3,
        96.0f,
        (float)L3_Y3 + TrussHeight);

    // L4 – center, platform 3 -> platform 4
    SetL3FullLadder(L3_Ladders[4], L3_Extra4,
        96.0f,
        (float)L3_Y4 + TrussHeight);

    // L5 – right side, platform 3 -> platform 4
    SetL3FullLadder(L3_Ladders[5], L3_Extra5,
        176.0f,
        (float)L3_Y4 + TrussHeight);

    // L6 – left of top, platform 4 -> top
    SetL3FullLadder(L3_Ladders[6], L3_Extra6,
        128.0f,
        (float)L3_Y5 + TrussHeight);

    // L7 – right of top, platform 4 -> top
    SetL3FullLadder(L3_Ladders[7], L3_Extra7,
        192.0f,
        (float)L3_Y5 + TrussHeight);

    // ALL hitboxes DISABLED (zero size)
    for (int i = 0; i < 8; i++) {
        L3_Ladders[i].Hitbox = { 0, 0, 0, 0 };
    }

    // DownZones all zero (disabled)
    for (int i = 0; i < 8; i++) {
        L3_DownZone[i] = { 0, 0, 0, 0 };
    }
}

void Level3LadderDraw() {
    Ladder* extras[8] = {
        L3_Extra0, L3_Extra1, L3_Extra2, L3_Extra3,
        L3_Extra4, L3_Extra5, L3_Extra6, L3_Extra7
    };
    for (int i = 0; i < 8; i++) {
        DrawTextureRec(L3_Ladders[i].texture, L3_Ladders[i].SpriteSelector, L3_Ladders[i].Position, WHITE);
        for (int j = 0; j < 6; j++) {
            DrawTextureRec(extras[i][j].texture, extras[i][j].SpriteSelector, extras[i][j].Position, WHITE);
        }
    }
}

// DISABLED – hitboxes are zero
void Level3LadderCollisions(Entity& entity) {
    (void)entity;
}

// DISABLED
bool Level3CheckDownZone(Entity& entity) {
    (void)entity;
    return false;
}

// -----------------------------------------------------------------------
//  DEBUG DRAW – disabled, draws nothing
// -----------------------------------------------------------------------
void DrawLevel3Colliders() {
    // All disabled – uncomment each DrawRectangle when re-enabling collisions
}