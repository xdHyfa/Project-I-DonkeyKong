#include "raylib.h"
#include "Entities/Ground.h"
#include "Entities/Ladders.h"
#include "Entities/entity.h"
#include "Core/constants.h"
#include <iostream>
using namespace std;

Texture Ladder::texture;

Ladder Level1Ladders[9];
Ladder ExtraPieces[16];

void Level1LadderSetter() {
	Ladder::LoadSharedTexture();
	for (int i = 0; i < 9; i++){
		Level1Ladders[i].setSprite(1, true);
	}
	for (int i = 0; i < 10; i++) {
		ExtraPieces[i].setSprite(1, false);
	}
	
	Level1Ladders[0].setPos(Ramp_1[11].TrussPos.x + Ramp_1[11].TrussBox.width/4 +2, Ramp_1[11].TrussPos.y + Ramp_1[11].TrussBox.height*2);
	Level1Ladders[0].Hitbox.y += 1;
	ExtraPieces[0].setPos(Level1Ladders[0].Position.x, Level1Ladders[0].Position.y + 15);

	Level1Ladders[1].setPos(Ramp_2[1].TrussPos.x, Ramp_2[1].TrussPos.y + Ramp_2[1].TrussBox.height * 2);

	Level1Ladders[2].setPos(Ramp_2[5].TrussPos.x, Ramp_2[5].TrussPos.y + Ramp_2[5].TrussBox.height * 2);
	Level1Ladders[2].Hitbox.height += 8;
	ExtraPieces[1].setPos(Level1Ladders[2].Position.x, Level1Ladders[2].Position.y + 15);
	for (int i = 2; i < 4; i++) {
		ExtraPieces[i].setPos(ExtraPieces[i-1].Position.x, ExtraPieces[i - 1].Position.y + 3);
	}

	Level1Ladders[3].setPos(Ramp_3[7].TrussPos.x, Ramp_3[7].TrussPos.y + Ramp_3[7].TrussBox.height * 2);
	Level1Ladders[3].Hitbox.height += 8;
	Level1Ladders[3].Hitbox.y += 1;
	ExtraPieces[4].setPos(Level1Ladders[3].Position.x, Level1Ladders[3].Position.y + 15);
	for (int i = 5; i < 8; i++) {
		ExtraPieces[i].setPos(ExtraPieces[i - 1].Position.x, ExtraPieces[i - 1].Position.y + 3);
	}

	Level1Ladders[4].setPos(Ramp_3[11].TrussPos.x + Ramp_3[11].TrussBox.width/4 +2, Ramp_3[11].TrussPos.y + Ramp_3[11].TrussBox.height * 2);
	Level1Ladders[4].Hitbox.y += 1;
	ExtraPieces[8].setPos(Level1Ladders[4].Position.x, Level1Ladders[4].Position.y + 15);

	Level1Ladders[5].setPos(Ramp_4[1].TrussPos.x, Ramp_4[1].TrussPos.y + Ramp_4[1].TrussBox.height * 2);
	Level1Ladders[5].Hitbox.y += 1;
	ExtraPieces[9].setPos(Level1Ladders[5].Position.x, Level1Ladders[5].Position.y + 15);

	Level1Ladders[6].setPos(Ramp_4[3].TrussPos.x + Ramp_4[3].TrussBox.width / 4 +2, Ramp_4[3].TrussPos.y + Ramp_4[3].TrussBox.height * 2);
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
}

void DrawLadderCollider() {
	for (int i = 0; i < 9; i ++){
	DrawRectangle(Level1Ladders[i].Hitbox.x, Level1Ladders[i].Hitbox.y, Level1Ladders[i].Hitbox.width, Level1Ladders[i].Hitbox.height, SKYBLUE);
	}
}
void Level1LadderDraw() {
	for (int i = 0; i < 9; i++){
		DrawTextureRec(Level1Ladders[i].texture, Level1Ladders[i].SpriteSelector, Level1Ladders[i].Position, WHITE);
	}
	for (int i = 0; i < 16; i++) {
		DrawTextureRec(ExtraPieces[i].texture, ExtraPieces[i].SpriteSelector, ExtraPieces[i].Position, WHITE);
	}
}

bool Ladder :: CheckEntityOnLadder(Entity& entity) {
	if (CheckCollisionPointRec(entity.FloorCollider, Hitbox) && entity.getIsGrounded()) {
		return true;
	}
	else{
		return false;
	}
}

void Level1LadderCollision(Entity &entity) {
	for (int i = 0; i < 9; i++){
		if (Level1Ladders[i].CheckEntityOnLadder(entity)) {
			entity.CanClimb = true;
			cout << "Can Climb" << endl;
			return;
		}
	}
	entity.OnLadder = false;
	entity.CanClimb = false;
}