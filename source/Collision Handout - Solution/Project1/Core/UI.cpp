#include "Scenes/scenes.h"
#include "Scenes/HighScoreScreen.h"   // <-- añadido para HS_SetJustPlayed
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
Vector2 BonusPos = { BonusTexturePos.x + 6, BonusTexturePos.y + 9 };


ui UI;
Rectangle rectitle1 = { 0, 0, 180, 88 };
Rectangle rectitle2 = { 0, 92, 180, 88 };
Texture PauseTitleBlue;
Texture PauseTitleRed;
float PauseTextTimer = 0.0f;
Music PauseMusic = { 0 };

bool isPaused = false;
bool hasLoaded = false;
bool firstLoop = true;
Vector2 PauseText = { 87, 180 };
void CheckPause() {
	if (IsKeyPressed(KEY_P) && !GetHammerTime()) {
		isPaused = !isPaused;
	}
	if (isPaused && !hasLoaded) {
		PauseTitleBlue = LoadTexture("SPRITES/Main menu.png");
		PauseTitleRed = LoadTexture("SPRITES/Main menu.png");
		PauseMusic.looping = true;
		if (firstLoop) firstLoop = false, PauseMusic = LoadMusicStream("Audio/MusicScreen1.wav"), PlayMusicStream(PauseMusic);
		else ResumeMusicStream(PauseMusic);
		hasLoaded = true;
	}

	if (hasLoaded && !isPaused) {
		PauseMusicStream(PauseMusic);
		UnloadTexture(PauseTitleBlue);
		UnloadTexture(PauseTitleRed);
		hasLoaded = false;

	}
	if (!isPaused) return;

	if (isPaused) {
		UpdateMusicStream(PauseMusic);
		DrawRectangle(0, 0, 224, 256, Fade(BLACK, 0.8f));
		DrawTextEx(UI_Font, "PAUSE", PauseText, 10, 0.5f, WHITE);
		if (PauseTextTimer < 0.025f) {
			DrawTextureRec(PauseTitleBlue, rectitle1, { 22.0f, 64.0f }, WHITE);
			PauseTextTimer += GetFrameTime();
		}
		else {
			DrawTextureRec(PauseTitleRed, rectitle2, { 22.0f, 64.0f }, WHITE);
			if (PauseTextTimer >= 0.05f) {
				PauseTextTimer = 0;
			}
			else {
				PauseTextTimer += GetFrameTime();
			}
		}
	}
}

bool GetPause() {
	return isPaused;
}

void ResetUI() {
	UI.HiScore = UI.score;
	UI.score = 0;
	UI.Lives = 2;
	UI.Level = 1;

}

void AddPoints(int points) {
	if (UI.PointsCooldown >= 0.5f || GetHammerTime()) {
		cout << "Adding points " << endl;
		UI.score += points;
		UI.PointsCooldown = 0;
	}
}
void AddPoints(int points, bool isBonus) {
	cout << "Adding points " << endl;
	UI.score += points;
	UI.PointsCooldown = 0;
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
	// En LEVEL15 el L=02 baja para no solaparse con el bonus
	Vector2 DrawLevelPos = (GetCurrentScene() == LEVEL15) ? Vector2{ 170, 22 } : LevelPos;
	DrawTextEx(UI_Font, TextFormat("L=%02d", UI.Level), DrawLevelPos, 7.5, 0.5f, BLUE);
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
		TriggerGameOver();
	}
}

// --- GAME OVER ---
bool gameOverActive = false;
float gameOverTimer = 0.0f;

// Score/Level guardados antes del reset, para que HighScoreScreen los lea
static int savedScoreForHS = 0;
static int savedLevelForHS = 0;

void TriggerGameOver() {
	gameOverActive = true;
	gameOverTimer = 0.0f;
	// Guardar valores ahora, antes de cualquier reset
	savedScoreForHS = UI.score;
	savedLevelForHS = UI.Level;
}

bool IsGameOver() {
	return gameOverActive;
}

void TickGameOver() {
	if (!gameOverActive) return;
	gameOverTimer += GetFrameTime();
}

float GetGameOverTimer() {
	return gameOverTimer;
}

// Llamado desde main cuando el timer acaba: resetea estado y va a HIGHSCORE
void EndGameOver() {
	gameOverActive = false;
	gameOverTimer = 0.0f;
	// Avisar al highscore que venimos de una partida real
	HS_SetJustPlayed(true);
	// El reset de score/lives/level lo hace HighScoreScreen después de leer los valores
}

// Solo dibuja el panel, no cambia nada
void UpdateDrawGameOver() {
	if (!gameOverActive) return;
	const int rectW = 100;
	const int rectH = 20;
	const int screenW = 224;
	const int screenH = 256;
	int rectX = (screenW - rectW) / 2;
	int rectY = (screenH - rectH) / 2;
	DrawRectangle(rectX, rectY, rectW, rectH, BLACK);
	Vector2 textPos = { (float)rectX + 10, (float)rectY + 6 };
	DrawTextEx(UI_Font, "GAME  OVER", textPos, 10, 0.5f, WHITE);
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
	// En LEVEL15 el bonus va a la derecha al nivel del HIGH SCORE
	Vector2 DrawBonusPos = (GetCurrentScene() == LEVEL15) ? Vector2{ 170, 0 } : BonusTexturePos;
	Vector2 DrawBonusNumPos = { DrawBonusPos.x + 6, DrawBonusPos.y + 9 };
	if (GetCurrentScene() == LEVEL1) {
		BonusRec = { 93,0,45, 21 };
		DrawTextureRec(BonusTexture, BonusRec, DrawBonusPos, WHITE);
		DrawRectangle(DrawBonusPos.x + 6, DrawBonusPos.y + 10, 35, 8, BLACK);
		DrawTextEx(UI_Font, TextFormat("%04d", bonus), DrawBonusNumPos, 10, 0.5f, SKYBLUE);
	}
	else {
		BonusRec = { 46,0,45, 21 };
		DrawTextureRec(BonusTexture, BonusRec, DrawBonusPos, WHITE);
		DrawRectangle(DrawBonusPos.x + 6, DrawBonusPos.y + 10, 35, 8, BLACK);
		DrawTextEx(UI_Font, TextFormat("%04d", bonus), DrawBonusNumPos, 10, 0.5f, YELLOW);
	};


}

void AddBonus() {
	AddPoints(bonus, true);
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

int GetHiScore()
{
	return UI.HiScore;
}

int GetScore()
{
	return UI.score;
}

void ResetAfterGameOver()
{
	if (UI.HiScore < UI.score) UI.HiScore = UI.score;
	UI.score = 0;
	UI.Lives = 2;
	UI.Level = 1;
}