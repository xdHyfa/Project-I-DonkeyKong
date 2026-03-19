#pragma once

class Fire {
public:
    Texture2D texture;
    int posX;
    int posY;
};

extern Fire Fire1;
extern Fire Fire2;
extern bool Fire1Spawned;
extern bool Fire2Spawned;

void SpawnFire();