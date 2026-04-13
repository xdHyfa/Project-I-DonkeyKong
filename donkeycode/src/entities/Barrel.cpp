#include "raylib.h"
#include "entities/Barrel.h"
#include "core/constants.h"
#include "entities/Ground.h"

Barrel barrel1;
Barrel barrel2;

void SpawnBarrel() {
    if (!barrel1.has_Spawned) {
        barrel1.texture = LoadTexture("sprites/commonbarrel.png");
        barrel1.barrelFrameWidth = BARRELSIZE+4; 
        barrel1.frameRec.width = barrel1.barrelFrameWidth;                            
        barrel1.SetPos(30, 30);
        barrel1.velocityX = BARRELVELOCITY;
        barrel1.has_Spawned = true;
    }
}

void BarrelSpawner() {
    if (IsKeyPressed(KEY_J) && !barrel1.has_Spawned) SpawnBarrel();

    if (barrel1.has_Spawned) {
        barrel1.MoveBarrel();
        BarrelGroundCollisions(barrel1);
        barrel1.BarrelAnimation();
        DrawTextureRec(barrel1.texture, barrel1.frameRec, barrel1.BarrelPosition, WHITE);
    }
}