#include "raylib.h"
#include "Entities/Ground.h"
#include "Entities/Player.h"
#include "Core/constants.h"

Texture Truss::truss;

float TrussHeight = 16;
float Ramp_0_YPos;
float Ramp_1_YPos;
float Ramp_2_YPos;
float Ramp_3_YPos;
float Ramp_4_YPos;
float Ramp_5_YPos;

/*---ACTIVE AREA HITBOX FOR EACH RAMP---*/
//Els numeros sueltos els vaig fer manualment a ull comparant amb el joc original 
Rectangle Ramp_0_Zone = {0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20};
Rectangle Ramp_1_Zone = { 0, SCREEN_HEIGHT - 20 - 36, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_2_Zone = { 16, SCREEN_HEIGHT - 20 - 36 -34, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_3_Zone = { 0, SCREEN_HEIGHT - 20 - 36 -34 -32, SCREEN_WIDTH -16, 22 };
Rectangle Ramp_4_Zone = { 16, SCREEN_HEIGHT - 20 - 36 -34 -32 -34, SCREEN_WIDTH -16, 22 };
Rectangle Ramp_5_Zone = { 0, SCREEN_HEIGHT - 20 - 36 -34 -32 -34 -32, SCREEN_WIDTH -16, 22 };

Truss Ramp_0[14];
Truss Ramp_1[13];
Truss Ramp_2[13];
Truss Ramp_3[13];
Truss Ramp_4[13];
Truss Ramp_5[13];

void DrawColliders() {
	DrawRectangle(0, 256 - 20, 224, 20, WHITE);
	DrawRectangle(0, 256 - 20 - 36, 224 - 16, 22, WHITE);
	DrawRectangle(16, 256 - 20 - 36 - 34, 224 - 16, 22, WHITE);
	DrawRectangle(0, 256 - 20 - 36 - 34 - 32, 224 - 16, 22, WHITE);
	DrawRectangle(16, 256 - 20 - 36 - 34 - 32 - 34, 224 - 16, 22, WHITE);
	DrawRectangle(0, 256 - 20 - 36 - 34 - 32 - 34 -32, 224 - 16, 22, WHITE);
}

void RampSetter(Truss* Ramp, int size, bool level0, bool TiltLeft, int plane, int adderY) {
	int adderX = 0;
	
	if (TiltLeft && !level0) {
		adderX += 16;
	}
	for (int i = 0; i < size; i++) {
		if (i >= plane) {
			if (TiltLeft)	adderY--;
			else			adderY++;
		}
			Ramp[i].setPos(Ramp[i].TrussPos.x + adderX, adderY);
			adderX += 16;
	}

}

void MultiRampSetter() {
	Truss::LoadSharedTexture();

	Ramp_0_YPos = (SCREEN_HEIGHT - TrussHeight) - 1;
	RampSetter(Ramp_0, 14, true, true, 7, Ramp_0_YPos);

	Ramp_1_YPos = (SCREEN_HEIGHT - TrussHeight) - 43;
	RampSetter(Ramp_1, 13, false, false, 0, Ramp_1_YPos);

	Ramp_2_YPos = (SCREEN_HEIGHT - TrussHeight) - 62;
	RampSetter(Ramp_2, 13, false, true, 0, Ramp_2_YPos);

	Ramp_3_YPos = (SCREEN_HEIGHT - TrussHeight) - 110;
	RampSetter(Ramp_3, 13, false, false, 0, Ramp_3_YPos);

	Ramp_4_YPos = (SCREEN_HEIGHT - TrussHeight) - 130;
	RampSetter(Ramp_4, 13, false, true, 0, Ramp_4_YPos);

	Ramp_5_YPos = (SCREEN_HEIGHT - TrussHeight) - 168;
	RampSetter(Ramp_5, 13, false, false, 9, Ramp_5_YPos);
}


void RampDrawer(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		DrawTexture(Ramp[i].truss, Ramp[i].TrussPos.x, Ramp[i].TrussPos.y, WHITE);
	}
}

void RampCollision(Truss* Ramp, int size,Entity &entity, bool isEntityMario) {
	//Collision point should be X middle value and Y lowest point in sprite.
	
	for (int i = 0; i < size; i++) {
		if (entity.FloorCollider.x < (Ramp[i].TrussPos.x + 16) && entity.FloorCollider.x >= Ramp[i].TrussPos.x) {
			//First check if in range of Truss x (Width)
			
			if (entity.FloorCollider.y > (Ramp[i].TrussPos.y+8)) {
				//Check if lower than the Truss
				entity.Position.y = Ramp[i].TrussPos.y+8 -entity.SpriteSize;
				if(isEntityMario){
				Mario.marioVelocity.y = 0.0f;
				Mario.isJumping = false;
				Mario.setGrounded(true);
				Mario.isFalling = false;
				}
			}
			
			if ((isEntityMario && !Mario.isJumping && !Mario.isFalling) || !isEntityMario) {
				//Check if higher than the Truss and not Jumping
				if(entity.FloorCollider.y < (Ramp[i].TrussPos.y + 8)){
				entity.Position.y = Ramp[i].TrussPos.y +8 - entity.SpriteSize;
				}
			}
		}
	}
}

void MarioGroundCollisions() {
	
	//DrawColliders();

	if (CheckCollisionPointRec(Mario.FloorCollider, Ramp_0_Zone)) {
		RampCollision(Ramp_0, 14, Mario, true);
	}
	else if (CheckCollisionPointRec(Mario.FloorCollider, Ramp_1_Zone)) {
		RampCollision(Ramp_1, 13, Mario, true);
	}
	else if (CheckCollisionPointRec(Mario.FloorCollider, Ramp_2_Zone)) {
		RampCollision(Ramp_2, 13, Mario, true);
	}
	else if (CheckCollisionPointRec(Mario.FloorCollider, Ramp_3_Zone)) {
		RampCollision(Ramp_3, 13, Mario, true);
	}
	else if (CheckCollisionPointRec(Mario.FloorCollider, Ramp_4_Zone)) {
		RampCollision(Ramp_4, 13, Mario, true);
	}
	else if (CheckCollisionPointRec(Mario.FloorCollider, Ramp_5_Zone)) {
		RampCollision(Ramp_5, 13, Mario, true);
	}
	else {
		Mario.isFalling = true;
	}
}

void BarrelGroundCollisions(Barrel& barrel) {
	Vector2& pos = barrel.BarrelPosition;
	Vector2& col = barrel.BarrelFloorCollider;
	bool onGround = false;

	auto checkTrussArray = [&](Truss* Ramp, int size, float leftEdge, float rightEdge) {
		if (onGround) return;
		for (int i = 0; i < size; i++) {
			float trussLeft = Ramp[i].TrussPos.x;
			float trussRight = Ramp[i].TrussPos.x + 16;
			float trussSurface = Ramp[i].TrussPos.y + 8;

			bool inRangeX = (col.x >= trussLeft && col.x < trussRight);
			bool hitSurface = (col.y >= trussSurface && col.y <= trussSurface + barrel.velocityY + 2);

			if (inRangeX && hitSurface) {
				pos.y = trussSurface - BARRELSIZE;
				barrel.velocityY = 0.0f;
				onGround = true;
				barrel.justFlipped = false;
				return;
			}
		}
		};

	checkTrussArray(Ramp_0, 14, 0, SCREEN_WIDTH);
	checkTrussArray(Ramp_1, 13, 0, SCREEN_WIDTH - 16);
	checkTrussArray(Ramp_2, 13, 16, SCREEN_WIDTH);
	checkTrussArray(Ramp_3, 13, 0, SCREEN_WIDTH - 16);
	checkTrussArray(Ramp_4, 13, 16, SCREEN_WIDTH);
	checkTrussArray(Ramp_5, 13, 0, SCREEN_WIDTH - 16);

	if (!onGround) return;

	if (pos.x + BARRELSIZE >= SCREEN_WIDTH) {
		pos.x = SCREEN_WIDTH - BARRELSIZE;
		barrel.FlipDirection();
	}
	else if (pos.x <= 0) {
		pos.x = 0;
		barrel.FlipDirection();
	}
}