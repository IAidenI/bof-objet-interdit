#ifndef GAME_H
#define GAME_H

#define AMOUNT_TO_FINISH_GAME 32

// ---- Structures de base ----
typedef enum {
    // Globale
    OK,
    // Inventaire
    INVENTORY_LENGTH_MAX_REACH,
} ReturnCode;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    int width;
    int height;
} Hitbox;

#endif // GAME_H