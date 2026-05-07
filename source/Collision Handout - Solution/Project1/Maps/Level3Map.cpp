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
//  Matches the whiteboard design:
//
//  Floor 0  : x=0..224  (14 trusses) full width              y = ground
//  Platform1L: x=0..96  ( 6 trusses) left side               y = ground-40
//  Platform1R: x=128..224( 6 trusses) right side             y = ground-40
//  Platform2 : x=64..192 ( 8 trusses) central                y = ground-80
//  Platform3L: x=0..80  ( 5 trusses) left portion            y = ground-120
//  Platform3R: x=96..208 ( 7 trusses) right portion          y = ground-120
//  Platform4 : x=112..224( 7 trusses) upper-right, STAR here y = ground-160
//  Platform7 : x=0..80  ( 5 trusses) upper-left, DK here     y = ground-160
//  Top (P5)  : x=112..224( 7 trusses) topmost-right, Lady    y = ground-200
//
//  Ladders (matching whiteboard left->right, bottom->top):
//   L0: x=8,   ground -> P1L       (far left)
//   L1: x=48,  P1L    -> P3L       (left mid)
//   L2: x=32,  P3L    -> P7        (left top)
//   L3: x=160, P1R    -> P2        (right low)
//   L4: x=144, P2     -> P3R       (right mid)
//   L5: x=160, P3R    -> P4        (right high)
// -----------------------------------------------------------------------

// Platform Y positions – declared at file scope so ALL functions can see them
int L3_Y0;   // ground / floor
int L3_Y1;   // P1L and P1R  (ground-40)
int L3_Y2;   // P2 central   (ground-80)
int L3_Y3;   // P3L and P3R  (ground-120)
int L3_Y4;   // P4 upper-right + Star  (ground-160)
int L3_Y5;   // Top right / Lady       (ground-200)
int L3_Y7;   // P7 upper-left / DK     (ground-200, same row as Top)

// Truss arrays
Truss L3_Base0[14];
Truss L3_Base1L[6];
Truss L3_Base1R[6];
Truss L3_Base2[8];
Truss L3_Base3L[5];
Truss L3_Base3R[7];
Truss L3_Base4[7];
Truss L3_Base5[7];   // Top-right (Lady platform)
Truss L3_Base7[5];   // Upper-left (DK platform)

// Collision zones
Rectangle L3_Zone0 = { 0,0,0,0 };
Rectangle L3_Zone1L = { 0,0,0,0 };
Rectangle L3_Zone1R = { 0,0,0,0 };
Rectangle L3_Zone2 = { 0,0,0,0 };
Rectangle L3_Zone3L = { 0,0,0,0 };
Rectangle L3_Zone3R = { 0,0,0,0 };
Rectangle L3_Zone4 = { 0,0,0,0 };
Rectangle L3_Zone5 = { 0,0,0,0 };
Rectangle L3_Zone7 = { 0,0,0,0 };

// Win state
bool  level3WinTriggered = false;
float level3WinTimer = 0.0f;

// Star – placed on P4 (upper-right platform), centred
Rectangle L3_StarHitbox = { 142, 0, 14, 14 };

// -----------------------------------------------------------------------
//  WIN CONDITION
// -----------------------------------------------------------------------
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

    // --- Platform Y positions ---
    L3_Y0 = (int)(SCREEN_HEIGHT - TrussHeight - 1);
    L3_Y1 = L3_Y0 - 40;
    L3_Y2 = L3_Y0 - 80;
    L3_Y3 = L3_Y0 - 120;
    L3_Y4 = L3_Y0 - 160;
    L3_Y5 = L3_Y0 - 200;   // Top-right (Lady)
    L3_Y7 = L3_Y0 - 160;   // Upper-left (DK) – same row as P4 (star)

    // --- Trusses ---
    // Floor
    BaseSetter(L3_Base0, 14, 0, L3_Y0, false);

    // P1L – left side, x=0..96
    BaseSetter(L3_Base1L, 6, 0, L3_Y1, false);

    // P1R – right side, x=128..224
    BaseSetter(L3_Base1R, 6, 128, L3_Y1, false);

    // P2 – central, x=64..192
    BaseSetter(L3_Base2, 8, 64, L3_Y2, false);

    // P3L – left, x=0..80
    BaseSetter(L3_Base3L, 5, 0, L3_Y3, false);

    // P3R – right, x=96..208
    BaseSetter(L3_Base3R, 7, 96, L3_Y3, false);

    // P4 – upper-right, x=112..224  (Star lives here)
    BaseSetter(L3_Base4, 7, 112, L3_Y4, false);

    // Top-right (Lady), x=112..224
    BaseSetter(L3_Base5, 7, 112, L3_Y5, false);

    // Upper-left (DK), x=0..80
    BaseSetter(L3_Base7, 5, 0, L3_Y7, false);

    // --- Collision zones (FloorCollider point must land inside these) ---
    // Zone height = 22: 6px above truss surface + 16px truss body
    L3_Zone0 = { 0, (float)(L3_Y0 - 6), 224, 22 };
    L3_Zone1L = { 0, (float)(L3_Y1 - 6),  96, 22 };
    L3_Zone1R = { 128, (float)(L3_Y1 - 6),  96, 22 };
    L3_Zone2 = { 64, (float)(L3_Y2 - 6), 128, 22 };
    L3_Zone3L = { 0, (float)(L3_Y3 - 6),  80, 22 };
    L3_Zone3R = { 96, (float)(L3_Y3 - 6), 112, 22 };
    L3_Zone4 = { 112, (float)(L3_Y4 - 6), 112, 22 };
    L3_Zone5 = { 112, (float)(L3_Y5 - 6), 112, 22 };
    L3_Zone7 = { 0, (float)(L3_Y7 - 6),  80, 22 };

    // Star sits on the surface of P4, centred around x=142
    L3_StarHitbox.y = (float)(L3_Y4 - 14);
    L3_StarHitbox.x = 142.0f;
}

// -----------------------------------------------------------------------
//  RAMP DRAW
// -----------------------------------------------------------------------
void Level3RampDraw() {
    RampDrawer(L3_Base0, 14);
    RampDrawer(L3_Base1L, 6);
    RampDrawer(L3_Base1R, 6);
    RampDrawer(L3_Base2, 8);
    RampDrawer(L3_Base3L, 5);
    RampDrawer(L3_Base3R, 7);
    RampDrawer(L3_Base4, 7);
    RampDrawer(L3_Base5, 7);
    RampDrawer(L3_Base7, 5);
}

// -----------------------------------------------------------------------
//  RAMP COLLISIONS
//  Order matters: check lower platforms first to avoid floor grabbing
//  overriding upper ones when zones overlap vertically.
// -----------------------------------------------------------------------
void Level3RampCollisions(Entity& entity) {
    // Floor first (widest, lowest)
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone0))  RampCollision(L3_Base0, 14, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1L)) RampCollision(L3_Base1L, 6, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1R)) RampCollision(L3_Base1R, 6, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone2))  RampCollision(L3_Base2, 8, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3L)) RampCollision(L3_Base3L, 5, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3R)) RampCollision(L3_Base3R, 7, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone4))  RampCollision(L3_Base4, 7, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone5))  RampCollision(L3_Base5, 7, entity);
    else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone7))  RampCollision(L3_Base7, 5, entity);
    else {
        if (entity.tag == EntityTag::PLAYER) Mario.isFalling = true;
    }
}

// Returns which platform an entity is standing on (-1 = none / air)
int Level3CheckEntityPlatform(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone0))  return 0;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1L)) return 1;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1R)) return 11;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone2))  return 2;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3L)) return 31;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3R)) return 32;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone4))  return 4;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone5))  return 5;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone7))  return 7;
    return -1;
}

// -----------------------------------------------------------------------
//  LADDERS  (6 active, indices 6-7 off-screen dummies)
// -----------------------------------------------------------------------
Ladder L3_Ladders[8];
Ladder L3_Extra0[6], L3_Extra1[6], L3_Extra2[6], L3_Extra3[6];
Ladder L3_Extra4[6], L3_Extra5[6], L3_Extra6[6], L3_Extra7[6];
Rectangle L3_DownZone[8];
static float ladderBottomY[8] = { 0 };  // bottom Y each ladder reaches; set in LadderSetter

// Builds one full ladder column: top rung + 6 inner rungs spaced 3px apart (original style)
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

    // topY = surface of the UPPER platform (L3_YN).
    // The ladder emerges at the platform surface and descends to the lower one.

    // L0 – far left: P1L -> ground  (x=13)
    SetL3FullLadder(L3_Ladders[0], L3_Extra0,
        13.0f, (float)L3_Y1);

    // L1 – left: P2 -> P1L  (x=69)
    SetL3FullLadder(L3_Ladders[1], L3_Extra1,
        69.0f, (float)L3_Y2);

    // L2 – REMOVED; hidden off-screen
    SetL3FullLadder(L3_Ladders[2], L3_Extra2,
        -100.0f, 0.0f);

    // L3 – right low: P2 -> P1R  (x=151)
    SetL3FullLadder(L3_Ladders[3], L3_Extra3,
        151.0f, (float)L3_Y2);

    // L4 – right mid: P3R -> P2  (x=132)
    SetL3FullLadder(L3_Ladders[4], L3_Extra4,
        132.0f, (float)L3_Y3);

    // L5 – right high: P4 -> P3R  (x=162)
    SetL3FullLadder(L3_Ladders[5], L3_Extra5,
        162.0f, (float)L3_Y4);

    // L6, L7 – unused (hidden off-screen)
    SetL3FullLadder(L3_Ladders[6], L3_Extra6, -100.0f, 0.0f);
    SetL3FullLadder(L3_Ladders[7], L3_Extra7, -100.0f, 0.0f);

    // bottomY = surface Y of the LOWER platform (L3_YN).
    // The draw loop stamps tiles from topY up to (but not including) bottomY,
    // so the last tile ends exactly at the platform surface – no gap, no overshoot.
    // The hitbox height is recomputed below to cover the full climbable gap.
    // L0: P1L(L3_Y1) -> ground(L3_Y0)
    ladderBottomY[0] = (float)L3_Y0;
    // L1: P2(L3_Y2) -> P1L(L3_Y1)
    ladderBottomY[1] = (float)L3_Y1;
    // L2: unused
    ladderBottomY[2] = 0.0f;
    // L3: P2(L3_Y2) -> P1R(L3_Y1)
    ladderBottomY[3] = (float)L3_Y1;
    // L4: P3R(L3_Y3) -> P2(L3_Y2)
    ladderBottomY[4] = (float)L3_Y2;
    // L5: P4(L3_Y4) -> P3R(L3_Y3)
    ladderBottomY[5] = (float)L3_Y3;
    // L6,L7: unused
    ladderBottomY[6] = 0.0f;
    ladderBottomY[7] = 0.0f;

    // Hitboxes: cover the full climbable gap from ladder top to platform surface below.
    // DownZone sits just above the ladder top so Mario can grab it from the platform above.
    for (int i = 0; i < 6; i++) {
        float height = ladderBottomY[i] - L3_Ladders[i].Position.y;
        if (height < 0) height = 0;
        L3_Ladders[i].Hitbox.y = L3_Ladders[i].Position.y;
        L3_Ladders[i].Hitbox.height = height;
        L3_DownZone[i] = {
            L3_Ladders[i].Hitbox.x,
            L3_Ladders[i].Position.y - 2,
            10, 4
        };
    }
    for (int i = 6; i < 8; i++) {
        L3_Ladders[i].Hitbox = { 0, 0, 0, 0 };
        L3_DownZone[i] = { 0, 0, 0, 0 };
    }
}

void Level3LadderDraw() {
    // Golden ladder sprite from Stairs.png:
    //   Full rung piece : x=51, y=16, w=10, h=16  – stamp every 16px at natural size
    //   Cap piece       : x=51, y=0,  w=10, h=3   – drawn once at top
    const Rectangle FULL_SRC = { 51.0f, 16.0f, 10.0f, 16.0f };
    const Rectangle CAP_SRC = { 51.0f,  0.0f, 10.0f,  3.0f };

    for (int i = 0; i < 8; i++) {
        if (L3_Ladders[i].Hitbox.width == 0) continue;  // off-screen dummy

        int x = (int)L3_Ladders[i].Position.x;
        int topY = (int)L3_Ladders[i].Position.y;
        int bottomY = (int)ladderBottomY[i];
        if (bottomY <= topY) continue;

        for (int ry = topY; ry < bottomY; ry += 16) {
            int remaining = bottomY - ry;
            Rectangle src = FULL_SRC;
            if (remaining < 16) src.height = (float)remaining;
            DrawTextureRec(L3_Ladders[i].texture, src, { (float)x, (float)ry }, WHITE);
        }

        DrawTextureRec(L3_Ladders[i].texture, CAP_SRC, { (float)x, (float)(topY - 3) }, WHITE);
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
//  DEBUG – draw all collision boxes. Call from newscene.cpp when needed.
// -----------------------------------------------------------------------
void DrawLevel3Colliders() {
    Color zoneCol = { 255, 255, 255, 60 };
    Color starCol = YELLOW;
    Color downCol = DARKPURPLE;

    DrawRectangleRec(L3_Zone0, zoneCol);
    DrawRectangleRec(L3_Zone1L, zoneCol);
    DrawRectangleRec(L3_Zone1R, zoneCol);
    DrawRectangleRec(L3_Zone2, zoneCol);
    DrawRectangleRec(L3_Zone3L, zoneCol);
    DrawRectangleRec(L3_Zone3R, zoneCol);
    DrawRectangleRec(L3_Zone4, zoneCol);
    DrawRectangleRec(L3_Zone5, zoneCol);
    DrawRectangleRec(L3_Zone7, zoneCol);

    DrawRectangleRec(L3_StarHitbox, starCol);

    DrawLadderCollider(L3_Ladders, 8);

    for (int i = 0; i < 8; i++)
        DrawRectangle((int)L3_DownZone[i].x, (int)L3_DownZone[i].y,
            (int)L3_DownZone[i].width, (int)L3_DownZone[i].height, downCol);

    // Mario FloorCollider (magenta dot)
    DrawCircle((int)Mario.FloorCollider.x, (int)Mario.FloorCollider.y, 2, MAGENTA);
}