#pragma once
#include "raylib.h"
#include "Entities/entity.h"
// Level 1 Ladder Sprite Starts at X = 3   
// Level 2 Ladder Sprite Starts at X = 19 (Offset 16.) 
// Level 3 Ladder Sprite Starts at X = 34 and Has X = 12 Width.
// Level 4 Ladder Sprite Starts at X = 
// Broken Pieces Start at Y = 0, Full Pieces start at Y = 16.
// Broken Pieces have x = 10 width and y = 3 height.
// Full Pieces have x = 10  width and y = 16 height.

class Ladder {
public:
	static Texture texture;
	Vector2 Position = { 0 ,0 };
	Rectangle Hitbox = { Position.x, Position.y , 8, 16 };
	Rectangle SpriteSelector = {3,0,10,3};

	void UpdateHitbox() {
		Hitbox.x = Position.x; Hitbox.y = Position.y -9;
		Hitbox.width = SpriteSelector.width; Hitbox.height = SpriteSelector.height + 10;
	}

	void setPos(float x, float y) {
		Position.x = x; Position.y = y;
		UpdateHitbox();
	}

	void setSprite(int levelNum, bool isFull){
		if (levelNum == 1) {
			SpriteSelector.width = 10;
			SpriteSelector.x = 3;
			UpdateHitbox();
		}
		else if (levelNum == 2) {
			SpriteSelector.width = 10;
			SpriteSelector.x = 19;
			UpdateHitbox();
		}
		else if (levelNum == 3) {
			SpriteSelector.width = 12;
			SpriteSelector.x = 34;
			UpdateHitbox();
		}
		else {
			SpriteSelector.width = 10;
			SpriteSelector.x = 51;
			UpdateHitbox();
		}
		if (isFull){
			SpriteSelector.y = 16;
			SpriteSelector.height = 16;
			UpdateHitbox();
		}
		else {
			SpriteSelector.y = 0;
			SpriteSelector.height = 3;
			UpdateHitbox();
		}
	}
	static void LoadSharedTexture() {
		texture = LoadTexture("sprites/Stairs.png");
	}
	static void UnloadSharedTexture() {
		UnloadTexture(texture);
	}
	bool CheckEntityOnLadder(Entity& entity);

};

void DrawLadderCollider(Ladder* Map_Ladders, int size);

void LadderCollisions(Entity& entity, Ladder* Map_Ladders, int size);


