#ifndef GAME_H
#define GAME_H

#include <time.h>

#include <algorithm>
#include <vector>
#include "PNJ.hpp"
#include "player.hpp"
#include "gdb.hpp"
#include "raylib-utils.hpp"
#include "game_settings.hpp"

// ---- Assets ----
inline constexpr const char *ICON             = "assets/icon.png";
inline constexpr const char *BACKGROUND       = "assets/background.png";
inline constexpr const char *DIALOGUE_TEXTURE = "assets/dialogue.png";

// ---- Structure texture/font ----
inline constexpr const char *ENTITY_FONT     = "assets/fonts/ByteBounce.ttf";
inline constexpr const char *DIALOGUE_FONT   = "assets/fonts/Jersey10-Regular.ttf";
inline constexpr const char *INFO_FONT       = "assets/fonts/Inconsolata-Regular.ttf";
inline constexpr const int   SMALL_SIZE      = 20;
inline constexpr const int   BIG_SIZE        = 50;
inline constexpr const int   FONT_SIZE_COUNT = 2;
// ASCII + accents FR courants
static const char32_t FR_CHARS[] = U" !\"#$%&'()*+,-./0123456789:;<=>?@"
                                   U"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                                   U"abcdefghijklmnopqrstuvwxyz{|}~"
                                   U"àâäçéèêëîïôöùûüÿÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸ’«»œŒ";

inline constexpr Frame START_FRAME = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };

enum TextureID {
    TEX_BACKGROUND,
    // Objets
    TEX_POTATO,
    TEX_POTATO_STATIC,
    TEX_CARROT_STATIC,
    TEX_APPLE,
    TEX_APPLE_STATIC,
    // PNJ
    TEX_FARMER,
    TEX_GUARD,
    TEX_SORCERER,
    // Joueur
    TEX_PLAYER,
    // Autre
    TEX_DIALOGUE,
    TEX_INVENTORY,
    // Sentinelle pour savoir la taille
    TEX_MAX
};

enum FontID {
    ENTITY_LABEL,
    DIALOGUE_LABEL,
    INFO_LABEL,
    // Sentinelle pour savoir la taille
    FONT_MAX
};

// ---- Structures dialogues ----
enum DialogueEntity {
    ITEM_POTATO,
    ITEM_APPLE,
    FARMER,
    GUARD,
    SORCERER
};

struct DialogueInfo {
    bool request = false;
    DialogueEntity entity = ITEM_POTATO;
};

class Game {
    private:
        // Les managers
        Texture2D tmgr[TEX_MAX];
        Font fmgr[FONT_MAX][FONT_SIZE_COUNT];

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
        bool isAppleTaken = false;

        // ---- Les Animations ----
        // Le joueur
        AnimationState playerAnim;

        // Les PNJ
        AnimationState farmerAnim;
        AnimationState guardAnim;
        AnimationState sorcererAnim;

        // Les objets
        AnimationState potatoAnim;
        AnimationState appleAnim;

        // Les Requêtes
        bool hitboxRequest = false;
        bool inventoryRequest = false;
        bool dialogueRequest = false;
        bool stackRequest = false;

        // ---- Les informations sur les sprites ----
        // Le joueur
        const SpriteSheetInfo playerSprite = { PLAYER_FRAME_W, PLAYER_FRAME_H, PLAYER_COLS, PLAYER_ROW_IDLE, PLAYER_ROW_MOVE};

        // Les PNJ
        const SpriteSheetInfo farmerSprite = { FARMER_FRAME_W, FARMER_FRAME_H, FARMER_COLS, FARMER_ROW_IDLE, FARMER_ROW_IDLE };
        const SpriteSheetInfo guardSprite = { GUARD_FRAME_W, GUARD_FRAME_H, GUARD_COLS, GUARD_ROW_IDLE, GUARD_ROW_IDLE };
        const SpriteSheetInfo sorcererSprite = { SORCERER_FRAME_W, SORCERER_FRAME_H, SORCERER_COLS, SORCERER_ROW_IDLE, SORCERER_ROW_IDLE };

        // Les objets
        const SpriteSheetInfo potatoSprite = { POTATO_FRAME_W, POTATO_FRAME_H, POTATO_COLS, POTATO_ROW_IDLE, POTATO_ROW_IDLE };
        const SpriteSheetInfo carrotSprite = { CARROT_FRAME_W, CARROT_FRAME_H, CARROT_COLS, CARROT_ROW_IDLE, CARROT_ROW_IDLE };
        const SpriteSheetInfo appleSprite = { APPLE_FRAME_W, APPLE_FRAME_H, APPLE_COLS, APPLE_ROW_IDLE, APPLE_ROW_IDLE };

        // Dialogue
        DialogueInfo displayDialogue;
        TextStyle newName;
        int  invSelectorIndex = 0;
        bool invSelectorVisible = false;
        bool renameInit = true;
        bool isTyping = false;
        int sorcererStep = 0;
        int appleStep = 0;
        int farmerStep = 0;

        // Affichage stack
        bool stackShowMore = false;
        bool stackShowMoreHover = false;
        int  invPage = 0;
        Rectangle invBtnPrev, invBtnNext = {0}; 

        // Autre
        bool isMoving = false;
        bool isPause = false;
        bool gameEnded = false;

        void getNewName();
        void changeItemName();
        void moveInvSelector(int dx, int dy);
        void dialogueContinue(TextStyle label);

        // GDB
        GDB gdb;
    public:
        Game(const char **texturesPath, const char **fontPath);
        Font& getFont(FontID id, int size);
        
        void handlePlayerMovements();
        void handlePlayerInput();
        
        void displayBackground();
        void displayCommands();
        void displayPNJ();
        void displayItems();
        void displayPlayer();

        void playerInteractions();
        void dialogue();

        void renderInventory();
        void renderStack();
        void renderStack2();

        void resetRequests();
        bool hasEnded() const { return this->gameEnded; }

        ~Game();
};

#endif // GAME_H
