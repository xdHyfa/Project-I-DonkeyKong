#include "raylib.h"
#include "Entities/Ground.h"
#include "Entities/Player.h"
#include "Core/constants.h"



Texture Truss::truss;


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
			Ramp[i].setPos(adderX, adderY);
			adderX += 16;
	}

}

void BaseSetter(Truss* Base, int size, int InitialX ,int Ypos, bool isHalved) {
	int adderX = InitialX;
	if (isHalved) {
		Base[0].isHalf = true;
		Base[size-1].isHalf = true;
	}
	for (int i = 0; i < size; i++) {
		Base[i].setPos(adderX, Ypos);
		if (Base[i].isHalf) {
			adderX += 8;
		}
		else {
			adderX += 16;
		}
	}
}

void RampDrawer(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		if (!Ramp[i].isHalf)
			DrawTexture(Ramp[i].truss, Ramp[i].TrussPos.x, Ramp[i].TrussPos.y, WHITE);
		else
			DrawTextureRec(Ramp[i].truss, Ramp[i].HalfSprite, Ramp[i].TrussPos, WHITE);
	}
}

void RampCollision(Truss* Ramp, int size, Entity &entity) {
	//Collision point should be X middle value and Y lowest point in sprite.
	
	for (int i = 0; i < size; i++) {
		if (entity.FloorCollider.x < (Ramp[i].TrussPos.x + 16) && entity.FloorCollider.x >= Ramp[i].TrussPos.x) {
			//First check if in range of Truss x (Width)

			if (entity.FloorCollider.y >= (Ramp[i].TrussPos.y+8)) {
				//Check if lower than the Truss
				if (!Ramp[i].hasLadderBelow || !entity.OnLadder) {
					entity.OnLadder = false;
					entity.Position.y = Ramp[i].TrussPos.y + 8 - entity.SpriteSize;

					if (entity.tag == EntityTag::PLAYER1) {
						Mario.marioVelocity.y = 0.0f;
						Mario.isJumping = false;
						Mario.setGrounded(true);
						Mario.isFalling = false;
					}
					if (entity.tag == EntityTag::PLAYER2) {
						Luigi.marioVelocity.y = 0.0f;
						Luigi.isJumping = false;
						Luigi.setGrounded(true);
						Luigi.isFalling = false;
					}
				}
			}
			
			if (!entity.OnLadder && ((entity.tag == EntityTag::PLAYER1 && !Mario.isJumping && !Mario.isFalling) || (entity.tag == EntityTag::PLAYER2 && !Luigi.isJumping && !Luigi.isFalling) || entity.tag != EntityTag::PLAYER1 && entity.tag != EntityTag::PLAYER2)) {
				//Check if higher than the Truss and not Jumping or climbing a ladder
				if(entity.FloorCollider.y < (Ramp[i].TrussPos.y + 8)){
				entity.Position.y = Ramp[i].TrussPos.y +8 - entity.SpriteSize;
				}
			}
			return;
		}
	}
}

