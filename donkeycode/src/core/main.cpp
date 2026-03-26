#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "scenes/scenes.h"
#include "scenes/level1.h"
#include "core/constants.h"

Scene current_scene = LEVEL1;
bool Scene_Init = false;

int main ()
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
			

			break;
		case TITLE:
			

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
