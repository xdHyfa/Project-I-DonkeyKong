#include "Scenes/WinCutscene15.h"
#include "Scenes/scenes.h"
#include "Entities/Donkey.h"
#include "Entities/Lady.h"
#include "Entities/Player.h"
#include "Maps/Level15Map.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "raylib.h"

// ---- PHASES ----
enum WinPhase15 { WP15_WAIT, WP15_WALK, WP15_CARRY, WP15_END };
static WinPhase15 winPhase15 = WP15_WAIT;

// ---- TEXTURES ----
static Texture2D wc15_dkTex = { 0 };
static Texture2D wc15_ladyTex = { 0 };

// ---- AUDIO ----
static Sound wc15_clearSound = { 0 };

// ---- DK FRAMES ----
static Rectangle wc15_dkIdleFrame = { 1.0f,  2.0f, 40.0f, 32.0f };
static Rectangle wc15_dkCarryFrame = { 3.0f, 78.0f, 38.0f, 36.0f };

// ---- POSITIONS ----
static Vector2 wc15_dkPos = { 0.0f, 0.0f };
static Vector2 wc15_ladyPos = { 0.0f, 0.0f };

// ---- TIMERS ----
static float wc15_timer = 0.0f;
static float wc15_starTimer = 0.0f;

// ---- STAR BLINK ----
static Color GetWc15StarTint() {
    int step = (int)(wc15_starTimer / 0.1f) % 6;
    switch (step) {
    case 0: return Color{ 255, 50,  50,  255 };
    case 1: return Color{ 255, 165, 0,   255 };
    case 2: return Color{ 255, 255, 50,  255 };
    case 3: return Color{ 50,  255, 50,  255 };
    case 4: return Color{ 50,  50,  255, 255 };
    case 5: return Color{ 180, 50,  255, 255 };
    default: return WHITE;
    }
}

extern float P15_Ramp4L_YPos;

static void WinCutscene15Init() {
    SearchAndSetResourceDir("resources");

    wc15_dkTex = LoadTexture("Sprites/donko 2-0.png");
    wc15_ladyTex = LoadTexture("sprites/LADY.png");
    wc15_clearSound = LoadSound("Audio/Stage-Cleared-1.wav");

    Level15RampSetter();
    Level15LadderSetter();

    // Mario stays at star position (top floor left, where star was)
    Mario.Texture = LoadTexture("sprites/MARIO.png");
    Mario.Setup();
    Mario.Position = { 33.0f, P15_Ramp4L_YPos - (float)Mario.SpriteSize + 8 };
    Mario.frameRec.width = abs(Mario.frameRec.width);

    // Lady fixed position
    wc15_ladyPos = { 181.0f, 34.0f };

    // DK at his level position
    wc15_dkPos = { (float)(SCREEN_WIDTH / 2) + 15.0f, 24.0f };

    wc15_timer = 0.0f;
    wc15_starTimer = 0.0f;
    winPhase15 = WP15_WAIT;

    PlaySound(wc15_clearSound);
}

void runWinCutscene15() {

    if (!Scene_Init) {
        WinCutscene15Init();
        Scene_Init = true;
    }

    float dt = GetFrameTime();
    wc15_timer += dt;
    wc15_starTimer += dt;

    Level15LadderDraw();
    Level15RampDraw();
    Level15TrussOverLadderDraw();

    // Lady visible until grabbed
    if (winPhase15 == WP15_WAIT || winPhase15 == WP15_WALK)
        DrawTextureRec(wc15_ladyTex, { 1.0f, 1.0f, 14.0f, 22.0f }, wc15_ladyPos, WHITE);

    // --- WAIT: DK idle 2 seconds ---
    if (winPhase15 == WP15_WAIT) {
        DrawTextureRec(wc15_dkTex, wc15_dkIdleFrame, wc15_dkPos, WHITE);
        if (wc15_timer >= 2.0f) {
            winPhase15 = WP15_WALK;
            wc15_timer = 0.0f;
        }
    }
    // --- WALK: DK moves fast right, no animation ---
    else if (winPhase15 == WP15_WALK) {
        wc15_dkPos.x += 2.5f;
        DrawTextureRec(wc15_dkTex, wc15_dkIdleFrame, wc15_dkPos, WHITE);
        if (wc15_dkPos.x >= wc15_ladyPos.x - 8.0f) {
            winPhase15 = WP15_CARRY;
            wc15_timer = 0.0f;
        }
    }
    // --- CARRY: DK jumps off top of screen with Lady ---
    else if (winPhase15 == WP15_CARRY) {
        wc15_dkPos.y -= 3.0f;
        DrawTextureRec(wc15_dkTex, wc15_dkCarryFrame, wc15_dkPos, WHITE);
        if (wc15_dkPos.y < -50.0f) {
            winPhase15 = WP15_END;
            wc15_timer = 0.0f;
        }
    }
    // --- END: transition to next scene ---
    else if (winPhase15 == WP15_END) {
        if (wc15_timer >= 0.3f) {
            UnloadTexture(wc15_dkTex);
            UnloadTexture(wc15_ladyTex);
            UnloadTexture(Mario.Texture);
            StopSound(wc15_clearSound);
            UnloadSound(wc15_clearSound);
            Truss::UnloadSharedTexture();
            Ladder::UnloadSharedTexture();
            Scene_Init = false;
            ChangeScene();
        }
    }

    // Mario stands still, blinking rainbow
    DrawTextureRec(Mario.Texture, Mario.frameRec, Mario.Position, GetWc15StarTint());
}