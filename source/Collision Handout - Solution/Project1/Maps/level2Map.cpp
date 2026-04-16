#include "raylib.h"
#include "Maps/Level1Map.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Core/constants.h"
#include "Entities/entity.h"
#include "Entities/Player.h"
#include <iostream>
using namespace std;

#define TrussHeight 16.0f

//---RAMPS---


//RAMP DATA FOR LEVEL 1 DEFINED HERE
float Ramp_0_YPos2;
float Ramp_1_YPos2;
float Ramp_2_YPos2;
float Ramp_3_YPos2;
float Ramp_4_YPos2;
float Ramp_5_YPos2;
float Ramp_6_YPos2;

//ACTIVE AREA HITBOX FOR EACH RAMP
//Els numeros sueltos els vaig fer manualment a ull comparant amb el joc original 
Rectangle Ramp_0_Zone2 = { 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20 };
Rectangle Ramp_1_Zone2 = { 0, SCREEN_HEIGHT - 20 - 36, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_2_Zone2 = { 16, SCREEN_HEIGHT - 20 - 36 - 34, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_3_Zone2 = { 0, SCREEN_HEIGHT - 20 - 36 - 34 - 33, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_4_Zone2 = { 16, SCREEN_HEIGHT - 20 - 36 - 34 - 32 - 35, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_5_Zone2 = { 0, SCREEN_HEIGHT - 20 - 36 - 34 - 32 - 34 - 35, SCREEN_WIDTH - 16, 22 };

Truss Ramp2_0[14];
Truss Ramp2_1[12];
Truss Ramp2_2[13];
Truss Ramp2_3[13];
Truss Ramp2_4[13];
Truss Ramp2_5[13];
Truss Ramp2_6[3];

//RAMP POSITIONS DEFINED HERE
void Level2RampSetter() {
	Truss::LoadSharedTexture(2);

	Ramp_0_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 1;
	BaseSetter(Ramp2_0,14 ,0, Ramp_0_YPos2);

	Ramp_1_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 43;
	RampSetter(Ramp2_1, 13, false, false, 12, Ramp_1_YPos2);

	Ramp2_1[11].hasLadderBelow = true;

	Ramp_2_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 62;
	RampSetter(Ramp2_2, 13, false, true, 0, Ramp_2_YPos2);

	Ramp2_2[1].hasLadderBelow = true;
	Ramp2_2[5].hasLadderBelow = true;

	Ramp_3_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 110;
	RampSetter(Ramp2_3, 13, false, false, 0, Ramp_3_YPos2);

	Ramp2_3[7].hasLadderBelow = true;
	Ramp2_3[11].hasLadderBelow = true;

	Ramp_4_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 130;
	RampSetter(Ramp2_4, 13, false, true, 0, Ramp_4_YPos2);

	Ramp2_4[1].hasLadderBelow = true;
	Ramp2_4[3].hasLadderBelow = true;

	Ramp_5_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 169;
	RampSetter(Ramp2_5, 13, false, false, 9, Ramp_5_YPos2);

	Ramp2_5[11].hasLadderBelow = true;

	Ramp_6_YPos2 = (SCREEN_HEIGHT - TrussHeight) - 200;
	for (int i = 0; i < 3; i++) {
		Ramp2_6[i].setPos(SCREEN_WIDTH / 3 + (i + 1) * 16, Ramp_6_YPos2);
	}
}

void Level2RampDraw() {
	RampDrawer(Ramp2_0, 14);
	RampDrawer(Ramp2_1, 12);
	RampDrawer(Ramp2_2, 13);
	RampDrawer(Ramp2_3, 13);
	RampDrawer(Ramp2_4, 13);
	RampDrawer(Ramp2_5, 13);
	RampDrawer(Ramp2_6, 3);
}
void Level2RampCollisions(Entity& entity) {

	//DrawColliders();

	if (CheckCollisionPointRec(entity.FloorCollider, Ramp_0_Zone2)) {
		RampCollision(Ramp2_0, 14, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_1_Zone2)) {
		RampCollision(Ramp2_1, 12, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_2_Zone2)) {
		RampCollision(Ramp2_2, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_3_Zone2)) {
		RampCollision(Ramp2_3, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_4_Zone2)) {
		RampCollision(Ramp2_4, 13, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_5_Zone2)) {
		RampCollision(Ramp2_5, 13, entity);
	}
	else {
		if (entity.tag == EntityTag::PLAYER) {
			Mario.isFalling = true;
		}
	}
}

//---LADDERS---

//LADDER DATA FOR LEVEL 1 DEFINED HERE
Ladder Level2Ladders[9];
Ladder ExtraPieces2[16];

//LADDER POSITION/SPRITES FOR LEVEL 1 DEFINED HERE
void Level2LadderSetter() {
	Ladder::LoadSharedTexture();
	for (int i = 0; i < 9; i++) {
		Level2Ladders[i].setSprite(1, true);
	}
	for (int i = 0; i < 10; i++) {
		ExtraPieces2[i].setSprite(1, false);
	}

	Level2Ladders[0].setPos(Ramp2_1[11].TrussPos.x + Ramp2_1[11].TrussBox.width / 4 + 2, Ramp2_1[11].TrussPos.y + Ramp2_1[11].TrussBox.height * 2);
	Level2Ladders[0].Hitbox.y += 1;
	ExtraPieces2[0].setPos(Level2Ladders[0].Position.x, Level2Ladders[0].Position.y + 15);

	Level2Ladders[1].setPos(Ramp2_2[1].TrussPos.x, Ramp2_2[1].TrussPos.y + Ramp2_2[1].TrussBox.height * 2);

	Level2Ladders[2].setPos(Ramp2_2[5].TrussPos.x, Ramp2_2[5].TrussPos.y + Ramp2_2[5].TrussBox.height * 2);
	Level2Ladders[2].Hitbox.height += 7;
	Level2Ladders[2].Hitbox.y += 1;
	ExtraPieces2[1].setPos(Level2Ladders[2].Position.x, Level2Ladders[2].Position.y + 15);
	for (int i = 2; i < 4; i++) {
		ExtraPieces2[i].setPos(ExtraPieces2[i - 1].Position.x, ExtraPieces2[i - 1].Position.y + 3);
	}

	Level2Ladders[3].setPos(Ramp2_3[7].TrussPos.x, Ramp2_3[7].TrussPos.y + Ramp2_3[7].TrussBox.height * 2);
	Level2Ladders[3].Hitbox.height += 8;
	Level2Ladders[3].Hitbox.y += 1;
	ExtraPieces2[4].setPos(Level2Ladders[3].Position.x, Level2Ladders[3].Position.y + 15);
	for (int i = 5; i < 8; i++) {
		ExtraPieces2[i].setPos(ExtraPieces2[i - 1].Position.x, ExtraPieces2[i - 1].Position.y + 3);
	}

	Level2Ladders[4].setPos(Ramp2_3[11].TrussPos.x + Ramp2_3[11].TrussBox.width / 4 + 2, Ramp2_3[11].TrussPos.y + Ramp2_3[11].TrussBox.height * 2);
	Level2Ladders[4].Hitbox.y += 1;
	ExtraPieces2[8].setPos(Level2Ladders[4].Position.x, Level2Ladders[4].Position.y + 15);

	Level2Ladders[5].setPos(Ramp2_4[1].TrussPos.x, Ramp2_4[1].TrussPos.y + Ramp2_4[1].TrussBox.height * 2);
	Level2Ladders[5].Hitbox.y += 1;
	ExtraPieces2[9].setPos(Level2Ladders[5].Position.x, Level2Ladders[5].Position.y + 15);

	Level2Ladders[6].setPos(Ramp2_4[3].TrussPos.x + Ramp2_4[3].TrussBox.width / 4 + 2, Ramp2_4[3].TrussPos.y + Ramp2_4[3].TrussBox.height * 2);
	Level2Ladders[6].Hitbox.y += 1;
	Level2Ladders[6].Hitbox.height += 4;
	ExtraPieces2[10].setPos(Level2Ladders[6].Position.x, Level2Ladders[6].Position.y + 15);
	ExtraPieces2[11].setPos(ExtraPieces2[10].Position.x, ExtraPieces2[10].Position.y + 3);

	Level2Ladders[7].setPos(Ramp2_5[11].TrussPos.x + Ramp2_5[11].TrussBox.width / 4 + 2, Ramp2_5[11].TrussPos.y + Ramp2_5[11].TrussBox.height * 2);
	Level2Ladders[7].Hitbox.y += 1;
	ExtraPieces2[12].setPos(Level2Ladders[7].Position.x, Level2Ladders[7].Position.y + 15);

	Level2Ladders[8].setPos(Ramp2_6[2].TrussPos.x + Ramp2_6[2].TrussBox.width / 4 + 2, Ramp2_6[2].TrussPos.y + Ramp2_6[2].TrussBox.height * 2);
	Level2Ladders[8].Hitbox.y += 1;
	Level2Ladders[8].Hitbox.height += 6;
	ExtraPieces2[13].setPos(Level2Ladders[8].Position.x, Level2Ladders[8].Position.y + 15);
	ExtraPieces2[14].setPos(ExtraPieces2[13].Position.x, ExtraPieces2[13].Position.y + 3);
	ExtraPieces2[15].setPos(ExtraPieces2[14].Position.x, ExtraPieces2[14].Position.y + 3);
}

void Level2LadderDraw() {
	for (int i = 0; i < 9; i++) {
		DrawTextureRec(Level2Ladders[i].texture, Level2Ladders[i].SpriteSelector, Level2Ladders[i].Position, WHITE);
	}
	for (int i = 0; i < 16; i++) {
		DrawTextureRec(ExtraPieces2[i].texture, ExtraPieces2[i].SpriteSelector, ExtraPieces2[i].Position, WHITE);
	}
}

void Level2LadderCollision(Entity& entity) {
	LadderCollisions(entity, Level2Ladders, 9);
}



//---DEBUGGING TOOLS: DRAW COLLIDER AREAS---

void DrawLevel2Colliders() {
	DrawLadderCollider(Level2Ladders, 9);
	DrawRectangle(Ramp_0_Zone2.x, Ramp_0_Zone2.y, Ramp_0_Zone2.width, Ramp_0_Zone2.height, WHITE);
	DrawRectangle(Ramp_1_Zone2.x, Ramp_1_Zone2.y, Ramp_1_Zone2.width, Ramp_1_Zone2.height, WHITE);
	DrawRectangle(Ramp_2_Zone2.x, Ramp_2_Zone2.y, Ramp_2_Zone2.width, Ramp_2_Zone2.height, WHITE);
	DrawRectangle(Ramp_3_Zone2.x, Ramp_3_Zone2.y, Ramp_3_Zone2.width, Ramp_3_Zone2.height, WHITE);
	DrawRectangle(Ramp_4_Zone2.x, Ramp_4_Zone2.y, Ramp_4_Zone2.width, Ramp_4_Zone2.height, WHITE);
	DrawRectangle(Ramp_5_Zone2.x, Ramp_5_Zone2.y, Ramp_5_Zone2.width, Ramp_5_Zone2.height, WHITE);
}