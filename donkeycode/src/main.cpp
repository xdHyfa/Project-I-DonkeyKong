#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define MARIO_SIZE 16

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256


Texture2D mario;
Rectangle source = { 0, 0, MARIO_SIZE, MARIO_SIZE };
Rectangle dest = { 0, SCREEN_HEIGHT-20, source.width, source.height };
Vector2 origin = { 0, 0 };


int mariosize = MARIO_SIZE;

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	const int screenWidth = SCREEN_WIDTH;
	const int screenHeight = SCREEN_HEIGHT;
	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Donkey Code");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	
	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	SetTargetFPS(60);
	
	static double x = 500, y = 900;
	
	static double speed_x = 2, speed_y = 1;
	static double jump_time = 0.5;


	mario = LoadTexture("MARIO.png");


	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		if (IsKeyDown(KEY_RIGHT)) x += speed_x;
		if (IsKeyDown(KEY_LEFT)) x -= speed_x;
		if (IsKeyPressed(KEY_SPACE)) {
			float goomba = GetFrameTime();
			while (goomba < jump_time) {
				y -= speed_y;
				goomba+= 0.01f;
				DrawTexture(wabbit, x, y, WHITE);
			}
		}

		if (y > screenHeight-40) y = screenHeight-40;
		if (y < 0) y = 0;
		if (x > screenWidth) x = screenWidth;
		if (x < 0) x = 0;
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Donkey Kong", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, x, y, WHITE);

		DrawTexturePro(mario, source, dest, origin, 0.0f, WHITE);
		source.x += MARIO_SIZE;


		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);
	UnloadTexture(mario);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
