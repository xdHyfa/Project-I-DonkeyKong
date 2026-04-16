#include "raylib.h"
#include "Entities/Barrel.h"
#include "Core/constants.h"
#include "Entities/Ground.h"
#include "Maps/Level1Map.h"

Barrel barrel1;
Barrel barrel2;

void SpawnBarrel() {
    if (!barrel1.has_Spawned) {
        barrel1.Texture = LoadTexture("sprites/commonbarrel.png");
        barrel1.barrelFrameWidth = BARRELSIZE+4; 
        barrel1.frameRec.width = barrel1.barrelFrameWidth;                            
        barrel1.SetPos(30, 30);
        barrel1.velocityX = BARRELVELOCITY;
        barrel1.has_Spawned = true;
    }
}

void BarrelRoutine() {
    if (IsKeyPressed(KEY_J) && !barrel1.has_Spawned) SpawnBarrel();

    if (barrel1.has_Spawned) {
        barrel1.Movement();
        BarrelGroundCollisions(barrel1);
        barrel1.BarrelAnimation();
        DrawTextureRec(barrel1.Texture, barrel1.frameRec, barrel1.Position, WHITE);
    }
}