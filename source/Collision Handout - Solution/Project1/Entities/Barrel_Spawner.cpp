#include "Entities/Barrel_Spawner.h"
#include "Maps/Level1Map.h"
#include "raylib.h"
#include "Entities/Donkey.h"

BarrelSpawner barrelSpawner;

void BarrelSpawner::Init() {
    barrels.clear();
    barrels.reserve(MAX_BARRELS);
    barrelTexture = LoadTexture("sprites/commonbarrel.png");
    blueBarrelTexture = LoadTexture("sprites/blue barrel.png");

    Barrel intro;
    intro.Texture = blueBarrelTexture;
    intro.barrelFrameWidth = BARRELSIZE + 4;
    intro.frameRec.width = intro.barrelFrameWidth;
    intro.SetPos(spawnX, spawnY);
    intro.velocityX = 0.0f;
    intro.velocityY = 0.0f;
    intro.has_Spawned = true;
    intro.ignoreGround = true;
    barrels.push_back(intro);
}

void BarrelSpawner::ResetBarrel(Barrel& b) {
    donkey.PlayThrowAnim();
    b.SetPos(spawnX, spawnY);
    b.velocityX = BARRELVELOCITY;
    b.velocityY = 0.0f;
    b.justFlipped = false;
    b.frameIndex = 0;
    b.frameDelayCounter = 0;
    b.frameRec.x = 0.0f;
    b.has_Spawned = true;
}

void BarrelSpawner::Update()
{
    if (donkey.spawnBarrel) {
        donkey.spawnBarrel = false;
        if ((int)barrels.size() < MAX_BARRELS) {
            Barrel b;
            b.Texture = barrelTexture;
            b.barrelFrameWidth = BARRELSIZE + 4;
            b.frameRec.width = b.barrelFrameWidth;
            b.SetPos(spawnX, spawnY);
            b.velocityX = BARRELVELOCITY;
            b.has_Spawned = true;
            barrels.push_back(b);
        }
    }

    for (int i = (int)barrels.size() - 1; i >= 0; i--) {
        Barrel& b = barrels[i];

        if (!b.has_Spawned) {
            barrels.erase(barrels.begin() + i);
            continue;
        }

        // Barril de intro
        if (!introBarrelDone && i == 0) {
            b.velocityX = 0.0f;
            b.ignoreGround = true;
            if (b.Position.y > SCREEN_HEIGHT + 10) {
                introBarrelDone = true;
                barrels.erase(barrels.begin() + i);
                continue;
            }
        }
        else {
            if (b.Position.y > SCREEN_HEIGHT + 10) {
                barrels.erase(barrels.begin() + i);
                continue;
            }
        }

        if (CheckDownZone(b)) {
            b.DownZoneTimer += GetFrameTime();
        }
        else {
            b.DownZoneTimer = 0;
        }

        if (b.DownZoneTimer >= 0.1f && GetRandomValue(1, 10) == 1)
            b.isLaddering = true, b.groundCooldown = 0, b.FlipDirection();

        b.Movement();
        if (!b.ignoreGround) BarrelGroundCollisions(b);
        b.BarrelAnimation();
    }
}

void BarrelSpawner::Draw() {
    for (Barrel& b : barrels) {
        if (b.has_Spawned) {
            Vector2 drawPos = { b.Position.x, b.Position.y + 1 };
            DrawTextureRec(b.Texture, b.frameRec, drawPos, WHITE);
        }
    }
}

void BarrelSpawner::Shutdown() {
    UnloadTexture(barrelTexture);
    UnloadTexture(blueBarrelTexture);
    barrels.clear();
}

void BarrelSpawner::Reset() {
    barrels.clear();
    spawnTimer = 0.0f;
    introBarrelDone = false;
}