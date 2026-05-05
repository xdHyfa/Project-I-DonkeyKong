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
//  PLATFORM LAYOUT  (6 horizontal floors, bottom -> top)
//
//  Floor 0 (ground)  :  full width,  y = SCREEN_HEIGHT - 17
//  Floor 1           :  x 0..207,    y = ~220
//  Floor 2           :  x 16..207,   y = ~185
//  Floor 3           :  x 0..191,    y = ~150
//  Floor 4           :  x 16..191,   y = ~115
//  Floor 5 (top)     :  x 56..167,   y = ~72   (DK + Lady platform)
// -----------------------------------------------------------------------

// Zone rectangles used for fast broad-phase per-entity platform detection.
// They are intentionally a bit taller (22px) so a moving entity doesn't slip through.
Rectangle L3_Zone0 = { 0,   SCREEN_HEIGHT - 22,          SCREEN_WIDTH,      22 };
Rectangle L3_Zone1 = { 0,   SCREEN_HEIGHT - 22 - 36,     SCREEN_WIDTH - 16, 22 };
Rectangle L3_Zone2 = { 16,  SCREEN_HEIGHT - 22 - 71,     SCREEN_WIDTH - 16, 22 };
Rectangle L3_Zone3 = { 0,   SCREEN_HEIGHT - 22 - 106,    SCREEN_WIDTH - 32, 22 };
Rectangle L3_Zone4 = { 16,  SCREEN_HEIGHT - 22 - 141,    SCREEN_WIDTH - 32, 22 };
Rectangle L3_Zone5 = { 56,  SCREEN_HEIGHT - 22 - 184,    112,               22 };

// Truss arrays  (1 Truss = 16px wide)
Truss L3_Base0[14];   // floor
Truss L3_Base1[13];   // platform 1  (x starts 0)
Truss L3_Base2[13];   // platform 2  (x starts 16)
Truss L3_Base3[13];   // platform 3  (x starts 0)
Truss L3_Base4[11];   // platform 4  (x starts 16)
Truss L3_Base5[7];    // top         (x starts 56)

// Y positions (top of trusses, computed once in RampSetter)
int L3_Y0, L3_Y1, L3_Y2, L3_Y3, L3_Y4, L3_Y5;

// --------------- Win condition ---------------
bool level3WinTriggered = false;
float level3WinTimer    = 0.0f;

// Star pickup hitbox – placed just below DK on the top platform.
// Exact pixel position: horizontally centred on the top platform (~x=100),
// vertically one sprite-height above the truss surface.
Rectangle L3_StarHitbox = { 95, 0, 14, 14 };   // y filled in RampSetter

void Level3CheckWinCondition(Entity& entity) {
    if (entity.tag != EntityTag::PLAYER) return;
    if (level3WinTriggered) {
        level3WinTimer += GetFrameTime();
        if (level3WinTimer >= 2.0f) {
            level3WinTriggered = false;
            level3WinTimer     = 0.0f;
            ChangeScene();   // -> HOWHIGH2
        }
        return;
    }
    if (CheckCollisionPointRec(entity.FloorCollider, L3_StarHitbox)) {
        level3WinTriggered = true;
        level3WinTimer     = 0.0f;
    }
}

// -----------------------------------------------------------------------
//  RAMP SETUP
// -----------------------------------------------------------------------
void Level3RampSetter() {
    // Use Blue truss sprite (Level 2 style, index 2)
    Truss::LoadSharedTexture(2);

    // Floor 0 – full width, flat
    L3_Y0 = (int)((SCREEN_HEIGHT - TrussHeight) - 1);
    BaseSetter(L3_Base0, 14, 0, L3_Y0, false);

    // Platform 1
    L3_Y1 = (int)((SCREEN_HEIGHT - TrussHeight) - 36);
    BaseSetter(L3_Base1, 13, 0, L3_Y1, false);
    L3_Base1[4].hasLadderBelow  = true;
    L3_Base1[10].hasLadderBelow = true;

    // Platform 2
    L3_Y2 = (int)((SCREEN_HEIGHT - TrussHeight) - 71);
    BaseSetter(L3_Base2, 13, 16, L3_Y2, false);
    L3_Base2[0].hasLadderBelow  = true;
    L3_Base2[6].hasLadderBelow  = true;
    L3_Base2[12].hasLadderBelow = true;

    // Platform 3
    L3_Y3 = (int)((SCREEN_HEIGHT - TrussHeight) - 106);
    BaseSetter(L3_Base3, 13, 0, L3_Y3, false);
    L3_Base3[3].hasLadderBelow  = true;
    L3_Base3[9].hasLadderBelow  = true;

    // Platform 4
    L3_Y4 = (int)((SCREEN_HEIGHT - TrussHeight) - 141);
    BaseSetter(L3_Base4, 11, 16, L3_Y4, false);
    L3_Base4[1].hasLadderBelow  = true;
    L3_Base4[5].hasLadderBelow  = true;
    L3_Base4[9].hasLadderBelow  = true;

    // Top platform (DK + Lady + Star)
    L3_Y5 = (int)((SCREEN_HEIGHT - TrussHeight) - 184);
    BaseSetter(L3_Base5, 7, 56, L3_Y5, false);
    L3_Base5[0].hasLadderBelow = true;
    L3_Base5[6].hasLadderBelow = true;

    // Update star hitbox y to sit just above the top platform surface
    L3_StarHitbox.y = (float)(L3_Y5 + 8 - 16);   // one sprite above surface
}

void Level3RampDraw() {
    RampDrawer(L3_Base0, 14);
    RampDrawer(L3_Base1, 13);
    RampDrawer(L3_Base2, 13);
    RampDrawer(L3_Base3, 13);
    RampDrawer(L3_Base4, 11);
    RampDrawer(L3_Base5, 7);
}

void Level3RampCollisions(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone0)) {
        RampCollision(L3_Base0, 14, entity);
    } else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1)) {
        RampCollision(L3_Base1, 13, entity);
    } else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone2)) {
        RampCollision(L3_Base2, 13, entity);
    } else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3)) {
        RampCollision(L3_Base3, 13, entity);
    } else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone4)) {
        RampCollision(L3_Base4, 11, entity);
    } else if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone5)) {
        RampCollision(L3_Base5, 7, entity);
    } else {
        if (entity.tag == EntityTag::PLAYER) {
            Mario.isFalling = true;
        }
    }
}

int Level3CheckEntityPlatform(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone0)) return 0;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone1)) return 1;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone2)) return 2;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone3)) return 3;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone4)) return 4;
    if (CheckCollisionPointRec(entity.FloorCollider, L3_Zone5)) return 5;
    return -1;
}

// -----------------------------------------------------------------------
//  LADDERS
// -----------------------------------------------------------------------
// 8 main ladders connecting adjacent platforms
Ladder L3_Ladders[8];

// Each main ladder has 6 extra small pieces to fill the gap visually
Ladder L3_Extra0[6], L3_Extra1[6], L3_Extra2[6], L3_Extra3[6];
Ladder L3_Extra4[6], L3_Extra5[6], L3_Extra6[6], L3_Extra7[6];

Rectangle L3_DownZone[8];

static void SetL3FullLadder(Ladder& ladder, Ladder* extras, float x, float y) {
    ladder.setPos(x, y);
    extras[0].setPos(ladder.Position.x, ladder.Position.y + 15);
    for (int i = 1; i < 6; i++) {
        extras[i].setPos(extras[i-1].Position.x, extras[i-1].Position.y + 3);
    }
}

void Level3LadderSetter() {
    Ladder::LoadSharedTexture();

    // Sprite: level 2 style (index 4 in Ladders.h = sprite sheet offset 51)
    for (int i = 0; i < 8; i++) L3_Ladders[i].setSprite(4, true);

    Ladder* extras[8] = {
        L3_Extra0, L3_Extra1, L3_Extra2, L3_Extra3,
        L3_Extra4, L3_Extra5, L3_Extra6, L3_Extra7
    };
    for (int g = 0; g < 8; g++)
        for (int i = 0; i < 6; i++)
            extras[g][i].setSprite(4, false);

    // Ladder 0: floor -> platform1, left side  (near L3_Base1[4])
    SetL3FullLadder(L3_Ladders[0], L3_Extra0,
        L3_Base1[4].TrussPos.x + 4, L3_Base1[4].TrussPos.y + L3_Base1[4].TrussBox.height * 2);

    // Ladder 1: floor -> platform1, right side (near L3_Base1[10])
    SetL3FullLadder(L3_Ladders[1], L3_Extra1,
        L3_Base1[10].TrussPos.x + 4, L3_Base1[10].TrussPos.y + L3_Base1[10].TrussBox.height * 2);

    // Ladder 2: platform1 -> platform2, left  (near L3_Base2[0])
    SetL3FullLadder(L3_Ladders[2], L3_Extra2,
        L3_Base2[0].TrussPos.x + 4, L3_Base2[0].TrussPos.y + L3_Base2[0].TrussBox.height * 2);

    // Ladder 3: platform1 -> platform2, mid   (near L3_Base2[6])
    SetL3FullLadder(L3_Ladders[3], L3_Extra3,
        L3_Base2[6].TrussPos.x + 4, L3_Base2[6].TrussPos.y + L3_Base2[6].TrussBox.height * 2);

    // Ladder 4: platform2 -> platform3, left  (near L3_Base3[3])
    SetL3FullLadder(L3_Ladders[4], L3_Extra4,
        L3_Base3[3].TrussPos.x + 4, L3_Base3[3].TrussPos.y + L3_Base3[3].TrussBox.height * 2);

    // Ladder 5: platform2 -> platform3, right (near L3_Base3[9])
    SetL3FullLadder(L3_Ladders[5], L3_Extra5,
        L3_Base3[9].TrussPos.x + 4, L3_Base3[9].TrussPos.y + L3_Base3[9].TrussBox.height * 2);

    // Ladder 6: platform3 -> platform4, left  (near L3_Base4[1])
    SetL3FullLadder(L3_Ladders[6], L3_Extra6,
        L3_Base4[1].TrussPos.x + 4, L3_Base4[1].TrussPos.y + L3_Base4[1].TrussBox.height * 2);

    // Ladder 7: platform4 -> top, right side  (near L3_Base5[0])
    SetL3FullLadder(L3_Ladders[7], L3_Extra7,
        L3_Base5[0].TrussPos.x + 4, L3_Base5[0].TrussPos.y + L3_Base5[0].TrussBox.height * 2);

    // Extend hitboxes so Mario can grab on from a platform
    for (int i = 0; i < 8; i++) {
        L3_Ladders[i].Hitbox.y      += 1;
        L3_Ladders[i].Hitbox.height += 16;
    }

    // Down zones (for entities that can descend)
    for (int i = 0; i < 8; i++) {
        L3_DownZone[i] = { L3_Ladders[i].Hitbox.x, L3_Ladders[i].Hitbox.y - 2, 10, 4 };
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

void Level3LadderCollisions(Entity& entity) {
    LadderCollisions(entity, L3_Ladders, 8);
}

bool Level3CheckDownZone(Entity& entity) {
    for (int i = 0; i < 8; i++) {
        if (CheckCollisionPointRec(entity.FloorCollider, L3_DownZone[i])) return true;
    }
    return false;
}

// -----------------------------------------------------------------------
//  DEBUG DRAW
// -----------------------------------------------------------------------
void DrawLevel3Colliders() {
    DrawRectangle(L3_Zone0.x, L3_Zone0.y, L3_Zone0.width, L3_Zone0.height, WHITE);
    DrawRectangle(L3_Zone1.x, L3_Zone1.y, L3_Zone1.width, L3_Zone1.height, WHITE);
    DrawRectangle(L3_Zone2.x, L3_Zone2.y, L3_Zone2.width, L3_Zone2.height, WHITE);
    DrawRectangle(L3_Zone3.x, L3_Zone3.y, L3_Zone3.width, L3_Zone3.height, WHITE);
    DrawRectangle(L3_Zone4.x, L3_Zone4.y, L3_Zone4.width, L3_Zone4.height, WHITE);
    DrawRectangle(L3_Zone5.x, L3_Zone5.y, L3_Zone5.width, L3_Zone5.height, WHITE);
    DrawRectangle(L3_StarHitbox.x, L3_StarHitbox.y, L3_StarHitbox.width, L3_StarHitbox.height, YELLOW);
    DrawLadderCollider(L3_Ladders, 8);
    for (int i = 0; i < 8; i++) {
        DrawRectangle(L3_DownZone[i].x, L3_DownZone[i].y, L3_DownZone[i].width, L3_DownZone[i].height, DARKPURPLE);
    }
}
