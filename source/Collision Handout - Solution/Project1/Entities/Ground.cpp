#include "raylib.h"
#include "Entities/Ground.h"
#include "Entities/Player.h"
#include "Core/constants.h"


/*---MAP DATA VARIABLES HERE---*/
Texture Truss::truss;

float TrussHeight = 16;
float Ramp_0_YPos;
float Ramp_1_YPos;
float Ramp_2_YPos;
float Ramp_3_YPos;
float Ramp_4_YPos;
float Ramp_5_YPos;
float Ramp_6_YPos;

/*---ACTIVE AREA HITBOX FOR EACH RAMP---*/
//Els numeros sueltos els vaig fer manualment a ull comparant amb el joc original 
Rectangle Ramp_0_Zone = { 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20 };
Rectangle Ramp_1_Zone = { 0, SCREEN_HEIGHT - 20 - 36, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_2_Zone = { 16, SCREEN_HEIGHT - 20 - 36 - 34, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_3_Zone = { 0, SCREEN_HEIGHT - 20 - 36 - 34 - 33, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_4_Zone = { 16, SCREEN_HEIGHT - 20 - 36 - 34 - 32 - 35, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_5_Zone = { 0, SCREEN_HEIGHT - 20 - 36 - 34 - 32 - 34 - 35, SCREEN_WIDTH - 16, 22 };

Truss Ramp_0[14];
Truss Ramp_1[13];
Truss Ramp_2[13];
Truss Ramp_3[13];
Truss Ramp_4[13];
Truss Ramp_5[13];
Truss Ramp_6[3];

void DrawLevel1Colliders() {
	DrawRectangle(Ramp_0_Zone.x, Ramp_0_Zone.y, Ramp_0_Zone.width, Ramp_0_Zone.height, WHITE);
	DrawRectangle(Ramp_1_Zone.x, Ramp_1_Zone.y, Ramp_1_Zone.width, Ramp_1_Zone.height, WHITE);
	DrawRectangle(Ramp_2_Zone.x, Ramp_2_Zone.y, Ramp_2_Zone.width, Ramp_2_Zone.height, WHITE);
	DrawRectangle(Ramp_3_Zone.x, Ramp_3_Zone.y, Ramp_3_Zone.width, Ramp_3_Zone.height, WHITE);
	DrawRectangle(Ramp_4_Zone.x, Ramp_4_Zone.y, Ramp_4_Zone.width, Ramp_4_Zone.height, WHITE);
	DrawRectangle(Ramp_5_Zone.x, Ramp_5_Zone.y, Ramp_5_Zone.width, Ramp_5_Zone.height, WHITE);
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

void Level1RampSetter() {
	Truss::LoadSharedTexture();

	Ramp_0_YPos = (SCREEN_HEIGHT - TrussHeight) - 1;
	RampSetter(Ramp_0, 14, true, true, 7, Ramp_0_YPos);

	Ramp_1_YPos = (SCREEN_HEIGHT - TrussHeight) - 43;
	RampSetter(Ramp_1, 13, false, false, 0, Ramp_1_YPos);

	Ramp_1[11].hasLadderBelow = true;

	Ramp_2_YPos = (SCREEN_HEIGHT - TrussHeight) - 62;
	RampSetter(Ramp_2, 13, false, true, 0, Ramp_2_YPos);

	Ramp_2[1].hasLadderBelow = true;
	Ramp_2[5].hasLadderBelow = true;

	Ramp_3_YPos = (SCREEN_HEIGHT - TrussHeight) - 110;
	RampSetter(Ramp_3, 13, false, false, 0, Ramp_3_YPos);

	Ramp_3[7].hasLadderBelow = true;
	Ramp_3[11].hasLadderBelow = true;

	Ramp_4_YPos = (SCREEN_HEIGHT - TrussHeight) - 130;
	RampSetter(Ramp_4, 13, false, true, 0, Ramp_4_YPos);

	Ramp_4[1].hasLadderBelow = true;
	Ramp_4[3].hasLadderBelow = true;

	Ramp_5_YPos = (SCREEN_HEIGHT - TrussHeight) - 169;
	RampSetter(Ramp_5, 13, false, false, 9, Ramp_5_YPos);

	Ramp_5[11].hasLadderBelow = true;

	Ramp_6_YPos = (SCREEN_HEIGHT - TrussHeight) - 200;
	for (int i = 0; i < 3; i++) {
		Ramp_6[i].setPos(SCREEN_WIDTH/3+(i+1)*16, Ramp_6_YPos);
	}
}


void RampDrawer(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		DrawTexture(Ramp[i].truss, Ramp[i].TrussPos.x, Ramp[i].TrussPos.y, WHITE);
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

					if (entity.tag == EntityTag::PLAYER) {
						Mario.marioVelocity.y = 0.0f;
						Mario.isJumping = false;
						Mario.setGrounded(true);
						Mario.isFalling = false;
					}
				}
			}
			
			if (!entity.OnLadder && ((entity.tag == EntityTag::PLAYER && !Mario.isJumping && !Mario.isFalling) || entity.tag != EntityTag::PLAYER)) {
				//Check if higher than the Truss and not Jumping
				if(entity.FloorCollider.y < (Ramp[i].TrussPos.y + 8)){
				entity.Position.y = Ramp[i].TrussPos.y +8 - entity.SpriteSize;
				}
			}
			return;
		}
	}
}

void MapCollision(Entity &entity) {
	
	//DrawColliders();

	if (CheckCollisionPointRec(entity.FloorCollider, Ramp_0_Zone)) {
		RampCollision(Ramp_0, 14, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_1_Zone)) {
		RampCollision(Ramp_1, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_2_Zone)) {
		RampCollision(Ramp_2, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_3_Zone)) {
		RampCollision(Ramp_3, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_4_Zone)) {
		RampCollision(Ramp_4, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_5_Zone)) {
		RampCollision(Ramp_5, 13, entity);
	}
	else {
		if(entity.tag == EntityTag::PLAYER){
			Mario.isFalling = true;
		}
	}
}

void BarrelGroundCollisions(Barrel& barrel) {
	Vector2& pos = barrel.Position;
	Vector2& col = barrel.FloorCollider;
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