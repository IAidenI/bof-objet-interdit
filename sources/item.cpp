#include "item.hpp"
#include "entity.hpp"

// ---- Objets ----
Item::Item(int id, const char *name, long long max_amount, Hitbox hb) : Entity(name, hb) {
    this->id = id;
    this->max_amount = max_amount;
}

Item getItem(ItemType item) {
    switch (item) {
        case POTATO: return Item(ID_POTATO, "Patate",  MAX_POTATO, {{5, 5}, 30});
        case CARROT: return Item(ID_CARROT, "Carotte", MAX_CARROT, {{10, 10}, 30});
        case APPLE:  return Item(ID_APPLE,  "Pomme",   MAX_APPLE,  {{0, 0}, 30});
        default:     return Item{};
    }
}
