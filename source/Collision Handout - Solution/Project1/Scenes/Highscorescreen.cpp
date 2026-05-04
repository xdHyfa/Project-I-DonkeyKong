#define _CRT_SECURE_NO_WARNINGS

#include "raylib.h"
#include "include/resource_dir.h"
#include "Core/constants.h"
#include "Scenes/scenes.h"
#include "Core/UI.h"
#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------
//  Constantes
// ---------------------------------------------------------------
#define MAX_SCORES  5
#define SAVE_FILE   "scores.dat"
#define FILE_MAGIC  0xD4201u          // cambia si cambias la struct

// ---------------------------------------------------------------
//  Estructura de entrada (con nombre de 3 letras)
// ---------------------------------------------------------------
struct ScoreEntry {
    int  score;
    int  level;
    char name[4];   // 3 chars + '\0'
};

static ScoreEntry scores[MAX_SCORES];

// ---------------------------------------------------------------
//  Persistencia
// ---------------------------------------------------------------
static void LoadScores() {
    for (int i = 0; i < MAX_SCORES; i++) {
        scores[i] = { 0, 0, "---" };
    }
    FILE* f = fopen(SAVE_FILE, "rb");
    if (!f) return;

    // Cabecera mágica para detectar formato viejo
    unsigned int magic = 0;
    fread(&magic, sizeof(magic), 1, f);
    if (magic != FILE_MAGIC) {
        fclose(f);
        return; // archivo viejo / corrupto ? empezar vacío
    }
    fread(scores, sizeof(ScoreEntry), MAX_SCORES, f);
    fclose(f);
}

static void SaveScores() {
    FILE* f = fopen(SAVE_FILE, "wb");
    if (!f) return;
    unsigned int magic = FILE_MAGIC;
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(scores, sizeof(ScoreEntry), MAX_SCORES, f);
    fclose(f);
}

// Inserta newEntry y reordena de mayor a menor puntuación.
// Devuelve el índice final de la entrada insertada (-1 si no entró).
static int InsertAndSort(ScoreEntry newEntry) {
    // ¿Supera al mínimo actual?
    // Buscamos el último slot con puntuación menor o igual para reemplazarlo
    int replaceIdx = -1;
    int minScore = newEntry.score; // solo entra si mejora algo
    for (int i = MAX_SCORES - 1; i >= 0; i--) {
        if (scores[i].score <= minScore) {
            replaceIdx = i;
            break;
        }
    }
    if (replaceIdx == -1) return -1; // no entró en el top

    scores[replaceIdx] = newEntry;

    // Bubble-sort descendente
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

    // Devuelve la posición final de la entrada que acaba de entrar
    for (int i = 0; i < MAX_SCORES; i++) {
        if (scores[i].score == newEntry.score &&
            strcmp(scores[i].name, newEntry.name) == 0)
            return i;
    }
    return -1;
}

// ---------------------------------------------------------------
//  Estado de la escena
// ---------------------------------------------------------------
static Font  hsFont = { 0 };
static float hsTimer = 0.0f;
static float blinkTimer = 0.0f;
static bool  blinkOn = true;
static int   playerRank = -1;

// Fases internas
typedef enum HsPhase { HS_NAME_ENTRY, HS_SHOW_TABLE } HsPhase;
static HsPhase hsPhase;

// Entrada de nombre
static char  nameInput[4] = "AAA";
static int   nameCursor = 0;  // 0..2
static float repeatTimer = 0.0f;
static float repeatDelay = 0.15f;

// Flag: ¿venimos de una partida real?  Lo pone UI a true en EndGameOver.
// Si es false solo mostramos la tabla (arranque / demo).
static bool  hsJustPlayed = false;

// ---------------------------------------------------------------
//  API pública: llamar desde EndGameOver() en UI.cpp
// ---------------------------------------------------------------
void HS_SetJustPlayed(bool v) { hsJustPlayed = v; }

// ---------------------------------------------------------------
//  Dibuja la tabla de puntuaciones
// ---------------------------------------------------------------
static void DrawTable() {
    const float cx = SCREEN_WIDTH / 2.0f;

    const char* title = "HIGH SCORES";
    Vector2 titleSize = MeasureTextEx(hsFont, title, 12, 0.5f);
    DrawTextEx(hsFont, title, { cx - titleSize.x / 2.0f, 16 }, 12, 0.5f, RED);

    DrawTextEx(hsFont, "RANK", { 14, 40 }, 8, 0.5f, YELLOW);
    DrawTextEx(hsFont, "NAME", { 50, 40 }, 8, 0.5f, YELLOW);
    DrawTextEx(hsFont, "SCORE", { 95, 40 }, 8, 0.5f, YELLOW);
    DrawTextEx(hsFont, "LV", { 165, 40 }, 8, 0.5f, YELLOW);
    DrawLine(10, 51, SCREEN_WIDTH - 10, 51, YELLOW);

    for (int i = 0; i < MAX_SCORES; i++) {
        float y = 58.0f + i * 20.0f;
        Color rowCol = (i == playerRank) ? SKYBLUE : WHITE;

        DrawTextEx(hsFont, TextFormat("#%d", i + 1), { 18, y }, 8, 0.5f, rowCol);

        if (scores[i].score > 0) {
            DrawTextEx(hsFont, scores[i].name, { 48, y }, 8, 0.5f, rowCol);
            DrawTextEx(hsFont, TextFormat("%06d", scores[i].score), { 88, y }, 8, 0.5f, rowCol);
            DrawTextEx(hsFont, TextFormat("%02d", scores[i].level), { 166, y }, 8, 0.5f, rowCol);
        }
        else {
            DrawTextEx(hsFont, "---", { 48, y }, 8, 0.5f, DARKGRAY);
            DrawTextEx(hsFont, "------", { 88, y }, 8, 0.5f, DARKGRAY);
            DrawTextEx(hsFont, "--", { 166, y }, 8, 0.5f, DARKGRAY);
        }
    }
}

// ---------------------------------------------------------------
//  Pantalla de entrada de nombre (estilo arcade)
// ---------------------------------------------------------------
static void UpdateDrawNameEntry() {
    const float cx = SCREEN_WIDTH / 2.0f;

    // Título
    const char* title = "GAME  OVER";
    Vector2 ts = MeasureTextEx(hsFont, title, 12, 0.5f);
    DrawTextEx(hsFont, title, { cx - ts.x / 2.0f, 16 }, 12, 0.5f, RED);

    // Instrucción
    const char* instr = "ENTER YOUR NAME";
    Vector2 is = MeasureTextEx(hsFont, instr, 8, 0.5f);
    DrawTextEx(hsFont, instr, { cx - is.x / 2.0f, 50 }, 8, 0.5f, YELLOW);

    // Las tres letras
    float startX = cx - 16.0f;
    for (int i = 0; i < 3; i++) {
        Color c = (i == nameCursor) ? SKYBLUE : WHITE;
        char buf[2] = { nameInput[i], '\0' };
        DrawTextEx(hsFont, buf, { startX + i * 14.0f, 74 }, 14, 0.5f, c);
    }

    // Cursor parpadeante bajo la letra activa
    if (blinkOn) {
        float cx2 = startX + nameCursor * 14.0f;
        DrawRectangle((int)cx2, 90, 11, 2, SKYBLUE);
    }

    // Instrucciones de controles
    DrawTextEx(hsFont, "UP/DOWN: change letter", { 14, 120 }, 7, 0.5f, GRAY);
    DrawTextEx(hsFont, "RIGHT/ENTER: confirm", { 14, 132 }, 7, 0.5f, GRAY);

    // ---- Input ----
    // IsKeyPressed = primer frame del press (dispara UNA vez).
    // IsKeyDown    = frames siguientes mientras se mantiene (repeat).
    // Nunca los dos juntos: evita saltar 2 letras por press.
    auto changeChar = [&](int dir) {
        nameInput[nameCursor] = 'A' + (nameInput[nameCursor] - 'A' + dir + 26) % 26;
        };

    if (IsKeyPressed(KEY_UP)) {
        changeChar(+1);
        repeatTimer = 0.0f;
    }
    else if (IsKeyDown(KEY_UP)) {
        repeatTimer += GetFrameTime();
        if (repeatTimer >= repeatDelay) { changeChar(+1); repeatTimer = 0.0f; }
    }

    if (IsKeyPressed(KEY_DOWN)) {
        changeChar(-1);
        repeatTimer = 0.0f;
    }
    else if (IsKeyDown(KEY_DOWN)) {
        repeatTimer += GetFrameTime();
        if (repeatTimer >= repeatDelay) { changeChar(-1); repeatTimer = 0.0f; }
    }

    // Avanzar / confirmar
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        nameCursor++;
        repeatTimer = repeatDelay;
        if (nameCursor >= 3) {
            // Nombre completado ? insertar en tabla
            ScoreEntry entry;
            entry.score = GetScore();
            entry.level = GetLevel();
            memcpy(entry.name, nameInput, 4);

            ResetAfterGameOver();       // resetea score/lives/level ANTES de insertar
            // (GetScore ya devolvía el valor guardado)
// Reinsertar con el score que teníamos guardado antes del reset
// (necesitamos haberlo leído antes — ver init)
            playerRank = InsertAndSort(entry);
            hsPhase = HS_SHOW_TABLE;
            hsTimer = 0.0f;
        }
    }
}

// ---------------------------------------------------------------
//  Punto de entrada principal
// ---------------------------------------------------------------
void runHighScoreScreen() {

    if (!Scene_Init) {
        SearchAndSetResourceDir("resources");
        if (hsFont.baseSize == 0)
            hsFont = LoadFont("Fonts/donkey-kong-arcade-1981.otf");

        hsTimer = 0.0f;
        blinkTimer = 0.0f;
        blinkOn = true;
        playerRank = -1;

        LoadScores();

        if (hsJustPlayed) {
            // Viene de una partida ? pedir nombre primero
            hsPhase = HS_NAME_ENTRY;
            nameCursor = 0;
            nameInput[0] = nameInput[1] = nameInput[2] = 'A';
            nameInput[3] = '\0';
            repeatTimer = repeatDelay;
            // Guardamos score/level ANTES de que ResetAfterGameOver los borre
            // (ya los guardará UpdateDrawNameEntry al confirmar)
        }
        else {
            // Arranque o demo: solo mostrar tabla
            hsPhase = HS_SHOW_TABLE;
            playerRank = -1;
        }

        Scene_Init = true;
    }

    blinkTimer += GetFrameTime();
    if (blinkTimer >= 0.5f) { blinkOn = !blinkOn; blinkTimer = 0.0f; }

    // ---- Fase: entrada de nombre ----
    if (hsPhase == HS_NAME_ENTRY) {
        UpdateDrawNameEntry();
        return;
    }

    // ---- Fase: mostrar tabla ----
    hsTimer += GetFrameTime();

    DrawTable();

    // "PRESS START" parpadeante
    if (blinkOn) {
        const char* ps = "PRESS START";
        Vector2 psSize = MeasureTextEx(hsFont, ps, 8, 0.5f);
        DrawTextEx(hsFont, ps,
            { SCREEN_WIDTH / 2.0f - psSize.x / 2.0f, 220 }, 8, 0.5f, WHITE);
    }

    // Salir a TITLE
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || hsTimer >= 15.0f) {
        UnloadFont(hsFont);
        hsFont = { 0 };
        Scene_Init = false;
        hsTimer = 0.0f;
        hsJustPlayed = false;
        ChangeScene();   // HIGHSCORE -> TITLE
    }
}