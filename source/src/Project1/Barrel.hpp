#pragma once
#include <raylib.h>

class Barrel
{
public:
	Barrel();
	~Barrel();

	void Draw();
	void Movement();
	void FlipDirection();

	//void UpdateCollider();

	Rectangle GetHitbox();

	void UpdateAnimation();

private:
	Texture2D spritesheet;
	Vector2 position;
	Rectangle framerec;
	float barrelsize;



	Rectangle hitbox;

	bool hasSpawned;

	bool isGrounded;

	float velocityY;
	float velocityX;

	bool justFlipped;

	// Animación
	unsigned  numFrames;
	float     frameDelay;
	unsigned  frameDelayCounter;
	unsigned  frameIndex;
	float barrelFrameWidth;

};
