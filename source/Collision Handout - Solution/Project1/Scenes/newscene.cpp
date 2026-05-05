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
//  Star sprite  (drawn at the win-condition hitbox position)
//  "Sprites/Star.png" – 16x16, single frame.
//  Replace with DrawTextureRec animation if you have a sheet.
// -----------------------------------------------------------------------
static Texture2D starTexture  = { 0 };
static bool      starLoaded   = false;
static float     starAnimTimer = 0.0f;
static int       starAnimFrame = 0;

// -----------------------------------------------------------------------
//  Music
// -----------------------------------------------------------------------
static Music newSceneMusic = { 0 };
static bool  musicLoaded   = false;

// -----------------------------------------------------------------------
//  Win flash overlay
// -----------------------------------------------------------------------
static float flashTimer = 0.0f;

// -----------------------------------------------------------------------
//  runNewScene
// -----------------------------------------------------------------------
void runNewScene() {

    // ---- ONE-TIME INIT (runs once each time we enter this scene) ----
    if (!Scene_Init) {
        Scene_Init = true;

        // Map
        Level3RampSetter();
        Level3LadderSetter();

        // Player
        Mario.Position     = { 64.0f, (float)(SCREEN_HEIGHT - 16 - 17) };
        Mario.marioVelocity = { 0.0f, 0.0f };
        Mario.isAlive      = true;
        Mario.isJumping    = false;
        Mario.isFalling    = false;
        Mario.OnLadder     = false;
        Mario.CanClimb     = false;
        Mario.setGrounded(false);
        Mario.FloorCollider = { Mario.Position.x + 8, Mario.Position.y + 16 };

        // DK + Lady (they know their own default positions from their headers)
        donkey.Setup();
        lady.Setup();

        // Entities
        Level3EntitiesSetup();

        // Star texture
        if (!starLoaded) {
            starTexture = LoadTexture("Sprites/Star.png");
            starLoaded  = true;
        }

        // Music
        if (!musicLoaded) {
            newSceneMusic  = LoadMusicStream("Audio/Level3.wav");
            newSceneMusic.looping = true;
            musicLoaded    = true;
        }
        PlayMusicStream(newSceneMusic);

        // Reset win state
        level3WinTriggered = false;

        // Bonus timer
        ResetBonus();
        AddLevel();
    }

    // ---- EVERY FRAME ----

    UpdateMusicStream(newSceneMusic);

    // --- Win flash overlay when Mario touches the Star ---
    if (level3WinTriggered) {
        flashTimer += GetFrameTime();
        // Alternate white flash every 0.15s for 2 seconds
        if ((int)(flashTimer / 0.15f) % 2 == 0) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 255,255,255,80 });
        }
    }

    // --- Draw map ---
    Level3RampDraw();
    Level3LadderDraw();

    // --- Star (win object) – blink gently ---
    if (!level3WinTriggered && starLoaded) {
        starAnimTimer += GetFrameTime();
        if (starAnimTimer >= 0.3f) { starAnimTimer = 0.0f; starAnimFrame ^= 1; }
        Rectangle starSrc  = { (float)(starAnimFrame * 16), 0.0f, 16.0f, 16.0f };
        // L3_StarHitbox is defined in Level3Map.cpp; position is set in RampSetter
        // We replicate the x,y here; these match what Level3CheckWinCondition uses.
        Vector2 starPos = { 95.0f, (float)(SCREEN_HEIGHT - 16 - 184 - 16) };
        DrawTextureRec(starTexture, starSrc, starPos, YELLOW);
    }

    // --- DK + Lady ---
    donkey.Update();
    donkey.Draw();
    lady.Update();
    lady.Draw();

    // --- Entities (Goombas + Bills) ---
    if (Mario.isAlive) {
        Level3EntitiesRoutine();
    }

    // --- Player ---
    if (Mario.isAlive) {
        Mario.Movement();
        Level3LadderCollisions(Mario);
        Level3RampCollisions(Mario);
        Level3CheckWinCondition(Mario);
        UpdateBonus();
    } else {
        // Death animation – still draw, handle respawn
        Mario.Movement();

        // Respawn after death animation finishes (deathLoopCount >= 2)
        if (Mario.deathLoopCount >= 2) {
            // Reset player to start of scene
            Mario.Position      = { 64.0f, (float)(SCREEN_HEIGHT - 16 - 17) };
            Mario.marioVelocity = { 0.0f, 0.0f };
            Mario.isAlive       = true;
            Mario.isJumping     = false;
            Mario.isFalling     = false;
            Mario.OnLadder      = false;
            Mario.CanClimb      = false;
            Mario.setGrounded(false);
            Mario.FloorCollider = { Mario.Position.x + 8, Mario.Position.y + 16 };

            Level3EntitiesReset();
        }
    }

    // Draw Mario sprite
    DrawTextureRec(Mario.Texture, frameRec, Mario.Position, WHITE);

    // --- Bonus UI ---
    PrintBonus();
}
