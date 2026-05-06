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
//  PLATFORM LAYOUT  (224x256 screen, bottom -> top)
//
//  Floor 0  : x=0..224   (14 trusses) full width          y = ground
//  Platform1: x=0..112   ( 7 trusses) left side           y = ground-40
//  Platform2: x=80..208  ( 8 trusses) right portion only  y = ground-80
//  Platform4: x=112..224 ( 7 trusses) right side          y = ground-160
//  Top      : x=112..224 ( 7 trusses) top-right, DK+Lady  y = ground-200
//
//  Platform1R: x=112..224 (7 trusses) RIGHT mirror of P1    y = ground-40  [NEW]
//  Platform3L: x=0..80    (5 trusses) left portion          y = ground-120 [NEW]
//  Platform3R: x=80..176  (6 trusses) right portion         y = ground-120 [NEW]
//  Platform7:  x=0..80    (5 trusses) upper-left            y = ground-165 [NEW]
//
//  Ladders:
//   L0: x=8,   ground -> P1        (far left)               [KEPT]
//   L1: x=24,  P7     -> P3L       (left H ladder)          [NEW]
//   L2: x=128, Top    -> P4        (right top H)            [NEW]
//   L3: x=144, P4     -> P3R       (right mid H)            [NEW]
//   L4: x=128, P3R    -> P2        (right bottom H)         [NEW]
// -----------------------------------------------------------------------

int L3_Y0, L3_Y1, L3_Y2, L3_Y3, L3_Y4, L3_Y5;

Truss L3_Base0[14];
Truss L3_Base1[7];
Truss L3_Base1R[7];  // NEW: right mirror of P1, x=112..224
Truss L3_Base2[8];
Truss L3_Base3[6];   // kept in memory but NOT drawn/collided (P3 removed)
Truss L3_Base3L[5];  // NEW: P3 left portion  x=0..80
Truss L3_Base3R[6];  // NEW: P3 right portion x=80..176
Truss L3_Base4[7];
Truss L3_Base5[7];
Truss L3_Base6[4];   // kept in memory but NOT drawn/collided (P6 removed)
Truss L3_Base7[5];   // NEW: upper-left platform x=0..80

Rectangle L3_Zone0 = { 0,0,0,0 };
Rectangle L3_Zone1 = { 0,0,0,0 };
Rectangle L3_Zone1R = { 0,0,0,0 };  // NEW: right mirror of P1
Rectangle L3_Zone2 = { 0,0,0,0 };
Rectangle L3_Zone3 = { 0,0,0,0 };  // unused slot
Rectangle L3_Zone3L = { 0,0,0,0 };  // NEW: P3 left
Rectangle L3_Zone3R = { 0,0,0,0 };  // NEW: P3 right
Rectangle L3_Zone4 = { 0,0,0,0 };
Rectangle L3_Zone5 = { 0,0,0,0 };
Rectangle L3_Zone6 = { 0,0,0,0 };  // unused slot
Rectangle L3_Zone7 = { 0,0,0,0 };  // NEW: upper-left platform

bool level3WinTriggered = false;
float level3WinTimer = 0.0f;

Rectangle L3_StarHitbox = { 130, 0, 14, 14 };

void Level3CheckWinCondition(Entity& entity) {
    if (entity.tag != EntityTag::PLAYER) return;
    if (level3WinTriggered) {
        level3WinTimer += GetFrameTime();
        if (level3WinTimer >= 2.0f) {
            level3WinTriggered = false;
            level3WinTimer = 0.0f;
            ChangeScene();
        }
        return;
    }
    if (CheckCollisionPointRec(entity.FloorCollider, L3_StarHitbox)) {
        level3WinTriggered = true;
        level3WinTimer = 0.0f;
    }
}

// -----------------------------------------------------------------------
//  RAMP SETUP
// -----------------------------------------------------------------------
void Level3RampSetter() {
    Truss::LoadSharedTexture(2);

    // Floor 0 – full width
    L3_Y0 = (int)(SCREEN_HEIGHT - TrussHeight - 1);
    BaseSetter(L3_Base0, 14, 0, L3_Y0, false);

    // Platform 1 LEFT – left side, 7 trusses (x=0..112)
    L3_Y1 = L3_Y0 - 40;
    BaseSetter(L3_Base1, 7, 0, L3_Y1, false);

    // Platform 1 RIGHT – mirror of P1, 7 trusses (x=112..224)  [NEW]
    BaseSetter(L3_Base1R, 7, 112, L3_Y1, false);

    // Platform 2 – right portion only, 8 trusses (x=80..208)
    L3_Y2 = L3_Y0 - 80;
    BaseSetter(L3_Base2, 8, 80, L3_Y2, false);

    // Platform 3 Y reference (ground-120)
    L3_Y3 = L3_Y0 - 120;

    // Platform 3 LEFT – 5 trusses (x=0..80)  [NEW]
    BaseSetter(L3_Base3L, 5, 0, L3_Y3, false);

    // Platform 3 RIGHT – 6 trusses (x=80..176)  [NEW]
    BaseSetter(L3_Base3R, 6, 80, L3_Y3, false);

    // Platform 4 – right side, 7 trusses (x=112..224)
    L3_Y4 = L3_Y0 - 160;
    BaseSetter(L3_Base4, 7, 112, L3_Y4, false);

    // Top – right side, 7 trusses (x=112..224), DK + Lady + Star
    L3_Y5 = L3_Y0 - 200;
    BaseSetter(L3_Base5, 7, 112, L3_Y5, false);

    // Platform 7 – upper-left, 5 trusses (x=0..80)  [NEW]
    int L3_Y7 = L3_Y0 - 165;
    BaseSetter(L3_Base7, 5, 0, L3_Y7, false);

    // ---- Collision zones ----
    L3_Zone0 = { 0,   (float)(L3_Y0 - 6),  224,  22 };
    L3_Zone1 = { 0,   (float)(L3_Y1 - 6),  112,  22 };
    L3_Zone1R = { 112, (float)(L3_Y1 - 6),  112,  22 };  // right mirror
    L3_Zone2 = { 80,  (float)(L3_Y2 - 6),  128,  22 };
    L3_Zone3 = { 0,   0,                    0,    0 };  // unused
    L3_Zone3L = { 0,   (float)(L3_Y3 - 6),  80,   22 };
    L3_Zone3R = { 80,  (float)(L3_Y3 - 6),  96,   22 };
    L3_Zone4 = { 112, (float)(L3_Y4 - 6),  112,  22 };
    L3_Zone5 = { 112, (float)(L3_Y5 - 6),  112,  22 };
    L3_Zone6 = { 0,   0,                    0,    0 };  // unused
    L3_Zone7 = { 0,   (float)(L3_Y7 - 6),  80,   22 };

    // Star sits just above the top platform surface
    L3_StarHitbox.y = (float)(L3_Y5 - 14);
}

void Level3RampDraw() {
    RampDrawer(L3_Base0, 14);
    RampDrawer(L3_Base1, 7);
    RampDrawer(L3_Base1R, 7);  // right mirror of P1
    RampDrawer(L3_Base2, 8);
    RampDrawer(L3_Base3L, 5);  // P3 left
    RampDrawer(L3_Base3R, 6);  // P3 right
    RampDrawer(L3_Base4, 7);
    RampDrawer(L3_Base5, 7);
    RampDrawer(L3_Base7, 5);  // upper-left
}

void Level3RampCollisions(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone0))  RampCollision(L3_Base0, 14, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1))  RampCollision(L3_Base1, 7, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1R)) RampCollision(L3_Base1R, 7, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone2))  RampCollision(L3_Base2, 8, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3L)) RampCollision(L3_Base3L, 5, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3R)) RampCollision(L3_Base3R, 6, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone7))  RampCollision(L3_Base7, 5, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone4))  RampCollision(L3_Base4, 7, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone5))  RampCollision(L3_Base5, 7, entity);
    else {
        if (entity.tag == EntityTag::PLAYER) Mario.isFalling = true;
    }
}

int Level3CheckEntityPlatform(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone0))  return 0;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1))  return 1;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1R)) return 11;  // right mirror
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone2))  return 2;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3L)) return 31;  // P3 left
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3R)) return 32;  // P3 right
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone7))  return 7;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone4))  return 4;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone5))  return 5;
    return -1;
}

// -----------------------------------------------------------------------
//  LADDERS  (5 active, L5-L7 off-screen)
// -----------------------------------------------------------------------
Ladder L3_Ladders[8];
Ladder L3_Extra0[6], L3_Extra1[6], L3_Extra2[6], L3_Extra3[6];
Ladder L3_Extra4[6], L3_Extra5[6], L3_Extra6[6], L3_Extra7[6];
Rectangle L3_DownZone[8];

static void SetL3FullLadder(Ladder& ladder, Ladder* extras, float x, float y) {
    ladder.setPos(x, y);
    extras[0].setPos(ladder.Position.x, ladder.Position.y + 15);
    for (int i = 1; i < 6; i++)
        extras[i].setPos(extras[i - 1].Position.x, extras[i - 1].Position.y + 3);
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

    // L0 – far left wall, ground -> P1  (x=8)
    SetL3FullLadder(L3_Ladders[0], L3_Extra0,
        8.0f, (float)L3_Y1 + TrussHeight);

    // L1 – left H: P7 (upper-left) -> P3L  (x=24)  [NEW]
    SetL3FullLadder(L3_Ladders[1], L3_Extra1,
        24.0f, (float)L3_Y3 + TrussHeight);

    // L2 – right top H: Top -> P4  (x=128)  [NEW]
    SetL3FullLadder(L3_Ladders[2], L3_Extra2,
        128.0f, (float)L3_Y4 + TrussHeight);

    // L3 – right mid H: P4 -> P3R  (x=144)  [NEW]
    SetL3FullLadder(L3_Ladders[3], L3_Extra3,
        144.0f, (float)L3_Y3 + TrussHeight);

    // L4 – right bottom H: P3R -> P2  (x=128)  [NEW]
    SetL3FullLadder(L3_Ladders[4], L3_Extra4,
        128.0f, (float)L3_Y2 + TrussHeight);

    // L5, L6, L7 – unused
    SetL3FullLadder(L3_Ladders[5], L3_Extra5, -100.0f, 0.0f);
    SetL3FullLadder(L3_Ladders[6], L3_Extra6, -100.0f, 0.0f);
    SetL3FullLadder(L3_Ladders[7], L3_Extra7, -100.0f, 0.0f);

    // Enable hitboxes for active ladders (0..4)
    for (int i = 0; i < 5; i++) {
        L3_Ladders[i].Hitbox.y += 1;
        L3_Ladders[i].Hitbox.height += 16;
        L3_DownZone[i] = { L3_Ladders[i].Hitbox.x, L3_Ladders[i].Hitbox.y - 2, 10, 4 };
    }
    // Disable unused
    for (int i = 5; i < 8; i++) {
        L3_Ladders[i].Hitbox = { 0, 0, 0, 0 };
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
        for (int j = 0; j < 6; j++)
            DrawTextureRec(extras[i][j].texture, extras[i][j].SpriteSelector, extras[i][j].Position, WHITE);
    }
}

void Level3LadderCollisions(Entity& entity) {
    LadderCollisions(entity, L3_Ladders, 8);
}

bool Level3CheckDownZone(Entity& entity) {
    for (int i = 0; i < 8; i++)
        if (CheckCollisionPointRec(entity.FloorCollider, L3_DownZone[i])) return true;
    return false;
}

// -----------------------------------------------------------------------
//  DEBUG DRAW
// -----------------------------------------------------------------------
void DrawLevel3Colliders() {
    DrawRectangle(L3_Zone0.x, L3_Zone0.y, L3_Zone0.width, L3_Zone0.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone1.x, L3_Zone1.y, L3_Zone1.width, L3_Zone1.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone1R.x, L3_Zone1R.y, L3_Zone1R.width, L3_Zone1R.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone2.x, L3_Zone2.y, L3_Zone2.width, L3_Zone2.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone3L.x, L3_Zone3L.y, L3_Zone3L.width, L3_Zone3L.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone3R.x, L3_Zone3R.y, L3_Zone3R.width, L3_Zone3R.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone4.x, L3_Zone4.y, L3_Zone4.width, L3_Zone4.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone5.x, L3_Zone5.y, L3_Zone5.width, L3_Zone5.height, { 255,255,255,60 });
    DrawRectangle(L3_Zone7.x, L3_Zone7.y, L3_Zone7.width, L3_Zone7.height, { 255,255,255,60 });
    DrawRectangle(L3_StarHitbox.x, L3_StarHitbox.y, L3_StarHitbox.width, L3_StarHitbox.height, YELLOW);
    DrawLadderCollider(L3_Ladders, 8);
    for (int i = 0; i < 8; i++)
        DrawRectangle(L3_DownZone[i].x, L3_DownZone[i].y, L3_DownZone[i].width, L3_DownZone[i].height, DARKPURPLE);
}