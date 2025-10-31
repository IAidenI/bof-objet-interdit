#ifndef ITEM_H
#define ITEM_H

#include "entity.hpp"

// Id pour les items
inline constexpr int ID_NONE = 0;
inline constexpr int ID_POTATO = 65;
inline constexpr int ID_CARROT = 66;
inline constexpr int ID_APPLE  = 67;

// Taille maximal autorisé dans l'inventaire pour les items
inline constexpr int MAX_VOID   = 0;
inline constexpr int MAX_POTATO = 32;
inline constexpr int MAX_CARROT = 16;
inline constexpr int MAX_APPLE  = 1;

// ---- Textures ----
inline constexpr int         POTATO_FRAME_W             = 19;
inline constexpr int         POTATO_FRAME_H             = 19;
inline constexpr int         POTATO_COLS                = 5;
inline constexpr int         POTATO_ROW_IDLE            = 0;
inline constexpr const char *POTATO_TEXTURE        = "assets/potato.png";
inline constexpr const char *POTATO_STATIC_TEXTURE = "assets/potato_static.png";

inline constexpr int         CARROT_FRAME_W             = 32;
inline constexpr int         CARROT_FRAME_H             = 32;
inline constexpr int         CARROT_COLS                = 4;
inline constexpr int         CARROT_ROW_IDLE            = 0;
inline constexpr const char *CARROT_STATIC_TEXTURE = "assets/carrot.png";

inline constexpr int         APPLE_FRAME_W              = 32;
inline constexpr int         APPLE_FRAME_H              = 32;
inline constexpr int         APPLE_COLS                 = 4;
inline constexpr int         APPLE_ROW_IDLE             = 0;
inline constexpr const char *APPLE_STATIC_TEXTURE  = "assets/apple.png";

// ---- Structure pour les objets ----
enum ItemType {
    POTATO,
    CARROT,
    APPLE
};

// ---- Classe Objets ----
class Item : public Entity {
    private:
        int id;
        int max_amount;

    public:
        // Constructeur par défaut
        Item() = default;

        Item(int id, const char *name, long long max_amount, Hitbox hb);

        void displayInfos();

        // Getters
        int getId() const { return this->id; };
        int getMaxAmount() const { return this->max_amount; };
};

Item getItem(ItemType item);

#endif // ITEM_H