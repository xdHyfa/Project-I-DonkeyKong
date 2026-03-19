#pragma once

class Fire {
public:
    float posX;
    float posY;
    Texture2D texture;
    void SetPos(float x, float y) {
        posX = x; posY = y;
    }
    void MoveFire() {
        posX += 0.5;
        //Future expansion of movement tech
    }
};

extern Fire Fire1;
extern Fire Fire2;
extern bool Fire1Spawned;
extern bool Fire2Spawned;

void SpawnFire();