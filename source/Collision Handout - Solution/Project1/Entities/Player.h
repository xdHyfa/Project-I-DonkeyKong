#pragma once
#include "raylib.h"
#include "Entities/entity.h"

class Player : public Entity
{   
public:
    const int playerSize = 16;

    Vector2 marioVelocity = { 0.0f, 0.0f };
 
    bool  isAlive = true;
    bool isJumping = false;
    bool isFalling = false;

    bool justClimbedLadder = false;
    int climbFinishFrame = 0;
    int PlayerNum = 1;

    bool deathStarted = false;
    float deathStartTimer = 0.0f;

    const Rectangle boxCollider = { 0.0f, 0.0f, SpriteSize, SpriteSize };

    const float velocity = 1.5f;
    const int jumpHeight = 6;

    int deathLoopCount = 0;

    Rectangle HammerHitbox = { 0,0, 16,16 };
    

    void die();

    bool tryJump()
    {
        if (isGrounded && !isFalling)
        {
            isGrounded = false;
            return true;
        }
        return false;
    }
    void Movement() override;
    void Setup();
    void Unload();

    void UpdateCollider() {
        FloorCollider.x = Position.x + 8;
        FloorCollider.y = Position.y + 16;
    }
    Player(bool isPlayer1) {
        if (isPlayer1) {
            tag = EntityTag::PLAYER1;
        }
        else
            tag = EntityTag::PLAYER2;
    }
    bool CheckHammerHitbox(Entity& entity) {
        return CheckCollisionRecs(entity.getHitbox(), HammerHitbox);
    }
};

extern Player Mario;
extern Player Luigi;
extern Rectangle frameRec;

void DrawMarioCollider();