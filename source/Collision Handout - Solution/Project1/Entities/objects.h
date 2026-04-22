#pragma once
#include "Entities/entity.h"
#include "Core/UI.h"

enum Items {
	Hammer = 0,
	Hat = 1,
	Purse = 2,
	Umbrella = 3
};

class Interactable {
private:
	Texture2D Sprite;
	Vector2 Position;
	Rectangle Hitbox = { 0,0, 16, 16 };
	Rectangle SpriteSelector = { 0,0,16,16 };
	Items tag;
	Sound PointSound = { 0 };
	bool Interacted = false;

	void UpdateHitbox() {
		Hitbox.x = Position.x;
		Hitbox.y = Position.y;
		Hitbox.width = SpriteSelector.width;
		Hitbox.height = SpriteSelector.height;
	}

	void UnloadObject() {
		UnloadTexture(Sprite);
		UnloadSound(PointSound);
	}
	void OnInteraction();

public:

	void SetObject(int x, int y, Items tag);

	void DrawObject() {
		if(!Interacted) DrawTextureRec(Sprite, SpriteSelector, Position, WHITE);
	}

	void CheckInteraction(Entity &entity) {
		if (Interacted) return;
		if (CheckCollisionRecs(Hitbox, entity.getHitbox())) {
			Interacted = true;
			OnInteraction();
			if (tag != Hammer){
				ShowScorePopup(entity.Position, 300);
			}
		}
	}
	
	void ResetObject() {
		Interacted = false;
	}

	void DrawCollider() {
		DrawRectangle(Hitbox.x, Hitbox.y, Hitbox.width, Hitbox.height, PINK);
	 }

};


