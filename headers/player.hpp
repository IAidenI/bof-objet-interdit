#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"
#include "inventory.hpp"

#define PLAYER_TEXTURE "assets/player.png"

// ---- Textures ----
#define PLAYER_FRAME_W  32
#define PLAYER_FRAME_H  32
#define PLAYER_COLS     4
#define PLAYER_ROW_IDLE 0
#define PLAYER_ROW_MOVE 1

// ---- Classe Player ----
class Player : public Entity {
    private:
        Inventory inv;

    public:
        // Constructeur par défaut
        Player() = default;

        Player(const char *name, Hitbox hb);

        int isEnough();

        void displayInfos();

        // Accès à l’inventaire
        Inventory& inventory() { return this->inv; }
};

#endif // PLAYER_H