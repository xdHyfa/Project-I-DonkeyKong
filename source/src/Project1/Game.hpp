#pragma once
#include "Player.hpp"
#include "Barrel.hpp"

class Game
{
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	void Update();

private:
	Player player;
	Barrel barrel;
};
