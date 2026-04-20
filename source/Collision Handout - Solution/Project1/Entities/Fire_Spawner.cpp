#include "raylib.h"
#include "Entities/Fire_Spawner.h"
#include "Core/constants.h"
#include "Maps/Level1Map.h"
#include <iostream>
using namespace std;

#define FIREVELOCITY 1.25f


void Fire::PlayAnimation() {
    fireTick += GetFrameTime();
    if (Facing_left) { FireSprite.width = -16.0f; }
    else { FireSprite.width = 16.0f; }

    if (fireTick >= 0.1f && !Anim_blink) {
        FireSprite.x += 16;
        Anim_blink = true;
        fireTick = 0;
    }
    if (fireTick >= 0.1f && Anim_blink) {
        FireSprite.x -= 16;
        Anim_blink = false;
        fireTick = 0;
    }
}

void Fire::Movement() {
        PlayAnimation();
        hitbox = { Position.x, Position.y };
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
        if(CanClimb && OnLadder){
            if (CanClimbDown) {
                Position.y += 1;
            }
            else{
            Position.y -= 1;
            }
            return;
        }
  
        if (!Facing_left) Position.x += FIREVELOCITY;
        else Position.x -= FIREVELOCITY;
        if (GetRandomValue(1, 150) == 100) {
            ChangeDirection();
        }

        //Future expansion of movement tech

}





void SpawnFire(Fire &fire, int x, int y, int sprite) {
	if (!fire.has_Spawned) {
        if (sprite == 1){
		    fire.Texture = LoadTexture("sprites/FIREBALL.png");
        }
        else {
            fire.Texture = LoadTexture("sprites/SMALLFIREBALL.png");
        }
        fire.SetPos(x,y);
		fire.has_Spawned = true;
	}

}



