#ifndef GAME_H
#define GAME_H

#define AMOUNT_TO_FINISH_GAME 32

#include <algorithm>
#include "PNJ.hpp"
#include "player.hpp"

#define BUFFER_SIZE 18     // Vulnérabilité

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

// Paramètres du jeu
#define PLAYER_SPEED 4
#define POTATO_AVAILABLE 8

#define INVENTORY_SCALE 5.0f

// Définitions des couleurs
#define HITBOX_COLOR CLITERAL(Color){ 184, 184, 184, 102 }

// Dialogues
#define UI_DIALOGUE_WIDTH  700
#define UI_DIALOGUE_HEIGHT 100

#define UI_DIALOGUE_CONTENT_WIDTH  597
#define UI_DIALOGUE_CONTENT_HEIGHT 88

#define DIALOGUE_POS_X (SCREEN_WIDTH / 2 - (UI_DIALOGUE_WIDTH / 2))
#define DIALOGUE_POS_Y (SCREEN_HEIGHT - UI_DIALOGUE_HEIGHT - 5.0f) // 5.0f padding du bas

#define DIALOGUE_CONTENT_POS_X (DIALOGUE_POS_X + 110)
#define DIALOGUE_CONTENT_POS_Y (DIALOGUE_POS_Y + 15)

#define DIALOGUE_CONTENT_END_X (DIALOGUE_POS_X + 97 + UI_DIALOGUE_CONTENT_WIDTH)
#define DIALOGUE_CONTENT_END_Y (DIALOGUE_POS_Y + 6 + UI_DIALOGUE_CONTENT_HEIGHT)

#define DIALOGUE_TEXTURE "assets/dialogue.png"

// ---- Structure texture/font ----
#define ENTITY_FONT   "assets/fonts/ByteBounce.ttf"
#define DIALOGUE_FONT "assets/fonts/Jersey10-Regular.ttf"

typedef struct {
    Font font;
    string text;
    float font_size;
    float spacing;
    Color color;
} TextStyle;

typedef enum {
    // Objets
    TEX_POTATO,
    TEX_POTATO_STATIC,
    TEX_CARROT_STATIC,
    TEX_APPLE_STATIC,
    // PNJ
    TEX_FARMER,
    TEX_GUARD,
    TEX_SORCERER,
    // Joueur
    TEX_PLAYER,
    // Autre
    TEX_DIALOGUE,
    TEX_SLOT,
    // Sentinelle pour savoir la taille
    TEX_MAX
} TextureID;

typedef enum {
    ENTITY_LABEL,
    DIALOGUE_LABEL,
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
        bool hitboxRequest = false;
        bool inventoryRequest = false;
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

        // Dialogue
        DialogueInfo displayDialogue;
        char *newName = nullptr;
        int  invSelectorIndex = 0;
        bool invSelectorVisible = false;
        int sorcererStep = 0;

        // Autre
        bool isMoving = false;
        bool isPause = false;
        bool gameEnded = false;

        void getNewName();
        void changeItemName();
        void moveInvSelector(int dx, int dy);
        void dialogueContinue(TextStyle label);
        bool dialogueChoice(TextStyle label);
    public:
        Game(const char **texturesPath, const char **fontPath);
        
        void handlePlayerMovements();
        void handlePlayerInput();
        
        void displayCommands();
        void displayPNJ();
        void displayItems();
        void displayPlayer();

        void playerInteractions();
        void dialogue();

        void renderInventory();

        void resetRequests();
        bool hasEnded() const { return this->gameEnded; }

        ~Game();
};

void DrawCornerMarkers(const Rectangle& r, float len, float thick, Color color);
void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Vector2 pos, bool moving, SpriteSheetInfo entitySprite, Color color);
void DrawStaticItem(const Texture2D& texture, Vector2 pos, float scale);
void DrawInfoLabel(Hitbox entity, int entitySize, TextStyle text);
void DrawDialogueFrame(Texture2D dialogue, Texture2D entity, AnimationState entityAnim, SpriteSheetInfo entitySprite, Color color);

#endif // GAME_H