#include "raylib.h"
#include "Entities/barrel.h"
#include "Entities/Fire_Spawner.h"
#include "Maps/Level1Map.h"
#include <iostream>
using namespace std;

/* THE ENTITIES ARE DEFINED HERE */
Fire Fire1;
Fire Fire2;
float level1StartTime = 0;
float FireCooldown = 0;
bool LevelStart = false;

void SetStartTime() {
	level1StartTime = GetTime();

}
void Level1FireRoutine(Fire& fire) {
	if (!LevelStart) {
		if (!fire.has_Spawned) SpawnFire(fire, 20 ,230, 1);
		Fire1.setGrounded(true);
		Fire2.setGrounded(true);
	}
	else {
		if (!fire.has_Spawned && FireCooldown > 8.0f ) {
			cout << "Fire Respawned" << endl;
			SpawnFire(fire, 20, 230, 1);
			FireCooldown = 0;
		}
		else if(!fire.has_Spawned) {
			FireCooldown += GetFrameTime();
		}
	}

	if (fire.has_Spawned) {
		fire.UpdateSpawnFx();
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

	
		Level1LadderCollisions(fire);
		
		if (fire.CanClimb) fire.ladderContactTime += GetFrameTime();
		else { 
			fire.ladderContactTime = 0.0f; 
			fire.CanClimbDown = false;
		}

		if ((fire.ladderContactTime > 0.1f && (GetRandomValue(0,2) == 1)) && fire.ladderCooldown > 1.5f) {
			fire.OnLadder = true;
			cout << "ONLADDER" << endl;
			if (CheckDownZone(fire)) {
				fire.CanClimbDown = true;
			}
		}
		if (fire.OnLadder) {
			fire.ladderCooldown = 0;
		}
		else {
			Level1RampCollisions(fire);
			fire.ladderCooldown += GetFrameTime();
		}

		fire.bounceTick += GetFrameTime();
		if (fire.bounceTick >= 0.5f){
			DrawTextureRec(fire.Texture, fire.FireSprite, fire.Position, WHITE);
			fire.DrawSpawnFx();
			if (fire.bounceTick >= 0.65f){
				fire.bounceTick = 0;
			}
		}
		else {
			Vector2 BouncePos = { fire.Position.x, fire.Position.y - 2 };
			DrawTextureRec(fire.Texture, fire.FireSprite, BouncePos, WHITE);
			fire.DrawSpawnFx();	
		}
	}
}

void Level1EntitiesRoutine() {

	float time_buffer = GetTime();
	if (time_buffer-level1StartTime >= 5){
		Level1FireRoutine(Fire1);
	}
	if(time_buffer-level1StartTime >= 12){
		Level1FireRoutine(Fire2);
	}

	if (Fire2.has_Spawned && !LevelStart) {
		LevelStart = true;
	}

}

void UnloadLevel1Entities() {
	UnloadTexture(Fire1.Texture);
	UnloadTexture(Fire2.Texture);
}

void ResetLevel1Entities() {
	Fire1.has_Spawned = false;
	Fire2.has_Spawned = false;
	LevelStart = false;
}

