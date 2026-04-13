#pragma once
#include "raylib.h"
#include <iostream>

enum class EntityTag { FIRE, BARREL, PLAYER, DONKO };

class Entity
{
protected:

    bool  isGrounded = false;
   

public:
    EntityTag tag;
    Texture2D Texture;
    Vector2 Position = { 0.0f, 0.0f };
    Vector2 FloorCollider = { 0.0f, 0.0f };
    float SpriteSize = 16.0f;
    bool OnLadder = false;
    bool CanClimb = false;

    bool  getIsGrounded() const { return isGrounded; }
    void setGrounded(bool g) { isGrounded = g; }
    virtual void Movement() = 0;
};


