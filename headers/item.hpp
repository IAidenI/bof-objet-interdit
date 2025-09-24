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

#define ITEM_POTATO_TEXTURE "assets/potato.png"

#define ITEM_POTATO_STATIC_TEXTURE "assets/potato_static.png"
#define ITEM_CARROT_STATIC_TEXTURE "assets/carrot.png"
#define ITEM_APPLE_STATIC_TEXTURE  "assets/apple.png"

// ---- Textures ----
#define POTATO_FRAME_W  19
#define POTATO_FRAME_H  19
#define POTATO_COLS     5
#define POTATO_ROW_IDLE 0

#define CARROT_FRAME_W  32
#define CARROT_FRAME_H  32
#define CARROT_COLS     4
#define CARROT_ROW_IDLE 0

#define APPLE_FRAME_W  32
#define APPLE_FRAME_H  32
#define APPLE_COLS     4
#define APPLE_ROW_IDLE 0

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

        Item(int id, const char *name, long long max_amount, Hitbox hb);

        // Getters
        const int getId() const { return this->id; };
        const int getMaxAmount() const { return this->max_amount; };
};

Item getItem(ItemType item);

#endif // ITEM_H