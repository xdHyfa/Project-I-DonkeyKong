#pragma once
#include "raylib.h"
#include <iostream>



class Entity
{
protected:

    bool  isGrounded = false;
   

public:
    Texture2D Texture;
    Vector2 Position = { 0.0f, 0.0f };
    Vector2 FloorCollider = { 0.0f, 0.0f };
    float SpriteSize = 16.0f;

    bool  getIsGrounded() const { return isGrounded; }
    void setGrounded(bool g) { isGrounded = g; }

};


