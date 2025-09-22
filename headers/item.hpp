#ifndef ITEM_H
#define ITEM_H

#include "entity.hpp"

// Id pour les items
#define ID_NONE 0
#define ID_POTATO 65
#define ID_CARROT 66
#define ID_APPLE  67

// Taille maximal autorisé dans l'inventaire pour les items
#define MAX_VOID   0
#define MAX_POTATO 32
#define MAX_CARROT 16
#define MAX_APPLE  1

#define ITEM_POTATO_TEXTURE "assets/player.png"
#define ITEM_CARROT_TEXTURE "assets/player.png"
#define ITEM_APPLE_TEXTURE  "assets/player.png"

// ---- Structure pour les objets ----
typedef enum {
    POTATO,
    CARROT,
    APPLE
} ItemType;

// ---- Classe Objets ----
class Item : public Entity {
    private:
        int id;
        int max_amount;

    public:
        // Constructeur par défaut
        Item() = default;

        Item(int id, const char *name, long long max_amount, Hitbox hb, const char *texture);

        // Pas de copie
        Item(const Item&) = delete;
        Item& operator=(const Item&) = delete;

        // Move
        Item(Item&&) noexcept = default;
        Item& operator=(Item&&) noexcept = default;

        // Getters
        const int getId() const { return this->id; };
        const int getMaxAmount() const { return this->max_amount; };
};

Item getItem(ItemType item);

#endif // ITEM_H