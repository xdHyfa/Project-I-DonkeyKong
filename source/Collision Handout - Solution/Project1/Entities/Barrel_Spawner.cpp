#include "Entities/Barrel_Spawner.h"
#include "Maps/Level1Map.h"  
#include "raylib.h"

BarrelSpawner barrelSpawner;

void BarrelSpawner::Init() {
    barrels.reserve(MAX_BARRELS);
}

void BarrelSpawner::Update() {
    // Spawneo progresivo hasta llegar a MAX_BARRELS
    if ((int)barrels.size() < MAX_BARRELS) {
        spawnTimer += GetFrameTime();
        if (spawnTimer >= BARREL_SPAWN_INTERVAL) {
            spawnTimer = 0.0f;

            Barrel b;
            b.Texture = LoadTexture("sprites/commonbarrel.png");
            b.barrelFrameWidth = BARRELSIZE + 4;
            b.frameRec.width = b.barrelFrameWidth;
            b.SetPos(spawnX, spawnY);
            b.velocityX = BARRELVELOCITY;
            b.has_Spawned = true;

            barrels.push_back(b);
        }
    }

    // Update de cada barril activo
    for (Barrel& b : barrels) {
        if (b.has_Spawned) {
            b.Movement();
            BarrelGroundCollisions(b);
            b.BarrelAnimation();
        }
    }
}

void BarrelSpawner::Draw() {
    for (Barrel& b : barrels) {
        if (b.has_Spawned) {
            DrawTextureRec(b.Texture, b.frameRec, b.Position, WHITE);
        }
    }
}

void BarrelSpawner::Shutdown() {
    for (Barrel& b : barrels) {
        UnloadTexture(b.Texture);
    }
    barrels.clear();
}