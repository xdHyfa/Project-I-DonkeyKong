#pragma once
#include "Entities/entity.h"
#include "Entities/Barrel.h"

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



extern float TrussHeight;
extern float Ramp_0_YPos;
extern float Ramp_1_YPos;
extern float Ramp_2_YPos;
extern float Ramp_3_YPos;
extern float Ramp_4_YPos;
extern float Ramp_5_YPos;


void RampSetter(Truss* Ramp, int size, bool level0, bool TiltLeft, int plane, int adderY);

void Level1RampSetter();

void RampDrawer(Truss* Ramp, int size);

void RampCollision(Truss* Ramp, int size, Entity &entity);

void DrawLevel1Colliders();

void MapCollision(Entity &entity);

extern Truss Ramp_0[14];
extern Rectangle Ramp_0_Zone;

extern Truss Ramp_1[13];
extern Rectangle Ramp_1_Zone;

extern Truss Ramp_2[13];
extern Rectangle Ramp_2_Zone;

extern Truss Ramp_3[13];
extern Rectangle Ramp_3_Zone;

extern Truss Ramp_4[13];
extern Rectangle Ramp_4_Zone;

extern Truss Ramp_5[13];
extern Rectangle Ramp_5_Zone;

extern Truss Ramp_6[3];

void BarrelGroundCollisions(Barrel& barrel);