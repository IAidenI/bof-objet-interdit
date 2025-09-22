#include "item.hpp"
#include "entity.hpp"

// ---- Objets ----
Item::Item(int id, const char *name, long long max_amount, Hitbox hb, const char *texture) : Entity(name, hb, texture) {
    this->id = id;
    this->max_amount = max_amount;
}

Item getItem(ItemType item) {
    switch (item) {
        case POTATO: return Item(ID_POTATO, "Patate",  MAX_POTATO, {{0, 0}, 20}, ITEM_POTATO_TEXTURE);
        case CARROT: return Item(ID_CARROT, "Carotte", MAX_CARROT, {{0, 0}, 20}, ITEM_CARROT_TEXTURE);
        case APPLE:  return Item(ID_APPLE,  "Pomme",   MAX_APPLE,  {{0, 0}, 20}, ITEM_APPLE_TEXTURE);
        default:     return Item{};
    }
}
