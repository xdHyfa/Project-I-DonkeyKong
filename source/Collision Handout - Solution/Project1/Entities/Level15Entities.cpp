#include "raylib.h"
#include "Entities/Level15Entities.h"
#include "Entities/Goomba.h"
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
}

void ResetLevel15Entities() {
    for (int i = 0; i < NUM_GOOMBAS; i++) {
        Goombas[i].Reset();
    }
    GoombaNeedsInit = true;
    goombaKilledPlayer = false;
}