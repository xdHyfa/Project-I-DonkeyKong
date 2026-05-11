#include "raylib.h"
#include "include/resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Scenes/scenes.h"
#include "Scenes/level1.h"
#include "Scenes/level2.h"
#include "Scenes/level15.h"
#include "Core/constants.h"
#include "Scenes/TitleScreen.h"
#include "Scenes/InitialScreen.h"
#include "Entities/Player.h"
#include "Scenes/HowHigh.h"
#include "Scenes/Cutscene.h"
#include "Scenes/WinCutscene.h"
#include "Core/UI.h"
#include "Scenes/WinCutscene2.h"
#include "Scenes/HighScoreScreen.h"


int main()
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(SCALED_WIDTH, SCALED_HEIGHT, "Donkey Code");
	InitAudioDevice(); // <-- A?ade esto despu?s de InitWindow
	Camera2D camera = { 0 };
	camera.offset = { SCALED_WIDTH / 2.0f, SCALED_HEIGHT / 2.0f };
	camera.target = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
	camera.zoom = 3.0f;

	SetTargetFPS(24);

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(camera);

		// --- GAME OVER ---
		// Bloquea el switch entero mientras esta activo.
		// Cuando llega a 3s llama ChangeScene(true) -> HIGHSCORE.
		if (IsGameOver()) {
			TickGameOver();
			PrintUI();
			UpdateDrawGameOver();
			if (GetGameOverTimer() >= 3.0f) {
				EndGameOver();
				ChangeScene(true);   // -> HIGHSCORE
			}
			EndMode2D();
			EndDrawing();
			continue;
		}

		switch (GetCurrentScene()) {
		case INTRO:
			runInitialScreen();
			break;



		case TITLE:


			runMainMenu();


			break;

		case CUTSCENE:


			runCutscene();
			PrintUI();

			break;


		case HOWHIGH:


			runHowHigh();
			PrintUI();

			break;


		case LEVEL1:

			
			PrintUI();
			PrintBonus();
			runLevel1();
			CheckPause();

			break;

		case LEVEL15:

			PrintUI();
			PrintBonus();
			runLevel15();
			CheckPause();

			break;

		case WINCUTSCENE:
			PrintUI();
			runWinCutscene();
			break;

		case HOWHIGH2:
			PrintUI();
			runHowHigh();
			break;

		case LEVEL2:

			PrintUI();
			PrintBonus();
			runLevel2();
			CheckPause();
			break;

		case WINCUTSCENE2:
			PrintUI();
			runWinCutscene2();
			break;

		case HIGHSCORE:
			runHighScoreScreen();
			break;
		}

		EndMode2D();
		EndDrawing();
	}

	Mario.Unload();
	CloseAudioDevice(); // <-- Y esto antes de CloseWindow
	CloseWindow();
	return 0;
}