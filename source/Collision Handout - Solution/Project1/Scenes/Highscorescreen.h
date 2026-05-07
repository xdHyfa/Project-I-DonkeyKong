#pragma once

void runHighScoreScreen();

// Llamar desde UI.cpp (EndGameOver) para indicar que se viene de una partida real.
// Si NO se llama, la pantalla solo muestra la tabla sin pedir nombre.
void HS_SetJustPlayed(bool v);