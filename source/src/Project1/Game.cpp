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

    for (int i = 0; i<1; i++)
    {
        barrel.Draw();
    }
}

void Game::HandleInput()
{
    if (IsKeyDown(KEY_LEFT))
        player.MoveLeft();

    if (IsKeyDown(KEY_RIGHT))
        player.MoveRight();

    if (IsKeyPressed(KEY_SPACE))
        player.Jump();

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
        player.ClimbLadder();
}

void Game::Update()
{
	player.Gravity();
    player.UpdateAnimation();

    for (int i = 0; i < 1; i++)
    {
        barrel.Movement();
        barrel.UpdateAnimation();
    }
}

void Game::CheckForCollisions()
{
    for (int i = 0; i < 1; i++) 
    {
        if (CheckCollisionRecs(player.GetHitbox(), barrel.GetHitbox())) 
        {
            player.Die();
        }
    }
}

