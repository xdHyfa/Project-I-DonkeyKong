#include "raylib.h"
#include "Scenes/scenes.h"
#include "Scenes/newscene.h"
#include "Maps/Level3Map.h"
#include "Entities/Level3Entities.h"
#include "Entities/Player.h"
#include "Entities/Donkey.h"
#include "Entities/Lady.h"
#include "Core/constants.h"
#include "Core/UI.h"

// -----------------------------------------------------------------------
//  Debug overlay toggle – set to true to see all hitboxes / colliders.
//  Flip to false for release builds.
// -----------------------------------------------------------------------
#define LEVEL3_DEBUG false

// -----------------------------------------------------------------------
//  Star sprite  (16×16 single frame, or 32×16 two-frame sheet)
// -----------------------------------------------------------------------
static Texture2D starTexture = { 0 };
static bool      starLoaded = false;
static float     starAnimTimer = 0.0f;
static int       starAnimFrame = 0;

// -----------------------------------------------------------------------
//  Music
// -----------------------------------------------------------------------
static Music newSceneMusic = { 0 };
static bool  musicLoaded = false;

// -----------------------------------------------------------------------
//  Win flash overlay
// -----------------------------------------------------------------------
static float flashTimer = 0.0f;

// -----------------------------------------------------------------------
//  DK animation  (donko 2-0.png)
//  Frame 0: x=1,   y=2, w=40, h=32  – idle,  4 s
//  Frame 1: x=42,  y=2, w=40, h=32  – beat,  1 s
//  Frame 2: x=85,  y=2, w=46, h=32  – beat,  1 s
//  Frame 3: x=134, y=2, w=46, h=32  – beat,  1 s
// -----------------------------------------------------------------------
static Texture2D dkTexture = { 0 };
static bool      dkLoaded = false;
static int       dkSeqIdx = 0;   // index into dkSequence
static float     dkFrameTimer = 0.0f;

// Sequence: frame 0 (idle), frame 1, then 2,3 repeated 4 times
// Sprite indices: 0=idle, 1=beat1, 2=beat2, 3=beat3
static const int   dkSequence[] = { 0, 1, 2, 3, 2, 3, 2, 3, 2, 3 };
static const float dkDurations[] = { 1.5f, 2.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f };
static const int   DK_SEQ_LEN = 10;
static const Rectangle dkFrames[4] = {
    {   1.0f, 2.0f, 40.0f, 32.0f },
    {  42.0f, 2.0f, 40.0f, 32.0f },
    {  85.0f, 2.0f, 46.0f, 32.0f },
    { 134.0f, 2.0f, 46.0f, 32.0f },
};

// -----------------------------------------------------------------------
//  Lady animation  (Lady.png or similar)
//  Frame 0: x=1,  y=1, w=14, h=22  – 0.5 s
//  Frame 1: x=17, y=1, w=15, h=22  – 0.5 s
//  Pattern: rapid flash (2 cycles) then rest on frame 0 for ~2 s
// -----------------------------------------------------------------------
static Texture2D ladyTexture = { 0 };
static bool      ladyLoaded = false;
static int       ladyFrame = 0;
static float     ladyAnimTimer = 0.0f;
static int       ladyFlashCount = 0;   // how many 0.5 s ticks done in current flash burst
static float     ladyRestTimer = 0.0f;
static bool      ladyResting = false;
static const Rectangle ladyFrames[2] = {
    {  1.0f, 1.0f, 14.0f, 22.0f },
    { 17.0f, 1.0f, 15.0f, 22.0f },
};
static const int   LADY_FLASH_TICKS = 4;   // 4 half-second ticks = 2 full cycles
static const float LADY_REST_TIME = 2.0f;
void runNewScene() {

    // ================================================================
    //  ONE-TIME INIT  (runs once each time we enter this scene)
    // ================================================================
    if (!Scene_Init) {
        Scene_Init = true;

        // --- Map ---
        Level3RampSetter();
        Level3LadderSetter();

        // --- Player – starts bottom-left of the floor ---
        Mario.Position = { 32.0f, (float)(SCREEN_HEIGHT - 16 - 17) };
        Mario.marioVelocity = { 0.0f, 0.0f };
        Mario.isAlive = true;
        Mario.isJumping = false;
        Mario.isFalling = false;
        Mario.OnLadder = false;
        Mario.CanClimb = false;
        Mario.setGrounded(false);
        Mario.FloorCollider = { Mario.Position.x + 8, Mario.Position.y + 16 };

        // --- Entities (Goombas + Bills) ---
        Level3EntitiesSetup();

        // --- Star texture ---
        if (!starLoaded) {
            starTexture = LoadTexture("Sprites/Star.png");
            starLoaded = true;
        }

        // --- DK texture ---
        if (dkTexture.id > 0) UnloadTexture(dkTexture);
        dkTexture = { 0 };
        dkLoaded = false;
        dkTexture = LoadTexture("Sprites/donko 2-0.png");
        dkLoaded = (dkTexture.id > 0);
        dkSeqIdx = 0;
        dkFrameTimer = 0.0f;

        // --- Lady texture ---
        if (ladyLoaded && ladyTexture.id > 0) UnloadTexture(ladyTexture);
        ladyTexture = LoadTexture("Sprites/Lady.png");
        ladyLoaded = (ladyTexture.id > 0);
        ladyFrame = 0;
        ladyAnimTimer = 0.0f;
        ladyFlashCount = 0;
        ladyRestTimer = 0.0f;
        ladyResting = true;

        // --- Music ---
        if (!musicLoaded) {
            newSceneMusic = LoadMusicStream("Audio/Level3.wav");
            newSceneMusic.looping = true;
            musicLoaded = true;
        }
        PlayMusicStream(newSceneMusic);

        // --- Reset win state ---
        level3WinTriggered = false;
        flashTimer = 0.0f;

        // --- Bonus timer ---
        ResetBonus();
    }

    // ================================================================
    //  EVERY FRAME
    // ================================================================

    UpdateMusicStream(newSceneMusic);

    // --- Win flash overlay ---
    if (level3WinTriggered) {
        flashTimer += GetFrameTime();
        if ((int)(flashTimer / 0.15f) % 2 == 0)
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 255,255,255,80 });
    }

    // ----------------------------------------------------------------
    //  MAP
    // ----------------------------------------------------------------
    Level3RampDraw();
    Level3LadderDraw();

    // ----------------------------------------------------------------
    //  PLACEHOLDERS replaced with animated sprites
    // ----------------------------------------------------------------

    // ----------------------------------------------------------------
    //  DK – animated sprite on P7 platform
    // ----------------------------------------------------------------
    {
        dkFrameTimer += GetFrameTime();
        if (dkFrameTimer >= dkDurations[dkSeqIdx]) {
            dkFrameTimer = 0.0f;
            dkSeqIdx = (dkSeqIdx + 1) % DK_SEQ_LEN;
        }
        int dkFrame = dkSequence[dkSeqIdx];

        Vector2 dkPos = { 4.0f, (float)(L3_Y7 - 24) };
        if (dkLoaded && dkTexture.id > 0) {
            DrawTextureRec(dkTexture, dkFrames[dkFrame], dkPos, WHITE);
        }
        else {
            // Fallback: try reloading once more in case it wasn't ready at init
            dkTexture = LoadTexture("Sprites/donko 2-0.png");
            if (dkTexture.id > 0) {
                dkLoaded = true;
                DrawTextureRec(dkTexture, dkFrames[dkFrame], dkPos, WHITE);
            }
            else {
                DrawRectangle((int)dkPos.x, (int)dkPos.y, 40, 32, BROWN);
                DrawText("DK", (int)dkPos.x + 2, (int)dkPos.y + 6, 8, WHITE);
            }
        }
    }

    // ----------------------------------------------------------------
    //  Lady – animated sprite on P5 platform
    //  Pattern: flash (4 x 0.5 s ticks = 2 cycles) then rest 2 s on frame 0
    // ----------------------------------------------------------------
    {
        Vector2 ladyPos = { 130.0f, (float)(L3_Y5 - 14) };

        if (ladyResting) {
            ladyRestTimer += GetFrameTime();
            ladyFrame = 0;
            if (ladyRestTimer >= LADY_REST_TIME) {
                ladyRestTimer = 0.0f;
                ladyResting = false;
                ladyFlashCount = 0;
                ladyAnimTimer = 0.0f;
            }
        }
        else {
            ladyAnimTimer += GetFrameTime();
            if (ladyAnimTimer >= 0.5f) {
                ladyAnimTimer = 0.0f;
                ladyFrame ^= 1;
                ladyFlashCount++;
                if (ladyFlashCount >= LADY_FLASH_TICKS) {
                    ladyResting = true;
                    ladyFrame = 0;
                    ladyFlashCount = 0;
                    ladyRestTimer = 0.0f;
                }
            }
        }

        if (ladyLoaded && ladyTexture.id > 0) {
            DrawTextureRec(ladyTexture, ladyFrames[ladyFrame], ladyPos, WHITE);
        }
        else {
            DrawRectangle((int)ladyPos.x, (int)ladyPos.y, 16, 20, PINK);
            DrawText("L", (int)ladyPos.x + 4, (int)ladyPos.y + 6, 8, WHITE);
        }
    }

    // Star (win object) – blinks on P4 until collected
    if (!level3WinTriggered) {
        if (starLoaded && starTexture.id > 0) {
            starAnimTimer += GetFrameTime();
            if (starAnimTimer >= 0.3f) { starAnimTimer = 0.0f; starAnimFrame ^= 1; }
            Rectangle starSrc = { (float)(starAnimFrame * 16), 0.0f, 16.0f, 16.0f };
            Vector2   starPos = { L3_StarHitbox.x, L3_StarHitbox.y };
            DrawTextureRec(starTexture, starSrc, starPos, YELLOW);
        }
        else {
            // Fallback placeholder when texture is missing
            DrawRectangle((int)L3_StarHitbox.x, (int)L3_StarHitbox.y,
                (int)L3_StarHitbox.width, (int)L3_StarHitbox.height, YELLOW);
            DrawText("*", (int)L3_StarHitbox.x + 2, (int)L3_StarHitbox.y, 14, GOLD);
        }
    }

    // ----------------------------------------------------------------
    //  ENTITIES  (only when Mario is alive)
    // ----------------------------------------------------------------
    if (Mario.isAlive) {
        Level3EntitiesRoutine();
    }

    // ----------------------------------------------------------------
    //  PLAYER
    // ----------------------------------------------------------------
    if (Mario.isAlive) {
        // Bajar escalera: si está en el suelo sobre un DownZone y pulsa abajo,
        // empujar a Mario dentro del hitbox de la escalera
        if (!Mario.OnLadder && !Mario.CanClimb && Mario.getIsGrounded()
            && (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            && Level3CheckDownZone(Mario))
        {
            Mario.Position.y += 2.0f;
            Mario.UpdateCollider();
            Mario.setGrounded(false);
            Mario.OnLadder = true;
            Mario.isFalling = false;
        }

        Mario.Movement();
        Level3LadderCollisions(Mario);
        Level3RampCollisions(Mario);
        Level3CheckWinCondition(Mario);
        UpdateBonus();
    }
    else {
        Mario.Movement();   // plays death animation

        // Respawn condition: deathLoopCount flag OR fallback 1.5 s timer
        static float deathRespawnTimer = 0.0f;
        deathRespawnTimer += GetFrameTime();

        bool shouldRespawn = (Mario.deathLoopCount >= 2) || (deathRespawnTimer >= 1.5f);
        if (shouldRespawn) {
            deathRespawnTimer = 0.0f;
            // Respawn at start position
            Mario.Position = { 32.0f, (float)(SCREEN_HEIGHT - 16 - 17) };
            Mario.marioVelocity = { 0.0f, 0.0f };
            Mario.isAlive = true;
            Mario.isJumping = false;
            Mario.isFalling = false;
            Mario.OnLadder = false;
            Mario.CanClimb = false;
            Mario.setGrounded(false);
            Mario.FloorCollider = { Mario.Position.x + 8, Mario.Position.y + 16 };

            Level3EntitiesReset();
            flashTimer = 0.0f;
        }
    }

    // Draw Mario sprite
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

    // ----------------------------------------------------------------
    //  DEBUG OVERLAY  (hitboxes / colliders / downzones)
    // ----------------------------------------------------------------
#if LEVEL3_DEBUG
    DrawLevel3Colliders();

    // Goomba hitboxes (green)
    for (int i = 0; i < GOOMBA_COUNT; i++) {
        if (!Level3Goombas[i].isDead) {
            Rectangle hb = Level3Goombas[i].getHitbox();
            DrawRectangleLinesEx(hb, 1, GREEN);
        }
    }

    // Bill Bala hitboxes (orange)
    for (int i = 0; i < BILL_COUNT; i++) {
        if (Level3Bills[i].active) {
            Rectangle hb = Level3Bills[i].getHitbox();
            DrawRectangleLinesEx(hb, 1, ORANGE);
        }
    }

    // Mario hitbox (skyblue)
    DrawRectangleLines((int)Mario.Position.x, (int)Mario.Position.y, 16, 16, SKYBLUE);
#endif

    // NOTE: PrintBonus() and PrintUI() are called by main.cpp
}