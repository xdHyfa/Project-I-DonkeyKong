#include "raylib.h"
#include "Entities/barrel.h"
#include "Entities/Fire_Spawner.h"
#include "Maps/Level1Map.h"
#include <iostream>
using namespace std;

/* THE ENTITIES ARE DEFINED HERE */
Fire Fire1;
Fire Fire2;
float level1StartTime = GetTime();


void Level1FireRoutine(Fire& fire) {
	if (!fire.has_Spawned) SpawnFire(fire);

	if (fire.has_Spawned) {
		fire.Movement();
		int FirePlatform = CheckEntityPlatform(fire);
		if (FirePlatform == 0){
		if (fire.Position.x > 208 || fire.Position.x < 0) {
			fire.ChangeDirection();
			}
		}
		if (FirePlatform == 1 || FirePlatform == 3 || FirePlatform == 5 || FirePlatform == 7) {
			if (fire.Position.x > 192 || fire.Position.x < 0) {
				fire.ChangeDirection();
			}
		}
		if (FirePlatform == 2 || FirePlatform == 4 || FirePlatform == 6) {
			if (fire.Position.x > 208 || fire.Position.x < 16) {
				fire.ChangeDirection();
			}
		}

	}
	Level1LadderCollisions(fire);
	
	if (fire.CanClimb) fire.ladderContactTime += GetFrameTime();
	else fire.ladderContactTime = 0.0f;

	if (fire.ladderContactTime > 0.2f && GetRandomValue(0,2) == 1) {
		fire.OnLadder = true;
		cout << "ONLADDER" << endl;
	}
	if (fire.OnLadder) {

	}
	else {
		Level1RampCollisions(fire);
	}
	DrawTextureRec(fire.Texture, fire.FireSprite, fire.Position, WHITE);
}

void Level1EntitiesRoutine() {
	float time_buffer = GetTime();
	if (time_buffer-level1StartTime >= 5){
		Level1FireRoutine(Fire1);
	}
	if(time_buffer-level1StartTime >= 15){
		Level1FireRoutine(Fire2);
	}

}

void UnloadLevel1Entities() {
	UnloadTexture(Fire1.Texture);
	UnloadTexture(Fire2.Texture);
}

void ResetLevel1Entities() {
	Fire1.has_Spawned = false;
	Fire2.has_Spawned = false;
}