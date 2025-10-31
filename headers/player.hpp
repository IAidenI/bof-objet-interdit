#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"
#include "inventory.hpp"

// ---- Textures ----
inline constexpr int         PLAYER_FRAME_W  = 32;
inline constexpr int         PLAYER_FRAME_H  = 32;
inline constexpr int         PLAYER_COLS     = 4;
inline constexpr int         PLAYER_ROW_IDLE = 0;
inline constexpr int         PLAYER_ROW_MOVE = 1;
inline constexpr const char *PLAYER_TEXTURE  = "assets/player.png";

// ---- Classe Player ----
class Player : public Entity {
    private:
        Inventory inv;

    public:
        Player(const char *name, Hitbox hb);

        void displayInfos();

        // Accès à l’inventaire
        Inventory& inventory() { return this->inv; }
};

#endif // PLAYER_H