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
float Base_0_YPos;
float Base_1_YPos;
float Base_2_YPos;
float Base_3_YPos;
float Base_4_YPos;
float Base_5_YPos;
float Base_6_YPos;

//ACTIVE AREA HITBOX FOR EACH RAMP
//Els numeros sueltos els vaig fer manualment a ull comparant amb el joc original 
Rectangle Base_0_Zone = { 0, SCREEN_HEIGHT - 22 -1, SCREEN_WIDTH, 22 };
Rectangle Base_1_Zone = { 8, SCREEN_HEIGHT - 22 - 49 + 8, SCREEN_WIDTH - 16, 22 };
Rectangle Base_2_Zone = { 16, SCREEN_HEIGHT - 22 - 98 + 16, SCREEN_WIDTH - 32, 22 };
Rectangle Base_3_Zone = { 24, SCREEN_HEIGHT - 22 -147 + 24, SCREEN_WIDTH - 48, 22 };
Rectangle Base_4_Zone = { 32, SCREEN_HEIGHT - 22 - 196 + 32, SCREEN_WIDTH - 64, 22 };

Truss Base_0[14];
Truss Base_1[14];
Truss Base_2[12];
Truss Base_3[12];
Truss Base_4[10];
Truss Base_5[8];

Texture button;


//RAMP POSITIONS DEFINED HERE
void Level2RampSetter() {
	Truss::LoadSharedTexture(2);

	Base_0_YPos = (SCREEN_HEIGHT - TrussHeight) - 1;
	BaseSetter(Base_0,14 ,0, Base_0_YPos, false);

	Base_1_YPos = (SCREEN_HEIGHT - TrussHeight) - 49+8;
	BaseSetter(Base_1, 14, 8, Base_1_YPos, true);

	Base_2_YPos = (SCREEN_HEIGHT - TrussHeight) - 98+16;
	BaseSetter(Base_2, 12, 16, Base_2_YPos, false);

	Base_3_YPos = (SCREEN_HEIGHT - TrussHeight) - 147+24;
	BaseSetter(Base_3, 12, 24, Base_3_YPos, true);

	Base_4_YPos = (SCREEN_HEIGHT - TrussHeight) - 196+32;
	BaseSetter(Base_4, 10, 32, Base_4_YPos, false);

	Base_5_YPos = (SCREEN_HEIGHT - TrussHeight) - 245 + 40;
	BaseSetter(Base_5, 8, 56, Base_5_YPos, true);

	button = LoadTexture("sprites/Button stage 3.png");
}

void Level2RampDraw() {
	RampDrawer(Base_0, 14);
	RampDrawer(Base_1, 14);
	RampDrawer(Base_2, 12);
	RampDrawer(Base_3, 12);
	RampDrawer(Base_4, 10);
	RampDrawer(Base_5, 8);
	DrawTexture(button, 59, Base_1[0].TrussPos.y, WHITE);
	DrawTexture(button, 163, Base_1[0].TrussPos.y, WHITE);
	DrawTexture(button, 59, Base_2[0].TrussPos.y, WHITE);
	DrawTexture(button, 163, Base_2[0].TrussPos.y, WHITE);
	DrawTexture(button, 59, Base_3[0].TrussPos.y, WHITE);
	DrawTexture(button, 163, Base_3[0].TrussPos.y, WHITE);
	DrawTexture(button, 59, Base_4[0].TrussPos.y, WHITE);
	DrawTexture(button, 163, Base_4[0].TrussPos.y, WHITE);

	/*Rectangle test = { 0,0,16,16 };
	Vector2 test2 = { 0,0 };
	for (int i = 0; i < 14; i++){
		DrawTextureRec(Base_0[i].truss, test,test2 , WHITE);
		test2.x += 16;
	}*/
}
void Level2RampCollisions(Entity& entity) {

	//DrawColliders();

	if (CheckCollisionPointRec(entity.FloorCollider, Base_0_Zone)) {
		RampCollision(Base_0, 14, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_1_Zone)) {
		RampCollision(Base_1, 14, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_2_Zone)) {
		RampCollision(Base_2, 12, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_3_Zone)) {
		RampCollision(Base_3, 12, entity);
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_4_Zone)) {
		RampCollision(Base_4, 10, entity);
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

	/*Level2Ladders[0].setPos(Ramp2_1[11].TrussPos.x + Ramp2_1[11].TrussBox.width / 4 + 2, Ramp2_1[11].TrussPos.y + Ramp2_1[11].TrussBox.height * 2);
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
	ExtraPieces2[15].setPos(ExtraPieces2[14].Position.x, ExtraPieces2[14].Position.y + 3);*/
}

void Level2LadderDraw() {
	/*for (int i = 0; i < 9; i++) {
		DrawTextureRec(Level2Ladders[i].texture, Level2Ladders[i].SpriteSelector, Level2Ladders[i].Position, WHITE);
	}
	for (int i = 0; i < 16; i++) {
		DrawTextureRec(ExtraPieces2[i].texture, ExtraPieces2[i].SpriteSelector, ExtraPieces2[i].Position, WHITE);
	}*/
}

void Level2LadderCollision(Entity& entity) {
	LadderCollisions(entity, Level2Ladders, 9);
}



//---DEBUGGING TOOLS: DRAW COLLIDER AREAS---

void DrawLevel2Colliders() {
	DrawLadderCollider(Level2Ladders, 9);
	DrawRectangle(Base_0_Zone.x, Base_0_Zone.y, Base_0_Zone.width, Base_0_Zone.height, WHITE);
	DrawRectangle(Base_1_Zone.x, Base_1_Zone.y, Base_1_Zone.width, Base_1_Zone.height, WHITE);
	DrawRectangle(Base_2_Zone.x, Base_2_Zone.y, Base_2_Zone.width, Base_2_Zone.height, WHITE);
	DrawRectangle(Base_3_Zone.x, Base_3_Zone.y, Base_3_Zone.width, Base_3_Zone.height, WHITE);
	DrawRectangle(Base_4_Zone.x, Base_4_Zone.y, Base_4_Zone.width, Base_4_Zone.height, WHITE);
}