#include "Player.hpp"
#include "constants.hpp"
#include <iostream>

Player::Player()
{
	spritesheet = LoadTexture("resources/mario.png");
	position.x = GetScreenWidth() / 4 / 4;				// Screen divided by camera zoom divided by position at 1/4
	position.y = GetScreenHeight() / 4  - 32;			// Screen divided by camera zoom divided by the size of mario times two
	
	mariosize = 16;

	hitbox = { 0, 0, mariosize, mariosize };
	framerec = { 0, 0, mariosize, mariosize };

	velocityY = 0.0f;
	velocityX = 0.0f;
	lockedVelocityX = 0.0f;

	isGrounded = false;
	isJumping = false;
	isMoving = false;
	isDead = false;

	numFrames = 4;
	frameDelay = 0.5f;
	frameDelayCounter = 0;
	frameIndex = 0;
	deathLoopCount = 0;
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

Rectangle Player::GetHitbox()
{
	return Rectangle(hitbox);
}



void Player::Draw()
{
	DrawTextureRec(spritesheet, framerec, position, WHITE);
}

void Player::UpdateAnimation()
{
	if (isDead)
	{
		framerec.width = mariosize;
		framerec.y = 3 * mariosize;

		if (deathLoopCount < 2)
		{
			++frameIndex %= numFrames;
			framerec.x = mariosize * (float)frameIndex;
			if (frameIndex == 0) ++deathLoopCount;
		}
		else
		{
			framerec.x = 5 * mariosize;  
		}
	}

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

void Player::Die()
{
	isDead = true;
	isGrounded = false;
	isJumping = false;
}



void Player::MoveLeft()
{
	if (!isGrounded) return;  
	if (isDead) return;

	isMoving = true;
	velocityX = -MARIOVELOCITY;
	position.x += velocityX;
	if (position.x < 0) position.x = 0;
}

void Player::MoveRight()
{
	if (!isGrounded) return;  
	if (isDead) return;

	isMoving = true;
	velocityX = MARIOVELOCITY;
	position.x += velocityX;
	if (position.x > GetScreenWidth() / 4 - framerec.width)
		position.x = GetScreenWidth() / 4 - framerec.width;		// Constrain mario inside the window considering the x4 camera zoom
}

void Player::Jump()
{
	if (!isGrounded) return;
	if (isDead) return;

	lockedVelocityX = velocityX;          
	velocityY = -(float)JUMPHEIGHT; 
	isGrounded = false;
	isJumping = true;
}

void Player::ClimbLadder()
{
	if (isDead) return;

	if (IsKeyDown(KEY_UP)) {
		position.y -= MARIOVELOCITY;
	}
	else position.y += MARIOVELOCITY;
}

void Player::Gravity()
{
	if (isDead) return;

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

	hitbox.x = position.x;
	hitbox.y = position.y;
}
