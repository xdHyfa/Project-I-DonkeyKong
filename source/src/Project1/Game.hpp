#pragma once
#include "Player.hpp"
#include "Barrel.hpp"
#include <vector>
using namespace std;

class Game
{
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	void Update();

	void CheckForCollisions();

private:
	Player player;
	Barrel barrel;
};
