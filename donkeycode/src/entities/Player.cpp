#include "entities/Player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "core/constants.h"

const float GRAVITY = 0.9f;



Mario     entityMario;
Texture2D marioTexture;
unsigned  numFrames = 4;
float     marioFrameWidth = 0.0f;
float     marioFrameHeight = 0.0f;
Rectangle frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };
Vector2   marioPosition = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
Vector2   marioVelocity = { 0.0f, 0.0f };
float  frameDelay = 0.5;
unsigned  frameDelayCounter = 0;
unsigned  frameIndex = 0;
Vector2 marioFloorCollider;

const float GROUND_Y = SCREEN_HEIGHT - 16.0f;
float       lockedVelocityX = 0.0f;
bool        isJumping = false;

bool isTextureValid(const Texture2D& texture)
{
    return texture.id > 0;
}

void Setup()
{
    marioFrameWidth = (float)entityMario.MARIO_SIZE;
    marioFrameHeight = (float)entityMario.MARIO_SIZE;
    frameRec = { 0.0f, 0.0f, marioFrameWidth, marioFrameHeight };
    entityMario.setPosX(marioPosition.x);
    entityMario.setPosY(marioPosition.y);
}

void Mario_Movement()
{
    if (!isTextureValid(marioTexture))
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("ERROR: No se pudo cargar la textura de Mario.", 20, 20, 20, RED);
            EndDrawing();
        }
        return;
    }

    // --- Input horizontal (siempre se lee, en suelo y aire) ---
    marioVelocity.x = 0.0f;
    if (IsKeyDown(KEY_RIGHT))
    {
        marioVelocity.x = (float)entityMario.VELOCITY;
        if (frameRec.width < 0 && entityMario.getIsGrounded()) frameRec.width = -frameRec.width;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        marioVelocity.x = -(float)entityMario.VELOCITY;
        if (frameRec.width > 0 && entityMario.getIsGrounded()) frameRec.width = -frameRec.width;
    }

    // --- Salto: captura la velocidad X DESPUÉS de leer input ---
    if (IsKeyPressed(KEY_UP))
    {
        if (entityMario.tryJump())
        {
            lockedVelocityX = marioVelocity.x; // ahora sí tiene la dirección correcta
            isJumping = true;
            marioVelocity.y = -(float)entityMario.JUMP;
        }
    }

    // --- En el aire: congelar velocidad horizontal ---
    if (isJumping)
    {
        marioVelocity.x = lockedVelocityX;
    }

    // --- Física: gravedad ---
    if (!entityMario.getIsGrounded())
    {
        marioVelocity.y += GRAVITY;
    }

    // --- Aplicar velocidad ---
    marioPosition = Vector2Add(marioPosition, marioVelocity);

    // --- Detección de suelo ---
    if (marioPosition.y >= GROUND_Y)
    {
        marioPosition.y = GROUND_Y;
        marioVelocity.y = 0.0f;
        // FIX: NO reseteamos x aquí, para que la animación funcione correctamente
        lockedVelocityX = 0.0f;
        isJumping = false;
        entityMario.setGrounded(true);
    }
    // --- Detección de paredes ---
    if (marioPosition.x > 208) {
        marioPosition.x = 208;
    }
    if (marioPosition.x < 0) {
        marioPosition.x = 0;
    }

    // --- Sincronizar entidad ---
    entityMario.setPosX(marioPosition.x);
    entityMario.setPosY(marioPosition.y);
    marioFloorCollider.x = marioPosition.x + 8;
    marioFloorCollider.y = marioPosition.y + 16;
    // --- Animación ---
    bool marioMoving = marioVelocity.x != 0.0f || marioVelocity.y != 0.0f;

    ++frameDelayCounter;
    if (frameDelayCounter > frameDelay)
    {
        frameDelayCounter = 0;
        if (marioMoving && !isJumping)
        {
            ++frameIndex;
            frameIndex %= numFrames;
            frameRec.x = marioFrameWidth * (float)frameIndex;
        }
        else if (isJumping)
        {
            frameRec.x = 3 * entityMario.MARIO_SIZE;
        }
        else
        {
            frameIndex = 0;
            frameRec.x = 0.0f;
        }
    }
}