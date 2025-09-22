#ifndef GAME_H
#define GAME_H

#define AMOUNT_TO_FINISH_GAME 32

#include <cmath>

#include "raylib.h"

#define BUFFER_SIZE 18     // Vulnérabilité

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

#define UI_DIALOGUE_WIDTH 700
#define UI_DIALOGUE_HEIGHT 100

// Paramètres du jeu
#define PLAYER_SPEED 4 
#define POTATO_AVAILABLE 8

// Définitions des couleurs
#define HITBOX_COLOR CLITERAL(Color){ 184, 184, 184, 102 }

// ---- Textures ----
// Dimensions d'une frame dans le spritesheet
#define PLAYER_FRAME_W  32
#define PLAYER_FRAME_H  32
#define PLAYER_COLS     4
#define PLAYER_ROW_IDLE 0
#define PLAYER_ROW_MOVE 1

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

typedef struct {
    Font font;
    const char *text;
    float font_size;
    float spacing;
    Color color;
} TextStyle;

typedef struct {
    int frame = 0;
    float timer = 0.0f;
    bool facingRight = false;
} AnimationState;

void GameInit(Font *info_font);

void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Vector2 pos, bool moving, int rowIdle, int rowMove, int frameCols, int frameW, int frameH, float frameTimeIdle, float frameTimeMove, float scale, Color color);
void DrawInfoLabel(Hitbox entity, Font font, const char *label, float fontSize, float spacing);
void DrawDialogueName(TextStyle text, Vector2 position, Vector2 size);
void DrawDialogueFrame(TextStyle name);
void Dialogue(Font *font);

void GameClean(Font *info_font);

#endif // GAME_H