#include "item.hpp"
#include "entity.hpp"

// ---- Objets ----
Item::Item(int id, const char *name, long long max_amount, Hitbox hb) : Entity(name, hb) {
    this->id = id;
    this->max_amount = max_amount;
}

void Item::displayInfos() {
    cout << endl;
    cout << "[ ITEM ]" << endl;
    cout << "Nom : " << this->getName() << endl;
    cout << "Position : X=" << this->getHitbox().pos.x << " ; Y=" << this->getHitbox().pos.y << endl;
    cout << "Id : " << this->getId() << endl;
    cout << "Max Amount : " << this->getMaxAmount() << endl;
    cout << endl;
}

Item getItem(ItemType item) {
    switch (item) {
        case POTATO: return Item(ID_POTATO, "Patate",  MAX_POTATO, { {0.0, 0.0}, 30 });
        case CARROT: return Item(ID_CARROT, "Carotte", MAX_CARROT, { {1.0, 1.0}, 30 });
        case APPLE:  return Item(ID_APPLE,  "Pomme",   MAX_APPLE,  { {0.0, 0.0}, 30 });
        default:     return Item{};
    }
}
