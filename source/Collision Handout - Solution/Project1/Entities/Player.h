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

    const Rectangle boxCollider = { 0.0f, 0.0f, SpriteSize, SpriteSize };

    const float velocity = 1.5f;
    const int jumpHeight = 6;
    

    void die() { isAlive = false; }

    bool tryJump()
    {
        if (isGrounded)
        {
            isGrounded = false;
            return true;
        }
        return false;
    }
    void Movement();
    void Setup();
};

extern Player Mario;
extern Rectangle frameRec;