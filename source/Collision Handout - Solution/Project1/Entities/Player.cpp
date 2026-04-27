#include "Entities/Player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <iostream>
#include "Core/constants.h"
#include "Entities/entity.h"
#include "Scenes/scenes.h"


using namespace std;
Player Mario;
Player Luigi;

/*---Animation Stuff---*/
Rectangle frameRec = { 0.0f, 0.0f, 0.0f, 0.0f };
unsigned  numFrames = 4;
float     marioFrameWidth = 0.0f;
float     marioFrameHeight = 0.0f;
float  frameDelay = 0.5;
unsigned  frameDelayCounter = 0;
unsigned  frameIndex = 0;
unsigned finishFrameDelayCounter = 0;
float HammerTimer = 0.0f;
float HammerTick = 0.0f;
bool LooksRight = true;
/*---Audio Level---*/
Sound jumpSound = { 0 };
Music walkMusic = { 0 };        //music en vez de Sound para que se repita
Sound deathSound = { 0 };
Music climbMusic = { 0 };
Texture2D HammerTexture;
Vector2 HammerPos;
Rectangle HammerSelector = { 0,0,12,16 };

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

    frameRec.y = 1 * Mario.SpriteSize;
    frameRec.width = marioFrameWidth;
    ++frameDelayCounter;
    if (frameDelayCounter > 4)
    {
        frameDelayCounter = 0;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
        {
            ++frameIndex;
            frameIndex %= 2;  
            frameRec.x = marioFrameWidth * (float)frameIndex;
        }
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
    HammerTexture = LoadTexture("Sprites/Hammer.png");


    //Walking Def 3 Para subir y bajar escaleras
    //Walkinf Def 2 para caminar normal
}


void Player::Movement()
{
    if (!isAlive) {
        frameRec.width = abs(frameRec.width);
        frameRec.y = 4 * SpriteSize;

        if (!deathStarted) {
            frameRec.x = 0.0f; // primer frame fijo
            deathStartTimer += GetFrameTime();
            if (deathStartTimer >= 1.0f) {
                deathStarted = true;
                frameIndex = 1; // empieza en el segundo frame, el primero ya lo vimos
                frameDelayCounter = 0;
            }
            return;
        }

        if (deathLoopCount < 2) {
            ++frameDelayCounter;
            if (frameDelayCounter > 6) {
                frameDelayCounter = 0;
                frameRec.x = SpriteSize * (float)frameIndex;
                ++frameIndex %= 4;
                if (frameIndex == 0) ++deathLoopCount;
            }
        } else {
            frameRec.x = 4 * SpriteSize;
        }
        return;
    }
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
    if (IsKeyDown(KEY_RIGHT) && PlayerNum == 1 || IsKeyDown(KEY_D) && PlayerNum == 2)
    {
        marioVelocity.x = (float)Mario.velocity;
        if (frameRec.width < 0 && Mario.getIsGrounded()) frameRec.width = -frameRec.width;
        
        if (Mario.getIsGrounded())
        {
            if (!IsMusicStreamPlaying(walkMusic)) PlayMusicStream(walkMusic);
            UpdateMusicStream(walkMusic);
        }
        LooksRight = true;
    }
    else if (IsKeyDown(KEY_LEFT) && PlayerNum == 1 || IsKeyDown(KEY_A) && PlayerNum == 2)
    {
        marioVelocity.x = -(float)Mario.velocity;
        if (frameRec.width > 0 && Mario.getIsGrounded()) frameRec.width = -frameRec.width;
       
        if (Mario.getIsGrounded())
        {
            if (!IsMusicStreamPlaying(walkMusic)) PlayMusicStream(walkMusic);
            UpdateMusicStream(walkMusic);
        }
        LooksRight = false;
    }
    else
    {
        StopMusicStream(walkMusic); // para cuando sueltas la tecla
    }


    //Hammer time
    if (GetHammerTime()) {
        if (HammerTimer > 11.5f) {
            StopHammerTime();
            HammerTimer = 0.0f;
        }
        else {
            if (!GetIsKilling()) HammerTimer += GetFrameTime();
            if (HammerTick > 0.15f ){
                if(LooksRight){
                    HammerHitbox.x = Position.x + 16;
                    HammerPos.x = Position.x + 16;
                    HammerPos.y = Position.y +2;
                    HammerSelector.x = 12;
                    HammerSelector.width = 12;
                    DrawTextureRec(HammerTexture, HammerSelector, HammerPos, WHITE);
                }
                else {
                    HammerHitbox.x = Position.x - 16;
                    HammerPos.x = Position.x - 16;
                    HammerPos.y = Position.y +1;
                    HammerSelector.width = -12;
                    HammerSelector.x = 12;
                    DrawTextureRec(HammerTexture, HammerSelector, HammerPos, WHITE);
                }
                HammerHitbox.y = Position.y;
                HammerTick += GetFrameTime();
                if (HammerTick > 0.3f) {
                    HammerTick = 0.0f;
                }

            }
            else {
                HammerHitbox.x = Position.x;
                HammerHitbox.y = Position.y - 16;
                HammerPos.x = Position.x;
                HammerHitbox.y = Position.y - 16;
                HammerPos.y = Position.y - 16;
                HammerSelector.x = 0;
                HammerSelector.width = 12;
                DrawTextureRec(HammerTexture, HammerSelector, HammerPos, WHITE);
                HammerTick += GetFrameTime();
            }
        }
    }

    else{

    //Ladder Climb
    if (OnLadder && CanClimb) {
        cout << "ON LADDER" << endl;
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
        if (Mario.justClimbedLadder)
        {
            frameRec.y = 1 * SpriteSize;
            frameRec.width = marioFrameWidth;
            frameRec.x = marioFrameWidth * (float)(4 + Mario.climbFinishFrame);
            ++finishFrameDelayCounter;
            if (finishFrameDelayCounter > 2)
            {
                finishFrameDelayCounter = 0;
                if (Mario.climbFinishFrame >= 2)
                {
                    Mario.justClimbedLadder = false;
                    Mario.climbFinishFrame = 0;
                    frameRec.y = 0.0f;
                }
                else
                {
                    ++Mario.climbFinishFrame;
                }
            }
        }
        else if (marioMoving && !isJumping)
        {
            frameRec.y = 0.0f; 
            ++frameIndex;
            frameIndex %= numFrames;
            frameRec.x = marioFrameWidth * (float)frameIndex;
        }
        else if (isJumping)
        {
            frameRec.y = 0.0f;  
            frameRec.x = 3 * SpriteSize;
        }
        else
        {
            frameRec.y = 0.0f;  
            frameIndex = 0;
            frameRec.x = 0.0f;
        }
    }
}

void DrawMarioCollider() {
    DrawPixel(Mario.FloorCollider.x, Mario.FloorCollider.y, YELLOW);
    DrawPixel(Mario.Position.x, Mario.Position.y, RED);
    if (GetHammerTime()){
    DrawRectangle(Mario.HammerHitbox.x, Mario.HammerHitbox.y, Mario.HammerHitbox.width, Mario.HammerHitbox.height, BROWN);
    }
}

void Player::Unload()
{
    UnloadSound(jumpSound);
    UnloadMusicStream(walkMusic);
    UnloadSound(deathSound);
    UnloadMusicStream(climbMusic);
}

void Player::die() {
    isGrounded = false;
    isJumping = false;
    isAlive = false;
    deathLoopCount = 0;
    frameIndex = 0;
    frameDelayCounter = 0;
    deathStarted = false;
    deathStartTimer = 0.0f;
}