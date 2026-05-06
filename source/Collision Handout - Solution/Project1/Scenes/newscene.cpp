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
//  runNewScene – called every frame by main.cpp while NEWSCENE is active
// -----------------------------------------------------------------------
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
    //  PLACEHOLDERS
    //  Replace each DrawRectangle/DrawText block with the real sprite
    //  once assets are ready.
    // ----------------------------------------------------------------

    // DK – upper-left platform (P7), x=0..80, y=L3_Y7
    {
        Vector2 dkPos = { 16.0f, (float)(L3_Y7 - 24) };
        DrawRectangle((int)dkPos.x, (int)dkPos.y, 24, 24, BROWN);
        DrawText("DK", (int)dkPos.x + 2, (int)dkPos.y + 6, 8, WHITE);
        // TODO: donkey.Update(); donkey.Draw();
    }

    // Lady – top-right platform (P5), x=112..224, y=L3_Y5
    {
        Vector2 ladyPos = { 168.0f, (float)(L3_Y5 - 20) };
        DrawRectangle((int)ladyPos.x, (int)ladyPos.y, 16, 20, PINK);
        DrawText("L", (int)ladyPos.x + 4, (int)ladyPos.y + 6, 8, WHITE);
        // TODO: lady.Update(); lady.Draw();
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