#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Headers/scenes.h"
#include "Headers/level1.h"

Scene current_scene = LEVEL1;
bool Scene_Init = false;
////////////////////////////////
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
////////////////////////////////

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	const int screenWidth = SCREEN_WIDTH;
	const int screenHeight = SCREEN_HEIGHT;

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Donkey Code");

	SetTargetFPS(24);

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		switch (current_scene) {
		case INTRO:
			

			break;
		case TITLE:
			

			break;
		case LEVEL1:

			runLevel1();

			break;
		}
	
	
	}

	CloseWindow();
	return 0;
}
