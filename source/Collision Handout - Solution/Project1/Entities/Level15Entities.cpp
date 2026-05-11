#include "raylib.h"
#include "Entities/Level15Entities.h"
#include "Maps/Level15Map.h"
#include <iostream>
using namespace std;

// Tiempo de inicio del nivel (para temporizaciones de spawn de entidades futuras)
float level15StartTime = 0.0f;

void SetStartTime15() {
    level15StartTime = GetTime();
}

void Level15EntitiesRoutine() {
    // Las entidades nuevas del nivel 1.5 se implementaran en pasos futuros
}

void UnloadLevel15Entities() {
    // Unload texturas de entidades nuevas cuando se implementen
}

void ResetLevel15Entities() {
    // Reset de entidades nuevas cuando se implementen
}
