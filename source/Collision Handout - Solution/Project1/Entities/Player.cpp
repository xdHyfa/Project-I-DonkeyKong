#include "Entities/Player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "Core/constants.h"
#include "Entities/entity.h"

Player Mario;

/*---Animation Stuff---*/
Rectangle frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };
unsigned  numFrames = 4;
float     marioFrameWidth = 0.0f;
float     marioFrameHeight = 0.0f;
float  frameDelay = 0.5;
unsigned  frameDelayCounter = 0;
unsigned  frameIndex = 0;


const float GROUND_Y = SCREEN_HEIGHT - 16.0f;
float       lockedVelocityX = 0.0f;

bool isTextureValid(const Texture2D& texture)
{
    return texture.id > 0;
}

void Player::Setup()
{
    SpriteSize = (float)playerSize;
    marioFrameWidth = (float)Mario.SpriteSize;
    marioFrameHeight = (float)Mario.SpriteSize;
    frameRec = { 0.0f, 0.0f, marioFrameWidth, marioFrameHeight };
    Position = { 64, SCREEN_HEIGHT - (float)Mario.SpriteSize - 17 };
}

void Player::Movement()
{
  
    //UNCOMMENT TO CHECK IF TEXTURE WORKS
    
    /*  if (!isTextureValid(Texture))
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("ERROR: No se pudo cargar la textura de Mario.", 20, 20, 20, RED);
            EndDrawing();
        }
        return;
    }*/


    // --- Input horizontal (siempre se lee, en suelo y aire) ---
    marioVelocity.x = 0.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        marioVelocity.x = (float)Mario.velocity;
        if (frameRec.width < 0 && Mario.getIsGrounded()) frameRec.width = -frameRec.width;
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        marioVelocity.x = -(float)Mario.velocity;
        if (frameRec.width > 0 && Mario.getIsGrounded()) frameRec.width = -frameRec.width;
    }

    // --- Salto: captura la velocidad X DESPUÉS de leer input -s--
    if (IsKeyPressed(KEY_SPACE))
    {
        if (Mario.tryJump())
        {
            lockedVelocityX = marioVelocity.x; // ahora sí tiene la dirección correcta
            isJumping = true;
            marioVelocity.y = -(float)Mario.jumpHeight;
        }
    }

    // --- En el aire: congelar velocidad horizontal ---
    if (isJumping)
    {
        marioVelocity.x = lockedVelocityX;
    }

    // --- Física: gravedad ---
    if (!Mario.getIsGrounded() || isFalling)
    {
        marioVelocity.y += GRAVITY;
    }

    // --- Aplicar velocidad ---
    Position = Vector2Add(Position, marioVelocity);

    // --- Detección de suelo ---
    if (Position.y >= GROUND_Y)
    {
        Position.y = GROUND_Y;
        marioVelocity.y = 0.0f;
        // FIX: NO reseteamos x aquí, para que la animación funcione correctamente
        lockedVelocityX = 0.0f;
        isJumping = false;
        Mario.setGrounded(true);
    }
    // --- Detección de paredes ---
    if (Position.x > 208) {
        Position.x = 208;
    }
    if (Position.x < 0) {
        Position.x = 0;
    }

    if (IsKeyPressed(KEY_R)) {
        Position.y -= 32;
    }
    if (IsKeyPressed(KEY_Q)) {
        Position.y += 32;
    }

    // --- Sincronizar entidad ---
    FloorCollider.x = Position.x + 8;
    FloorCollider.y = Position.y + 16;
    // --- Animación ---
    bool marioMoving = marioVelocity.x != 0.0f || marioVelocity.y != 0.0f;
    
    //Check for Animation Check DrawRectangle(Mario.Position.x, Mario.Position.y, (int)SpriteSize, (int)SpriteSize, WHITE);
    
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
            frameRec.x = 3 * SpriteSize;
        }
        else
        {
            frameIndex = 0;
            frameRec.x = 0.0f;
        }
    }
}