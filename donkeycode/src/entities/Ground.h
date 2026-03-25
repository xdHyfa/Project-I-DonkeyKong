#pragma once

class Truss {
public:
	Texture truss;
	Vector2 TrussPos = { 0 ,0 };
	Rectangle TrussBox = {TrussPos.x, TrussPos.y , 16, 8};
	
	void setPos(float x, float y) {
		TrussPos.x = x; TrussPos.y = y;
		TrussBox.x = TrussPos.x; TrussBox.y = TrussPos.y;
	}
	void setPos(float x) {
		TrussPos.x = x;
		TrussBox.x = TrussPos.x;
	}
};

void RampSetter(Truss* Ramp, int size, int level, bool TiltLeft, int plane, int adderY);

void RampDrawer(Truss* Ramp, int size);

void RampCollision(Truss* Ramp, int size, int plane);

extern Truss Ramp_0 [14];
extern Truss Ramp_1[13];
extern Truss Ramp_2[13];
extern Truss Ramp_3[13];