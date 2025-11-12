#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <cstring>
#include <memory>
#include "geometry.hpp"

inline constexpr int MAX_NAME_LENGTH = 16;

// ---- Classe Entity ----
class Entity {
    private:
        Hitbox hitbox;
        char name[MAX_NAME_LENGTH];

    public:
        // Constructeur par défaut
        Entity() : hitbox({ { 0, 0 }, 0 }) { name[0] = '\0'; }
        Entity(const char *name, Hitbox hb);

        // Setter
        void changeName(const char *newName); // Vulnérabilité
        void setPosition(Position pos) { this->hitbox.pos = pos; };
        void setPosX(float x) { this->hitbox.pos.x = x; };
        void setPosY(float y) { this->hitbox.pos.y = y; };

        // Getter
        Hitbox getHitbox() const { return this->hitbox; };
        const char *getName() const { return this->name; };
};

#endif // ENTITIES_H