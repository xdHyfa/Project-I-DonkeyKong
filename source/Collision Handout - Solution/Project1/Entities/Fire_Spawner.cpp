#include "raylib.h"
#include "Entities/Fire_Spawner.h"
#include "Core/constants.h"
#include "Maps/Level1Map.h"

#define FIREVELOCITY 2.0f


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
        if (Position.x > 208 || Position.x < 0) {
            Facing_left = !Facing_left;
        }
        if (!Facing_left) Position.x += FIREVELOCITY;
        else Position.x -= FIREVELOCITY;
        if (GetRandomValue(1, 150) == 100) {
            Facing_left = !Facing_left;
        }
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
        //Future expansion of movement tech

}

    /* THE ENTITIES ARE DEFINED HERE */
    Fire Fire1;
    Fire Fire2;



void SpawnFire() {
	if (!Fire1.has_Spawned) {
		Fire1.Texture = LoadTexture("sprites/FIREBALL.png");
		Fire1.SetPos(100,230);
		Fire1.has_Spawned = true;
	}

}

void FireRoutine() {
	if (IsKeyPressed(KEY_K) && !Fire1.has_Spawned) SpawnFire();
	
	if (Fire1.has_Spawned) {
		Fire1.Movement();
		Level1RampCollisions(Fire1);
		DrawTextureRec(Fire1.Texture, Fire1.FireSprite, Fire1.Position, WHITE);
	}
}

