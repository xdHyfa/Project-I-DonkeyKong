#include "Barrel.hpp"
#include "constants.hpp"

Barrel::Barrel() 
{
	spritesheet = LoadTexture("resources/commonbarrel.png");
	hasSpawned = true;

	position = { 0, 0 };

	isGrounded = false;
	
	velocityX = BARRELVELOCITY;
	velocityY = 0.0f;

	barrelsize = 13;

	hitbox = { 0.0f, 0.0f, barrelsize + 2, barrelsize + 2 };
	framerec = { 0.0f, 0.0f, barrelsize + 2, barrelsize + 2 };
	frameDelay = 1.0f;
	frameDelayCounter = 0;
	frameIndex = 0;
	barrelFrameWidth = 18;
	numFrames = 4;

	justFlipped = false;

}

Barrel::~Barrel() 
{
	UnloadTexture(spritesheet);
}

void Barrel::Draw()
{
	DrawTextureRec(spritesheet, framerec, position, WHITE);
}

void Barrel::UpdateAnimation()
{
	++frameDelayCounter;
	if (frameDelayCounter > frameDelay) {
		frameDelayCounter = 0;
		++frameIndex;
		frameIndex %= numFrames;
		framerec.x = barrelFrameWidth * frameIndex;
	}
	
}

Rectangle Barrel::GetHitbox()
{
	return Rectangle(hitbox);
}

//void Barrel::UpdateCollider() {
//	FloorCollider.x = Position.x + 8;
//	FloorCollider.y = Position.y + barrelsize;
//}

void Barrel::Movement() {
	velocityY += BARRELGRAVITY;
	position.y += velocityY;
	position.x += velocityX;

	if (position.x + barrelsize + 3 >= SCREEN_WIDTH || position.x - 2 <= 0) {
		FlipDirection();
	}
	else {
		justFlipped = false; 
	}

	if (position.y >= GetScreenHeight() / 4 - framerec.height) {	// Condicional para cuando toca el suelo o trusses (IMPLEMENTAR)
		position.y = GetScreenHeight() / 4 - framerec.height;
	}

	hitbox.x = position.x;
	hitbox.y = position.y;

	//UpdateCollider();
}

void Barrel::FlipDirection() {
	if (!justFlipped) {
		velocityX = -velocityX;
		justFlipped = true;
	}
}