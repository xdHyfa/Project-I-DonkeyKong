#include "entities/Player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "core/constants.h"

const float GRAVITY = 1.0f;

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

Mario     entityMario;
Texture2D marioTexture;
unsigned  numFrames = 4;
float     marioFrameWidth = 0.0f;
float     marioFrameHeight = 0.0f;
Rectangle frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };
Vector2   marioPosition = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
Vector2   marioVelocity = { 0.0f, 0.0f };
unsigned  frameDelay = 5;
unsigned  frameDelayCounter = 0;
unsigned  frameIndex = 0;

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

    // --- Sincronizar entidad ---
    entityMario.setPosX(marioPosition.x);
    entityMario.setPosY(marioPosition.y);

    // --- Animación ---
    bool marioMoving = marioVelocity.x != 0.0f || marioVelocity.y != 0.0f;

    ++frameDelayCounter;
    if (frameDelayCounter > frameDelay)
    {
        frameDelayCounter = 0;
        if (marioMoving)
        {
            ++frameIndex;
            frameIndex %= numFrames;
            frameRec.x = marioFrameWidth * (float)frameIndex;
        }
        else
        {
            frameIndex = 0;
            frameRec.x = 0.0f;
        }
    }
}