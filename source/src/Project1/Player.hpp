#pragma once
#include <raylib.h>

class Player
{
public:
	// Constructor and destructor
	Player();
	~Player();

	// Main functions
	void Draw();
	void MoveLeft();
	void MoveRight();
	void Jump();
	void ClimbLadder();
	void Gravity();
	void UpdateAnimation();

	// Getters and setters
	bool GetIsGrounded();
	void SetIsGrounded(bool trueorfalse);

	void SetVelocityY(float velocityY);

private:
	Texture2D spritesheet;
	Vector2 position;
	Rectangle framerec;
	float mariosize;
	bool isGrounded;
	float velocityY;		// Usado para el salto en parabola (así hacemos que no sea instantaneo)
	float velocityX;
	float lockedVelocityX;  // Usado para que en el salto no puedes moverte horizontalmente
	bool isJumping;
	bool isMoving;

	// Animación
	unsigned  numFrames       = 4;
	float     frameDelay      = 0.5f;
	unsigned  frameDelayCounter = 0;
	unsigned  frameIndex      = 0;
};

