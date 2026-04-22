#include "raylib.h"
#include "Entities/Fire_Spawner.h"
#include "Core/constants.h"
#include "Maps/Level1Map.h"
#include "Scenes/scenes.h"
#include <iostream>
using namespace std;

#define FIREVELOCITY 1.25f
#define FIRE2VELOCITY 1.5f

void Fire::UpdateSpawnFx() {
    if (!spawnFxPlaying) return;

    spawnFxTick += GetFrameTime();
    if (spawnFxTick >= 0.25f) {
        spawnFxTick = 0.0f;
        spawnFxFrame++;

        if (spawnFxFrame >= 4) {
            spawnFxFrame = 0;
            spawnFxCycle++;
        }
        if (spawnFxCycle >= 2) {
            spawnFxPlaying = false;
            UnloadTexture(SpawnTexture);
        }
    }
    SpawnFxSprite.x = spawnFxFrame * 16.0f;
}

void Fire::DrawSpawnFx() {
    if (!spawnFxPlaying) return;
    DrawTextureRec(SpawnTexture, SpawnFxSprite, spawnFxPos, WHITE);
}

void Fire::PlayAnimation() {
    fireTick += GetFrameTime();
    if (Facing_left) { FireSprite.width = -16.0f; }
    else { FireSprite.width = 16.0f; }

    if (fireTick >= 0.1f && !Anim_blink) {
        FireSprite.x += 16;
        Anim_blink = true;
        fireTick = 0;
    }
    if (fireTick >= 0.1f && Anim_blink) {
        FireSprite.x -= 16;
        Anim_blink = false;
        fireTick = 0;
    }
}

void Fire::Movement() {
    PlayAnimation();
    hitbox = { Position.x, Position.y };
    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 16;
    if (CanClimb && OnLadder) {
        if (CanClimbDown) {
            Position.y += 1;
        }
        else {
            Position.y -= 1;
        }
        return;
    }
    if (GetCurrentScene() == LEVEL1) {
        if (!Facing_left) Position.x += FIREVELOCITY;
        else Position.x -= FIREVELOCITY;
    }
    else {
        if (!Facing_left) Position.x += FIRE2VELOCITY;
        else Position.x -= FIRE2VELOCITY;
    }

    if (GetRandomValue(1, 150) == 100) {
        ChangeDirection();
    }
}

void SpawnFire(Fire& fire, int x, int y, int sprite) {
    if (!fire.has_Spawned) {
        if (sprite == 1) {
            fire.Texture = LoadTexture("sprites/FIREBALL.png");
        }
        else {
            fire.Texture = LoadTexture("sprites/SMALLFIREBALL.png");
        }
        // Always load blue variant for hammer time
        fire.BlueTexture = LoadTexture("sprites/FIREBALL2.png");
        fire.SetPos(x, y);
        fire.has_Spawned = true;
    }

    fire.SpawnTexture = LoadTexture("sprites/fire.png");
    fire.spawnFxPlaying = true;
    fire.spawnFxTick = 0.0f;
    fire.spawnFxFrame = 0;
    fire.spawnFxCycle = 0;
    fire.spawnFxPos = { (float)x + 3, (float)y - 15 };
}