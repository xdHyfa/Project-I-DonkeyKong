#include "raylib.h"
#include "entities/Ground.h"
#include "entities/Player.h"

Truss Ramp_0[14];

void RampSetter(Truss* Ramp, int size, int level, bool TiltLeft, int plane) {
	int adderX = -13;
	int adderY = 225;
	
	for (int i = 0; i < size; i++) {
		if (i >= plane) {
			if (TiltLeft)	adderY--;
			else			adderY++;
		}
			Ramp[i].setPos(Ramp[i].TrussPos.x + adderX, adderY);
			adderX += 16;
			Ramp[i].truss = LoadTexture("sprites/TRUSS.png");
	}

}

void RampDrawer(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		DrawTexture(Ramp[i].truss, Ramp[i].TrussPos.x, Ramp[i].TrussPos.y, WHITE);
	}
}

void RampCollision(Truss* Ramp, int size, int plane) {
	for (int i = 0; i < size; i++) {
		if (marioPosition.x <= (Ramp[i].TrussPos.x + 8) && marioPosition.x >= (Ramp[i].TrussPos.x - 8)) {
			//First check if in range of Truss
			
			if (marioPosition.y >= (Ramp[i].TrussPos.y-9)) {
				//Check if lower than the Truss
				if (i < plane)
					marioPosition.y = Ramp[i].TrussPos.y - 9;

				else
					marioPosition.y = Ramp[i].TrussPos.y - 8;
				marioVelocity.y = 0.0f;
				isJumping = false;
				entityMario.setGrounded(true);
			}
			if (isJumping == false && marioPosition.y < (Ramp[i].TrussPos.y - 9)) {
				//Check if higher than the Truss and not Jumping
				if (i < plane)
					marioPosition.y = Ramp[i].TrussPos.y - 9;
				
				else
				marioPosition.y = Ramp[i].TrussPos.y - 8;
			}
		}
	}
}