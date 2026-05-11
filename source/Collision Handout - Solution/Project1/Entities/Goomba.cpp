#include "Entities/Goomba.h"
#include "raylib.h"
#include "Maps/Level15Map.h"
#include <iostream>
using namespace std;

#define GOOMBA_SPEED        0.4f
#define GOOMBA_RESPAWN_SEC  4.0f
#define GOOMBA_FLAT_SEC     1.0f

void Goomba::Spawn(float x, float y, float minX, float maxX) {
    Position = { x, y };
    spawnX = x;
    spawnY = y;
    platformMinX = minX;
    platformMaxX = maxX;
    isGrounded = true;
    isAlive = true;
    isDead = false;
    deadTimer = 0.0f;
    facingLeft = false;
    UpdateFloorCollider();
}

void Goomba::Kill() {
    if (!isAlive || isDead) return;
    isDead = true;
    isAlive = false;
    deadTimer = 0.0f;
}

void Goomba::Reset() {
    isAlive = false;
    isDead = false;
    deadTimer = 0.0f;
}

void Goomba::Update() {
    if (isDead) {
        deadTimer += GetFrameTime();
        // Flat sprite shown for 1 second, then invisible until respawn at 4s
        if (deadTimer >= GOOMBA_RESPAWN_SEC) {
            Spawn(spawnX, spawnY, platformMinX, platformMaxX);
        }
        return;
    }
    if (!isAlive) return;

    float speed = facingLeft ? -GOOMBA_SPEED : GOOMBA_SPEED;
    Position.x += speed;

    if (Position.x <= platformMinX) { Position.x = platformMinX; facingLeft = false; }
    if (Position.x >= platformMaxX) { Position.x = platformMaxX; facingLeft = true; }

    UpdateFloorCollider();
    Level15RampCollisions(*this);
}

void Goomba::Draw() {
    if (isDead) {
        // Only show flat sprite for the first second
        if (deadTimer <= GOOMBA_FLAT_SEC) {
            Vector2 pos = { Position.x + 1.0f, Position.y + 7.0f };
            DrawTextureRec(Texture, flatSprite, pos, WHITE);
        }
        return;
    }
    if (!isAlive) return;

    Rectangle src = walkSprite;
    if (facingLeft) src.width = -16.0f;
    DrawTextureRec(Texture, src, Position, WHITE);
}