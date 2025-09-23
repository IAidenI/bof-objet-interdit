#ifndef GAME_H
#define GAME_H

#define AMOUNT_TO_FINISH_GAME 32

#include "PNJ.hpp"
#include "player.hpp"

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

#define DIALOGUE_TEXTURE "assets/dialogue.png"

// ---- Structure texture/font ----
typedef struct {
    Font font;
    const char *text;
    float font_size;
    float spacing;
    Color color;
} TextStyle;

typedef enum {
    // Objets
    TEX_POTATO,
    TEX_CARROT,
    TEX_APPLE,
    // PNJ
    TEX_FARMER,
    TEX_GUARD,
    TEX_SORCERER,
    // Joueur
    TEX_PLAYER,
    // Autre
    TEX_DIALOGUE,
    // Sentinelle pour savoir la taille
    TEX_MAX
} TextureID;

typedef enum {
    BASIC,
    // Sentinelle pour savoir la taille
    FONT_MAX
} FontID;

// ---- Structures animations ----
typedef struct {
    int frame = 0;
    float timer = 0.0f;
    bool facingRight = false;
    float frameTimeIdle = 0.12f;
    float frameTimeMove = 0.12f;
    float scale = 4.0f;
} AnimationState;

typedef struct {
    const int frameW;
    const int frameH;
    const int frameCols;
    const int rowIdle;
    const int rowMove;
} SpriteSheetInfo;

// ---- Structures dialogues ----
typedef enum {
    ITEM,
    FARMER,
    GUARD,
    SORCERER
} DialogueEntity;

typedef struct {
    bool request = false;
    DialogueEntity entity = ITEM;
} DialogueInfo;

class Game {
    private:
        // Les managers
        Texture2D tmgr[TEX_MAX];
        Font fmgr[FONT_MAX];

        // ---- Entités ----
        // Le joueur
        Player player;

        // Les PNJ
        PNJ farmer;
        PNJ guard;
        PNJ sorcerer;

        // Les objets
        Item potato[POTATO_AVAILABLE];
        Item carrot;
        Item apple;

        // ---- Les Animations ----
        // Le joueur
        AnimationState playerAnim;

        // Les PNJ
        AnimationState farmerAnim;
        AnimationState guardAnim;
        AnimationState sorcererAnim;

        // Les objets
        AnimationState potatoAnim;

        // Les Requêtes
        bool displayHitbox = false;
        bool displayInventory = false;
        bool dialogueRequest = false;


        // ---- Les informations sur les sprites ----
        // Le joueur
        SpriteSheetInfo playerSprite = {PLAYER_FRAME_W, PLAYER_FRAME_H, PLAYER_COLS, PLAYER_ROW_IDLE, PLAYER_ROW_MOVE};

        // Les PNJ
        SpriteSheetInfo farmerSprite = {FARMER_FRAME_W, FARMER_FRAME_H, FARMER_COLS, FARMER_ROW_IDLE, FARMER_ROW_IDLE};
        SpriteSheetInfo guardSprite = {GUARD_FRAME_W, GUARD_FRAME_H, GUARD_COLS, GUARD_ROW_IDLE, GUARD_ROW_IDLE};
        SpriteSheetInfo sorcererSprite = {SORCERER_FRAME_W, SORCERER_FRAME_H, SORCERER_COLS, SORCERER_ROW_IDLE, SORCERER_ROW_IDLE};

        // Les objets
        SpriteSheetInfo potatoSprite = {POTATO_FRAME_W, POTATO_FRAME_H, POTATO_COLS, POTATO_ROW_IDLE, POTATO_ROW_IDLE};
        SpriteSheetInfo carrotSprite = {CARROT_FRAME_W, CARROT_FRAME_H, CARROT_COLS, CARROT_ROW_IDLE, CARROT_ROW_IDLE};
        SpriteSheetInfo appleSprite = {APPLE_FRAME_W, APPLE_FRAME_H, APPLE_COLS, APPLE_ROW_IDLE, APPLE_ROW_IDLE};

        // Autre
        bool isMoving = false;
        DialogueInfo displayDialogue;
    public:
        Game(const char **texturesPath, const char **fontPath);
        
        void handlePlayerMovements();
        void handlePlayerInput();
        
        void displayPNJ();
        void displayItems();
        void displayPlayer();

        void playerInteractions();
        void dialogue();

        ~Game();
};

void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Vector2 pos, bool moving, SpriteSheetInfo entitySprite, Color color);
void DrawStaticItem(const Texture2D& texture, Vector2 pos, float scale);
void DrawInfoLabel(Hitbox entity, int entitySize, TextStyle text);
void DrawDialogueFrame(Texture2D dialogue, Texture2D entity, AnimationState entityAnim, SpriteSheetInfo entitySprite, Color color);

#endif // GAME_H