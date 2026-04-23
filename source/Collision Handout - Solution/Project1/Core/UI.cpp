#include "Scenes/scenes.h"
#include "raylib.h"
#include <iostream>
#include "UI.h"
using namespace std;

void UpdateDrawScorePopup();

class ui {
public:
	int score = 0;
	int HiScore = 0;
	int Lives = 2;
	int Level = 1;

	bool FontLoaded = false;
	bool BonusLoaded = false;
	float PointsCooldown = 0.0f;
};

Font UI_Font;
Texture BonusTexture;

Texture2D LifeIcon = { 0 };
bool LifeIconLoaded = false;


Rectangle BonusRec = { 93,0,45, 21 };
Vector2 BonusTexturePos = { 170, 35 };

Vector2 OneUpPos = { 18, 0 };
Vector2 scorePos = { 4,8 };
Vector2 HiScorePos = { 75, 0 };
Vector2 HiScoreNumPos = { HiScorePos.x + 16, HiScorePos.y + 8 };
Vector2 LevelPos = { 170, 20 };
Vector2 BonusPos = { BonusTexturePos.x + 6, BonusTexturePos.y + 9};




ui UI;

void ResetUI() {
	UI.HiScore = UI.score;
	UI.score = 0;
	UI.Lives = 2;
	UI.Level = 1;
	
}

void AddPoints(int points) {
	if(UI.PointsCooldown >= 0.5f){
		cout << "Adding points " << endl;
		UI.score += points;
		UI.PointsCooldown = 0;
	}
}

void SetCooldown() {
	if (UI.PointsCooldown > 1.01f) return;
	UI.PointsCooldown += GetFrameTime();
}

void AddLevel() {
	UI.Level++;
}

void PrintUI() {
	if (!UI.FontLoaded) {
		UI_Font = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
		UI.FontLoaded = true;
	}
	if (!LifeIconLoaded) {
		LifeIcon = LoadTexture("Sprites/MARIO.png");
		LifeIconLoaded = true;
	}

	DrawTextEx(UI_Font, "1UP", OneUpPos, 10, 0.5f, RED);
	DrawTextEx(UI_Font, TextFormat("%06d", UI.score), scorePos, 10, 0.5f, WHITE);
	DrawTextEx(UI_Font, "HIGH SCORE", HiScorePos, 10, 0.5f, RED);
	DrawTextEx(UI_Font, TextFormat("%06d", UI.HiScore), HiScoreNumPos, 10, 0.5f, WHITE);
	DrawTextEx(UI_Font, TextFormat("L=%02d", UI.Level), LevelPos, 7.5, 0.5f, BLUE);
	UpdateDrawScorePopup();

	// Draw life icons below 1UP/score
	int livesToShow = UI.Lives + 1;
	Rectangle lifeFrame = { 32.0f, 1.0f, 14.0f, 14.0f };
	for (int i = 0; i < livesToShow; i++) {
		Rectangle destRect = { 4.0f + (i * 8.0f), 20.0f, 7.0f, 7.0f };
		DrawTexturePro(LifeIcon, lifeFrame, destRect, { 0, 0 }, 0.0f, WHITE);
	}
}

void RemoveLife() {
	UI.Lives--;
}

void CheckLives() {
	if (UI.Lives < 0) {
		UI.Lives = 2;
		ChangeScene(true);
		UI.Level = 1;
		if (UI.HiScore < UI.score) UI.HiScore = UI.score;
			UI.score = 0;
	}
}


int bonus = 5000;
float bonus_tick = 0.0f;

void ResetBonus() {
	bonus = 5000;
	bonus_tick = 0.0f;
}

void UpdateBonus() {
	if (bonus_tick > 3.0f) {
		bonus -= 100;
		bonus_tick = 0.0f;
	}
	else
	{
		bonus_tick += GetFrameTime();
	}
}

void PrintBonus() {
	if (!UI.FontLoaded) {
		UI_Font = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
		UI.FontLoaded = true;
	}
	if (!UI.BonusLoaded) {
		BonusTexture = LoadTexture("Sprites/REAL BONUS.png");
		UI.BonusLoaded = true;
	}
	if (GetCurrentScene() == LEVEL1) {
		BonusRec = { 93,0,45, 21 };
		DrawTextureRec(BonusTexture, BonusRec, BonusTexturePos, WHITE);
		DrawRectangle(BonusTexturePos.x + 6, BonusTexturePos.y + 10, 35, 8, BLACK);
		DrawTextEx(UI_Font, TextFormat("%04d", bonus), BonusPos, 10, 0.5f, SKYBLUE);
	}
	else {
		BonusRec = { 46,0,45, 21 };
		DrawTextureRec(BonusTexture, BonusRec, BonusTexturePos, WHITE);
		DrawRectangle(BonusTexturePos.x + 6, BonusTexturePos.y + 10, 35, 8, BLACK);
		DrawTextEx(UI_Font, TextFormat("%04d", bonus), BonusPos, 10, 0.5f, YELLOW);
	};


}

void AddBonus() {
	AddPoints(bonus);
	bonus_tick = 0.0f;
	ResetBonus();
}

bool popupActive = false;
float popupTimer = 0.0f;
Vector2 popupPos = { 0, 0 };
int PrintedPoints = 100;

void ShowScorePopup(Vector2 pos, int points) {
	popupActive = true;
	popupTimer = 0.0f;
	popupPos = { pos.x, pos.y - 12 };
	PrintedPoints = points;
}

void UpdateDrawScorePopup() {
	if (!popupActive) return;
	popupTimer += GetFrameTime();
	DrawTextEx(UI_Font, TextFormat("%d", PrintedPoints), popupPos, 10, 0.5f, WHITE);
	if (popupTimer >= 1.0f) {
		popupActive = false;
	}
}



int GetLevel()
{
	return UI.Level;
}
