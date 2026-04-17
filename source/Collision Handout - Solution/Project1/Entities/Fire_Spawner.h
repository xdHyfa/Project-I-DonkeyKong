#pragma once
#include "Entities/entity.h"

class Fire : public Entity {
public:
    Fire() {
        tag = EntityTag::FIRE;
        isGrounded = true;
    }

    bool has_Spawned = false;
    
    Rectangle FireSprite = { 0.0f, 0.0f, 16, 16 };
    float fireTick = 0.0f;
    bool Anim_blink = false;
    bool Facing_left = false;
    
    void SetPos(float x, float y) {
        Position.x = x; Position.y = y;
        FloorCollider.x = Position.x +8;
        FloorCollider.y = Position.y +16;
    }
    void PlayAnimation();
 
    void Movement() override;

    void ChangeDirection() {
        Facing_left = !Facing_left;
    }
};

extern Fire Fire1;
extern Fire Fire2;

void SpawnFire(Fire &fire);