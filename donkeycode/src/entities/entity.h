#pragma once
#include "raylib.h"
#include <iostream>

#define SPRITESIZE 16

class Entity
{
protected:
    float x = 0.0f;
    float y = 0.0f;
    bool  isGrounded = false;

public:
    Entity() :x(0), y(0), isGrounded(false) {}
    Entity(float posX, float posY, bool isOnTheGround) : x(posX), y(posY), isGrounded(isOnTheGround) {}
    ~Entity() { std::cout << "Entity destructor" << std::endl; }

    float getPosX()       const { return x; }
    float getPosY()       const { return y; }
    bool  getIsGrounded() const { return isGrounded; }

    void setPosX(float newX) { x = newX; }
    void setPosY(float newY) { y = newY; }
    void setGrounded(bool g) { isGrounded = g; }

};


class Player : public Entity
{
protected:
    bool  isAlive = true;
    
public:
    Player() : Entity(0.0f, 0.0f, false), isAlive(true) {}
    Player(float posX, float posY) : Entity(posX, posY, false), isAlive(true) {}

    const Rectangle boxCollider = { 0.0f, 0.0f, SPRITESIZE, SPRITESIZE };

    const float velocity = 1.5f;
    const int jumpHeight = 6;
    const int playerSize = 16;

    void die() { isAlive = false; }
    void moveLeft() { x -= velocity; }
    void moveRight() { x += velocity; }

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
    extern Player entityMario;
    extern bool isJumping;
    extern bool isFalling;
    extern Vector2 marioFloorCollider;

    void Mario_Movement();
    void Setup();