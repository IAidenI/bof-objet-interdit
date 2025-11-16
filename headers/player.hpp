#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"
#include "inventory.hpp"

// ---- Textures ----
inline constexpr float       PLAYER_FRAME_W  = 32.0f;
inline constexpr float       PLAYER_FRAME_H  = 32.0f;
inline constexpr float       PLAYER_COLS     = 4.0f;
inline constexpr float       PLAYER_ROW_IDLE = 0.0f;
inline constexpr float       PLAYER_ROW_MOVE = 1.0f;

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