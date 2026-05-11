#include "Entities/Player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "Core/constants.h"
#include "Entities/entity.h"
#include "Scenes/scenes.h"

using namespace std;

Player Mario(true);
Player Luigi(false);

// --- Static audio members: defined once, shared by both players ---
Sound Player::jumpSound = { 0 };
Music Player::walkMusic = { 0 };
Sound Player::deathSound = { 0 };
Music Player::climbMusic = { 0 };

// --- Constants (no per-player state here) ---
const unsigned NUM_FRAMES = 4;
const float    FRAME_DELAY = 0.5f;
const float    GROUND_Y = SCREEN_HEIGHT - 16.0f;

// --- Gamepad index for Player 2 ---
#define P2_GAMEPAD 0

// --- Helpers: use gamepad when connected, fall back to keyboard otherwise ---
// IsGamepadAvailable() is checked every call, so hot-plugging works at runtime.
static bool P2Down(int button, int key)
{
    if (IsGamepadAvailable(P2_GAMEPAD))
        return IsGamepadButtonDown(P2_GAMEPAD, button);
    return IsKeyDown(key);
}
static bool P2Pressed(int button, int key)
{
    if (IsGamepadAvailable(P2_GAMEPAD))
        return IsGamepadButtonPressed(P2_GAMEPAD, button);
    return IsKeyPressed(key);
}

bool isTextureValid(const Texture2D& texture)
{
    return texture.id > 0;
}

// ------------------------------------------------------------
// Ladder movement — works entirely on the player passed in
// ------------------------------------------------------------
void PlayerLadderMovement(Player& player)
{
    bool up = (player.PlayerNum == 1) ? IsKeyDown(KEY_UP) : P2Down(GAMEPAD_BUTTON_LEFT_FACE_UP, KEY_W);
    bool down = (player.PlayerNum == 1) ? IsKeyDown(KEY_DOWN) : P2Down(GAMEPAD_BUTTON_LEFT_FACE_DOWN, KEY_S);

    if (up) {
        player.OnLadder = true;
        player.Position.y -= 1.0f;
        player.UpdateCollider();
    }
    else if (down) {
        player.OnLadder = true;
        player.Position.y += 1.0f;
        player.UpdateCollider();
    }

    player.frameRec.y = 1 * player.SpriteSize;
    player.frameRec.width = player.SpriteSize;

    ++player.frameDelayCounter;
    if (player.frameDelayCounter > 4)
    {
        player.frameDelayCounter = 0;
        if (up || down)
        {
            ++player.frameIndex;
            player.frameIndex %= 2;
            player.frameRec.x = player.SpriteSize * (float)player.frameIndex;
        }
    }
}

// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------
void Player::Setup()
{
    SpriteSize = (float)playerSize;
    frameRec = { 0.0f, 0.0f, SpriteSize, SpriteSize };
    Position = { 64, SCREEN_HEIGHT - SpriteSize - 17 };

    HammerTexture = LoadTexture("Sprites/Hammer.png");

    // Audio is static — only load once (guard with id check)
    if (jumpSound.frameCount == 0)
    {
        jumpSound = LoadSound("Audio/Game-Startup.wav");
        walkMusic = LoadMusicStream("Audio/WalkingDef2.wav");
        walkMusic.looping = true;
        climbMusic = LoadMusicStream("Audio/WalkingDef3.wav");
        climbMusic.looping = true;
        deathSound = LoadSound("Audio/Dead.wav");
    }
}

// ------------------------------------------------------------
// Movement
// ------------------------------------------------------------

void Player::DeathSequence() {

    frameRec.width = abs(frameRec.width);
    frameRec.y = 4 * SpriteSize;

    if (!deathStarted)
    {
        frameRec.x = 0.0f;
        deathStartTimer += GetFrameTime();
        if (deathStartTimer >= 1.0f)
        {
            deathStarted = true;
            frameIndex = 1;
            frameDelayCounter = 0;
        }
        return;
    }

    if (deathLoopCount < 2)
    {
        ++frameDelayCounter;
        if (frameDelayCounter > 6)
        {
            frameDelayCounter = 0;
            frameRec.x = SpriteSize * (float)frameIndex;
            ++frameIndex %= 4;
            if (frameIndex == 0) ++deathLoopCount;
        }
    }
    else
    {
        frameRec.x = 4 * SpriteSize;
    }
    return;
}

void Player::Movement()
{
    if (!isAlive) {
        DeathSequence();
        return;
    }

    // --- Horizontal input ---
    marioVelocity.x = 0.0f;

    bool moveRight = (PlayerNum == 1) ? IsKeyDown(KEY_RIGHT) : P2Down(GAMEPAD_BUTTON_LEFT_FACE_RIGHT, KEY_D);
    bool moveLeft = (PlayerNum == 1) ? IsKeyDown(KEY_LEFT) : P2Down(GAMEPAD_BUTTON_LEFT_FACE_LEFT, KEY_A);

    float speedMult = StarActive ? 3.0f : 1.0f;

    if (moveRight)
    {
        marioVelocity.x = (float)velocity * speedMult;
        if (frameRec.width < 0 && getIsGrounded()) frameRec.width = -frameRec.width;
        if (getIsGrounded())
        {
            if (!IsMusicStreamPlaying(walkMusic)) PlayMusicStream(walkMusic);
            UpdateMusicStream(walkMusic);
        }
        LooksRight = true;
    }
    else if (moveLeft)
    {
        marioVelocity.x = -(float)velocity * speedMult;
        if (frameRec.width > 0 && getIsGrounded()) frameRec.width = -frameRec.width;
        if (getIsGrounded())
        {
            if (!IsMusicStreamPlaying(walkMusic)) PlayMusicStream(walkMusic);
            UpdateMusicStream(walkMusic);
        }
        LooksRight = false;
    }
    else
    {
        StopMusicStream(walkMusic);
    }

    // --- Hammer ---
    if (GetHammerTime(PlayerNum))
    {
        if (HammerTimer > 11.5f)
        {
            StopHammerTime(PlayerNum);
            HammerTimer = 0.0f;
        }
        else
        {
            if (!GetIsKilling()) HammerTimer += GetFrameTime();

            if (HammerTick > 0.15f)
            {
                if (LooksRight)
                {
                    HammerHitbox.x = Position.x + 16;
                    HammerPos.x = Position.x + 16;
                    HammerPos.y = Position.y + 2;
                    HammerSelector.x = 12;
                    HammerSelector.width = 12;
                }
                else
                {
                    HammerHitbox.x = Position.x - 16;
                    HammerPos.x = Position.x - 16;
                    HammerPos.y = Position.y + 1;
                    HammerSelector.x = 12;
                    HammerSelector.width = -12;
                }
                HammerHitbox.y = Position.y;
                DrawTextureRec(HammerTexture, HammerSelector, HammerPos, WHITE);

                HammerTick += GetFrameTime();
                if (HammerTick > 0.3f) HammerTick = 0.0f;
            }
            else
            {
                HammerHitbox.x = Position.x;
                HammerHitbox.y = Position.y - 16;
                HammerPos.x = Position.x;
                HammerPos.y = Position.y - 16;
                HammerSelector.x = 0;
                HammerSelector.width = 12;
                DrawTextureRec(HammerTexture, HammerSelector, HammerPos, WHITE);
                HammerTick += GetFrameTime();
            }
        }
    }
    else
    {
        // --- Ladder climb ---
        if (OnLadder && CanClimb)
        {
            cout << "ON LADDER" << endl;
            PlayerLadderMovement(*this);

            bool upHeld = (PlayerNum == 1) ? IsKeyDown(KEY_UP) : P2Down(GAMEPAD_BUTTON_LEFT_FACE_UP, KEY_W);
            bool downHeld = (PlayerNum == 1) ? IsKeyDown(KEY_DOWN) : P2Down(GAMEPAD_BUTTON_LEFT_FACE_DOWN, KEY_S);
            if (upHeld || downHeld)
            {
                if (!IsMusicStreamPlaying(climbMusic)) PlayMusicStream(climbMusic);
                UpdateMusicStream(climbMusic);
            }
            else
            {
                StopMusicStream(climbMusic);
            }
            return;
        }

        if (CanClimb)
        {
            bool climbUp = (PlayerNum == 1) ? IsKeyPressed(KEY_UP) : P2Pressed(GAMEPAD_BUTTON_LEFT_FACE_UP, KEY_W);
            bool climbDown = (PlayerNum == 1) ? IsKeyPressed(KEY_DOWN) : P2Pressed(GAMEPAD_BUTTON_LEFT_FACE_DOWN, KEY_S);

            if (climbUp)
            {
                OnLadder = true;
                Position.y -= 1.0f;
                UpdateCollider();
            }
            else if (climbDown)
            {
                OnLadder = true;
                Position.y += 1.0f;
                UpdateCollider();
            }
        }

        // --- Jump ---
        // Gamepad: A button (GAMEPAD_BUTTON_RIGHT_FACE_DOWN) | Keyboard fallback: LEFT_SHIFT
        bool jumpKey = (PlayerNum == 1) ? IsKeyPressed(KEY_SPACE)
            : P2Pressed(GAMEPAD_BUTTON_RIGHT_FACE_DOWN, KEY_LEFT_SHIFT);
        if (jumpKey && tryJump())
        {
            PlaySound(jumpSound);
            lockedVelocityX = marioVelocity.x;
            isJumping = true;
            marioVelocity.y = -(float)jumpHeight;
        }
    }

    // --- Freeze horizontal velocity in air ---
    if (isJumping) marioVelocity.x = lockedVelocityX;

    // --- Gravity ---
    if (!getIsGrounded() || isFalling) marioVelocity.y += GRAVITY;

    // --- Apply velocity ---
    Position = Vector2Add(Position, marioVelocity);

    // --- Ground detection ---
    if (Position.y >= GROUND_Y)
    {
        Position.y = GROUND_Y;
        marioVelocity.y = 0.0f;
        lockedVelocityX = 0.0f;
        isJumping = false;
        setGrounded(true);
    }

    // --- Wall detection ---
    if (Position.x > 208) Position.x = 208;
    if (Position.x < 0)   Position.x = 0;

    // --- Debug keys ---
    if (IsKeyPressed(KEY_R)) Position.y -= 32;
    if (IsKeyPressed(KEY_Q)) Position.y += 32;

    // --- Sync floor collider ---
    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 16;

    // --- Animation ---
    bool marioMoving = marioVelocity.x != 0.0f || marioVelocity.y != 0.0f;

    ++frameDelayCounter;
    if (frameDelayCounter > FRAME_DELAY)
    {
        frameDelayCounter = 0;

        if (justClimbedLadder)
        {
            frameRec.y = 1 * SpriteSize;
            frameRec.width = SpriteSize;
            frameRec.x = SpriteSize * (float)(4 + climbFinishFrame);
            ++finishFrameDelayCounter;
            if (finishFrameDelayCounter > 2)
            {
                finishFrameDelayCounter = 0;
                if (climbFinishFrame >= 2)
                {
                    justClimbedLadder = false;
                    climbFinishFrame = 0;
                    frameRec.y = 0.0f;
                }
                else
                {
                    ++climbFinishFrame;
                }
            }
        }
        else if (marioMoving && !isJumping)
        {
            frameRec.y = 0.0f;
            ++frameIndex;
            frameIndex %= NUM_FRAMES;
            frameRec.x = SpriteSize * (float)frameIndex;
        }
        else if (isJumping)
        {
            frameRec.y = 0.0f;
            frameRec.x = 3 * SpriteSize;
        }
        else
        {
            frameRec.y = 0.0f;
            frameIndex = 0;
            frameRec.x = 0.0f;
        }
    }
}

// ------------------------------------------------------------
// Debug draw
// ------------------------------------------------------------
void DrawMarioCollider()
{
    DrawPixel(Mario.FloorCollider.x, Mario.FloorCollider.y, YELLOW);
    DrawPixel(Mario.Position.x, Mario.Position.y, RED);
    if (GetHammerTime())
        DrawRectangle(Mario.HammerHitbox.x, Mario.HammerHitbox.y,
            Mario.HammerHitbox.width, Mario.HammerHitbox.height, BROWN);
}

// ------------------------------------------------------------
// Unload
// ------------------------------------------------------------
void Player::Unload()
{
    UnloadTexture(HammerTexture);

    // Audio is static — only unload once
    if (jumpSound.frameCount != 0)
    {
        UnloadSound(jumpSound);
        UnloadMusicStream(walkMusic);
        UnloadSound(deathSound);
        UnloadMusicStream(climbMusic);
        jumpSound = { 0 };  // mark as unloaded
    }
}

// ------------------------------------------------------------
// Die
// ------------------------------------------------------------
void Player::die()
{
    // NOTE: callers should guard with (!player.StarActive) before calling die()
    // so the star power-up grants invincibility against barrels and enemies.
    isGrounded = false;
    isJumping = false;
    isAlive = false;
    deathLoopCount = 0;
    frameIndex = 0;
    frameDelayCounter = 0;
    deathStarted = false;
    deathStartTimer = 0.0f;
}