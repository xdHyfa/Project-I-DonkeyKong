#pragma once
#include "Entities/Barrel.h"
#include <vector>

#define MAX_BARRELS 10
#define BARREL_SPAWN_INTERVAL 3.0f 

class BarrelSpawner {
public:
    std::vector<Barrel> barrels;

    float spawnTimer = 0.0f;
    float spawnX = 30.0f;
    float spawnY = 30.0f;

    void Init();
    void Update();
    void Draw();
    void Shutdown();
};

extern BarrelSpawner barrelSpawner;