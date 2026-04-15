#pragma once
#include "Entities/entity.h"


class Truss {
public:
	static Texture truss;
	Vector2 TrussPos = { 0 ,0 };
	Rectangle TrussBox = {TrussPos.x, TrussPos.y , 16, 8};
	bool hasLadderBelow = false;
	
	void setPos(float x, float y) {
		TrussPos.x = x; TrussPos.y = y;
		TrussBox.x = TrussPos.x; TrussBox.y = TrussPos.y;
	}
	void setPos(float x) {
		TrussPos.x = x;
		TrussBox.x = TrussPos.x;
	}
	static void LoadSharedTexture() {
			truss = LoadTexture("sprites/TRUSS.png");
	}
	static void UnloadSharedTexture() {
		UnloadTexture(truss);
	}
};



void RampSetter(Truss* Ramp, int size, bool level0, bool TiltLeft, int plane, int adderY);

void RampDrawer(Truss* Ramp, int size);

void RampCollision(Truss* Ramp, int size, Entity &entity);

