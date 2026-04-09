#include "Ramp.hpp"

Ramp::Ramp(Vector2 position) {
image = LoadTexture("sprites/TRUSS.png");
this->position = position;

}
void Ramp::DrawHitBox()
{
	DrawRectangleLinesEx(GetHitBox(), 2, WHITE);
}
void Ramp::Draw() {

	DrawTextureRec(image, GetHitBox(),GetPosition(), WHITE);
	DrawHitBox();

}