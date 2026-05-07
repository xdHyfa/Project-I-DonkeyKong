#include "raylib.h"
#include "Entities/Level3Entities.h"
#include "Maps/Level3Map.h"
#include "Entities/Player.h"
#include "Core/constants.h"
#include "Core/UI.h"
#include "Scenes/scenes.h"
#include <iostream>
using namespace std;

// -----------------------------------------------------------------------
//  GLOBALS
// -----------------------------------------------------------------------
Goomba   Level3Goombas[GOOMBA_COUNT];
BillBala Level3Bills[BILL_COUNT];

// Two Bill Bala spawners:
//   Spawner 0: left edge of screen, fires RIGHT on P2 height
//   Spawner 1: right edge of screen, fires LEFT on P3R height
static BillSpawner spawners[2] = {
    { 0.0f, 0.0f, -1.0f, 0.0f, 3.5f },   // x/y filled in Setup
    { 0.0f, 0.0f,  1.0f, 1.8f, 3.5f },   // stagger spawn times
};

// -----------------------------------------------------------------------
//  GOOMBA
// -----------------------------------------------------------------------
void Goomba::Setup(float x, float y, float leftBound, float rightBound) {
    Position = { x, y };
    FloorCollider.x = x + 8;
    FloorCollider.y = y + 16;
    patrolLeft = leftBound;
    patrolRight = rightBound;
    isDead = false;
    deathTimer = 0.0f;
    animFrame = 0;
    animTimer = 0.0f;
    tag = EntityTag::FIRE;   // reuse so RampCollision accepts it

    // "Sprites/GOOMBAS.png" – sheet with two 15x15 walk frames and squished frame
    Texture = LoadTexture("Sprites/GOOMBAS.png");
    texture2 = Texture;
}

void Goomba::Movement() {
    if (isDead) return;

    Position.x += speed * direction;
    if (Position.x <= patrolLeft) { Position.x = patrolLeft;  direction = 1.0f; }
    if (Position.x >= patrolRight) { Position.x = patrolRight; direction = -1.0f; }

    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 16;

    Level3RampCollisions(*this);

    // No walk animation – single frame only. animFrame stays 0.
}

void Goomba::Draw() {
    if (isDead) {
        // Flat/squished frame: x=26, y=6, w=13, h=8
        Rectangle src = { 26.0f, 6.0f, 13.0f, 8.0f };
        DrawTextureRec(Texture, src, { Position.x + 1.0f, Position.y + 7.0f }, WHITE);
        return;
    }
    // Normal walk frame: x=6, y=4, w=16, h=15.
    // Same sprite for both directions – no flipping, no animation.
    Rectangle src = { 6.0f, 4.0f, 16.0f, 15.0f };
    DrawTextureRec(Texture, src, { Position.x, Position.y }, WHITE);
}

void Goomba::Die() {
    isDead = true;
    deathTimer = 0.0f;
}

void Goomba::Unload() {
    UnloadTexture(Texture);
}

// -----------------------------------------------------------------------
//  BILL BALA
// -----------------------------------------------------------------------
void BillBala::Activate(float x, float y, float dir) {
    Position = { x, y };
    FloorCollider.x = x + 8;
    FloorCollider.y = y + 8;
    direction = dir;
    active = true;
    tag = EntityTag::FIRE;
    Texture = LoadTexture("Sprites/BillBala.png");   // 16x13 at offset (4,3)
}

void BillBala::Movement() {
    if (!active) return;
    Position.x += speed * direction;
    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 8;

    if (Position.x > SCREEN_WIDTH + 16 || Position.x < -16) {
        active = false;
        UnloadTexture(Texture);
        Texture = { 0 };
    }
}

void BillBala::Draw() {
    if (!active) return;
    // BillBala sprite: x=4,y=3, w=16,h=13 – flip when travelling left
    Rectangle src = { 4.0f, 3.0f, direction >= 0 ? 16.0f : -16.0f, 13.0f };
    DrawTextureRec(Texture, src, Position, WHITE);
}

void BillBala::Unload() {
    if (active && Texture.id > 0) UnloadTexture(Texture);
}

// -----------------------------------------------------------------------
//  STOMP DETECTION
// -----------------------------------------------------------------------
static void CheckMarioStompsGoomba(Goomba& goomba) {
    if (goomba.isDead) return;

    Rectangle goombaBox = goomba.getHitbox();
    Rectangle marioBox = { Mario.Position.x, Mario.Position.y, 16, 16 };

    if (!CheckCollisionRecs(marioBox, goombaBox)) return;

    bool marioAbove = (Mario.Position.y + 16) <= (goomba.Position.y + 10);
    bool marioFalling = Mario.marioVelocity.y > 0;

    if (marioAbove && marioFalling) {
        goomba.Die();
        Mario.marioVelocity.y = -4.0f;
        Mario.setGrounded(false);
        ShowScorePopup(goomba.Position, 100);
        AddPoints(100);
    }
    else {
        if (!GetHammerTime()) {
            StartEntityDeath(Mario);
            Mario.die();
            RemoveLife();
            CheckLives();
        }
        else {
            goomba.Die();
            StartEntityDeath(goomba);
            ShowScorePopup(goomba.Position, 100);
            AddPoints(100);
        }
    }
}

// -----------------------------------------------------------------------
//  BILL BALA COLLISION WITH MARIO
// -----------------------------------------------------------------------
static void CheckMarioHitsBill(BillBala& bill) {
    if (!bill.active) return;

    Rectangle billBox = bill.getHitbox();
    Rectangle marioBox = { Mario.Position.x + 2, Mario.Position.y + 2, 12, 14 };

    if (CheckCollisionRecs(marioBox, billBox)) {
        if (!GetHammerTime()) {
            StartEntityDeath(Mario);
            Mario.die();
            RemoveLife();
            CheckLives();
        }
        // Bills are indestructible – Mario can only jump over them
    }
}

// -----------------------------------------------------------------------
//  SPAWNER UPDATE
// -----------------------------------------------------------------------
static void UpdateSpawners() {
    for (int s = 0; s < 2; s++) {
        spawners[s].timer += GetFrameTime();
        if (spawners[s].timer < spawners[s].interval) continue;
        spawners[s].timer = 0.0f;

        for (int b = 0; b < BILL_COUNT; b++) {
            if (!Level3Bills[b].active) {
                Level3Bills[b].Activate(spawners[s].x, spawners[s].y, spawners[s].direction);
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------
//  PUBLIC API
// -----------------------------------------------------------------------
void Level3EntitiesSetup() {
    // --- GOOMBAS ---
    // Goomba sprite is 16px tall; surface of platform Yn is at Yn px from top.
    // Entity y  = Yn - 16  (sprite sits on surface)
    // Matching whiteboard: one on P1L (left low), one on P2 (central mid)

    // Goomba 0: P1L (x=0..96),  patrols x=4..80
    Level3Goombas[0].Setup(
        40.0f,
        (float)(L3_Y1 - 15),
        4.0f, 80.0f
    );

    // Goomba 1: P2 central (x=64..192), patrols x=80..160
    Level3Goombas[1].Setup(
        100.0f,
        (float)(L3_Y2 - 15),
        80.0f, 160.0f
    );

    // Goomba 2: P3R (x=96..208), patrols x=100..192
    Level3Goombas[2].Setup(
        130.0f,
        (float)(L3_Y3 - 15),
        100.0f, 192.0f
    );

    // --- BILL BALAS – all start inactive ---
    for (int i = 0; i < BILL_COUNT; i++) {
        Level3Bills[i].active = false;
        Level3Bills[i].Texture = { 0 };
    }

    // Spawner 0: fires RIGHT from LEFT edge of screen, travels on P2 height
    spawners[0].x = -16.0f;          // off-screen left
    spawners[0].y = (float)(L3_Y2 - 7);   // 13 = BillBala sprite height
    spawners[0].direction = 1.0f;

    // Spawner 1: fires LEFT from RIGHT edge of screen, travels on P3R height
    spawners[1].x = (float)SCREEN_WIDTH;  // off-screen right
    spawners[1].y = (float)(L3_Y3 - 7);   // 13 = BillBala sprite height
    spawners[1].direction = -1.0f;
}

void Level3EntitiesRoutine() {
    // --- GOOMBAS ---
    for (int i = 0; i < GOOMBA_COUNT; i++) {
        Goomba& g = Level3Goombas[i];

        if (g.isDead) {
            g.deathTimer += GetFrameTime();
            if (g.deathTimer <= 0.5f) {
                g.Draw();   // show flat sprite for 0.5s
            }
            // then invisible for the rest of the 2.5s total wait
            if (g.deathTimer > 2.5f) {
                g.Position.x = g.patrolLeft;
                g.isDead = false;
                g.deathTimer = 0.0f;
            }
            continue;
        }

        g.Movement();
        Level3LadderCollisions(g);   // Goomba can't climb but call keeps FloorCollider synced
        g.Draw();
        CheckMarioStompsGoomba(g);
    }

    // --- BILL BALAS ---
    UpdateSpawners();
    for (int i = 0; i < BILL_COUNT; i++) {
        Level3Bills[i].Movement();
        Level3Bills[i].Draw();
        CheckMarioHitsBill(Level3Bills[i]);
    }
}

void Level3EntitiesReset() {
    for (int i = 0; i < GOOMBA_COUNT; i++) {
        Level3Goombas[i].isDead = false;
        Level3Goombas[i].deathTimer = 0.0f;
        Level3Goombas[i].animFrame = 0;
    }
    for (int i = 0; i < BILL_COUNT; i++) {
        if (Level3Bills[i].active) Level3Bills[i].Unload();
        Level3Bills[i].active = false;
    }
    // Re-stagger spawners
    spawners[0].timer = 0.0f;
    spawners[1].timer = 1.8f;
}

void Level3EntitiesUnload() {
    for (int i = 0; i < GOOMBA_COUNT; i++) Level3Goombas[i].Unload();
    for (int i = 0; i < BILL_COUNT; i++)   Level3Bills[i].Unload();
}