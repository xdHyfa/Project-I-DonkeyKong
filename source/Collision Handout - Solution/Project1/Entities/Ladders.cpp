#include "raylib.h"
#include "Entities/Ground.h"
#include "Entities/Ladders.h"
#include "Entities/entity.h"
#include "Core/constants.h"
#include <iostream>
using namespace std;

Texture Ladder::texture;

Ladder Level1Ladders[9];

void Level1LadderSetter() {
	Ladder::LoadSharedTexture();
	Level1Ladders[0].setPos(Ramp_1[11].TrussPos.x + Ramp_1[11].TrussBox.width/4, Ramp_1[11].TrussPos.y + Ramp_1[11].TrussBox.height*2);
	Level1Ladders->setSprite(1, true);
	
}

void DrawLadderCollider() {
	DrawRectangle(Level1Ladders[0].Hitbox.x, Level1Ladders[0].Hitbox.y, Level1Ladders[0].Hitbox.width, Level1Ladders[0].Hitbox.height, WHITE);
}
void Level1LadderDraw() {

	DrawTextureRec(Level1Ladders[0].texture, Level1Ladders[0].SpriteSelector, Level1Ladders[0].Position, WHITE);
}

void Ladder :: CheckEntityOnLadder(Entity& entity) {
	if (CheckCollisionPointRec(entity.FloorCollider, Hitbox)) {
		entity.OnLadder = true;
		cout << "On ladder" << endl;
	}
}