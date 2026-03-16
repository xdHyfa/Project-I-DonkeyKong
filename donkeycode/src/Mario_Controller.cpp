#include "Headers/Mario_Controller.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>


const int MARIO_SIZE = 16;
const int Screen_Width = 224;
const int Screen_Height = 256;

const int marioSpeed = 2;
const int gravity = 2;

Texture2D mario;
Rectangle source = { 0, 0, MARIO_SIZE, MARIO_SIZE };
Vector2 origin = { 0, 0 };

////////////////////////////////
bool isTextureValid(const Texture2D* texture) {
	return texture->id > 0;
}
////////////////////////////////

unsigned numFrames = 4;
float marioFrameWidth = MARIO_SIZE;
float marioFrameHeight = MARIO_SIZE;
Rectangle frameRec = { 0.0f, 0.0f, marioFrameWidth, marioFrameHeight };
Vector2 marioPosition = { Screen_Width / 2.0f, Screen_Height / 2.0f };
Vector2 marioVelocity = { 0.0f,0.0f };

unsigned frameDelay = 5;
unsigned frameDelayCounter = 0;
unsigned frameIndex = 0;

void Mario_Movement()
{
	if (!isTextureValid(&mario)) {

		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText(TextFormat("ERROR: Couldn't load %s.", mario), 20, 20, 20, BLACK);
			EndDrawing();
		}
	}


	if (IsKeyDown(KEY_UP)) {
		marioVelocity.y = -marioSpeed;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		marioVelocity.x = marioSpeed;
		if (frameRec.width < 0) {
			frameRec.width = -frameRec.width;
		}
	}
	else if (IsKeyDown(KEY_LEFT)) {
		marioVelocity.x = -marioSpeed;
		if (frameRec.width > 0) {
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
	if (frameDelayCounter > frameDelay) {
		frameDelayCounter = 0;

		if (marioMoving) {
			++frameIndex;
			frameIndex %= numFrames;
			frameRec.x = (float)MARIO_SIZE * frameIndex;
		}
	}

}

