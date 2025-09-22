#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"
#include "inventory.hpp"

// ---- Classe Player ----
class Player : public Entity {
    private:
        Inventory inv;

    public:
        // Constructeur par défaut
        Player() = default;

        Player(const char *name, Hitbox hb, Color hitboxColor);

        int isEnough();

        void displayInfos();

        // Accès à l’inventaire
        Inventory& inventory() { return this->inv; }
};

#endif // PLAYER_H