#pragma once
#include <raylib.h>
class Ramp
{
public:
	Ramp(Vector2 position);
	void DrawHitBox();
	void Draw();
	~Ramp();
	Vector2 GetPosition() { return position; }
	Texture2D GetImage() { return image; }
	Rectangle GetHitBox() { return collider;}
private:
	Texture2D image;
	Vector2 position;
	Rectangle collider;
	
};

