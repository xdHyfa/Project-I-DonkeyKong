#pragma once
#include "raylib.h"
#include "Core/constants.h"

// Speed of BillBala (slightly faster than Goomba's 0.4f)
#define BILLBALA_SPEED 0.65f
// Time between shots (seconds)
#define BILLBALA_FIRE_INTERVAL 3.5f
// How long bullet is active before despawning off-screen
#define BILLBALA_SPRITE_W 17
#define BILLBALA_SPRITE_H 14
#define BILLBALA_BASE_W   16
#define BILLBALA_BASE_H   18

class BillBala {
public:
    // Launcher position (edge of screen)
    Vector2 BasePos = { 0, 0 };
    // -1 = shoots left (from right edge), +1 = shoots right (from left edge)
    int direction = 1;
    // Y level for the bullet
    float bulletY = 0.0f;

    // Bullet state
    bool bulletActive = false;
    Vector2 bulletPos = { 0, 0 };
    float fireTimer = 0.0f;

    // Textures
    Texture2D bulletTex = { 0 };
    Texture2D baseTex = { 0 };

    // Sprite rects
    Rectangle bulletSrc = { 4, 3, (float)BILLBALA_SPRITE_W, (float)BILLBALA_SPRITE_H };
    Rectangle baseSrc   = { 0, 4, (float)BILLBALA_BASE_W,   (float)BILLBALA_BASE_H };

    void Init(float baseX, float baseY, int dir) {
        BasePos = { baseX, baseY };
        bulletY = baseY;
        direction = dir;
        bulletActive = false;
        fireTimer = 0.0f;
    }

    void Update() {
        fireTimer += GetFrameTime();
        if (!bulletActive && fireTimer >= BILLBALA_FIRE_INTERVAL) {
            // Fire bullet from base position
            if (direction > 0) {
                bulletPos = { BasePos.x + BILLBALA_BASE_W, bulletY };
            } else {
                bulletPos = { BasePos.x - BILLBALA_SPRITE_W, bulletY };
            }
            bulletActive = true;
            fireTimer = 0.0f;
        }

        if (bulletActive) {
            bulletPos.x += direction * BILLBALA_SPEED;
            // Despawn if off screen
            if (bulletPos.x > SCREEN_WIDTH + 20 || bulletPos.x < -20) {
                bulletActive = false;
            }
        }
    }

    void Draw() {
        // Draw base launcher at edge
        DrawTextureRec(baseTex, baseSrc, BasePos, WHITE);

        // Draw bullet (flip horizontally if going left)
        if (bulletActive) {
            Rectangle src = bulletSrc;
            if (direction < 0) src.width = -src.width;
            DrawTextureRec(bulletTex, src, bulletPos, WHITE);
        }
    }

    Rectangle GetHitbox() {
        return { bulletPos.x + 2, bulletPos.y + 1, (float)BILLBALA_SPRITE_W - 4, (float)BILLBALA_SPRITE_H - 2 };
    }

    bool IsActive() const { return bulletActive; }
    void Deactivate() { bulletActive = false; }
};
