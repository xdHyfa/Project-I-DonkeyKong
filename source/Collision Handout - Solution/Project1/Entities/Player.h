#pragma once
#include "raylib.h"
#include "Entities/entity.h"

class Player : public Entity
{
public:
    const int playerSize = 16;

    Vector2 marioVelocity = { 0.0f, 0.0f };

    bool  isAlive = true;
    bool isJumping = false;
    bool isFalling = false;

    bool justClimbedLadder = false;
    int climbFinishFrame = 0;
    int PlayerNum = 1;

    bool deathStarted = false;
    float deathStartTimer = 0.0f;

    const Rectangle boxCollider = { 0.0f, 0.0f, SpriteSize, SpriteSize };

    const float velocity = 1.5f;
    const int jumpHeight = 6;

    int deathLoopCount = 0;

    Rectangle HammerHitbox = { 0, 0, 16, 16 };

    // --- Animation state (per-player) ---
    Rectangle frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };
    unsigned  frameDelayCounter = 0;
    unsigned  frameIndex = 0;
    unsigned  finishFrameDelayCounter = 0;
    bool      LooksRight = true;

    // --- Movement state (per-player) ---
    float lockedVelocityX = 0.0f;

    // --- Hammer state (per-player) ---
    float     HammerTimer = 0.0f;
    float     HammerTick = 0.0f;
    Vector2   HammerPos = { 0.0f, 0.0f };
    Rectangle HammerSelector = { 0, 0, 12, 16 };
    Texture2D HammerTexture = { 0 };

    // --- Audio (static: one instance shared, loaded once) ---
    static Sound jumpSound;
    static Music walkMusic;
    static Sound deathSound;
    static Music climbMusic;

    void die();

    bool tryJump()
    {
        if (isGrounded && !isFalling)
        {
            isGrounded = false;
            return true;
        }
        return false;
    }
    void Movement() override;
    void Setup();
    void Unload();

    void UpdateCollider() {
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
    }

    Player(bool isPlayer1) {
        if (isPlayer1) {
            PlayerNum = 1;
            tag = EntityTag::PLAYER1;
        }
        else {
            PlayerNum = 2;
            tag = EntityTag::PLAYER2;
        }
    }

    bool CheckHammerHitbox(Entity& entity) {
        return CheckCollisionRecs(entity.getHitbox(), HammerHitbox);
    }
};

extern Player Mario;
extern Player Luigi;

void DrawMarioCollider();