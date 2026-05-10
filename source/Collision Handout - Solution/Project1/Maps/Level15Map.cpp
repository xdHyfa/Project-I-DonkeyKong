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

// --- LADDER DECLARATIONS ---
Ladder Level15_Ladders[4];
Ladder ExtraLadder15_0[6];
Ladder ExtraLadder15_1[6];
Ladder ExtraLadder15_2[6];
Ladder ExtraLadder15_3[6];
Rectangle Level15_DownZone[4];

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
Truss Ramp15_5[4];  // derecha  (nivel 6 - destruida, ya no se usa)

// Nivel 5: nueva plataforma azul (lado derecho)
Truss Ramp15_BlueNew[5];

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
Rectangle Ramp15_BlueNew_Zone;

static void SetFlatRamp(Truss* ramp, int size, int startX, int yPos) {
    for (int i = 0; i < size; i++) {
        ramp[i].setPos((float)(startX + i * 16), (float)yPos);
    }
}

void Level15RampSetter() {
    // Usar Yellow truss sprite (nivel 3 en LoadSharedTexture podria no tenerla, lo cargamos manualmente)
    // Cargamos la textura amarilla directamente en el campo compartido
    Truss::truss = LoadTexture("sprites/Yellow truss.png");

    // Alturas (de abajo a arriba), con separacion de ~40px entre plataformas
    // Todo bajado ~15px respecto a la version anterior para dar mas espacio visual
    P15_Ramp0_YPos = SCREEN_HEIGHT - TrussHeight - 1 + 5 - 10;
    P15_Ramp1L_YPos = P15_Ramp0_YPos - 54;
    P15_Ramp1R_YPos = P15_Ramp1L_YPos;
    P15_Ramp2L_YPos = P15_Ramp1L_YPos - 44;
    P15_Ramp2R_YPos = P15_Ramp2L_YPos;
    P15_Ramp3_YPos = P15_Ramp2L_YPos - 44;
    P15_Ramp4L_YPos = P15_Ramp3_YPos - 44;
    P15_Ramp4R_YPos = P15_Ramp4L_YPos;
    P15_Ramp5_YPos = P15_Ramp4L_YPos - 40;

    SetFlatRamp(Ramp15_0, 14, 0, (int)P15_Ramp0_YPos);

    // Nivel 1: 10px up already via -54
    SetFlatRamp(Ramp15_1L, 5, 8, (int)P15_Ramp1L_YPos);
    SetFlatRamp(Ramp15_1R, 5, 136, (int)P15_Ramp1R_YPos);

    SetFlatRamp(Ramp15_2L, 6, 64, (int)P15_Ramp2L_YPos);

    // Nivel 3: gap cerrado mucho mas - plataformas empujadas hacia el centro
    SetFlatRamp(Ramp15_3, 5, 16, (int)P15_Ramp3_YPos);  // izq: termina en X=96
    SetFlatRamp(Ramp15_4L, 5, 112, (int)P15_Ramp3_YPos);  // der: empieza en X=112, gap=16px

    // Nivel 4: gap cerrado
    SetFlatRamp(Ramp15_4R, 4, 28, (int)P15_Ramp4L_YPos);
    SetFlatRamp(Ramp15_5, 4, 116, (int)P15_Ramp5_YPos);

    // Nivel 5: nueva plataforma, misma Y que izquierda
    float blueY = P15_Ramp4L_YPos;
    SetFlatRamp(Ramp15_BlueNew, 5, 128, (int)blueY);

    // hasLadderBelow: marca los tiles donde hay escalera para que el jugador pueda bajar
    // L0: izquierdo de Ramp1L (tile [0]) -> baja a Ramp0
    Ramp15_1L[0].hasLadderBelow = true;
    // L1: izquierdo de Ramp2L (tile [0]) -> baja a Ramp1L
    Ramp15_2L[0].hasLadderBelow = true;
    // L2: derecho de Ramp2L (tile [5]) -> baja a Ramp1R
    Ramp15_2L[5].hasLadderBelow = true;
    // L3: derecho de Ramp4R (tile [3]) -> baja a Ramp3
    Ramp15_4R[3].hasLadderBelow = true;

    // Nivel 6 (Ramp15_5 original) ha sido destruido - ya no se inicializa ni dibuja

    // Active zones
    float zoneH = 22.0f;
    Ramp15_0_Zone = { 0,   P15_Ramp0_YPos - 6,  (float)SCREEN_WIDTH, zoneH };
    // Nivel 1
    Ramp15_1L_Zone = { 0,   P15_Ramp1L_YPos - 6,  100.0f, zoneH };
    Ramp15_1R_Zone = { 124, P15_Ramp1R_YPos - 6,  100.0f, zoneH };
    // Nivel 2 (centrada)
    Ramp15_2L_Zone = { 52,  P15_Ramp2L_YPos - 6,  120.0f, zoneH };
    Ramp15_2R_Zone = { 0,   0, 0, 0 };  // no usada
    // Nivel 3 izq y der (gap cerrado)
    Ramp15_3_Zone = { 4,   P15_Ramp3_YPos - 6,  96.0f, zoneH };   // X=16, 5t=80px
    Ramp15_4L_Zone = { 100, P15_Ramp3_YPos - 6,  96.0f, zoneH };   // X=112, 5t=80px
    // Nivel 4 izq y der (gap cerrado)
    Ramp15_4R_Zone = { 16,  P15_Ramp4L_YPos - 6,  84.0f,  zoneH };
    Ramp15_5_Zone = { 0,   0, 0, 0 };  // nivel 6 destruido - zona desactivada
    // Nivel 5: nueva plataforma azul
    Ramp15_BlueNew_Zone = { 116, blueY - 6, 96.0f, zoneH };
}

void Level15RampDraw() {
    RampDrawer(Ramp15_0, 14);  // suelo
    RampDrawer(Ramp15_1L, 5);  // nivel 1 izq
    RampDrawer(Ramp15_1R, 5);  // nivel 1 der
    RampDrawer(Ramp15_2L, 6);  // nivel 2 centrada (6 tiles)
    // Ramp15_2R no se dibuja (no usada)
    RampDrawer(Ramp15_3, 5);   // nivel 3 mitad izq
    RampDrawer(Ramp15_4L, 5);  // nivel 3 mitad der
    RampDrawer(Ramp15_4R, 4);  // nivel 4 izq
    // Ramp15_5 (nivel 6) destruido - ya no se dibuja
    // Nivel 5: nueva plataforma (misma textura y color que el resto)
    for (int i = 0; i < 5; i++) {
        DrawTexture(Ramp15_BlueNew[i].truss, (int)Ramp15_BlueNew[i].TrussPos.x, (int)Ramp15_BlueNew[i].TrussPos.y, WHITE);
    }
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
    else if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_BlueNew_Zone)) {
        RampCollision(Ramp15_BlueNew, 5, entity);  // nivel 5 azul (nuevo)
    }
    // Ramp15_5_Zone (nivel 6) destruido - sin colision
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
    DrawRectangleLines((int)Ramp15_3_Zone.x, (int)Ramp15_3_Zone.y, (int)Ramp15_3_Zone.width, (int)Ramp15_3_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_4L_Zone.x, (int)Ramp15_4L_Zone.y, (int)Ramp15_4L_Zone.width, (int)Ramp15_4L_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_4R_Zone.x, (int)Ramp15_4R_Zone.y, (int)Ramp15_4R_Zone.width, (int)Ramp15_4R_Zone.height, GREEN);
    DrawRectangleLines((int)Ramp15_BlueNew_Zone.x, (int)Ramp15_BlueNew_Zone.y, (int)Ramp15_BlueNew_Zone.width, (int)Ramp15_BlueNew_Zone.height, BLUE);

    DrawLadderCollider(Level15_Ladders, 4);

    // Player collider
    DrawCircle((int)Mario.FloorCollider.x, (int)Mario.FloorCollider.y, 2, RED);
}

int Level15CheckEntityPlatform(Entity& entity) {
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_0_Zone))       return 0;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_1L_Zone))      return 1;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_1R_Zone))      return 2;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_2L_Zone))      return 3;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_3_Zone))       return 5;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_4L_Zone))      return 6;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_4R_Zone))      return 7;
    if (CheckCollisionPointRec(entity.FloorCollider, Ramp15_BlueNew_Zone)) return 8;
    return -1;
}

bool Level15CheckDownZone(Entity& entity) {
    return (entity.Position.y > SCREEN_HEIGHT - 80);
}

// --- LADDERS ---
// Each ladder starts at its platform and is extended downward with full
// sprite pieces to reach the platform below, making it climbable end-to-end.

void SetFullLadder15(Ladder& ladder, Ladder* Extras, float X, float Y) {
    ladder.setPos(X, Y);
    for (int i = 0; i < 6; i++) {
        Extras[i].setPos(ladder.Position.x, ladder.Position.y + (float)((i + 1) * 16));
    }
}

void Level15LadderSetter() {
    Ladder::LoadSharedTexture();

    for (int i = 0; i < 4; i++) {
        Level15_Ladders[i].setSprite(1, true);
    }
    for (int i = 0; i < 6; i++) {
        ExtraLadder15_0[i].setSprite(1, true);
        ExtraLadder15_1[i].setSprite(1, true);
        ExtraLadder15_2[i].setSprite(1, true);
        ExtraLadder15_3[i].setSprite(1, true);
    }

    // L0: Ramp1L (piso 1) -> Ramp0 (suelo). extra=29
    SetFullLadder15(Level15_Ladders[0], ExtraLadder15_0,
        Ramp15_1L[0].TrussPos.x + 4,
        Ramp15_1L[0].TrussPos.y + Ramp15_1L[0].TrussBox.height * 2);
    Level15_Ladders[0].Hitbox.height += 29;

    // L1: Ramp2L (piso 2) -> Ramp1L (piso 1). extra=19
    SetFullLadder15(Level15_Ladders[1], ExtraLadder15_1,
        Ramp15_2L[0].TrussPos.x + 4,
        Ramp15_2L[0].TrussPos.y + Ramp15_2L[0].TrussBox.height * 2);
    Level15_Ladders[1].Hitbox.height += 19;

    // L2: Ramp2L (piso 2) -> Ramp1R (piso 1). extra=19
    SetFullLadder15(Level15_Ladders[2], ExtraLadder15_2,
        Ramp15_2L[5].TrussPos.x + 4,
        Ramp15_2L[5].TrussPos.y + Ramp15_2L[5].TrussBox.height * 2);
    Level15_Ladders[2].Hitbox.height += 19;

    // L3: Ramp4R (piso 4) -> Ramp3 (piso 3). extra=19
    SetFullLadder15(Level15_Ladders[3], ExtraLadder15_3,
        Ramp15_4R[3].TrussPos.x + 4,
        Ramp15_4R[3].TrussPos.y + Ramp15_4R[3].TrussBox.height * 2);
    Level15_Ladders[3].Hitbox.height += 19;

    for (int i = 0; i < 4; i++) {
        Level15_DownZone[i] = { Level15_Ladders[i].Hitbox.x, Level15_Ladders[i].Hitbox.y - 2, 10, 4 };
    }
}

void Level15LadderDraw() {
    for (int i = 0; i < 4; i++) {
        DrawTextureRec(Ladder::texture, Level15_Ladders[i].SpriteSelector, Level15_Ladders[i].Position, WHITE);
    }
    // L0 needs 3 extra pieces (gap=38px -> piece0+piece1+piece2)
    DrawTextureRec(ExtraLadder15_0[0].texture, ExtraLadder15_0[0].SpriteSelector, ExtraLadder15_0[0].Position, WHITE);
    DrawTextureRec(ExtraLadder15_0[1].texture, ExtraLadder15_0[1].SpriteSelector, ExtraLadder15_0[1].Position, WHITE);
    DrawTextureRec(ExtraLadder15_0[2].texture, ExtraLadder15_0[2].SpriteSelector, ExtraLadder15_0[2].Position, WHITE);
    // L1, L2, L3 need 2 extra pieces each (gap=28px)
    DrawTextureRec(ExtraLadder15_1[0].texture, ExtraLadder15_1[0].SpriteSelector, ExtraLadder15_1[0].Position, WHITE);
    DrawTextureRec(ExtraLadder15_1[1].texture, ExtraLadder15_1[1].SpriteSelector, ExtraLadder15_1[1].Position, WHITE);
    DrawTextureRec(ExtraLadder15_2[0].texture, ExtraLadder15_2[0].SpriteSelector, ExtraLadder15_2[0].Position, WHITE);
    DrawTextureRec(ExtraLadder15_2[1].texture, ExtraLadder15_2[1].SpriteSelector, ExtraLadder15_2[1].Position, WHITE);
    DrawTextureRec(ExtraLadder15_3[0].texture, ExtraLadder15_3[0].SpriteSelector, ExtraLadder15_3[0].Position, WHITE);
    DrawTextureRec(ExtraLadder15_3[1].texture, ExtraLadder15_3[1].SpriteSelector, ExtraLadder15_3[1].Position, WHITE);
}

void Level15LadderCollisions(Entity& entity) {
    LadderCollisions(entity, Level15_Ladders, 4);
}