#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

#define BUFFER_SIZE 14     // Vulnérabilité
#define MAX_NAME_LENGTH 12 //

#define MAX_INVENTORY_LENGTH 16
#define MAX_INVENTORY_ITEM 64

// Différents items
#define ITEM_NONE 0
#define ITEM_X 65
#define ITEM_Y 66

// ---- Structures de base ----
typedef enum {
    // Globale
    NONE,
    OK,
    // Inventaire
    INVENTORY_LENGTH_MAX_REACH,
    INVENTORY_ITEM_MAX_REACH,
} ReturnCode;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    int width;
    int height;
} Hitbox;

typedef struct {
    int type;
    int quantity;
} Item;

// ---- Classe Entity ----
class Entity {
    private:
        Hitbox hitbox;

    public:
        Entity(Hitbox hb);

        const Hitbox getHitbox() const { return hitbox; };
};

// ---- Classe Inventory ----
class Inventory {
    private:
        Item inventory[MAX_INVENTORY_LENGTH];
    
    public:
        Inventory();

        ReturnCode add(Item *newItem);
        int hasEnoughOf(Item item, const int needed);
        void display();
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