#include "raylib.h"
#include "Entities/Level15Entities.h"
#include "Entities/Goomba.h"
#include "Entities/BillBala.h"
#include "Maps/Level15Map.h"
#include "Entities/Player.h"
#include "Core/constants.h"
#include "Core/UI.h"
#include "Scenes/scenes.h"
#include <iostream>
using namespace std;

float level15StartTime = 0.0f;

void SetStartTime15() {
    level15StartTime = GetTime();
}

extern float P15_Ramp0_YPos;
extern float P15_Ramp1L_YPos;
extern float P15_Ramp1R_YPos;
extern float P15_Ramp2L_YPos;
extern float P15_Ramp3_YPos;
extern float P15_Ramp4L_YPos;

#define NUM_GOOMBAS 5
static Goomba Goombas[NUM_GOOMBAS];
static bool GoombaTextureLoaded = false;
static Texture2D GoombaTexture = { 0 };
static bool GoombaNeedsInit = true;
static bool goombaKilledPlayer = false;

static Sound StompSound = { 0 };
static bool StompSoundLoaded = false;
static Sound BonusSound = { 0 };
static bool BonusSoundLoaded = false;

// --- BillBalas ---
#define NUM_BILLBALAS 3
static BillBala BillBalas[NUM_BILLBALAS];
static Texture2D BillBalaTex = { 0 };
static Texture2D BaseBillBalaTex = { 0 };
static bool BillBalaTexLoaded = false;
static bool BillBalaNeedsInit = true;

bool GoombaKilledPlayer() {
    bool val = goombaKilledPlayer;
    goombaKilledPlayer = false;
    return val;
}

static void InitGoombas() {
    float y0 = P15_Ramp0_YPos - 8.0f;
    float y1 = P15_Ramp2L_YPos - 8.0f;
    float y2 = P15_Ramp3_YPos - 8.0f;
    float y3 = P15_Ramp1R_YPos - 8.0f;
    float y4 = P15_Ramp4L_YPos - 8.0f;

    Goombas[0].Spawn(148.0f, y0, 136.0f, 208.0f); // Ramp15_0 floor (right side)
    Goombas[1].Spawn(104.0f, y1, 64.0f, 144.0f);  // Ramp15_2L
    Goombas[2].Spawn(48.0f, y2, 16.0f, 80.0f);  // Ramp15_3
    Goombas[3].Spawn(168.0f, y3, 136.0f, 208.0f);  // Ramp15_1R
    Goombas[4].Spawn(52.0f, y4, 28.0f, 76.0f);  // Ramp15_4R
}

static void InitBillBalas() {
    // baseY: base bottom rests on platform top
    // bulletY: bullet flies 1px above platform top
    // +10px lower as requested (add 10 to both)
    float baseY1 = P15_Ramp1L_YPos - (float)BILLBALA_BASE_H + 10.0f;
    float bulletY1 = P15_Ramp1L_YPos - 1.0f - (float)BILLBALA_SPRITE_H + 10.0f;

    float baseY2 = P15_Ramp2L_YPos - (float)BILLBALA_BASE_H + 10.0f;
    float bulletY2 = P15_Ramp2L_YPos - 1.0f - (float)BILLBALA_SPRITE_H + 10.0f;

    float baseY3 = P15_Ramp3_YPos - (float)BILLBALA_BASE_H + 10.0f;
    float bulletY3 = P15_Ramp3_YPos - 1.0f - (float)BILLBALA_SPRITE_H + 10.0f;

    // Left launchers: baseX=-7 so content (starts at px 6 in texture) is at screen x=-1 ? flush left
    BillBalas[0].Init(-7.0f, baseY1 - 1.0f, bulletY1 - 1.0f, +1);
    BillBalas[1].Init(-7.0f, baseY2 - 1.0f, bulletY2 - 1.0f, +1);

    // Right launcher: baseX = SCREEN_WIDTH-10 so content (px 6-14 in 16px src) ends at screen right edge
    // Content starts at srcX=6, so it appears at screenX = baseX+6 = SCREEN_WIDTH-4... 
    // We want content right edge (px14) at SCREEN_WIDTH: baseX = SCREEN_WIDTH - 14 - 1 = SCREEN_WIDTH-15
    // Actually: content at x=6..14 in texture ? drawn at baseX+6..baseX+14
    // Want baseX+14 = SCREEN_WIDTH ? baseX = SCREEN_WIDTH - 14
    BillBalas[2].Init((float)(SCREEN_WIDTH - 14), baseY3, bulletY3, -1);

    for (int i = 0; i < NUM_BILLBALAS; i++) {
        BillBalas[i].bulletTex = BillBalaTex;
        BillBalas[i].baseTex = BaseBillBalaTex;
        BillBalas[i].fireTimer = (float)i * 1.2f;
    }
}

static bool billBalaKilledPlayer = false;

bool BillBalaKilledPlayer() {
    bool val = billBalaKilledPlayer;
    billBalaKilledPlayer = false;
    return val;
}

static void CheckBillBalaPlayerCollisions() {
    auto check = [](Player& player) {
        if (!player.isAlive) return;
        Rectangle playerRect = player.getHitbox();
        for (int i = 0; i < NUM_BILLBALAS; i++) {
            BillBala& bb = BillBalas[i];
            if (!bb.IsActive()) continue;

            Rectangle bbTop = bb.GetTopHitbox();
            Rectangle bbBody = bb.GetHitbox();

            bool stomped = CheckCollisionRecs(playerRect, bbTop)
                && player.marioVelocity.y > 0.5f;

            if (stomped) {
                bb.Stomp();
                player.marioVelocity.y = -5.0f;
                player.setGrounded(false);
                AddPoints(100);
                ShowScorePopup(bb.bulletPos, 100);
                if (BonusSoundLoaded) PlaySound(BonusSound);
                return;
            }

            if (CheckCollisionRecs(playerRect, bbBody)) {
                billBalaKilledPlayer = true;
                return;
            }
        }
        };
    check(Mario);
    if (GetTwoPlayers()) check(Luigi);
}


static void CheckGoombaPlayerCollisions() {
    auto checkPlayer = [](Player& player) {
        if (!player.isAlive) return;

        for (int i = 0; i < NUM_GOOMBAS; i++) {
            Goomba& g = Goombas[i];
            if (!g.isAlive) continue;

            Rectangle playerRect = player.getHitbox();
            Rectangle goombaBody = g.GetHitbox();
            Rectangle goombaTop = g.GetTopHitbox();

            bool stomped = CheckCollisionRecs(playerRect, goombaTop)
                && player.marioVelocity.y > 0.5f;

            if (stomped) {
                g.Kill();
                player.marioVelocity.y = -5.0f;
                player.setGrounded(false);
                AddPoints(100);
                ShowScorePopup(g.Position, 100);
                if (StompSoundLoaded) PlaySound(StompSound);
                if (BonusSoundLoaded) PlaySound(BonusSound);
                return;
            }

            if (CheckCollisionRecs(playerRect, goombaBody)) {
                goombaKilledPlayer = true;
                return;
            }
        }
        };

    checkPlayer(Mario);
    if (GetTwoPlayers()) checkPlayer(Luigi);
}

void Level15EntitiesRoutine() {
    if (!GoombaTextureLoaded) {
        GoombaTexture = LoadTexture("sprites/GOOMBAS.png");
        GoombaTextureLoaded = true;
    }

    if (!StompSoundLoaded) {
        StompSound = LoadSound("Audio/Stomp.wav");
        StompSoundLoaded = true;
    }

    if (!BonusSoundLoaded) {
        BonusSound = LoadSound("Audio/Bonus.wav");
        BonusSoundLoaded = true;
    }

    if (GoombaNeedsInit) {
        for (int i = 0; i < NUM_GOOMBAS; i++) {
            Goombas[i].Texture = GoombaTexture;
        }
        InitGoombas();
        GoombaNeedsInit = false;
    }

    if (!BillBalaTexLoaded) {
        BillBalaTex = LoadTexture("Sprites/BillBala.png");
        BaseBillBalaTex = LoadTexture("Sprites/BaseBillBala.png");
        BillBalaTexLoaded = true;
    }

    if (BillBalaNeedsInit) {
        InitBillBalas();
        BillBalaNeedsInit = false;
    }

    for (int i = 0; i < NUM_BILLBALAS; i++) {
        BillBalas[i].Update();
        BillBalas[i].Draw();
    }

    CheckBillBalaPlayerCollisions();

    for (int i = 0; i < NUM_GOOMBAS; i++) {
        Goombas[i].Update();
        Goombas[i].Draw();
    }

    CheckGoombaPlayerCollisions();
}

void UnloadLevel15Entities() {
    if (GoombaTextureLoaded) {
        UnloadTexture(GoombaTexture);
        GoombaTextureLoaded = false;
    }
    if (StompSoundLoaded) {
        UnloadSound(StompSound);
        StompSoundLoaded = false;
    }
    if (BonusSoundLoaded) {
        UnloadSound(BonusSound);
        BonusSoundLoaded = false;
    }
    if (BillBalaTexLoaded) {
        UnloadTexture(BillBalaTex);
        UnloadTexture(BaseBillBalaTex);
        BillBalaTexLoaded = false;
    }
}

void ResetLevel15Entities() {
    for (int i = 0; i < NUM_GOOMBAS; i++) {
        Goombas[i].Reset();
    }
    GoombaNeedsInit = true;
    goombaKilledPlayer = false;
    BillBalaNeedsInit = true;
    billBalaKilledPlayer = false;
}