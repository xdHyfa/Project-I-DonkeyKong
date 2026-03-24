#include "raylib.h"
#include "entities/Ground.h"

Truss Ramp_0[16];

int adder = 0;
void RampSetter(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		/*Ramp[i].setPos(Ramp[i].TrussPos.x + (float)adder, 225.0f);
		adder += 16;*/
		Ramp[i].truss = LoadTexture("TRUSS.png");
	}
}

void RampDrawer(Truss* Ramp, int size) {
	for (int i = 0; i < size; i++) {
		DrawTexture(Ramp[i].truss, Ramp[i].TrussPos.x, Ramp[i].TrussPos.y, WHITE);
	}
}