#pragma once
#include "raylib.h"

class Donkey {
public:
    Texture2D Texture = { 0 };
    Vector2 Position = { 25.0f, 47.0f }; // ajusta a donde quieres a DK
    Rectangle frameRec = { 0.0f, 0.0f, 41.0f, 32.0f };

    unsigned int frameIndex = 0;
    float frameTimer = 0.0f;
    float frameInterval = 0.4f; // velocidad de la animación
    bool isThrowing = false;

    void Setup();
    void Update();
    void Draw();
    void Unload();
    void PlayThrowAnim();
};

extern Donkey donkey;