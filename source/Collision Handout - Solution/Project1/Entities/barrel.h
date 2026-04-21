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
    bool isLaddering = false;
    float DownZoneTimer = 0.0f;
    float groundCooldown = 0.0f;
    float ladderFrameTimer = 0.0f;
    bool ladderFrameToggle = false;
    bool Unladder_anim = false;
    
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
        if (isLaddering) {
            Position.y += 2.0f;
        }
        else{
            velocityY += BARREL_GRAVITY;
            Position.y += velocityY;
            Position.x += velocityX;

            if (Position.x + BARRELSIZE >= SCREEN_WIDTH || Position.x <= 0) {
                FlipDirection();
            }
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
        
        if (isLaddering) {
            Unladder_anim = false;
            ladderFrameTimer += GetFrameTime();
            if (ladderFrameTimer >= 0.05f) {
                ladderFrameTimer = 0.0f;
                ladderFrameToggle = !ladderFrameToggle;
            }
            if (ladderFrameToggle) frameRec = { 88, 0, 18, 12 };
            else frameRec = { 72, 0, 16, 12 };
        }
        else{
            if (!Unladder_anim){
                frameRec = { 0.0f, 0.0f, BARRELSIZE + 3, BARRELSIZE + 3 };
                Unladder_anim = true;
            }
            if (frameDelayCounter > frameDelay) {
                frameDelayCounter = 0;

                if (velocityX > 0) {

                frameIndex = (frameIndex + 3) % 4;
                }
                
                else 
                {

                frameIndex = (frameIndex + 1) % 4;
                }

            frameRec.x = barrelFrameWidth * (float)frameIndex;
        
            }
        
        }
    }
};

extern Barrel barrel1;
extern Barrel barrel2;
void SpawnBarrel();
void BarrelRoutine();