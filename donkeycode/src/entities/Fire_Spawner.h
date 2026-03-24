#pragma once

class Fire {
public:

    bool has_Spawned = false;
    Vector2 FirePosition = { 100.0f, 200.0f };
    Texture2D texture;
    
    Rectangle FireSprite = { 0.0f, 0.0f, 16, 16 };
    float fireTick = 0.0f;
    bool Anim_blink = false;
    bool Facing_left = false;
    
    void SetPos(float x, float y) {
        FirePosition.x = x; FirePosition.y = y;
    }
    void PlayAnimation() {       
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
    void MoveFire() {
        PlayAnimation();
        if (!Facing_left) FirePosition.x += 0.5;
        else FirePosition.x -= 0.5;
        if (GetRandomValue(1, 100) == 100) {
            Facing_left = !Facing_left;
        }
        //Future expansion of movement tech
        
    }
};

extern Fire Fire1;
extern Fire Fire2;

void SpawnFire();