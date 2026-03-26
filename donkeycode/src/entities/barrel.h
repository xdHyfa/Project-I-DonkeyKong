#include "raylib.h"
#pragma once

#define NORMAL 0
#define RANDOM 1

class Barrel {
public:

    bool has_Spawned = false;
    Vector2 BarrelPosition = { 100.0f, 200.0f };
    Vector2 BarrelFloorCollider;
    Texture2D texture;

    Rectangle BarrelSprite = { 0.0f, 0.0f, 16, 16 };
    bool Anim_blink = false;
    bool Facing_left = false;

    void SetPos(float x, float y) {
        BarrelPosition.x = x; BarrelPosition.y = y;
        BarrelFloorCollider.x = BarrelPosition.x + 8;
        BarrelFloorCollider.y = BarrelPosition.y + 16;
    }
    void PlayAnimation() {
        if (Facing_left) { BarrelSprite.width = -16.0f; }
        else { BarrelSprite.width = 16.0f; }
    }
    void MoveBarrel() {
        PlayAnimation();
        if (BarrelPosition.x > 208 || BarrelPosition.x < 0) {
            Facing_left = !Facing_left;
        }
        if (!Facing_left) BarrelPosition.x += 20;
        else BarrelPosition.x -= 20;
        BarrelFloorCollider.x = BarrelPosition.x + 8;
        BarrelFloorCollider.y = BarrelPosition.y + 16;
        //Future expansion of movement tech

    }
};

extern Barrel barrel1;

void SpawnBarrel();