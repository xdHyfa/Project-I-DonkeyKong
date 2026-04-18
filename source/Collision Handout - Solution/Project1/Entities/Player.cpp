#include "Entities/Player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "Core/constants.h"
#include "Entities/entity.h"


using namespace std;
Player Mario;

/*---Animation Stuff---*/
Rectangle frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };
unsigned  numFrames = 4;
float     marioFrameWidth = 0.0f;
float     marioFrameHeight = 0.0f;
float  frameDelay = 0.5;
unsigned  frameDelayCounter = 0;
unsigned  frameIndex = 0;


/*---Audio Level---*/
Sound jumpSound = { 0 };
Music walkMusic = { 0 };        //music en vez de Sound para que se repita
Sound deathSound = { 0 };
Music climbMusic = { 0 };


const float GROUND_Y = SCREEN_HEIGHT - 16.0f;
float       lockedVelocityX = 0.0f;

bool isTextureValid(const Texture2D& texture)
{
    return texture.id > 0;
}

void MarioLadderMovement() {
    if (IsKeyDown(KEY_UP)) {
        Mario.OnLadder = true;
        Mario.Position.y -= 1.0f;
        Mario.UpdateCollider();
    }
    else if (IsKeyDown(KEY_DOWN)) {
        Mario.OnLadder = true;
        Mario.Position.y += 1.0f;
        Mario.UpdateCollider();
    }
    return;
}

void Player::Setup()
{
    SpriteSize = (float)playerSize;
    marioFrameWidth = (float)Mario.SpriteSize;
    marioFrameHeight = (float)Mario.SpriteSize;
    frameRec = { 0.0f, 0.0f, marioFrameWidth, marioFrameHeight };
    Position = { 64, SCREEN_HEIGHT - (float)Mario.SpriteSize - 17 };
    jumpSound = LoadSound("Audio/Game-Startup.wav"); 
    walkMusic = LoadMusicStream("Audio/WalkingDef2.wav");
    walkMusic.looping = true;
    climbMusic = LoadMusicStream("Audio/WalkingDef3.wav");
    climbMusic.looping = true;
    deathSound = LoadSound("Audio/Dead.wav");



    //Walking Def 3 Para subir y bajar escaleras
    //Walkinf Def 2 para caminar normal
}


void Player::Movement()
{
    if (!isAlive) return;
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
    if (OnLadder&& CanClimb){
        cout << "ON LADDER" <<endl;
        MarioLadderMovement();

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
        {
            if (!IsMusicStreamPlaying(climbMusic)) PlayMusicStream(climbMusic);
            UpdateMusicStream(climbMusic);
        }
        else
        {
            StopMusicStream(climbMusic);
        }

        return;
    }
    if (CanClimb) {
        if (IsKeyPressed(KEY_UP)) {
            Mario.OnLadder = true;
            Mario.Position.y -= 1.0f;
            Mario.UpdateCollider();


        }
        else if (IsKeyPressed(KEY_DOWN)) {
            Mario.OnLadder = true;
            Mario.Position.y += 1.0f;
            Mario.UpdateCollider();
        }
    }
    // --- Input horizontal (siempre se lee, en suelo y aire) ---
    marioVelocity.x = 0.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        marioVelocity.x = (float)Mario.velocity;
        if (frameRec.width < 0 && Mario.getIsGrounded()) frameRec.width = -frameRec.width;
        
        if (Mario.getIsGrounded())
        {
            if (!IsMusicStreamPlaying(walkMusic)) PlayMusicStream(walkMusic);
            UpdateMusicStream(walkMusic);
        }
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        marioVelocity.x = -(float)Mario.velocity;
        if (frameRec.width > 0 && Mario.getIsGrounded()) frameRec.width = -frameRec.width;
       
        if (Mario.getIsGrounded())
        {
            if (!IsMusicStreamPlaying(walkMusic)) PlayMusicStream(walkMusic);
            UpdateMusicStream(walkMusic);
        }
    }
    else
    {
        StopMusicStream(walkMusic); // para cuando sueltas la tecla
    }

    // --- Salto: captura la velocidad X DESPUÉS de leer input -s--
    if (IsKeyPressed(KEY_SPACE))
    {
        if (Mario.tryJump())
        {
            PlaySound(jumpSound);
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
        /*if (!isAlive)
        {
            frameRec.width = 16;
            frameRec.y = 3 * 16;

            if (deathLoopCount < 2)
            {
                ++frameIndex %= numFrames;
                frameRec.x = 16 * (float)frameIndex;
                if (frameIndex == 0) ++deathLoopCount;
            }
            else
            {
                frameRec.x = 5 * 16;
            }
        }*/
        
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

void DrawMarioCollider() {
    DrawPixel(Mario.FloorCollider.x, Mario.FloorCollider.y, YELLOW);
    DrawPixel(Mario.Position.x, Mario.Position.y, RED);
}

void Player::Unload()
{
    UnloadSound(jumpSound);
    UnloadMusicStream(walkMusic);
    UnloadSound(deathSound);
    UnloadMusicStream(climbMusic);
}

void Player::die() {
    PlaySound(deathSound);
    isGrounded = false;
    isJumping = false;
    isAlive = false;
}