#include "raylib.h"
#include "Entities/Ground.h"
#include "Entities/Ladders.h"
#include "Entities/entity.h"
#include "Entities/Player.h" 
#include "Core/constants.h"
#include <iostream>
using namespace std;

Texture Ladder::texture;

bool Ladder::CheckEntityOnLadder(Entity& entity) {
	if (CheckCollisionPointRec(entity.FloorCollider, Hitbox) && entity.getIsGrounded()) {
		return true;
	}
	else {
		return false;
	}
}


void DrawLadderCollider(Ladder* Map_Ladders, int size) {
	for (int i = 0; i < size; i ++){
	DrawRectangle(Map_Ladders[i].Hitbox.x, Map_Ladders[i].Hitbox.y, Map_Ladders[i].Hitbox.width, Map_Ladders[i].Hitbox.height, SKYBLUE);
	}
}




void LadderCollisions(Entity& entity, Ladder* Map_Ladders, int size) {
    for (int i = 0; i < size; i++) {
        if (Map_Ladders[i].CheckEntityOnLadder(entity)) {
            entity.CanClimb = true;
            return;
        }
    }
    if (entity.OnLadder && entity.tag == EntityTag::PLAYER) {
        Mario.justClimbedLadder = true;
        Mario.climbFinishFrame = 0;
    }
    entity.OnLadder = false;
    entity.CanClimb = false;
}
