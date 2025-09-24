#include "../headers/inventory.hpp"

// ---- Inventory ----
Inventory::Inventory() {
    this->removeAll();
}

/*
    /!\ Fonction vulnérable avec strcpy /!\
*/
void Inventory::changeItemName(const char *newName, int index) {
    this->inventory[index].first.changeName(newName);
}

ReturnCode Inventory::add(Item newItem, int amount) {
    int firstEmpty = -1;
    
    // Parcours tout l'inventaire
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        auto& slot = this->inventory[i].first;
        auto& slot_size = this->inventory[i].second;

        // Récupère l'index du premier slot vide
        if (slot.getId() == ID_NONE) {
            if (firstEmpty == -1) firstEmpty = i;
            continue;
        }

        // Si un slot est déjà utilisé alors on stack
        if (slot.getId() == newItem.getId()) {
            if (slot_size == slot.getMaxAmount()) continue; // Indique que le slot est complet
            slot_size = min(slot_size + amount, slot.getMaxAmount()); // Prend le plus petit entre le maximum autorisé et la nouvelle valeur, pour évité d'avoir plus que le maximum
            return OK;
        }
    }

    // Ajoute au slot suivant le nouvelle item
    if (firstEmpty != -1) {
        auto& slot = this->inventory[firstEmpty].first;
        auto& slot_size = this->inventory[firstEmpty].second;
        
        slot = newItem;
        slot_size = min(amount, slot.getMaxAmount());
        return OK;
    }

    // Inventaire plein
    return INVENTORY_LENGTH_MAX_REACH;
}

void Inventory::remove(int id, int amount) {
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        auto& slot = this->inventory[i].first;
        auto& slot_size = this->inventory[i].second;
        
        if (slot.getId() == id) {
            // Supprime la quantité demandé
            const int before = slot_size;
            slot_size = max(slot_size - amount, 0); // Pour pas avoir des nombres négatifs
            amount = max(amount - (before - slot_size), 0);
            
            if (slot_size == 0) slot = Item{}; // Si il n'y a plus d'une valeur, libère le slot
            if (before == 0) return; // Sort une fois la quantité enlevé
        }
    }
}

void Inventory::removeAll() {
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        this->inventory[i].first = Item{};
        this->inventory[i].second = 0;
    }
}

bool Inventory::hasEnoughOf(int id, int needed) {
    return this->getItemQuantity(id) >= needed;
}

int Inventory::getItemQuantity(int id) {
    // Regarde dans tout l'inventaire si il y a assez d'un certain item.
    int quantity = 0;
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        if (this->inventory[i].first.getId() == id) {
            quantity += this->inventory[i].second;
        }
    }
    return quantity;
}

bool Inventory::isEmpty() {
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        if (this->inventory[i].first.getId() != ID_NONE) {
            return false;
        }
    }
    return true;
}

void Inventory::display() {
    cout << "Inventaire du joueur :" << endl;
    int rowSize = static_cast<int>(std::sqrt(MAX_INVENTORY_LENGTH));

    printf("[ DEBUG ] id : %d\n", this->inventory[0].first.getId());

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < rowSize; j++) {
            int idx = i * rowSize + j;
            const Item& item = this->inventory[idx].first;
            int amount = this->inventory[idx].second;
            cout << "[ " << left << setw(7) << item.getName() << " (x" << setw(2) << amount << ") ] ";
        }
        cout << endl;
    }
}
