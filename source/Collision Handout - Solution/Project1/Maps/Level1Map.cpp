#include "raylib.h"
#include "Maps/Level1Map.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Core/constants.h"
#include "Entities/entity.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include <iostream>
using namespace std;

#define TrussHeight 16.0f


//---WIN CONDITION HITBOX:

Rectangle WinHitbox = { SCREEN_WIDTH / 2,40, SCREEN_WIDTH / 8 , 10 };

bool winTriggered = false;
float winDelay = 0.0f;

void Level1CheckWinCondition(Entity& entity) {
	if (entity.tag == EntityTag::PLAYER && CheckCollisionPointRec(entity.FloorCollider, WinHitbox)) {
		if (!winTriggered) {
			StopMusicStream(level1Music);
			
			winTriggered = true;
		}
		// Ya NO llamamos ChangeScene() aquí.
		// Level1 detecta winTriggered y lo gestiona en su propio loop.
	}
}



//---RAMPS---


//RAMP DATA FOR LEVEL 1 DEFINED HERE
float Ramp_0_YPos;
float Ramp_1_YPos;
float Ramp_2_YPos;
float Ramp_3_YPos;
float Ramp_4_YPos;
float Ramp_5_YPos;
float Ramp_6_YPos;

//ACTIVE AREA HITBOX FOR EACH RAMP
Rectangle Ramp_0_Zone = { 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20 };
Rectangle Ramp_1_Zone = { 0, SCREEN_HEIGHT - 20 - 36, SCREEN_WIDTH - 16, 22 };
Rectangle Ramp_2_Zone = { 16, SCREEN_HEIGHT - 20 - 36 - 34, SCREEN_WIDTH - 16, 23 };
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

//RAMP POSITIONS DEFINED HERE
void Level1RampSetter() {
	Truss::LoadSharedTexture(1);

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
		Ramp_6[i].setPos(SCREEN_WIDTH / 3 + (i + 1) * 16, Ramp_6_YPos);
	}
}

void Level1RampDraw() {
	RampDrawer(Ramp_0, 14);
	RampDrawer(Ramp_1, 13);
	RampDrawer(Ramp_2, 13);
	RampDrawer(Ramp_3, 13);
	RampDrawer(Ramp_4, 13);
	RampDrawer(Ramp_5, 13);
	RampDrawer(Ramp_6, 3);
}
void Level1RampCollisions(Entity& entity) {
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
		if (entity.tag == EntityTag::PLAYER) {
			Mario.isFalling = true;
		}
	}
}

void BarrelGroundCollisions(Barrel& barrel) {
	Vector2& pos = barrel.Position;
	Vector2& col = barrel.FloorCollider;
	bool onGround = false;
	bool onRamp0 = false;

	auto checkTrussArray = [&](Truss* Ramp, int size, bool isBottom) {
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
				if (isBottom) onRamp0 = true;
				barrel.justFlipped = false;
				return;
			}
		}
		};

	checkTrussArray(Ramp_0, 14, true);
	checkTrussArray(Ramp_1, 13, false);
	checkTrussArray(Ramp_2, 13, false);
	checkTrussArray(Ramp_3, 13, false);
	checkTrussArray(Ramp_4, 13, false);
	checkTrussArray(Ramp_5, 13, false);

	if (!onGround) return;

	if (onRamp0) {
		if (pos.x + BARRELSIZE >= SCREEN_WIDTH || pos.x <= 0) {
			barrel.has_Spawned = false;
		}
	}
	else {
		if (pos.x + BARRELSIZE >= SCREEN_WIDTH) {
			pos.x = SCREEN_WIDTH - BARRELSIZE;
			barrel.FlipDirection();
		}
		else if (pos.x <= 0) {
			pos.x = 0;
			barrel.FlipDirection();
		}
	}
}

//---LEVEL CHECK: 

int CheckEntityPlatform(Entity& entity) {
	if (CheckCollisionPointRec(entity.FloorCollider, Ramp_0_Zone)) return 0;
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_1_Zone)) return 1;
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_2_Zone)) return 2;
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_3_Zone)) return 3;
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_4_Zone)) return 4;
	else if (CheckCollisionPointRec(entity.FloorCollider, Ramp_5_Zone)) return 5;
	return -1;
}


//---LADDERS---

Ladder Level1Ladders[9];
Ladder ExtraPieces[16];

Ladder BrokenLadders[4];
Ladder ExtraBroken[12];

Rectangle Level1DownZone[9];
Rectangle BrokenDownZone[4];

void Level1LadderSetter() {
	Ladder::LoadSharedTexture();
	for (int i = 0; i < 9; i++) {
		Level1Ladders[i].setSprite(1, true);
	}
	for (int i = 0; i < 16; i++) {
		ExtraPieces[i].setSprite(1, false);
	}
	for (int i = 0; i < 4; i++) {
		BrokenLadders[i].setSprite(1, false);
	}
	for (int i = 0; i < 12; i++) {
		ExtraBroken[i].setSprite(1, false);
	}

	Level1Ladders[0].setPos(Ramp_1[11].TrussPos.x + Ramp_1[11].TrussBox.width / 4 + 2, Ramp_1[11].TrussPos.y + Ramp_1[11].TrussBox.height * 2);
	Level1Ladders[0].Hitbox.y += 1;
	ExtraPieces[0].setPos(Level1Ladders[0].Position.x, Level1Ladders[0].Position.y + 15);

	Level1Ladders[1].setPos(Ramp_2[1].TrussPos.x, Ramp_2[1].TrussPos.y + Ramp_2[1].TrussBox.height * 2);

	Level1Ladders[2].setPos(Ramp_2[5].TrussPos.x, Ramp_2[5].TrussPos.y + Ramp_2[5].TrussBox.height * 2);
	Level1Ladders[2].Hitbox.height += 7;
	Level1Ladders[2].Hitbox.y += 1;
	ExtraPieces[1].setPos(Level1Ladders[2].Position.x, Level1Ladders[2].Position.y + 15);
	for (int i = 2; i < 4; i++) {
		ExtraPieces[i].setPos(ExtraPieces[i - 1].Position.x, ExtraPieces[i - 1].Position.y + 3);
	}

	Level1Ladders[3].setPos(Ramp_3[7].TrussPos.x, Ramp_3[7].TrussPos.y + Ramp_3[7].TrussBox.height * 2);
	Level1Ladders[3].Hitbox.height += 8;
	Level1Ladders[3].Hitbox.y += 1;
	ExtraPieces[4].setPos(Level1Ladders[3].Position.x, Level1Ladders[3].Position.y + 15);
	for (int i = 5; i < 8; i++) {
		ExtraPieces[i].setPos(ExtraPieces[i - 1].Position.x, ExtraPieces[i - 1].Position.y + 3);
	}

	Level1Ladders[4].setPos(Ramp_3[11].TrussPos.x + Ramp_3[11].TrussBox.width / 4 + 2, Ramp_3[11].TrussPos.y + Ramp_3[11].TrussBox.height * 2);
	Level1Ladders[4].Hitbox.y += 1;
	ExtraPieces[8].setPos(Level1Ladders[4].Position.x, Level1Ladders[4].Position.y + 15);

	Level1Ladders[5].setPos(Ramp_4[1].TrussPos.x, Ramp_4[1].TrussPos.y + Ramp_4[1].TrussBox.height * 2);
	Level1Ladders[5].Hitbox.y += 1;
	ExtraPieces[9].setPos(Level1Ladders[5].Position.x, Level1Ladders[5].Position.y + 15);

	Level1Ladders[6].setPos(Ramp_4[3].TrussPos.x + Ramp_4[3].TrussBox.width / 4 + 2, Ramp_4[3].TrussPos.y + Ramp_4[3].TrussBox.height * 2);
	Level1Ladders[6].Hitbox.y += 1;
	Level1Ladders[6].Hitbox.height += 4;
	ExtraPieces[10].setPos(Level1Ladders[6].Position.x, Level1Ladders[6].Position.y + 15);
	ExtraPieces[11].setPos(ExtraPieces[10].Position.x, ExtraPieces[10].Position.y + 3);

	Level1Ladders[7].setPos(Ramp_5[11].TrussPos.x + Ramp_5[11].TrussBox.width / 4 + 2, Ramp_5[11].TrussPos.y + Ramp_5[11].TrussBox.height * 2);
	Level1Ladders[7].Hitbox.y += 1;
	ExtraPieces[12].setPos(Level1Ladders[7].Position.x, Level1Ladders[7].Position.y + 15);

	Level1Ladders[8].setPos(Ramp_6[2].TrussPos.x + Ramp_6[2].TrussBox.width / 4 + 2, Ramp_6[2].TrussPos.y + Ramp_6[2].TrussBox.height * 2);
	Level1Ladders[8].Hitbox.y += 1;
	Level1Ladders[8].Hitbox.height += 6;
	ExtraPieces[13].setPos(Level1Ladders[8].Position.x, Level1Ladders[8].Position.y + 15);
	ExtraPieces[14].setPos(ExtraPieces[13].Position.x, ExtraPieces[13].Position.y + 3);
	ExtraPieces[15].setPos(ExtraPieces[14].Position.x, ExtraPieces[14].Position.y + 3);

	BrokenLadders[0].setPos(Ramp_1[5].TrussPos.x, Ramp_1[5].TrussPos.y + Ramp_1[5].TrussBox.height * 2);
	ExtraBroken[0].setPos(BrokenLadders[0].Position.x, BrokenLadders[0].Position.y + 19);
	ExtraBroken[1].setPos(BrokenLadders[0].Position.x, BrokenLadders[0].Position.y + 22);
	ExtraBroken[2].setPos(BrokenLadders[0].Position.x, BrokenLadders[0].Position.y + 25);

	BrokenLadders[1].setPos(Ramp_3[4].TrussPos.x, Ramp_3[4].TrussPos.y + Ramp_3[4].TrussBox.height * 2);
	ExtraBroken[3].setPos(BrokenLadders[1].Position.x, BrokenLadders[1].Position.y + 3);
	ExtraBroken[4].setPos(BrokenLadders[1].Position.x, BrokenLadders[1].Position.y + 25);
	ExtraBroken[5].setPos(BrokenLadders[1].Position.x, BrokenLadders[1].Position.y + 28);

	BrokenLadders[2].setPos(Ramp_4[9].TrussPos.x + 6, Ramp_4[9].TrussPos.y + Ramp_4[9].TrussBox.height * 2);
	ExtraBroken[6].setPos(BrokenLadders[2].Position.x, BrokenLadders[2].Position.y + 3);
	ExtraBroken[7].setPos(BrokenLadders[2].Position.x, BrokenLadders[2].Position.y + 27);
	ExtraBroken[8].setPos(BrokenLadders[2].Position.x, BrokenLadders[2].Position.y + 30);

	BrokenLadders[3].setPos(Ramp_5[5].TrussPos.x + 6, Ramp_5[5].TrussPos.y + Ramp_5[4].TrussBox.height * 2);
	ExtraBroken[9].setPos(BrokenLadders[3].Position.x, BrokenLadders[3].Position.y + 3);
	ExtraBroken[10].setPos(BrokenLadders[3].Position.x, BrokenLadders[3].Position.y + 6);
	ExtraBroken[11].setPos(BrokenLadders[3].Position.x, BrokenLadders[3].Position.y + 23);

	for (int i = 0; i < 4; i++) {
		BrokenLadders[i].Hitbox.height += 26;
	}
	BrokenLadders[1].Hitbox.height += 2;
	BrokenLadders[2].Hitbox.height += 4;

	for (int i = 0; i < 9; i++) {
		Level1DownZone[i] = { Level1Ladders[i].Hitbox.x, Level1Ladders[i].Hitbox.y - 2 , 10, 4 };
	}

	for (int i = 0; i < 4; i++) {
		BrokenDownZone[i] = { BrokenLadders[i].Hitbox.x, BrokenLadders[i].Hitbox.y - 2, 10, 4 };
	}
}

void Level1LadderDraw() {
	for (int i = 0; i < 9; i++) {
		DrawTextureRec(Level1Ladders[i].texture, Level1Ladders[i].SpriteSelector, Level1Ladders[i].Position, WHITE);
	}
	for (int i = 0; i < 16; i++) {
		DrawTextureRec(ExtraPieces[i].texture, ExtraPieces[i].SpriteSelector, ExtraPieces[i].Position, WHITE);
	}
	for (int i = 0; i < 4; i++) {
		DrawTextureRec(BrokenLadders[i].texture, BrokenLadders[i].SpriteSelector, BrokenLadders[i].Position, WHITE);
	}
	for (int i = 0; i < 12; i++) {
		DrawTextureRec(ExtraBroken[i].texture, ExtraBroken[i].SpriteSelector, ExtraBroken[i].Position, WHITE);
	}
}

void BrokenLadderCollisions(Entity& entity) {
	LadderCollisions(entity, BrokenLadders, 4);
}

void Level1LadderCollisions(Entity& entity) {
	if (entity.tag == EntityTag::PLAYER) {
		LadderCollisions(entity, Level1Ladders, 9);
	}
	else {
		LadderCollisions(entity, Level1Ladders, BrokenLadders, 8, 4);
	}
}

void DrawDownZone(Rectangle* DownZone, int size) {
	for (int i = 0; i < size; i++) {
		DrawRectangle(DownZone[i].x, DownZone[i].y, DownZone[i].width, DownZone[i].height, DARKPURPLE);
	}
}

bool CheckDownZone(Entity& entity) {
	for (int i = 0; i < 9; i++) {
		if (CheckCollisionPointRec(entity.FloorCollider, Level1DownZone[i])) {
			return true;
		}
	}
	return false;
}

void DrawLevel1Colliders() {
	DrawRectangle(Ramp_0_Zone.x, Ramp_0_Zone.y, Ramp_0_Zone.width, Ramp_0_Zone.height, WHITE);
	DrawRectangle(Ramp_1_Zone.x, Ramp_1_Zone.y, Ramp_1_Zone.width, Ramp_1_Zone.height, WHITE);
	DrawRectangle(Ramp_2_Zone.x, Ramp_2_Zone.y, Ramp_2_Zone.width, Ramp_2_Zone.height, WHITE);
	DrawRectangle(Ramp_3_Zone.x, Ramp_3_Zone.y, Ramp_3_Zone.width, Ramp_3_Zone.height, WHITE);
	DrawRectangle(Ramp_4_Zone.x, Ramp_4_Zone.y, Ramp_4_Zone.width, Ramp_4_Zone.height, WHITE);
	DrawRectangle(Ramp_5_Zone.x, Ramp_5_Zone.y, Ramp_5_Zone.width, Ramp_5_Zone.height, WHITE);

	DrawRectangle(WinHitbox.x, WinHitbox.y, WinHitbox.width, WinHitbox.height, YELLOW);
	DrawLadderCollider(Level1Ladders, 9);
	DrawLadderCollider(BrokenLadders, 4);
	DrawDownZone(Level1DownZone, 9);
	DrawDownZone(BrokenDownZone, 4);
}