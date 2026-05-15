#pragma once
#include "raylib.h"
#include "Core/constants.h"

#define BILLBALA_SPEED          1.2f
#define BILLBALA_FIRE_INTERVAL  3.5f
#define BILLBALA_SPRITE_W       17
#define BILLBALA_SPRITE_H       14
#define BILLBALA_BASE_W         16
#define BILLBALA_BASE_H         18

// Death animation constants
#define BILLBALA_DEAD_RISE_VEL  -2.5f
#define BILLBALA_DEAD_GRAVITY    0.18f

class BillBala {
public:
    Vector2 BasePos = { 0, 0 };
    int     direction = 1;     // +1 right, -1 left
    float   bulletY = 0.0f;

    bool    bulletActive = false;
    bool    bulletDead = false;  // death animation playing
    Vector2 bulletPos = { 0, 0 };
    float   bulletVelY = 0.0f;  // vertical velocity during death
    float   fireTimer = 0.0f;

    Texture2D bulletTex = { 0 };
    Texture2D baseTex = { 0 };

    Rectangle bulletSrc = { 4, 3, (float)BILLBALA_SPRITE_W, (float)BILLBALA_SPRITE_H };
    Rectangle baseSrc = { 0, 4, (float)BILLBALA_BASE_W,   (float)BILLBALA_BASE_H };

    void Init(float baseX, float baseY, float bY, int dir) {
        BasePos = { baseX, baseY };
        bulletY = bY;
        direction = dir;
        bulletActive = false;
        bulletDead = false;
        fireTimer = 0.0f;
        bulletVelY = 0.0f;
    }

    // Called when Mario stomps the bullet
    void Stomp() {
        bulletActive = false;
        bulletDead = true;
        bulletVelY = BILLBALA_DEAD_RISE_VEL;
    }

    void Update() {
        // --- Death animation ---
        if (bulletDead) {
            bulletVelY += BILLBALA_DEAD_GRAVITY;
            bulletPos.y += bulletVelY;
            if (bulletPos.y > SCREEN_HEIGHT + 20) {
                bulletDead = false;  // done, reset for next fire
            }
            return;
        }

        // --- Normal fire logic ---
        if (!bulletActive) {
            fireTimer += GetFrameTime();
            if (fireTimer >= BILLBALA_FIRE_INTERVAL) {
                // Spawn off-screen so bullet travels in through the base
                if (direction > 0)
                    bulletPos = { -(float)BILLBALA_SPRITE_W, bulletY };
                else
                    bulletPos = { (float)SCREEN_WIDTH, bulletY };
                bulletActive = true;
                fireTimer = 0.0f;
            }
        }

        if (bulletActive) {
            bulletPos.x += direction * BILLBALA_SPEED;
            if (bulletPos.x > SCREEN_WIDTH + 20 || bulletPos.x < -20)
                bulletActive = false;
        }
    }

    void Draw() {
        // Draw bullet first (base draws on top at the wall edge)
        if (bulletActive || bulletDead) {
            Rectangle src = bulletSrc;
            if (direction < 0) src.width = -src.width;
            DrawTextureRec(bulletTex, src, bulletPos, WHITE);
        }

        // Draw base launcher (never flipped - same sprite works for both sides
        // because positioning handles the wall alignment)
        DrawTextureRec(baseTex, baseSrc, BasePos, WHITE);
    }

    // Full body hitbox (kills player on contact)
    Rectangle GetHitbox() {
        return { bulletPos.x + 2, bulletPos.y + 1,
                 (float)BILLBALA_SPRITE_W - 4, (float)BILLBALA_SPRITE_H - 2 };
    }

    // Narrow top hitbox (stomp detection)
    Rectangle GetTopHitbox() {
        return { bulletPos.x + 3, bulletPos.y + 1,
                 (float)BILLBALA_SPRITE_W - 6, 4.0f };
    }

    bool IsActive() const { return bulletActive; }
    bool IsDead()   const { return bulletDead; }
    void Deactivate() { bulletActive = false; }
};