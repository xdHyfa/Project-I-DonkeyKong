#include "Entities/Barrel_Spawner.h"
#include "Maps/Level1Map.h"
#include "raylib.h"
#include "Entities/Donkey.h"

BarrelSpawner barrelSpawner;

void BarrelSpawner::Init() {
    barrels.clear();
    barrels.reserve(MAX_BARRELS);
    barrelTexture = LoadTexture("sprites/commonbarrel.png");  // carga una sola vez
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

void BarrelSpawner::Update() {
    
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

    for (Barrel& b : barrels) {
        if (!b.has_Spawned) {
            // Reciclado: ya sea por salir por abajo o por llegar al borde en Ramp_0
            ResetBarrel(b);
            continue;
        }

        if (b.Position.y > SCREEN_HEIGHT + 10) {
            ResetBarrel(b);
            continue;
        }
        if (CheckDownZone(b)) {
            b.DownZoneTimer += GetFrameTime();
        }
        else {
            b.DownZoneTimer = 0;
        }
        if (b.DownZoneTimer >= 0.1f && GetRandomValue(1, 15) == 1) b.isLaddering = true, b.groundCooldown = 0, b.FlipDirection();
        b.Movement();
        BarrelGroundCollisions(b);
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
    UnloadTexture(barrelTexture);  // solo una textura que descargar
    barrels.clear();
}

void BarrelSpawner::Reset() {
    barrels.clear();
    spawnTimer = 0.0f;
}
