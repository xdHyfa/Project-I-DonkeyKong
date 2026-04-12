#include "Player.hpp"
#include "constants.hpp"

Player::Player()
{
	spritesheet = LoadTexture("resources/mario.png");
	position.x = GetScreenWidth() / 4 / 4;				// Screen divided by camera zoom divided by position at 1/4
	position.y = GetScreenHeight() / 4  - 32;			// Screen divided by camera zoom divided by the size of mario times two
	mariosize = 16;
	framerec = { 0, 0, mariosize, mariosize };

	isGrounded = false;

	velocityY = 0.0f;
	velocityX = 0.0f;
	lockedVelocityX = 0.0f;

	isJumping = false;
	isMoving = false;

	frameDelayCounter = 0;
	frameIndex = 0;
}

Player::~Player()
{
	UnloadTexture(spritesheet);
}



bool Player::GetIsGrounded() 
{
	return isGrounded;
}

void Player::SetIsGrounded(bool trueorfalse)
{
	this->isGrounded = trueorfalse;
}

void Player::SetVelocityY(float velocityY) 
{
	this->velocityY = velocityY;
}



void Player::Draw()
{
	DrawTextureRec(spritesheet, framerec, position, WHITE);
}

void Player::UpdateAnimation()
{
	++frameDelayCounter;
	if (frameDelayCounter <= frameDelay) return;
	frameDelayCounter = 0;

	if (isJumping)
	{
		framerec.x = 3 * mariosize;
	}
	else if (isMoving && IsKeyDown(KEY_RIGHT))
	{
		framerec.width = mariosize;   
		++frameIndex %= numFrames;
		framerec.x = mariosize * (float)frameIndex;
	}
	else if (isMoving && IsKeyDown(KEY_LEFT))
	{
		framerec.width = -mariosize;  
		++frameIndex %= numFrames;
		framerec.x = mariosize * (float)frameIndex;
	}
	else
	{
		frameIndex = 0;
		framerec.x = 0.0f;
	}

	isMoving = false;
}

void Player::MoveLeft()
{
	if (!isGrounded) return;  

	isMoving = true;
	velocityX = -MARIOVELOCITY;
	position.x += velocityX;
	if (position.x < 0) position.x = 0;
}

void Player::MoveRight()
{
	if (!isGrounded) return;  

	isMoving = true;
	velocityX = MARIOVELOCITY;
	position.x += velocityX;
	if (position.x > GetScreenWidth() / 4 - framerec.width)
		position.x = GetScreenWidth() / 4 - framerec.width;		// Constrain mario inside the window considering the x4 camera zoom
}

void Player::Jump()
{
	if (!isGrounded) return;

	lockedVelocityX = velocityX;          
	velocityY = -(float)JUMPHEIGHT; 
	isGrounded = false;
	isJumping = true;
}

void Player::ClimbLadder()
{
	if (IsKeyDown(KEY_UP)) {
		position.y -= MARIOVELOCITY;
	}
	else position.y += MARIOVELOCITY;
}

void Player::Gravity()
{
	if (!isGrounded) {
		velocityY += GRAVITY;
		position.y += velocityY;
		position.x += lockedVelocityX;
	}

	if (position.y >= GetScreenHeight() / 4 - framerec.height) {	// Condicional para cuando toca el suelo o trusses (IMPLEMENTAR)
		position.y = GetScreenHeight() / 4 - framerec.height; 
		velocityY = 0.0f;
		lockedVelocityX = 0.0f;
		isGrounded  = true;
		isJumping = false;
	}
	velocityX = 0;
}
