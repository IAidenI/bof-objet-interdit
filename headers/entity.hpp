#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"
#include <iostream>
#include <cstring>
#include <memory>
using namespace std;


#define MAX_NAME_LENGTH 16

typedef struct {
    Vector2 pos;
    float radius;
} Hitbox;

// ---- Classe Entity ----
class Entity {
    private:
        Hitbox hitbox;
        char name[MAX_NAME_LENGTH];

    public:
        // Constructeur par défaut
        Entity() : hitbox({{0,0},0}) { name[0] = '\0'; }
        
        Entity(const char *name, Hitbox hb);

        // Setter
        void changeName(const char *newName); // Vulnérabilité
        void setPosition(Vector2 pos) { this->hitbox.pos = pos; };
        void setPosX(int x) { this->hitbox.pos.x = x; };
        void setPosY(int y) { this->hitbox.pos.y = y; };

        // Getter
        const Hitbox getHitbox() const { return this->hitbox; };
        const char *getName() const { return this->name; };
};

#endif // ENTITIES_H