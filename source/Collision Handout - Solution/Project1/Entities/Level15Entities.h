#pragma once
#include "Entities/Level1Entities.h"
#include "Entities/Player.h"

void SetStartTime15();

void Level15EntitiesRoutine();

void UnloadLevel15Entities();

void ResetLevel15Entities();

bool GoombaKilledPlayer(int playerNum);

bool BillBalaKilledPlayer(int playerNum);

bool WinStarCollected15();

bool CheckHammerKills(Player& player);