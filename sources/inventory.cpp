#include "../headers/inventory.hpp"

// ---- Inventory ----
Inventory::Inventory() {
    this->removeAll();
}

ReturnCode Inventory::add(Item newItem, int amount) {
    int firstEmpty = -1;
    
    // Parcours tout l'inventaire
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        auto& slot = this->inventory[i].first;
        auto& slot_size = this->inventory[i].second;

        // Récupère l'index du premier slot vide
        if (slot.id == ID_NONE) {
            if (firstEmpty == -1) firstEmpty = i;
            continue;
        }

        // Si un slot est déjà utilisé alors on stack
        if (slot.id == newItem.id) {
            if (slot_size == slot.max_amount) continue; // Indique que le slot est complet
            slot_size = min(slot_size + amount, (int)slot.max_amount); // Prend le plus petit entre le maximum autorisé et la nouvelle valeur, pour évité d'avoir plus que le maximum
            return OK;
        }
    }

    // Ajoute au slot suivant le nouvelle item
    if (firstEmpty != -1) {
        auto& slot = this->inventory[firstEmpty].first;
        auto& slot_size = this->inventory[firstEmpty].second;
        
        slot = newItem;
        slot_size = min(amount, (int)slot.max_amount);
        return OK;
    }

    // Inventaire plein
    return INVENTORY_LENGTH_MAX_REACH;
}

void Inventory::remove(Item item, int amount) {
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        auto& slot = this->inventory[i].first;
        auto& slot_size = this->inventory[i].second;
        
        if (slot.id == item.id) {
            // Supprime la quantité demandé
            const int before = (int)slot_size;
            slot_size = max((int)slot_size - amount, 0); // Pour pas avoir des nombres négatifs
            amount = max(amount - (before - (int)slot_size), 0);
            
            if (slot_size == 0) slot = ITEM[VOID]; // Si il n'y a plus d'une valeur, libère le slot
            if (before == 0) return; // Sort une fois la quantité enlevé
        }
    }
}

void Inventory::removeAll() {
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        this->inventory[i].first = ITEM[VOID];
        this->inventory[i].second = 0;
    }
}

int Inventory::hasEnoughOf(Item item, int needed) {
    return this->getItemQuantity(item) >= needed;
}

int Inventory::getItemQuantity(Item item) {
    // Regarde dans tout l'inventaire si il y a assez d'un certain item.
    int quantity = 0;
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        if (this->inventory[i].first.id == item.id) {
            quantity += this->inventory[i].second;
        }
    }
    return quantity;
}

void Inventory::display() {
    cout << "Inventaire du joueur :" << endl;
    int rowSize = static_cast<int>(sqrt(MAX_INVENTORY_LENGTH));

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < rowSize; j++) {
            int idx = i * rowSize + j;
            auto item = this->inventory[idx].first;
            auto amount = this->inventory[idx].second;
            cout << "[ " << left << setw(7) << item.name << " (x" << setw(2) << amount << ") ] ";
        }
        cout << endl;
    }
}