#include "raylib.h"
#include "Core/constants.h"
#pragma once
#define NORMAL 0
#define RANDOM 1
#define BARRELSIZE 13
#define BARRELVELOCITY 2.0f
#define BARREL_GRAVITY 0.3f

class Barrel {
public:
    bool has_Spawned = false;
    Vector2 BarrelPosition = { 100.0f, 200.0f };
    Vector2 BarrelFloorCollider;
    float velocityX = BARRELVELOCITY;   
    float velocityY = 0.0f;
    Texture2D texture;
    Rectangle frameRec = { 0.0f, 0.0f, BARRELSIZE+2, BARRELSIZE+2 };
    float     frameDelay = 1.0f;
    unsigned  frameDelayCounter = 0;
    unsigned  frameIndex = 0;
    float     barrelFrameWidth = 0.0f;
    unsigned  numFrames = 2;

    bool justFlipped = false;

    
    void SetPos(float x, float y) {
        BarrelPosition.x = x;
        BarrelPosition.y = y;
        BarrelFloorCollider.x = BarrelPosition.x + 8;
        BarrelFloorCollider.y = BarrelPosition.y + BARRELSIZE;
    }

    void UpdateCollider() {
        BarrelFloorCollider.x = BarrelPosition.x + 8;
        BarrelFloorCollider.y = BarrelPosition.y + BARRELSIZE;
    }

    void MoveBarrel() {
        velocityY += BARREL_GRAVITY;
        BarrelPosition.y += velocityY;
        BarrelPosition.x += velocityX;

        if (BarrelPosition.x + BARRELSIZE >= SCREEN_WIDTH || BarrelPosition.x <= 0) {
            FlipDirection();
        }

        UpdateCollider();
    }

    void FlipDirection() {
        if (!justFlipped) {
            velocityX = -velocityX;
            justFlipped = true;
        }
    }

    void BarrelAnimation() {
        ++frameDelayCounter;
        if (frameDelayCounter > frameDelay) {
            frameDelayCounter = 0;
            ++frameIndex;
            frameIndex %= numFrames;
            frameRec.x = barrelFrameWidth * (float)frameIndex;
        }
    }
};

extern Barrel barrel1;
extern Barrel barrel2;
void SpawnBarrel();
void BarrelSpawner();