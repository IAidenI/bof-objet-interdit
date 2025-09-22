#ifndef ITEM_H
#define ITEM_H

#include "entity.hpp"

// Id pour les items
#define ID_NONE 0
#define ID_POTATO 65
#define ID_CARROT 66
#define ID_APPLE  67

// Taille maximal autorisé dans l'inventaire pour les items
#define MAX_POTATO 32
#define MAX_CARROT 16
#define MAX_APPLE  1

// ---- Structure pour les objets ----
typedef enum {
    VOID,
    POTATO,
    CARROT,
    APPLE
} ItemType;

// ---- Classe Objets ----
class Item : public Entity {
    private:
        // Long long pour mettre sur 8 octets et rendre plus lisible sur gdb
        long long id;
        long long max_amount;

    public:
        // Constructeur par défaut
        Item() = default;

        Item(long long id, const char *name, long long max_amount, Hitbox hb, Color hitboxColor);

        // Getters
        const long long getId() const { return this->id; };
        const long long getMaxAmount() const { return this->max_amount; };
};

Item getItem(ItemType item);

#endif // ITEM_H