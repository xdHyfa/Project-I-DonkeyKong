#pragma once

class Fire {
public:

    bool has_Spawned = false;
    Vector2 FirePosition = { 100.0f, 200.0f };
    Vector2 FireFloorCollider;
    Texture2D texture;
    
    Rectangle FireSprite = { 0.0f, 0.0f, 16, 16 };
    float fireTick = 0.0f;
    bool Anim_blink = false;
    bool Facing_left = false;
    
    void SetPos(float x, float y) {
        FirePosition.x = x; FirePosition.y = y;
        FireFloorCollider.x = FirePosition.x +8;
        FireFloorCollider.y = FirePosition.y +16;
    }
    void PlayAnimation() {       
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
    void MoveFire() {
        PlayAnimation();
        if (FirePosition.x > 208 || FirePosition.x < 0) {
            Facing_left = !Facing_left;
        }
        if (!Facing_left) FirePosition.x += 1.5;
        else FirePosition.x -= 1.5;
        if (GetRandomValue(1, 150) == 100) {
            Facing_left = !Facing_left;
        }
        FireFloorCollider.x = FirePosition.x + 8;
        FireFloorCollider.y = FirePosition.y + 16;
        //Future expansion of movement tech
        
    }
};

extern Fire Fire1;
extern Fire Fire2;

void FireSpawner();
void SpawnFire();