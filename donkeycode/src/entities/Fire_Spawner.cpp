#include "raylib.h"
#include "entities/Fire_Spawner.h"
#include "core/constants.h"
Fire Fire1;
Fire Fire2;


void SpawnFire() {
	if (!Fire1.has_Spawned) {
		Fire1.texture = LoadTexture("sprites/FIREBALL.png");
		Fire1.SetPos(100,200);
		Fire1.has_Spawned = true;
	}

}
