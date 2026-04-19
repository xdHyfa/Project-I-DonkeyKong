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

    bool isIdle = true;
    float idleTimer = 0.0f;
    float idleInterval = 1.0f;  // tiempo en reposo entre lanzamientos

    bool isThrowing = false;
    bool throwReady = false; // true cuando la animación termina
    bool spawnBarrel = false;
    void Setup();
    void Update();
    void Draw();
    void Unload();
    void PlayThrowAnim();
};

extern Donkey donkey;