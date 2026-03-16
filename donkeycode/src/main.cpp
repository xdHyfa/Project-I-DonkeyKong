#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir



////////////////////////////////
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
////////////////////////////////



#define MARIO_SIZE 16

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256


Texture2D mario;
Rectangle source = { 0, 0, MARIO_SIZE, MARIO_SIZE };
Rectangle dest = { 0, SCREEN_HEIGHT-20, source.width, source.height };
Vector2 origin = { 0, 0 };




////////////////////////////////
bool isTextureValid(const Texture2D* texture) {
	return texture->id > 0;
}
////////////////////////////////






int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	const int screenWidth = SCREEN_WIDTH;
	const int screenHeight = SCREEN_HEIGHT;

	//////////////////////////
	const int marioSpeed = 2;
	const int gravity = 2;
	//////////////////////////
	
	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Donkey Code");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");


	////////////////////////////////////////////////////////////////////

	mario = LoadTexture("MARIO.png");

	if (!isTextureValid(&mario)) {

		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText(TextFormat("ERROR: Couldn't load %s.", mario), 20, 20, 20, BLACK);
			EndDrawing();
		}
		return 10;
	}

	

	unsigned numFrames = 4;
	float marioFrameWidth = MARIO_SIZE;  
	float marioFrameHeight = MARIO_SIZE;
	Rectangle frameRec = { 0.0f, 0.0f, marioFrameWidth, marioFrameHeight };
	Vector2 marioPosition = { screenWidth / 2.0f, screenHeight / 2.0f };
	Vector2 marioVelocity = { 0.0f,0.0f };

	unsigned frameDelay = 5;
	unsigned frameDelayCounter = 0;
	unsigned frameIndex = 0;

	///////////////////////////////////////////////////////////////////

	SetTargetFPS(24);
	
	


	


	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		


		/////////////////////////////////////////////////////////////
		if (IsKeyDown(KEY_UP)) {
			marioVelocity.y = -marioSpeed;
		}
		if (IsKeyDown(KEY_RIGHT)) {
			marioVelocity.x = marioSpeed;
			if(frameRec.width < 0) {
				frameRec.width = -frameRec.width;
			}
        } else if (IsKeyDown(KEY_LEFT)) {
			marioVelocity.x = -marioSpeed;
			if(frameRec.width > 0) {
				frameRec.width = -frameRec.width;
			}
		}
		else {
			marioVelocity.x = 0;
		}
		bool marioMoving = marioVelocity.x != 0.0f || marioVelocity.y != 0.0f;
        //----------------------------------------------------------------------------------
		marioPosition = Vector2Add(marioPosition, marioVelocity);
		
		++frameDelayCounter;
		if(frameDelayCounter > frameDelay) {
			frameDelayCounter = 0;
			
			if(marioMoving) {
				++frameIndex;
				frameIndex %= numFrames;
				frameRec.x = (float) MARIO_SIZE * frameIndex;
			}
		}

		/////////////////////////////////////////////////////////////






		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Donkey Kong", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTextureRec(mario, frameRec, marioPosition, WHITE);

		//DrawTexturePro(mario, source, dest, origin, 0.0f, WHITE);
		//source.x += MARIO_SIZE;


		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(mario);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
