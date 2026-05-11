#include "raylib.h"
#include "Entities/objects.h"
#include "Entities/Star.h"
#include "Core/UI.h"
#include "Scenes/scenes.h"
#include <iostream>
using namespace std;

void Interactable::SetObject(int x, int y, Items Tag) {
	PointSound = LoadSound("Audio/Bonus.wav");
	Interacted = false;
	Position.x = x;
	Position.y = y;
	UpdateHitbox();
	if (Tag == Hammer) {
		tag = Hammer;
		SpriteSelector.width = -12;
		SpriteSelector.height = 16;
		Sprite = LoadTexture("Sprites/Hammer.png");
		UpdateHitbox();
		Hitbox.width = 12;
	}
	if (Tag == Hat) {
		tag = Hat;
		SpriteSelector.width = 16;
		SpriteSelector.height = 16;
		SpriteSelector.x = 0;
		SpriteSelector.y = 8;
		Sprite = LoadTexture("Sprites/Items.png");
		UpdateHitbox();
	}
	if (Tag == Purse) {
		tag = Purse;
		SpriteSelector.width = 10;
		SpriteSelector.height = 8;
		SpriteSelector.x = 20;
		SpriteSelector.y = 8;
		Sprite = LoadTexture("Sprites/Items.png");
		UpdateHitbox();
	}
	if (Tag == Umbrella) {
		tag = Umbrella;
		SpriteSelector.width = 18;
		SpriteSelector.height = 17;
		SpriteSelector.x = 32;
		SpriteSelector.y = 0;
		Sprite = LoadTexture("Sprites/Items.png");
		UpdateHitbox();
	}
	if (Tag == StarItem) {
		tag = StarItem;
		SpriteSelector.width = 22;
		SpriteSelector.height = 22;
		SpriteSelector.x = 0;
		SpriteSelector.y = 0;
		Sprite = LoadTexture("Sprites/Star.png");  // <-- añade Star.png a Sprites/
		UpdateHitbox();
		Hitbox.width = 15;
		Hitbox.height = 15;
	}

}

void Interactable::OnInteraction(int playerNum)
{
	cout << "Interacted With item" << endl;

	if (tag == Hammer) {
		StartHammerTime(playerNum);
	}
	else if (tag == StarItem) {
		StartStarTime(playerNum);
	}
	else if (tag == Hat || tag == Purse || tag == Umbrella) {
		AddPoints(300);
		PlaySound(PointSound);
		cout << "Added 300 points" << endl;
	}
}