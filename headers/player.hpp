#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"
#include "inventory.hpp"

#define PLAYER_TEXTURE "assets/player.png"

// ---- Classe Player ----
class Player : public Entity {
    private:
        Inventory inv;

    public:
        // Constructeur par défaut
        Player() = default;

        Player(const char *name, Hitbox hb, const char *texture);

        // Pas de copie
        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;

        // Move ok
        Player(Player&&) noexcept = default;
        Player& operator=(Player&&) noexcept = default;

        int isEnough();

        void displayInfos();

        // Accès à l’inventaire
        Inventory& inventory() { return this->inv; }
};

#endif // PLAYER_H