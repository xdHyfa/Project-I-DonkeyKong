#pragma once
#include "Entities/entity.h"
#include "raylib.h"

class Goomba : public Entity {
public:
    Goomba() {
        tag = EntityTag::FIRE;
        isGrounded = true;
    }

    bool isAlive = false;
    bool isDead = false;
    float deadTimer = 0.0f;

    float platformMinX = 0.0f;
    float platformMaxX = 0.0f;
    float spawnX = 0.0f;
    float spawnY = 0.0f;

    bool facingLeft = false;

    // Walking sprite: coord (6,4), size 16x16
    Rectangle walkSprite = { 6.0f, 4.0f, 16.0f, 16.0f };
    // Flat/dead sprite: coord (26,6), size 14x9
    Rectangle flatSprite = { 26.0f, 6.0f, 14.0f, 9.0f };

    void Spawn(float x, float y, float minX, float maxX);
    void Update();
    void Draw();
    void Kill();
    void Reset();

    void Movement() override {}

    void UpdateFloorCollider() {
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
    }

    // 1px smaller on each side vs before
    Rectangle GetHitbox() {
        return { Position.x + 2, Position.y + 1, 12.0f, 13.0f };
    }

    Rectangle GetTopHitbox() {
        return { Position.x + 3, Position.y + 1, 10.0f, 4.0f };
    }
};