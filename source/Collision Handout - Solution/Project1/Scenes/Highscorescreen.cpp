#define _CRT_SECURE_NO_WARNINGS

#include "raylib.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "Scenes/scenes.h"
#include "Core/UI.h"
#include <stdio.h>


#define MAX_SCORES 5
#define SAVE_FILE  "scores.dat"

struct ScoreEntry {
    int score;
    int level;
};

static ScoreEntry scores[MAX_SCORES];

static void LoadScores() {
    for (int i = 0; i < MAX_SCORES; i++) {
        scores[i] = { 0, 0 };
    }
    FILE* f = fopen(SAVE_FILE, "rb");
    if (!f) return;
    fread(scores, sizeof(ScoreEntry), MAX_SCORES, f);
    fclose(f);
}

static void SaveScores() {
    FILE* f = fopen(SAVE_FILE, "wb");
    if (!f) return;
    fwrite(scores, sizeof(ScoreEntry), MAX_SCORES, f);
    fclose(f);
}

static void InsertAndSort(int newScore, int newLevel) {
    scores[MAX_SCORES - 1] = { newScore, newLevel };
    // Ordenar de mayor a menor
    for (int i = 0; i < MAX_SCORES - 1; i++) {
        for (int j = i + 1; j < MAX_SCORES; j++) {
            if (scores[j].score > scores[i].score) {
                ScoreEntry tmp = scores[i];
                scores[i] = scores[j];
                scores[j] = tmp;
            }
        }
    }
    SaveScores();
}

// --- Estado de la escena ---
static Font  hsFont = { 0 };
static float hsTimer = 0.0f;
static float blinkTimer = 0.0f;
static bool  blinkOn = true;
static int   playerRank = -1;

void runHighScoreScreen() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        hsFont = LoadFont("Fonts/donkey-kong-arcade-1981.otf");
        hsTimer = 0.0f;
        blinkTimer = 0.0f;
        blinkOn = true;

        int currentScore = GetScore();
        int currentLevel = GetLevel();

        LoadScores();
        InsertAndSort(currentScore, currentLevel);

        // Encontrar la fila del jugador para resaltarla
        playerRank = -1;
        for (int i = 0; i < MAX_SCORES; i++) {
            if (scores[i].score == currentScore && scores[i].level == currentLevel) {
                playerRank = i;
                break;
            }
        }

        // Resetear estado de la partida ahora que ya leimos los valores
        ResetAfterGameOver();

        Scene_Init = true;
    }

    hsTimer += GetFrameTime();
    blinkTimer += GetFrameTime();
    if (blinkTimer >= 0.5f) {
        blinkOn = !blinkOn;
        blinkTimer = 0.0f;
    }

    // --- DIBUJO ---
    const float cx = SCREEN_WIDTH / 2.0f;

    // Titulo
    const char* title = "HIGH SCORES";
    Vector2 titleSize = MeasureTextEx(hsFont, title, 12, 0.5f);
    DrawTextEx(hsFont, title,
        { cx - titleSize.x / 2.0f, 20 }, 12, 0.5f, RED);

    // Cabecera columnas
    DrawTextEx(hsFont, "RANK", { 14, 45 }, 8, 0.5f, YELLOW);
    DrawTextEx(hsFont, "SCORE", { 70, 45 }, 8, 0.5f, YELLOW);
    DrawTextEx(hsFont, "LV", { 165, 45 }, 8, 0.5f, YELLOW);
    DrawLine(10, 56, SCREEN_WIDTH - 10, 56, YELLOW);

    // Entradas
    for (int i = 0; i < MAX_SCORES; i++) {
        float y = 64.0f + i * 18.0f;
        Color rowCol = (i == playerRank) ? SKYBLUE : WHITE;

        DrawTextEx(hsFont, TextFormat("#%d", i + 1), { 18, y }, 8, 0.5f, rowCol);

        if (scores[i].score > 0) {
            DrawTextEx(hsFont, TextFormat("%06d", scores[i].score), { 60, y }, 8, 0.5f, rowCol);
            DrawTextEx(hsFont, TextFormat("%02d", scores[i].level), { 166, y }, 8, 0.5f, rowCol);
        }
        else {
            DrawTextEx(hsFont, "------", { 60, y }, 8, 0.5f, DARKGRAY);
            DrawTextEx(hsFont, "--", { 166, y }, 8, 0.5f, DARKGRAY);
        }
    }

    // Parpadeo "PRESS START"
    if (blinkOn) {
        const char* ps = "PRESS START";
        Vector2     psSize = MeasureTextEx(hsFont, ps, 8, 0.5f);
        DrawTextEx(hsFont, ps,
            { cx - psSize.x / 2.0f, 220 }, 8, 0.5f, WHITE);
    }

    // Avanzar a TITLE: input o timeout de 15s
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || hsTimer >= 15.0f) {
        UnloadFont(hsFont);
        hsFont = { 0 };
        Scene_Init = false;
        hsTimer = 0.0f;
        ChangeScene();   // HIGHSCORE -> TITLE
    }
}