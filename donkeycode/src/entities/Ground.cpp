#include "raylib.h"
#include "entities/Ground.h"
#include "entities/Player.h"
#include "core/constants.h"

Texture Truss::truss;

float TrussHeight = 16;
float Ramp_0_YPos;
float Ramp_1_YPos;
float Ramp_2_YPos;
float Ramp_3_YPos;
float Ramp_4_YPos;
float Ramp_5_YPos;
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

void RampCollision(Truss* Ramp, int size, Vector2 &CollisionPoint, Vector2 &spritePosition, int spriteHeight, bool isEntityMario) {
	//Collision point should be X middle value and Y lowest point in sprite.
	
	for (int i = 0; i < size; i++) {
		if (CollisionPoint.x < (Ramp[i].TrussPos.x + 16) && CollisionPoint.x >= Ramp[i].TrussPos.x) {
			//First check if in range of Truss x (Width)
			
			if (CollisionPoint.y > (Ramp[i].TrussPos.y+8)) {
				//Check if lower than the Truss
				spritePosition.y = Ramp[i].TrussPos.y+8 -spriteHeight;
				if(isEntityMario){
				marioVelocity.y = 0.0f;
				isJumping = false;
				entityMario.setGrounded(true);
				isFalling = false;
				}
			}
			
			if ((isEntityMario && !isJumping && !isFalling) || !isEntityMario) {
				//Check if higher than the Truss and not Jumping
				if(CollisionPoint.y < (Ramp[i].TrussPos.y + 8)){
				spritePosition.y = Ramp[i].TrussPos.y +8 - spriteHeight;
				}
			}
		}
	}
}

void MarioGroundCollisions() {
	
	//DrawColliders();

	if (CheckCollisionPointRec(marioFloorCollider, Ramp_0_Zone)) {
		RampCollision(Ramp_0, 14, marioFloorCollider, marioPosition, 16, true);
	}
	else if (CheckCollisionPointRec(marioFloorCollider, Ramp_1_Zone)) {
		RampCollision(Ramp_1, 13, marioFloorCollider, marioPosition, 16, true);
	}
	else if (CheckCollisionPointRec(marioFloorCollider, Ramp_2_Zone)) {
		RampCollision(Ramp_2, 13, marioFloorCollider, marioPosition, 16, true);
	}
	else if (CheckCollisionPointRec(marioFloorCollider, Ramp_3_Zone)) {
		RampCollision(Ramp_3, 13, marioFloorCollider, marioPosition, 16, true);
	}
	else if (CheckCollisionPointRec(marioFloorCollider, Ramp_4_Zone)) {
		RampCollision(Ramp_4, 13, marioFloorCollider, marioPosition, 16, true);
	}
	else if (CheckCollisionPointRec(marioFloorCollider, Ramp_5_Zone)) {
		RampCollision(Ramp_5, 13, marioFloorCollider, marioPosition, 16, true);
	}
	else {
		isFalling = true;
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