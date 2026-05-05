#pragma once
#include "raylib.h"
#include "Entities/entity.h"

// -----------------------------------------------------------------------
//  GOOMBA
//  - Patrols left/right on its starting platform.
//  - Cannot change platform (no ladder use).
//  - Mario kills it by jumping on it (stomp from above).
// -----------------------------------------------------------------------
class Goomba : public Entity {
public:
    float speed      = 0.6f;
    float direction  = 1.0f;   // +1 right, -1 left
    float patrolLeft  = 0.0f;
    float patrolRight = 0.0f;

    bool  isDead     = false;
    float deathTimer = 0.0f;

    Texture2D texture2 = { 0 };   // second animation frame
    float animTimer    = 0.0f;
    int   animFrame    = 0;

    Rectangle getHitbox() { return { Position.x + 1, Position.y + 1, 14.0f, 14.0f }; }

    void Setup(float x, float y, float leftBound, float rightBound);
    void Movement() override;
    void Draw();
    void Unload();
    void Die();
};

// -----------------------------------------------------------------------
//  BILL BALA  (bullet)
//  - Spawns from one side of a platform and moves at constant velocity.
//  - Mario cannot kill it; can only jump over it.
//  - Despawns when it exits the screen.
// -----------------------------------------------------------------------
class BillBala : public Entity {
public:
    float speed     = 1.8f;
    float direction = 1.0f;   // +1 right, -1 left
    bool  active    = false;

    Rectangle getHitbox() { return { Position.x, Position.y + 2, 16.0f, 12.0f }; }

    void Activate(float x, float y, float dir);
    void Movement() override;
    void Draw();
    void Unload();
};

// -----------------------------------------------------------------------
//  SPAWNER – fires BillBala periodically from a fixed point
// -----------------------------------------------------------------------
struct BillSpawner {
    float x, y;
    float direction;   // which way bullets travel
    float timer    = 0.0f;
    float interval = 3.5f;
};

// -----------------------------------------------------------------------
//  Globals used by newscene.cpp
// -----------------------------------------------------------------------
#define GOOMBA_COUNT  3
#define BILL_COUNT    4   // live bullets at most

extern Goomba  Level3Goombas[GOOMBA_COUNT];
extern BillBala Level3Bills[BILL_COUNT];

void Level3EntitiesSetup();
void Level3EntitiesRoutine();
void Level3EntitiesReset();
void Level3EntitiesUnload();
