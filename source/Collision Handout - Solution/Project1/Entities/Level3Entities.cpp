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

// Two spawners: one on the left wall of platform 2 (fires right),
//              one on the right wall of platform 3 (fires left).
static BillSpawner spawners[2] = {
    { 0.0f,   0.0f,  1.0f, 0.0f, 3.5f },   // y filled in Setup
    { 208.0f, 0.0f, -1.0f, 1.8f, 3.5f },   // stagger spawn times
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
    tag = EntityTag::FIRE;   // reuse FIRE tag so map collision works

    // Sprites: "Sprites/Goomba.png" – expects a 32x16 sheet: two 16x16 frames side by side.
    // If you only have one frame, just load the same texture into both.
    Texture = LoadTexture("Sprites/Goomba.png");
    texture2 = Texture;   // same sheet, second frame selected via SpriteSelector
}

void Goomba::Movement() {
    if (isDead) return;

    // Patrol
    Position.x += speed * direction;
    if (Position.x <= patrolLeft) { Position.x = patrolLeft;  direction = 1.0f; }
    if (Position.x >= patrolRight) { Position.x = patrolRight; direction = -1.0f; }

    // Keep on its platform (simple gravity snap – reuse map collision)
    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 16;

    Level3RampCollisions(*this);

    // Animation (toggle frame every 0.2s)
    animTimer += GetFrameTime();
    if (animTimer >= 0.2f) {
        animTimer = 0.0f;
        animFrame = 1 - animFrame;
    }
}

void Goomba::Draw() {
    if (isDead) {
        // Squished frame: draw a flat rectangle as a placeholder until you
        // have a death sprite. Replace this with a DrawTextureRec call.
        Rectangle squishRec = { Position.x, Position.y + 8, 16, 8 };
        DrawRectangleRec(squishRec, RED);
        return;
    }
    // Two-frame walk animation from a 32x16 sprite sheet
    Rectangle src = { (float)(animFrame * 16), 0.0f, 16.0f, 16.0f };
    // Flip horizontally when moving left
    if (direction < 0) src.width = -16.0f;
    DrawTextureRec(Texture, src, Position, WHITE);
}

void Goomba::Die() {
    isDead = true;
    deathTimer = 0.0f;
}

void Goomba::Unload() {
    UnloadTexture(Texture);
    // texture2 is the same object – no need to unload twice
}

// -----------------------------------------------------------------------
//  BILL BALA
// -----------------------------------------------------------------------
void BillBala::Activate(float x, float y, float dir) {
    Position = { x, y };
    FloorCollider.x = x + 8;
    FloorCollider.y = y + 16;
    direction = dir;
    active = true;
    tag = EntityTag::FIRE;

    Texture = LoadTexture("Sprites/BillBala.png");
}

void BillBala::Movement() {
    if (!active) return;
    Position.x += speed * direction;
    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 8;

    // Despawn when off screen
    if (Position.x > SCREEN_WIDTH + 16 || Position.x < -16) {
        active = false;
        UnloadTexture(Texture);
        Texture = { 0 };
    }
}

void BillBala::Draw() {
    if (!active) return;
    Rectangle src = { 0, 0, direction > 0 ? 16.0f : -16.0f, 16.0f };
    DrawTextureRec(Texture, src, Position, WHITE);
}

void BillBala::Unload() {
    if (active && Texture.id > 0) UnloadTexture(Texture);
}

// -----------------------------------------------------------------------
//  STOMP DETECTION  (Mario jumps on top of a Goomba)
// -----------------------------------------------------------------------
static void CheckMarioStompsGoomba(Goomba& goomba) {
    if (goomba.isDead) return;

    Rectangle goombaBox = goomba.getHitbox();
    Rectangle marioBox = { Mario.Position.x, Mario.Position.y, 16, 16 };

    if (!CheckCollisionRecs(marioBox, goombaBox)) return;

    // Stomp: Mario must be above the Goomba's centre and moving downward
    bool marioAbove = Mario.Position.y + 16 <= goomba.Position.y + 10;
    bool marioFalling = Mario.marioVelocity.y > 0;

    if (marioAbove && marioFalling) {
        goomba.Die();
        // Bounce Mario slightly
        Mario.marioVelocity.y = -4.0f;
        Mario.setGrounded(false);
        // Award points
        ShowScorePopup(goomba.Position, 100);
        AddPoints(100);
    }
    else {
        // Side collision – Mario takes damage
        if (!GetHammerTime()) {
            StartEntityDeath(Mario);
            Mario.die();
            RemoveLife();
            CheckLives();
        }
        else {
            // Hammer kills the Goomba too
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
        // Bill is indestructible – Mario cannot kill it
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

        // Find an inactive Bill slot
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
    // Platform heights: P1=ground-40, P2=ground-80, P3=ground-120
    // Goomba y = SCREEN_HEIGHT - 1 - 16(truss) - Pheight - 16(goomba sprite)

    // Goomba 0: Platform 1 LEFT (x=0..112)
    Level3Goombas[0].Setup(32.0f,
        (float)(SCREEN_HEIGHT - 1 - 16 - 40 - 16),
        0.0f, 96.0f);

    // Goomba 1: Platform 3 RIGHT (x=80..176)
    Level3Goombas[1].Setup(112.0f,
        (float)(SCREEN_HEIGHT - 1 - 16 - 120 - 16),
        80.0f, 160.0f);

    // Goomba 2: Platform 1 RIGHT mirror (x=112..224)
    Level3Goombas[2].Setup(160.0f,
        (float)(SCREEN_HEIGHT - 1 - 16 - 40 - 16),
        112.0f, 208.0f);

    // --- BILL BALAS – all start inactive ---
    for (int i = 0; i < BILL_COUNT; i++) {
        Level3Bills[i].active = false;
        Level3Bills[i].Texture = { 0 };
    }

    // Spawner 0: shoots right from left edge of P2 (x=80 now, y=P2 surface)
    spawners[0].x = 80.0f;
    spawners[0].y = (float)(SCREEN_HEIGHT - 1 - 16 - 80 - 16);

    // Spawner 1: shoots left from right edge of P4 (x=208, y=P4 surface)
    spawners[1].x = 208.0f;
    spawners[1].y = (float)(SCREEN_HEIGHT - 1 - 16 - 160 - 16);
}

void Level3EntitiesRoutine() {
    // --- GOOMBAS ---
    for (int i = 0; i < GOOMBA_COUNT; i++) {
        Goomba& g = Level3Goombas[i];

        if (g.isDead) {
            g.deathTimer += GetFrameTime();
            g.Draw();                    // draw squish
            if (g.deathTimer > 0.5f) {
                // Respawn at its original patrol start after a short delay
                g.Die();                 // reset timer
                // Place back at patrol left so it reappears cleanly
                g.Position.x = g.patrolLeft;
                g.isDead = false;
                g.deathTimer = 0.0f;
            }
            continue;
        }

        g.Movement();
        Level3LadderCollisions(g);       // updates CanClimb (Goomba ignores it)
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
        if (Level3Bills[i].active) {
            Level3Bills[i].Unload();
        }
        Level3Bills[i].active = false;
    }
    spawners[0].timer = 0.0f;
    spawners[1].timer = 1.8f;   // stagger
}

void Level3EntitiesUnload() {
    for (int i = 0; i < GOOMBA_COUNT; i++) Level3Goombas[i].Unload();
    for (int i = 0; i < BILL_COUNT; i++) Level3Bills[i].Unload();
}