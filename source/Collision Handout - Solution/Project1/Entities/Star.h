#pragma once
#include "Entities/entity.h"
#include "Entities/Player.h"

// Duration of the star power-up in seconds
constexpr float STAR_DURATION = 10.0f;
// Blink interval in seconds (half-period, so full blink = 2x this)
constexpr float STAR_BLINK_INTERVAL = 0.5f;

void StartStarTime(int playerNum);
void StopStarTime(int playerNum);
bool GetStarTime(int playerNum = 1);

// Call once per frame for the active player — handles timer, blink state, and kill check
void UpdateStar(Player& player);

// Returns true if the star hitbox kills this entity (use in game loop)
bool IsStarKill(Player& player, Entity& entity);