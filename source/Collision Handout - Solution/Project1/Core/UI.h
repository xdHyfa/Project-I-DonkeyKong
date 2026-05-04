#pragma once

void AddPoints(int points);

void PrintUI();

void RemoveLife();

void CheckLives();

void AddLevel();

void PrintBonus();

void UpdateBonus();

void AddBonus();

void ResetBonus();

void SetCooldown();

void ShowScorePopup(Vector2 pos, int points);
void UpdateDrawScorePopup();

void TriggerGameOver();
bool IsGameOver();
void TickGameOver();
float GetGameOverTimer();
void EndGameOver();
void UpdateDrawGameOver();

int GetLevel();