#include "raylib.h"
#include "entities/Fire_Spawner.h"
#include "core/constants.h"
#include "entities/Ground.h"
Fire Fire1;
Fire Fire2;

void SpawnFire() {
	if (!Fire1.has_Spawned) {
		Fire1.texture = LoadTexture("sprites/FIREBALL.png");
		Fire1.SetPos(100,200);
		Fire1.has_Spawned = true;
	}

}

void FireSpawner() {
	if (IsKeyPressed(KEY_K) && !Fire1.has_Spawned) SpawnFire();
	
	if (Fire1.has_Spawned) {
		Fire1.MoveFire();
		RampCollision(Ramp_0, 14, Fire1.FireFloorCollider, Fire1.FirePosition, 16, false);
		DrawTextureRec(Fire1.texture, Fire1.FireSprite, Fire1.FirePosition, WHITE);
	}
}