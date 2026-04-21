#include "raylib.h"
#include "Entities/barrel.h"
#include "Entities/Fire_Spawner.h"
#include "Maps/Level2Map.h"
#include <iostream>
using namespace std;

/* THE ENTITIES ARE DEFINED HERE */
Fire Fire3;
Fire Fire4;
Fire Fire5;
Fire Fire6;
Fire Fire7;

float level2StartTime = 0;

enum Levels {
	Base1 = 231,
	Base2 = 191,
	Base3 = 150,
	Base4 = 109,
	Base5 = 68
};

void SetStartTime2() {
	level2StartTime = GetTime();
}
void Level2FireRoutine(Fire& fire) {
	

	if (!fire.has_Spawned) {
		int RandomLevel = GetRandomValue(1, 5);
		if (RandomLevel == 1) RandomLevel = Base1;
		else if (RandomLevel == 2) RandomLevel = Base2;
		else if (RandomLevel == 3) RandomLevel = Base3;
		else if (RandomLevel == 4) RandomLevel = Base4;
		else if (RandomLevel == 5) RandomLevel = Base5;
		SpawnFire(fire, 20, RandomLevel, 2);
	}
	if (fire.has_Spawned) {
		fire.Movement();
		int FirePlatform = Level2CheckEntityPlatform(fire);
		if (FirePlatform == 0) {
			if (fire.Position.x > 208) {
				fire.ChangeDirection();
				fire.Position.x = 208;
			}
			else if (fire.Position.x < 0) {
				fire.ChangeDirection();
				fire.Position.x = 0;
			}
		}
		if (FirePlatform == 1) {
			if (fire.Position.x > 200) {
				fire.ChangeDirection();
				fire.Position.x = 200;
			}
			else if (fire.Position.x < 8) {
				fire.ChangeDirection();
				fire.Position.x = 8;
			}
		}
		if (FirePlatform == 2) {
			if (fire.Position.x > 192) {
				fire.ChangeDirection();
				fire.Position.x = 192;
			}
			else if (fire.Position.x < 16) {
				fire.ChangeDirection();
				fire.Position.x = 16;
			}
		}
		if (FirePlatform == 3) {
			if (fire.Position.x > 184 || fire.Position.x < 24) {
				fire.ChangeDirection();
			}
			if (fire.Position.x > 184) {
				fire.ChangeDirection();
				fire.Position.x = 184;
			}
			else if (fire.Position.x < 24) {
				fire.ChangeDirection();
				fire.Position.x = 24;
			}
		}
		if (FirePlatform == 4) {
			if (fire.Position.x > 176) {
				fire.ChangeDirection();
				fire.Position.x = 176;
			}
			else if (fire.Position.x < 32) {
				fire.ChangeDirection();
				fire.Position.x = 32;
			}
		}

	}
	Level2LadderCollisions(fire);

	if (fire.CanClimb) fire.ladderContactTime += GetFrameTime();
	else {
		fire.ladderContactTime = 0.0f;
		fire.CanClimbDown = false;
	}

	if ((fire.ladderContactTime > 0.1f && (GetRandomValue(0, 2) == 1)) && fire.ladderCooldown > 1.5f) {
		fire.OnLadder = true;
		cout << "ONLADDER" << endl;
		if (Level2CheckDownZone(fire)) {
			fire.CanClimbDown = true;
		}
	}
	if (fire.OnLadder) {
		fire.ladderCooldown = 0;
		fire.climbTimer += GetFrameTime();
		if (fire.climbTimer > 1.75f) {
			fire.OnLadder = false;
			fire.climbTimer = 0.0f;
		}
	}
	else {
		fire.climbTimer = 0.0f;
		Level2RampCollisions(fire);
		fire.ladderCooldown += GetFrameTime();
	}

	fire.Position.y += 2;

	fire.bounceTick += GetFrameTime();
	if (fire.bounceTick >= 0.5f) {
		DrawTextureRec(fire.Texture, fire.FireSprite, fire.Position, WHITE);
		if (fire.bounceTick >= 0.65f) {
			fire.bounceTick = 0;
		}
	}
	else {
		Vector2 BouncePos = { fire.Position.x, fire.Position.y - 2 };

		DrawTextureRec(fire.Texture, fire.FireSprite, BouncePos, WHITE);

	}
	fire.Position.y -= 2;
}

void Level2EntitiesRoutine() {

	float time_buffer = GetTime();
	if (time_buffer-level2StartTime >= 5){
		Level2FireRoutine(Fire3);
	}
	if(time_buffer-level2StartTime >= 7){
		Level2FireRoutine(Fire4);
	}
	if (time_buffer - level2StartTime >= 10) {
		Level2FireRoutine(Fire5);
	}
	if (time_buffer - level2StartTime >= 12) {
		Level2FireRoutine(Fire6);
	}
	if (time_buffer - level2StartTime >= 15) {
		Level2FireRoutine(Fire7);
	}

}

void UnloadLevel2Entities() {
	UnloadTexture(Fire3.Texture);
	UnloadTexture(Fire4.Texture);
}

void ResetLevel2Entities() {
	Fire3.has_Spawned = false;
	Fire4.has_Spawned = false;
}