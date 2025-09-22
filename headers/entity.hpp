#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <cstring>
#include <memory>
using namespace std;

#include "game.hpp"

#define MAX_NAME_LENGTH 16

// ---- Classe Entity ----
class Entity {
    private:
        Hitbox hitbox;
        Texture2D texture;
        char name[MAX_NAME_LENGTH];

    public:
        // Constructeur par défaut
        Entity() : hitbox({{0,0},0}), texture{0} { name[0] = '\0'; }
        
        Entity(const char *name, Hitbox hb, const char *texture);

        // Interdire la copie
        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        // Autoriser le move
        Entity(Entity&& other) noexcept;
        Entity& operator=(Entity&& other) noexcept;

        virtual ~Entity();

        // Setter
        void changeName(const char *newName); // Vulnérabilité
        void setPosition(Vector2 pos) { this->hitbox.pos = pos; };
        void setPosX(int x) { this->hitbox.pos.x = x; };
        void setPosY(int y) { this->hitbox.pos.y = y; };

        // Getter
        const Hitbox getHitbox() const { return this->hitbox; };
        const char *getName() const { return this->name; };
        const Texture2D getTexture() const { return this->texture; };
};

#endif // ENTITIES_H