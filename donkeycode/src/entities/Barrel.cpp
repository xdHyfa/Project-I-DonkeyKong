#include "raylib.h"
#include "entities/Barrel.h"
#include "core/constants.h"
#include "entities/Ground.h"
Barrel barrel1;
Barrel barrel2;

void SpawnBarrel() {
	if (!barrel1.has_Spawned) {
		barrel1.texture = LoadTexture("sprites/commonbarrel.png");
		barrel1.SetPos(100, 200);
		barrel1.has_Spawned = true;
	}

}

void BarrelSpawner() {
	if (IsKeyPressed(KEY_J) && !barrel1.has_Spawned) SpawnBarrel();

	if (barrel1.has_Spawned) {
		barrel1.MoveBarrel();
		RampCollision(Ramp_0, 14, barrel1.BarrelFloorCollider, barrel1.BarrelPosition, 14, false);
		DrawTextureRec(barrel1.texture, barrel1.BarrelSprite, barrel1.BarrelPosition, WHITE);
	}
}