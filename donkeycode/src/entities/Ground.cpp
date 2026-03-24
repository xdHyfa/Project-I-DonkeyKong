#include "raylib.h"
#include "entities/Ground.h"

Truss Ramp_0[16];

int adderY = 225;
void RampSetter(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		Ramp[i].setPos(Ramp[i].TrussPos.x + adderX, adderY);
		adderX += 16;
		adderY -= 8;
		Ramp[i].truss = LoadTexture("sprites/TRUSS.png");
	}
}

void RampDrawer(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		DrawTexture(Ramp[i].truss, Ramp[i].TrussPos.x, Ramp[i].TrussPos.y, WHITE);
	}
}