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
#include "manager.hpp"
#include "notification.hpp"

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
        Manager manager;
        Notification notification;

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
        Game();

        bool init();
        void notificationUpdate() { this->notification.update(); }
        
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

        void notificationDraw() { this->notification.draw(); }

        ~Game();
};

#endif // GAME_H
