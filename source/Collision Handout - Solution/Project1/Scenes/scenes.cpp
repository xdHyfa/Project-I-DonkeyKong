#include "Scenes/scenes.h"
#include "Entities/entity.h"
#include "Core/constants.h"
#include "raylib.h"
#include "Core/UI.h"
#include <iostream>
using namespace std;
// INTRO ? HIGHSCORE (mostrar tabla al arrancar) ? TITLE ? ...
Scene current_scene = INTRO;
bool Scene_Init = false;
bool Hammer_time = false;      // legacy / fallback
bool Hammer_time_p1 = false;
bool Hammer_time_p2 = false;
float DeathTimer = 0.0f;
bool hasStarted = false;
Texture2D DeathEffect;
Rectangle DeathSpriteSelector = { 0,0, 16, 16 };
Vector2 SavedPosition = { 0,0 };
Sound DeathSFX = { 0 };
bool SoundPlayed = false;
bool TwoPlayerMode = false;
bool Start2PTextTimer = false;
float TextTimer = 0;
Vector2 Text2PlayerPos = { 50, 100 };

void CheckTwoPlayers() {
	if (Option3True)
	{
		TwoPlayerMode = true;
	}
	else {
		TwoPlayerMode = false;
	}
		/*if (!Option3True) return;
		if (!TwoPlayerMode) TwoPlayerMode = true, Start2PTextTimer = true;
		else TwoPlayerMode = false, Start2PTextTimer = false, TextTimer = 0;
	}
	if (Start2PTextTimer) {
		if (TextTimer < 1.5f) {
			TextTimer += GetFrameTime();
			DrawTextEx(UI_Font, "2 PLAYER MODE ON", Text2PlayerPos, 10, 0.5f, YELLOW);
		}
	}*/
}

bool GetTwoPlayers() {
	return TwoPlayerMode;
}

bool SfxUnloaded = false;
void StartEntityDeath(Entity& entity) {
	DeathTimer = 0.0f;
	hasStarted = true;
	DeathEffect = LoadTexture("Sprites/Kill aura.png");
	SfxUnloaded = false;
	SavedPosition = entity.Position;
	DeathSFX = LoadSound("Audio/Hammer-kill crop.wav");
}

bool GetIsKilling() {
	return hasStarted;
}

void EndEntityDeath() {
	hasStarted = false;
	UnloadTexture(DeathEffect);
	if (!SfxUnloaded) UnloadSound(DeathSFX), SfxUnloaded = true;;

	SoundPlayed = false;
}

void PlayEntityDeath() {
	DeathTimer += GetFrameTime();
	if (!SoundPlayed) PlaySound(DeathSFX), SoundPlayed = true;
	if (DeathTimer < 0.2f) {
		DeathSpriteSelector = { 0,0,18, 16 };
		DrawTextureRec(DeathEffect, DeathSpriteSelector, SavedPosition, WHITE);
		return;
	}
	else if (DeathTimer < 0.4f) {
		DeathSpriteSelector = { 20,0,10, 16 };
		Vector2 buffer = { SavedPosition.x + 4, SavedPosition.y };
		DrawTextureRec(DeathEffect, DeathSpriteSelector, buffer, WHITE);
		return;
	}
	else if (DeathTimer < 0.6f) {
		DeathSpriteSelector = { 31,0,7, 16 };
		Vector2 buffer = { SavedPosition.x + 4, SavedPosition.y };
		DrawTextureRec(DeathEffect, DeathSpriteSelector, buffer, WHITE);
		return;
	}
	else if (DeathTimer < 0.8f) {
		DeathSpriteSelector = { 39,0,20, 16 };
		DrawTextureRec(DeathEffect, DeathSpriteSelector, SavedPosition, WHITE);
		return;
	}
	EndEntityDeath();
}

// Per-player hammer
void StartHammerTime(int playerNum) {
	if (playerNum == 1) Hammer_time_p1 = true;
	else                Hammer_time_p2 = true;
}
bool GetHammerTime(int playerNum) {
	if (playerNum == 1) return Hammer_time_p1;
	else                return Hammer_time_p2;
}
void StopHammerTime(int playerNum) {
	if (playerNum == 1) Hammer_time_p1 = false;
	else                Hammer_time_p2 = false;
}

// Legacy (used by Player.cpp internamente via PlayerNum)
void StartHammerTime() { Hammer_time_p1 = true; Hammer_time_p2 = true; }
bool GetHammerTime() { return Hammer_time_p1 || Hammer_time_p2; }
void StopHammerTime() { Hammer_time_p1 = false; Hammer_time_p2 = false; }
void ChangeScene() {
	Scene_Init = false;

	// Al arrancar, INTRO salta directo al highscore (solo visualización)
	if (current_scene == INTRO) {
		current_scene = HIGHSCORE;
		cout << "Highscore scene" << endl;
		return;
	}
	if (current_scene == TITLE) {
		current_scene = CUTSCENE;
		cout << "cutscene scene" << endl;
		return;
	}
	if (current_scene == DONKEYSHOP) {
		current_scene = CUTSCENE;
		cout << "cutscene scene" << endl;
		return;
	}
	if (current_scene == CUTSCENE) {
		current_scene = HOWHIGH;
		cout << "Howhigh scene" << endl;
		return;
	}
	if (current_scene == HOWHIGH) {
		current_scene = LEVEL1;
		cout << "Level1 scene" << endl;
		return;
	}
	if (current_scene == LEVEL1) {
		current_scene = WINCUTSCENE;
		cout << "Wincutscene scene" << endl;
		return;
	}
	if (current_scene == LEVEL15) {
		current_scene = HOWHIGH2;
		cout << "Howhigh2 scene" << endl;
		return;
	}
	if (current_scene == WINCUTSCENE) {
		current_scene = LEVEL15;
		cout << "level1.5 scene" << endl;
		return;
	}
	if (current_scene == HOWHIGH2) {
		current_scene = LEVEL2;
		cout << "level2 scene" << endl;
		return;
	}
	if (current_scene == LEVEL2) {
		current_scene = WINCUTSCENE2;
		cout << "wincutscene2 scene" << endl;
		return;
	}
	if (current_scene == WINCUTSCENE2) {
		current_scene = HOWHIGH;
		cout << "howhigh scene" << endl;
		return;
	}
	if (current_scene == HIGHSCORE) {
		current_scene = TITLE;
		cout << "title scene" << endl;
		return;
	}
}

void ChangeScene(bool NoLives) {
	Scene_Init = false;
	current_scene = HIGHSCORE;
	cout << "Highscore scene" << endl;
}

void ChangeScene(int Secret) {
	Scene_Init = false;
	if (Secret == 67){
	current_scene = DONKEYSHOP;
	cout << "donkeyshop scene" << endl;
	}
}

Scene GetCurrentScene() {
	return current_scene;
}