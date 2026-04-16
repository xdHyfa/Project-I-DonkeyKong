#include "raylib.h"
#include "include/resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Scenes/scenes.h"
#include "Scenes/level1.h"
#include "Core/constants.h"
#include "Scenes/TitleScreen.h"
#include "Scenes/InitialScreen.h"

Scene current_scene = INTRO;
bool Scene_Init = false;

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(SCALED_WIDTH, SCALED_HEIGHT, "Donkey Code");
	Camera2D camera = { 0 };
	camera.offset = { SCALED_WIDTH / 2.0f, SCALED_HEIGHT / 2.0f };
	camera.target = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
	camera.zoom = 4.0f;

	SetTargetFPS(24);

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(camera);

		switch (current_scene) {
		case INTRO:

			runInitialScreen();
			break;



		case TITLE:
			runMainMenu();




			break;
		case LEVEL1:

			runLevel1();

			break;
		}

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
