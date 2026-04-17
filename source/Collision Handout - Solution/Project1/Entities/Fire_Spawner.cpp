#include "raylib.h"
#include "Entities/Fire_Spawner.h"
#include "Core/constants.h"
#include "Maps/Level1Map.h"
#include <iostream>
using namespace std;

#define FIREVELOCITY 1.5f


void Fire::PlayAnimation() {
    fireTick += GetFrameTime();
    if (Facing_left) { FireSprite.width = -16.0f; }
    else { FireSprite.width = 16.0f; }

    if (fireTick >= 0.05f && !Anim_blink) {
        FireSprite.x += 16;
        Anim_blink = true;
        fireTick = 0;
    }
    if (fireTick >= 0.05f && Anim_blink) {
        FireSprite.x -= 16;
        Anim_blink = false;
        fireTick = 0;
    }
}

void Fire::Movement() {
        PlayAnimation();
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
        if(CanClimb && OnLadder){
            Position.y -= 1;
            return;
        }
  
        if (!Facing_left) Position.x += FIREVELOCITY;
        else Position.x -= FIREVELOCITY;
        if (GetRandomValue(1, 150) == 100) {
            ChangeDirection();
        }

        //Future expansion of movement tech

}





void SpawnFire(Fire &fire) {
	if (!fire.has_Spawned) {
		fire.Texture = LoadTexture("sprites/FIREBALL.png");
		fire.SetPos(100,230);
		fire.has_Spawned = true;
	}

}



