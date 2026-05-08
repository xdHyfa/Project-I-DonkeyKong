#include "raylib.h"
#include "Maps/Level15Map.h"
#include "Entities/Ladders.h"
#include "Entities/Ground.h"
#include "Core/constants.h"
#include "Entities/entity.h"
#include "Entities/Player.h"
#include "Scenes/scenes.h"
#include <iostream>
using namespace std;

#define TrussHeight 16.0f

// --- WIN CONDITION ---
Rectangle Win15Hitbox = { SCREEN_WIDTH / 2 - 16, 40, 32, 10 };
bool win15Triggered = false;
float win15Delay = 0.0f;

void Level15CheckWinCondition(Entity& entity) {
    if ((entity.tag == EntityTag::PLAYER1 || entity.tag == EntityTag::PLAYER2) &&
        CheckCollisionPointRec(entity.FloorCollider, Win15Hitbox)) {
        if (!win15Triggered) {
            StopMusicStream(level15Music);
            win15Triggered = true;
        }
        if (win15Triggered && !IsSoundPlaying(stageCleared15Sound)) {
            PlaySound(stageCleared15Sound);
            ChangeScene();
        }
    }
}

// --- RAMPS ---
// SCREEN: 224 x 256
// Layout (from image, bottom to top):
//   Ramp_0: suelo completo (14 tiles, 0..223)
//   Ramp_1: plataforma izquierda (5 tiles) + plataforma derecha (5 tiles), separadas (agujero no saltable)
//   Ramp_2: plataforma izquierda (4 tiles) + plataforma derecha (4 tiles), separadas (agujero grande)
//   Ramp_3: plataforma centrada (5 tiles)
//   Ramp_4: plataforma izquierda (4 tiles) + plataforma derecha (4 tiles)
//   Ramp_5: plataforma derecha (4 tiles)

float P15_Ramp0_YPos;
float P15_Ramp1L_YPos;
float P15_Ramp1R_YPos;
float P15_Ramp2L_YPos;
float P15_Ramp2R_YPos;
float P15_Ramp3_YPos;
float P15_Ramp4L_YPos;
float P15_Ramp4R_YPos;
float P15_Ramp5_YPos;

// Plataforma 0: suelo completo
Truss Ramp15_0[14];

// Plataforma 1: dos mitades separadas
Truss Ramp15_1L[5]; // izquierda
Truss Ramp15_1R[5]; // derecha

// Nivel 2: centrada (antes era nivel 3), ahora 6 tiles
Truss Ramp15_2L[6];
Truss Ramp15_2R[1]; // placeholder, no usada

// Nivel 3: dos mitades separadas mas anchas (antes era nivel 2), 5 tiles cada una
Truss Ramp15_3[5];  // mitad izquierda
Truss Ramp15_4L[5]; // mitad derecha

// Nivel 4: izq+der separadas de borde, estiradas hacia centro, 4 tiles
Truss Ramp15_4R[4]; // izquierda
Truss Ramp15_5[4];  // derecha

// Active zones para colisiones (usando el eje Y de cada plataforma)
Rectangle Ramp15_0_Zone;
Rectangle Ramp15_1L_Zone;
Rectangle Ramp15_1R_Zone;
Rectangle Ramp15_2L_Zone;
Rectangle Ramp15_2R_Zone;
Rectangle Ramp15_3_Zone;
Rectangle Ramp15_4L_Zone;
Rectangle Ramp15_4R_Zone;
Rectangle Ramp15_5_Zone;

static void SetFlatRamp(Truss* ramp, int size, int startX, int yPos) {
    for (int i = 0; i < size; i++) {
        ramp[i].setPos((float)(startX + i * 16), (float)yPos);
    }
}

void Level15RampSetter() {
    Truss::truss = LoadTexture("sprites/Yellow truss.png");

    // Suelo subido 10px respecto a la posicion original
    P15_Ramp0_YPos = SCREEN_HEIGHT - TrussHeight - 1 - 5;  // -5 en vez de +5 -> 10px mas arriba
    P15_Ramp1L_YPos = P15_Ramp0_YPos - 44;
    P15_Ramp1R_YPos = P15_Ramp1L_YPos;
    P15_Ramp2L_YPos = P15_Ramp1L_YPos - 44;
    P15_Ramp2R_YPos = P15_Ramp2L_YPos;
    P15_Ramp3_YPos = P15_Ramp2L_YPos - 44;
    P15_Ramp4L_YPos = P15_Ramp3_YPos - 44;
    P15_Ramp4R_YPos = P15_Ramp4L_YPos;
    P15_Ramp5_YPos = P15_Ramp4L_YPos - 40;

    // Suelo: 14 tiles completo
    SetFlatRamp(Ramp15_0, 14, 0, (int)P15_Ramp0_YPos);

    // Nivel 1: izq+der
    SetFlatRamp(Ramp15_1L, 5, 8, (int)P15_Ramp1L_YPos);
    SetFlatRamp(Ramp15_1R, 5, 136, (int)P15_Ramp1R_YPos);

    // Nivel 2: centrada
    SetFlatRamp(Ramp15_2L, 6, 64, (int)P15_Ramp2L_YPos);

    // Nivel 3: izq+der separadas
    SetFlatRamp(Ramp15_3, 5, 0, (int)P15_Ramp3_YPos);
    SetFlatRamp(Ramp15_4L, 5, 144, (int)P15_Ramp3_YPos);

    // Nivel 4 (superior): izq+der
    SetFlatRamp(Ramp15_4R, 4, 16, (int)P15_Ramp4L_YPos);
    SetFlatRamp(Ramp15_5, 4, 128, (int)P15_Ramp5_YPos);

    // Plataforma intermedia: debajo de la de arriba del todo (Ramp15_5/Ramp15_4R),
    // permite saltar hacia la izquierda desde ella a la plataforma izquierda superior.
    // Se coloca a mitad de altura entre nivel 3 der y nivel 4, centrada-derecha.
    float P15_RampMid_YPos = P15_Ramp3_YPos - 22;  // a medio camino entre nivel 3 y nivel 4
    SetFlatRamp(Ramp15_2R, 1, 112, (int)P15_RampMid_YPos);  // plataforma puente (1 tile en X=112)

    // Active zones
    float zoneH = 22.0f;
    Ramp15_0_Zone = { 0,   P15_Ramp0_YPos - 6,  (float)SCREEN_WIDTH, zoneH };
    Ramp15_1L_Zone = { 0,   P15_Ramp1L_YPos - 6,  100.0f, zoneH };
    Ramp15_1R_Zone = { 124, P15_Ramp1R_YPos - 6,  100.0f, zoneH };
    Ramp15_2L_Zone = { 52,  P15_Ramp2L_YPos - 6,  120.0f, zoneH };
    Ramp15_2R_Zone = { 100, P15_RampMid_YPos - 6,  24.0f,  zoneH };  // plataforma puente
    Ramp15_3_Zone = { 0,   P15_Ramp3_YPos - 6,  92.0f,  zoneH };
    Ramp15_4L_Zone = { 132, P15_Ramp3_YPos - 6,  92.0f,  zoneH };
    Ramp15_4R_Zone = { 4,   P15_Ramp4L_YPos - 6,  84.0f,  zoneH };
    Ramp15_5_Zone = { 116, P15_Ramp5_YPos - 6,  108.0f, zoneH };
}

void Level15RampDraw() {
    RampDrawer(Ramp15_0, 14);  // suelo
    RampDrawer(Ramp15_1L, 5);  // nivel 1 izq
    RampDrawer(Ramp15_1R, 5);  // nivel 1 der
    RampDrawer(Ramp15_2L, 6);  // nivel 2 centrada (6 tiles)
    RampDrawer(Ramp15_2R, 1);  // plataforma puente (1 tile entre nivel 3 der y nivel 4)
    RampDrawer(Ramp15_3, 5);  // nivel 3 mitad izq
    RampDrawer(Ramp15_4L, 5);  // nivel 3 mitad der
    RampDrawer(Ramp15_4R, 4);  // nivel 4 izq
    RampDrawer(Ramp15_5, 4);  // nivel 4 der
}

void Level15RampCollisions(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_0_Zone)) {
        RampCollision(Ramp15_0, 14, entity);
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_1L_Zone)) {
        RampCollision(Ramp15_1L, 5, entity);
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_1R_Zone)) {
        RampCollision(Ramp15_1R, 5, entity);
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_2L_Zone)) {
        RampCollision(Ramp15_2L, 6, entity);   // nivel 2 centrada (6 tiles)
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_3_Zone)) {
        RampCollision(Ramp15_3, 5, entity);    // nivel 3 izq
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_4L_Zone)) {
        RampCollision(Ramp15_4L, 5, entity);   // nivel 3 der
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_4R_Zone)) {
        RampCollision(Ramp15_4R, 4, entity);   // nivel 4 izq
    }
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_5_Zone)) {
        RampCollision(Ramp15_5, 4, entity);    // nivel 4 der
    }
    else {
        // Fuera de cualquier plataforma -> falling
        if (entity.tag == EntityTag::PLAYER1) {
            Mario.isFalling = true;
        }
        if (entity.tag == EntityTag::PLAYER2) {
            Luigi.isFalling = true;
        }
    }
}

void DrawLevel15Colliders() {
    DrawRectangleLines((int)Ramp15_0_Zone.x, (int)Ramp15_0_Zone.y, (int)Ramp15_0_Zone.width, (int)Ramp15_0_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_1L_Zone.x, (int)Ramp15_1L_Zone.y, (int)Ramp15_1L_Zone.width, (int)Ramp15_1L_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_1R_Zone.x, (int)Ramp15_1R_Zone.y, (int)Ramp15_1R_Zone.width, (int)Ramp15_1R_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_2L_Zone.x, (int)Ramp15_2L_Zone.y, (int)Ramp15_2L_Zone.width, (int)Ramp15_2L_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_2R_Zone.x, (int)Ramp15_2R_Zone.y, (int)Ramp15_2R_Zone.width, (int)Ramp15_2R_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_3_Zone.x, (int)Ramp15_3_Zone.y, (int)Ramp15_3_Zone.width, (int)Ramp15_3_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_4L_Zone.x, (int)Ramp15_4L_Zone.y, (int)Ramp15_4L_Zone.width, (int)Ramp15_4L_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_4R_Zone.x, (int)Ramp15_4R_Zone.y, (int)Ramp15_4R_Zone.width, (int)Ramp15_4R_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_5_Zone.x, (int)Ramp15_5_Zone.y, (int)Ramp15_5_Zone.width, (int)Ramp15_5_Zone.height, GREEN);

    // Player collider
    DrawCircle((int)Mario.FloorCollider.x, (int)Mario.FloorCollider.y, 2, RED);
}

int Level15CheckEntityPlatform(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_0_Zone))  return 0;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_1L_Zone)) return 1;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_1R_Zone)) return 2;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_2L_Zone)) return 3;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_2R_Zone)) return 4;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_3_Zone))  return 5;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_4L_Zone)) return 6;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_4R_Zone)) return 7;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_5_Zone))  return 8;
    return -1;
}

bool Level15CheckDownZone(Entity& entity) {
    return (entity.Position.y > SCREEN_HEIGHT - 80);
}

// --- LADDERS ---
// Sin escaleras por ahora (el usuario las agregara en pasos futuros)
Ladder Level15_Ladders[1]; // placeholder
int Level15_LadderCount = 0;

void Level15LadderSetter() {
    Ladder::LoadSharedTexture();
    // Sin escaleras todavia
}

void Level15LadderDraw() {
    for (int i = 0; i < Level15_LadderCount; i++) {
        DrawTextureRec(Ladder::texture, Level15_Ladders[i].SpriteSelector, Level15_Ladders[i].Position, WHITE);
    }
}

void Level15LadderCollisions(Entity& entity) {
    LadderCollisions(entity, Level15_Ladders, Level15_LadderCount);
}