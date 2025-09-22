#ifndef GAME_H
#define GAME_H

#define AMOUNT_TO_FINISH_GAME 32

#include "raylib.h"

#define BUFFER_SIZE 18     // Vulnérabilité

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

// Paramètres du jeu
#define PLAYER_SPEED 4 
#define POTATO_AVAILABLE 8

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

void GameInit(Font *info_font);

void DrawInfoLabel(Hitbox entity, Font font, const char *label, float fontSize, float spacing);

void GameClean(Font *info_font);

#endif // GAME_H