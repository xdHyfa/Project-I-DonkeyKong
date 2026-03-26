#include "raylib.h"
#include "entities/Ground.h"
#include "entities/Player.h"

Texture Truss::truss;

Truss Ramp_0[14];
Truss Ramp_1[13];
Truss Ramp_2[13];
Truss Ramp_3[13];
Truss Ramp_4[13];
Truss Ramp_5[13];

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