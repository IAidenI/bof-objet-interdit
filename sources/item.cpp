#include "item.hpp"
#include "entity.hpp"

// ---- Objets ----
Item::Item(long long id, const char *name, long long max_amount, Hitbox hb, Color hitboxColor) : Entity(name, hb, hitboxColor) {
    this->id = id;
    this->max_amount = max_amount;
}

Item getItem(ItemType item) {
    switch (item) {
        case POTATO: return Item(ID_POTATO, "Patate", MAX_POTATO, {{0, 0}, 20}, BROWN);
        case CARROT: return Item(ID_CARROT, "Carotte", MAX_CARROT, {{0, 0}, 20}, ORANGE);
        case APPLE:  return Item(ID_APPLE, "Pomme", MAX_APPLE, {{0, 0}, 20}, GREEN);
        default:     return Item(0, "None", 0, {{0, 0}, 0}, MAGENTA);
    }
}
