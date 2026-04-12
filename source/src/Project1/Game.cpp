#include "Game.hpp"


Game::Game() 
{

}

Game::~Game()
{

}

void Game::Draw()
{
	player.Draw();
}

void Game::HandleInput()
{
	if (IsKeyDown(KEY_LEFT)) {
		player.MoveLeft();
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		player.MoveRight();
	}
	else if (IsKeyDown(KEY_SPACE)) {
		player.Jump();
	}
	else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) {
		player.ClimbLadder();
	}
	else return;
}

void Game::Update()
{
	player.Gravity();
}