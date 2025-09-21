#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <cstring>
using namespace std;

#include "game.hpp"
#include "inventory.hpp"

#define BUFFER_SIZE 18     // Vulnérabilité
#define MAX_NAME_LENGTH 16 //

// ---- Classe Entity ----
class Entity {
    private:
        Hitbox hitbox;

    public:
        Entity(Hitbox hb);

        const Hitbox getHitbox() const { return hitbox; };
};

// ---- Classe Player ----
class Player : public Entity {
    private:
        char name[MAX_NAME_LENGTH];
        Inventory inv;

    public:
        Player(const char *name, Hitbox hb);

        void changeName(const char *newName);
        int isEnough();

        void displayInfos();

        // Accès à l’inventaire
        Inventory& inventory() { return inv; }
};

#endif // ENTITIES_H