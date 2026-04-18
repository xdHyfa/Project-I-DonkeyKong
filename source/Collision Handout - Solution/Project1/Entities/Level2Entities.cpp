#include "raylib.h"
#include "Entities/barrel.h"
#include "Entities/Fire_Spawner.h"
#include "Maps/Level2Map.h"
#include <iostream>
using namespace std;

/* THE ENTITIES ARE DEFINED HERE */
Fire Fire3;
Fire Fire4;
float level2StartTime = 0;

void SetStartTime2() {
	level2StartTime = GetTime();
}
void Level2FireRoutine(Fire& fire) {
	if (!fire.has_Spawned) SpawnFire(fire, 20 ,230);

	if (fire.has_Spawned) {
		fire.Movement();
		//int FirePlatform = CheckEntityPlatform(fire);
	/*	if (FirePlatform == 0){
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
		}*/

	}
	//Level2LadderCollisions(fire);
	
	if (fire.CanClimb) fire.ladderContactTime += GetFrameTime();
	else { 
		fire.ladderContactTime = 0.0f; 
		fire.CanClimbDown = false;
	}

	if (fire.ladderContactTime > 0.2f && GetRandomValue(0,2) == 1) {
		fire.OnLadder = true;
		cout << "ONLADDER" << endl;
		/*if (CheckDownZone(fire)) {
			fire.CanClimbDown = true;
		}*/
	}
	if (fire.OnLadder) {
	
	}
	else {
		//Level1RampCollisions(fire);
	}
	DrawTextureRec(fire.Texture, fire.FireSprite, fire.Position, WHITE);
}

void Level2EntitiesRoutine() {

	float time_buffer = GetTime();
	//if (time_buffer-level1StartTime >= 5){
	//	Level1FireRoutine(Fire1);
	//}
	//if(time_buffer-level1StartTime >= 15){
	//	Level1FireRoutine(Fire2);
	//}

}

void UnloadLevel2Entities() {
	UnloadTexture(Fire3.Texture);
	UnloadTexture(Fire4.Texture);
}

void ResetLevel2Entities() {
	Fire3.has_Spawned = false;
	Fire4.has_Spawned = false;
}