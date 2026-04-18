#pragma once
#include "raylib.h"
#include "Entities/entity.h"

class Player : public Entity
{   
public:
    const int playerSize = 16;

    Vector2 marioVelocity = { 0.0f, 0.0f };
 
    bool  isAlive = true;
    bool isJumping = false;
    bool isFalling = false;

    bool justClimbedLadder = false;
    int climbFinishFrame = 0;

    const Rectangle boxCollider = { 0.0f, 0.0f, SpriteSize, SpriteSize };

    const float velocity = 1.5f;
    const int jumpHeight = 6;

    int deathLoopCount = 0;
    

    void die() { 
        
        isGrounded = false;
        isJumping = false;
        isAlive = false; }

    bool tryJump()
    {
        if (isGrounded)
        {
            isGrounded = false;
            return true;
        }
        return false;
    }
    void Movement() override;
    void Setup();

    void UpdateCollider() {
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
    }
    Player() {
        tag = EntityTag::PLAYER;
    }
};

extern Player Mario;
extern Rectangle frameRec;

void DrawMarioCollider();