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
        Color hitboxColor;
        char name[MAX_NAME_LENGTH];

    public:
        Entity(const char *name, Hitbox hb, Color hitboxColor);

        // Setter
        void changeName(const char *newName); // Vulnérabilité
        void setPosX(int x) { this->hitbox.pos.x = x; };
        void setPosY(int y) { this->hitbox.pos.y = y; };

        // Getter
        const Hitbox getHitbox() const { return this->hitbox; };
        const char *getName() const { return name; };
        const Color getColor() const { return this->hitboxColor; }
};

// ---- Classe PNJ ----
class PNJ : public Entity {    
    public:
        PNJ(const char *name, Hitbox hb, Color hitboxColor);
};

// ---- Classe Player ----
class Player : public Entity {
    private:
        Inventory inv;

    public:
        Player(const char *name, Hitbox hb, Color hitboxColor);

        int isEnough();

        void displayInfos();

        // Accès à l’inventaire
        Inventory& inventory() { return this->inv; }
};

#endif // ENTITIES_H