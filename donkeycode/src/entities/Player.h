#include "raylib.h"
#pragma once

class Mario
{
protected:
    bool  isAlive = true;
    bool  isGrounded = true;
    float x = 0.0f;
    float y = 0.0f;
public:
    const float VELOCITY = 1.5f;
    const int JUMP = 8;
    const int MARIO_SIZE = 16;

    float getPosX()       const { return x; }
    float getPosY()       const { return y; }
    bool  getIsGrounded() const { return isGrounded; }

    void setPosX(float newX) { x = newX; }
    void setPosY(float newY) { y = newY; }
    void setGrounded(bool g) { isGrounded = g; }
    void die() { isAlive = false; }
    void moveLeft() { x -= VELOCITY; }
    void moveRight() { x += VELOCITY; }

    bool tryJump()
    {
        if (isGrounded)
        {
            isGrounded = false;
            return true;
        }
        return false;
    }
};

extern Texture2D marioTexture;
extern Vector2 marioPosition;
extern Rectangle frameRec;
extern Vector2 marioVelocity;
extern Mario entityMario;
extern bool isJumping;
extern Vector2 marioFloorCollider;

void Mario_Movement();
void Setup();