#pragma once
#include "raylib.h"

class Lady {
public:
    Texture2D Texture = { 0 };
    Texture2D helpTexture = { 0 };
    Vector2 Position = { 91.0f, 26.0f }; // ajusta hasta que quede bien

    Rectangle frameRec = { 1.0f, 1.0f, 14.0f, 22.0f }; // frame base

    bool isAnimating = false;
    float animTimer = 0.0f;
    float animInterval = 8.0f;  // cada cuánto cambia al frame de animación
    float animDuration = 1.2f;  // cuánto dura el frame de animación

    bool showHelp = false;

    void Setup();
    void Update();
    void Draw();
    void Unload();
};

extern Lady lady;