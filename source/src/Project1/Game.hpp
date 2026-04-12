#pragma once
#include "Player.hpp"

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
};
