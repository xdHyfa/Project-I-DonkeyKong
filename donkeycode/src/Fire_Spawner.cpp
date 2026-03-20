#include "raylib.h"
#include "Headers/Fire_Spawner.h"
#include "Headers/constants.h"
Fire Fire1;
Fire Fire2;


void SpawnFire() {
	if (!Fire1.has_Spawned) {
		Fire1.texture = LoadTexture("FIREBALL.png");
		Fire1.SetPos(100,200);
		Fire1.has_Spawned = true;
	}

}
