#include "raylib.h"
#include "Headers/Fire_Spawner.h"

Fire Fire1;
Fire Fire2;
bool Fire1Spawned = false;
bool Fire2Spawned = false;

void SpawnFire() {
	if (!Fire1Spawned) {
		Fire1.texture = LoadTexture("FIREBALL.png");
		Fire1.SetPos(100,200);
		Fire1Spawned = true;
	}

}
