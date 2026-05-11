#pragma once
#include "Entities/entity.h"
#include "Core/UI.h"

enum Items {
	Hammer = 0,
	Hat = 1,
	Purse = 2,
	Umbrella = 3,
	StarItem = 4
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
	void OnInteraction(int playerNum);

public:

	void SetObject(int x, int y, Items tag);

	void DrawObject() {
		if (!Interacted) DrawTextureRec(Sprite, SpriteSelector, Position, WHITE);
	}

	void CheckInteraction(Entity& entity, int playerNum) {
		if (Interacted) return;
		if (CheckCollisionRecs(Hitbox, entity.getHitbox())) {
			Interacted = true;
			OnInteraction(playerNum);
			if (tag != Hammer) {
				ShowScorePopup(entity.Position, 300);
			}
		}
	}

	// Overload sin playerNum para items que no sean hammer (Purse, Hat, Umbrella)
	void CheckInteraction(Entity& entity) {
		CheckInteraction(entity, 1); // fallback, no importa para non-hammer
	}

	void ResetObject() {
		Interacted = false;
	}

	void DrawCollider() {
		DrawRectangle(Hitbox.x, Hitbox.y, Hitbox.width, Hitbox.height, PINK);
	}

};