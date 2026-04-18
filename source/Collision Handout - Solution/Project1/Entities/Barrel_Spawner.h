#pragma once
#include "Entities/Barrel.h"
#include <vector>

#define MAX_BARRELS 10
#define BARREL_SPAWN_INTERVAL 3.0f
#define BARREL_RESET_Y SCREEN_HEIGHT + 10  

class BarrelSpawner {
public:
    std::vector<Barrel> barrels;
    Texture2D barrelTexture = { 0 };  

    float spawnTimer = 0.0f;
    float spawnX = 30.0f;
    float spawnY = 30.0f;

    void Init();
    void Update();
    void Draw();
    void Shutdown();

private:
    void ResetBarrel(Barrel& b);
};

extern BarrelSpawner barrelSpawner;