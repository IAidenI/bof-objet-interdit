#ifndef GAME_H
#define GAME_H

#define AMOUNT_TO_FINISH_GAME 32

#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

// Paramètres du jeu
#define PLAYER_SPEED 4 

// Définitions des couleurs
#define HITBOX_COLOR CLITERAL(Color){ 184, 184, 184, 102 }

// ---- Structures de base ----
typedef enum {
    // Globale
    OK,
    // Inventaire
    INVENTORY_LENGTH_MAX_REACH,
} ReturnCode;

typedef struct {
    Vector2 pos;
    float radius;
} Hitbox;

#endif // GAME_H