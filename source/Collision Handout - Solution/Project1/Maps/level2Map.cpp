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
int Base_0_YPos;
int Base_1_YPos;
int Base_2_YPos;
int Base_3_YPos;
int Base_4_YPos;
int Base_5_YPos;
int Base_6_YPos;

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
	Base_1[0].hasLadderBelow = true;
	Base_1[6].hasLadderBelow = true;
	Base_1[13].hasLadderBelow = true;

	Base_2_YPos = (SCREEN_HEIGHT - TrussHeight) - 98+16;
	BaseSetter(Base_2, 12, 16, Base_2_YPos, false);
	Base_2[0].hasLadderBelow = true;
	Base_2[3].hasLadderBelow = true;
	Base_2[8].hasLadderBelow = true;
	Base_2[11].hasLadderBelow = true;

	Base_3_YPos = (SCREEN_HEIGHT - TrussHeight) - 147+24;
	BaseSetter(Base_3, 12, 24, Base_3_YPos, true);
	Base_3[0].hasLadderBelow = true;
	Base_3[5].hasLadderBelow = true;
	Base_3[11].hasLadderBelow = true;

	Base_4_YPos = (SCREEN_HEIGHT - TrussHeight) - 196+32;
	BaseSetter(Base_4, 10, 32, Base_4_YPos, false);
	Base_4[0].hasLadderBelow = true;
	Base_4[2].hasLadderBelow = true;
	Base_4[7].hasLadderBelow = true;
	Base_4[9].hasLadderBelow = true;
	
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
Ladder Level2Ladders[14];
Ladder ExtraLadder0[6];
Ladder ExtraLadder1[6];
Ladder ExtraLadder2[6];
Ladder ExtraLadder3[6];
Ladder ExtraLadder4[6];
Ladder ExtraLadder5[6];
Ladder ExtraLadder6[6];
Ladder ExtraLadder7[6];
Ladder ExtraLadder8[6];
Ladder ExtraLadder9[6];
Ladder ExtraLadder10[6];
Ladder ExtraLadder11[6];
Ladder ExtraLadder12[6];
Ladder ExtraLadder13[6];

Rectangle Level2DownZone[14];


void SetFullLadder(Ladder &ladder, Ladder* Extras, float X, float Y) {
	ladder.setPos(X,Y);
	Extras[0].setPos(ladder.Position.x, ladder.Position.y + 15);
	for (int i = 1; i < 6; i++) {
		Extras[i].setPos(ladder.Position.x, Extras[i - 1].Position.y + 3);
	}

}

//LADDER POSITION/SPRITES FOR LEVEL 1 DEFINED HERE
void Level2LadderSetter() {
	Ladder::LoadSharedTexture();
	for (int i = 0; i < 14; i++) {
		Level2Ladders[i].setSprite(4, true);
	}
	for (int i = 0; i < 6; i++) {
		ExtraLadder0[i].setSprite(4, false);
		ExtraLadder1[i].setSprite(4, false);
		ExtraLadder2[i].setSprite(4, false);
		ExtraLadder3[i].setSprite(4, false);
		ExtraLadder4[i].setSprite(4, false);
		ExtraLadder5[i].setSprite(4, false);
		ExtraLadder6[i].setSprite(4, false);
		ExtraLadder7[i].setSprite(4, false);
		ExtraLadder8[i].setSprite(4, false);
		ExtraLadder9[i].setSprite(4, false);
		ExtraLadder10[i].setSprite(4, false);
		ExtraLadder11[i].setSprite(4, false);
		ExtraLadder12[i].setSprite(4, false);
		ExtraLadder13[i].setSprite(4, false);

	}

	SetFullLadder(Level2Ladders[0], ExtraLadder0, Base_1[0].TrussPos.x, Base_1[0].TrussPos.y + Base_1[0].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[1], ExtraLadder1, Base_1[6].TrussPos.x + 8, Base_1[6].TrussPos.y + Base_1[6].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[2], ExtraLadder2, Base_1[13].TrussPos.x - 2, Base_1[13].TrussPos.y + Base_1[13].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[3], ExtraLadder3, Base_2[0].TrussPos.x, Base_2[0].TrussPos.y + Base_2[0].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[4], ExtraLadder4, Base_2[3].TrussPos.x + 8, Base_2[3].TrussPos.y + Base_2[3].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[5], ExtraLadder5, Base_2[8].TrussPos.x, Base_2[8].TrussPos.y + Base_2[8].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[6], ExtraLadder6, Base_2[11].TrussPos.x + 6, Base_2[11].TrussPos.y + Base_2[11].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[7], ExtraLadder7, Base_3[0].TrussPos.x, Base_3[0].TrussPos.y + Base_3[0].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[8], ExtraLadder8, Base_3[5].TrussPos.x + 8, Base_3[5].TrussPos.y + Base_3[5].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[9], ExtraLadder9, Base_3[11].TrussPos.x - 2, Base_3[11].TrussPos.y + Base_3[11].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[10], ExtraLadder10, Base_4[0].TrussPos.x, Base_4[0].TrussPos.y + Base_4[0].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[11], ExtraLadder11, Base_4[2].TrussPos.x, Base_4[2].TrussPos.y + Base_4[2].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[12], ExtraLadder12, Base_4[7].TrussPos.x+8, Base_4[7].TrussPos.y + Base_4[7].TrussBox.height * 2);

	SetFullLadder(Level2Ladders[13], ExtraLadder13, Base_4[9].TrussPos.x + 6, Base_4[9].TrussPos.y + Base_4[9].TrussBox.height * 2);

	for (int i = 0; i < 14; i++) {
		Level2Ladders[i].Hitbox.y += 1;
		Level2Ladders[i].Hitbox.height += 16;
	}

	for (int i = 0; i < 14; i++) {
		Level2DownZone[i] = { Level2Ladders[i].Hitbox.x, Level2Ladders[i].Hitbox.y - 2 , 10, 4 };
	}
}

void Level2LadderDraw() {
	for (int i = 0; i < 14; i++) {
		DrawTextureRec(Level2Ladders[i].texture, Level2Ladders[i].SpriteSelector, Level2Ladders[i].Position, WHITE);
	}
	for (int i = 0; i < 6; i++) {
		DrawTextureRec(ExtraLadder0[i].texture, ExtraLadder0[i].SpriteSelector, ExtraLadder0[i].Position, WHITE);
		DrawTextureRec(ExtraLadder1[i].texture, ExtraLadder1[i].SpriteSelector, ExtraLadder1[i].Position, WHITE);
		DrawTextureRec(ExtraLadder2[i].texture, ExtraLadder2[i].SpriteSelector, ExtraLadder2[i].Position, WHITE);
		DrawTextureRec(ExtraLadder3[i].texture, ExtraLadder3[i].SpriteSelector, ExtraLadder3[i].Position, WHITE);
		DrawTextureRec(ExtraLadder4[i].texture, ExtraLadder4[i].SpriteSelector, ExtraLadder4[i].Position, WHITE);
		DrawTextureRec(ExtraLadder5[i].texture, ExtraLadder5[i].SpriteSelector, ExtraLadder5[i].Position, WHITE);
		DrawTextureRec(ExtraLadder6[i].texture, ExtraLadder6[i].SpriteSelector, ExtraLadder6[i].Position, WHITE);
		DrawTextureRec(ExtraLadder7[i].texture, ExtraLadder7[i].SpriteSelector, ExtraLadder7[i].Position, WHITE);
		DrawTextureRec(ExtraLadder8[i].texture, ExtraLadder8[i].SpriteSelector, ExtraLadder8[i].Position, WHITE);
		DrawTextureRec(ExtraLadder9[i].texture, ExtraLadder9[i].SpriteSelector, ExtraLadder9[i].Position, WHITE);
		DrawTextureRec(ExtraLadder10[i].texture, ExtraLadder10[i].SpriteSelector, ExtraLadder10[i].Position, WHITE);
		DrawTextureRec(ExtraLadder11[i].texture, ExtraLadder11[i].SpriteSelector, ExtraLadder11[i].Position, WHITE);
		DrawTextureRec(ExtraLadder12[i].texture, ExtraLadder12[i].SpriteSelector, ExtraLadder12[i].Position, WHITE);
		DrawTextureRec(ExtraLadder13[i].texture, ExtraLadder13[i].SpriteSelector, ExtraLadder13[i].Position, WHITE);
		
	}
}

void Level2LadderCollisions(Entity& entity) {
	LadderCollisions(entity, Level2Ladders, 14);
}

void DrawDownZone2(Rectangle* DownZone, int size) {
	for (int i = 0; i < size; i++) {
		DrawRectangle(DownZone[i].x, DownZone[i].y, DownZone[i].width, DownZone[i].height, DARKPURPLE);
	}
}

bool Level2CheckDownZone(Entity& entity) {
	for (int i = 0; i < 14; i++) {
		if (CheckCollisionPointRec(entity.FloorCollider, Level2DownZone[i])) {
			return true;
		}
	}
	return false;
}

//---LEVEL CHECK: 

int Level2CheckEntityPlatform(Entity& entity) {
	if (CheckCollisionPointRec(entity.FloorCollider, Base_0_Zone)) {
		return 0;
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_1_Zone)) {
		return 1;
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_2_Zone)) {
		return 2;
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_3_Zone)) {
		return 3;
	}
	else if (CheckCollisionPointRec(entity.FloorCollider, Base_4_Zone)) {
		return 4;
	}
}

//---DEBUGGING TOOLS: DRAW COLLIDER AREAS---

void DrawLevel2Colliders() {
	/*DrawRectangle(Base_0_Zone.x, Base_0_Zone.y, Base_0_Zone.width, Base_0_Zone.height, WHITE);
	DrawRectangle(Base_1_Zone.x, Base_1_Zone.y, Base_1_Zone.width, Base_1_Zone.height, WHITE);
	DrawRectangle(Base_2_Zone.x, Base_2_Zone.y, Base_2_Zone.width, Base_2_Zone.height, WHITE);
	DrawRectangle(Base_3_Zone.x, Base_3_Zone.y, Base_3_Zone.width, Base_3_Zone.height, WHITE);
	DrawRectangle(Base_4_Zone.x, Base_4_Zone.y, Base_4_Zone.width, Base_4_Zone.height, WHITE);*/
	
	DrawLadderCollider(Level2Ladders, 14);
	//DrawDownZone2(Level2DownZone, 14);
} 