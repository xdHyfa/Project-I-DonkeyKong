#include "Headers/Mario_Controller.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "Headers/constants.h"

const float GRAVITY = 1.0f;

class Mario
{
protected:
	bool isAlive = true;
	bool isGrounded = true;
	float x = 0.0f;
	float y = 0.0f;
public:
	const int VELOCITY = 2;
    const int JUMP = 10;
	const int MARIO_SIZE = 16;

	float getPosX() const { return x; }
	float getPosY() const { return y; }
	bool  getIsGrounded() const { return isGrounded; }

	void setPosX(float newX) { x = newX; }
	void setPosY(float newY) { y = newY; }
	void setGrounded(bool g) { isGrounded = g; }

	bool die() {
		isAlive = false;
	}
	void moveLeft() { x -= VELOCITY; }
	void moveRight() { x += VELOCITY; }
	void moveJump() { if (isGrounded) { isGrounded = false; y -= JUMP; } }
};


Mario      entityMario;
Texture2D  marioTexture;

unsigned   numFrames = 4;
float      marioFrameWidth = 0.0f;   // se inicializa en Setup()
float      marioFrameHeight = 0.0f;

Rectangle  frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };  // se inicializa en Setup()
Vector2    marioPosition = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
Vector2    marioVelocity = { 0.0f, 0.0f };

unsigned   frameDelay = 5;
unsigned   frameDelayCounter = 0;
unsigned   frameIndex = 0;

bool isTextureValid(const Texture2D& texture)
{
    return texture.id > 0;
}

void Setup()
{
    marioFrameWidth = (float)entityMario.MARIO_SIZE;
    marioFrameHeight = (float)entityMario.MARIO_SIZE;
    frameRec = { 0.0f, 0.0f, marioFrameWidth, marioFrameHeight };
}

void Mario_Movement()
{
    if (!isTextureValid(marioTexture))
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("ERROR: No se pudo cargar la textura de Mario.", 20, 20, 20, RED);
            EndDrawing();
        }
        return; 
    }

    marioVelocity.x = 0.0f;

    if (IsKeyPressed(KEY_UP))
    {
        entityMario.moveJump();
        marioVelocity.y = -(float)entityMario.JUMP;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        entityMario.moveRight();
        marioVelocity.x = (float)entityMario.VELOCITY;
        if (frameRec.width < 0) frameRec.width = -frameRec.width; 
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        entityMario.moveLeft();
        marioVelocity.x = -(float)entityMario.VELOCITY;
        if (frameRec.width > 0) frameRec.width = -frameRec.width; 
    }

    if (!entityMario.getIsGrounded())
    {
        marioVelocity.y += GRAVITY;
    }
    
    

    marioPosition = Vector2Add(marioPosition, marioVelocity);

    entityMario.setPosX(marioPosition.x);
    entityMario.setPosY(marioPosition.y);

    bool marioMoving = marioVelocity.x != 0.0f || marioVelocity.y != 0.0f;

    ++frameDelayCounter;
    if (frameDelayCounter > frameDelay)
    {
        frameDelayCounter = 0;
        if (marioMoving)
        {
            ++frameIndex;
            frameIndex %= numFrames;
            frameRec.x = marioFrameWidth * (float)frameIndex;
        }
        else
        {
            frameIndex = 0;      
            frameRec.x = 0.0f;
        }
    }
}

