#include "Entities/Star.h"
#include "Entities/EntityCollision.hpp"
#include "Scenes/scenes.h"   // for any score/UI calls if needed
#include "raylib.h"

// ---------------------------------------------------------------------------
// Star state mirrors Hammer state in scenes.cpp (StartHammerTime pattern)
// ---------------------------------------------------------------------------

void StartStarTime(int playerNum)
{
    Player& p = (playerNum == 2) ? Luigi : Mario;
    p.StarActive = true;
    p.StarTimer = 0.0f;
    p.StarBlink = false;
    p.StarBlinkTimer = 0.0f;
}

void StopStarTime(int playerNum)
{
    Player& p = (playerNum == 2) ? Luigi : Mario;
    p.StarActive = false;
    p.StarTimer = 0.0f;
    p.StarBlink = false;
    p.StarBlinkTimer = 0.0f;
}

bool GetStarTime(int playerNum)
{
    Player& p = (playerNum == 2) ? Luigi : Mario;
    return p.StarActive;
}

// ---------------------------------------------------------------------------
// Call once per frame for each player (in game loop, same place as hammer)
// ---------------------------------------------------------------------------
void UpdateStar(Player& player)
{
    if (!player.StarActive) return;

    player.StarTimer += GetFrameTime();

    // Blink ticker
    player.StarBlinkTimer += GetFrameTime();
    if (player.StarBlinkTimer >= STAR_BLINK_INTERVAL)
    {
        player.StarBlinkTimer = 0.0f;
        player.StarBlink = !player.StarBlink;
    }

    // Expire
    if (player.StarTimer >= STAR_DURATION)
    {
        StopStarTime(player.PlayerNum);
    }
}

// ---------------------------------------------------------------------------
// Returns true if star-Mario's hitbox overlaps entity — call in game loop
// and kill the entity when it returns true
// ---------------------------------------------------------------------------
bool IsStarKill(Player& player, Entity& entity)
{
    if (!player.StarActive) return false;
    return EntityCollision(player, entity);
}