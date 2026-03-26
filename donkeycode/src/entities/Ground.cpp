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
				}
			}
			
			if ((isEntityMario && !isJumping) || !isEntityMario) {
				//Check if higher than the Truss and not Jumping
				if(CollisionPoint.y < (Ramp[i].TrussPos.y + 8)){
				spritePosition.y = Ramp[i].TrussPos.y +8 - spriteHeight;
				}
			}
		}
	}
}