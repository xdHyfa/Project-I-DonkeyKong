#pragma once
#include "raylib.h"
#include "Core/constants.h"
#include "Entities/entity.h"
#pragma once
#define NORMAL 0
#define RANDOM 1
#define BARRELSIZE 14
#define BARRELVELOCITY 2.2f
#define BARREL_GRAVITY 0.3f

class Barrel : public Entity {
public:
    bool has_Spawned = false;
    
    float velocityX = BARRELVELOCITY;   
    float velocityY = 0.0f;

    Rectangle frameRec = { 0.0f, 0.0f, BARRELSIZE+3, BARRELSIZE+ 3};
    float     frameDelay = 1.0f;
    unsigned  frameDelayCounter = 0;
    unsigned  frameIndex = 0;
    float     barrelFrameWidth = 0.0f;
    unsigned  numFrames = 4;

    bool justFlipped = false;

    
    void SetPos(float x, float y) {
        Position.x = x;
        Position.y = y;
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + BARRELSIZE;
    }

    void UpdateCollider() {
        hitbox = { Position.x, Position.y };
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + BARRELSIZE;
    }

    void Movement() override{
        velocityY += BARREL_GRAVITY;
        Position.y += velocityY;
        Position.x += velocityX;

        if (Position.x + BARRELSIZE >= SCREEN_WIDTH || Position.x <= 0) {
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

            if (velocityX > 0) {
                
                frameIndex = (frameIndex + 3) % 4;
            } else {
                
                frameIndex = (frameIndex + 1) % 4;
            }

            frameRec.x = barrelFrameWidth * (float)frameIndex;
        }
    }
};

extern Barrel barrel1;
extern Barrel barrel2;
void SpawnBarrel();
void BarrelRoutine();