#include "../headers/entities.hpp"

// ---- Entity ----
Entity::Entity(Hitbox hb) {
    this->hitbox = hb;
}

// ---- Inventory ----
Inventory::Inventory() {
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        inventory[i] = {0, ITEM_NONE};
    }
}

ReturnCode Inventory::add(Item *newItem) {
    int firstEmpty = -1;
    
    // Parcours tout l'inventaire
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        auto& slot = this->inventory[i];

        // Récupère l'index du premier slot vide
        if (slot.type == ITEM_NONE) {
            if (firstEmpty == -1) firstEmpty = i;
            continue;
        }

        // Si un slot est déjà utilisé alors on stack
        if (slot.type == newItem->type) {
            if (slot.quantity == MAX_INVENTORY_ITEM) continue; // Indique que le slot est complet

            const int before = slot.quantity;
            slot.quantity = min(slot.quantity + newItem->quantity, MAX_INVENTORY_ITEM); // Mets au maximum si il est atteint
            newItem->quantity = newItem->quantity - (slot.quantity - before); // Supprime au nouvelle item la quantité ajouté dans l'inventaire
            return (slot.quantity > before) ? OK : INVENTORY_ITEM_MAX_REACH;
        }
    }

    // Ajoute au slot suivant le nouvelle item
    if (firstEmpty != -1) {
        if (newItem->quantity > MAX_INVENTORY_ITEM) {
            // Remplit le slot de cette item
            this->inventory[firstEmpty].type = newItem->type;
            this->inventory[firstEmpty].quantity = MAX_INVENTORY_ITEM;

            // Supprime ce qui à été ajouté
            newItem->quantity -= MAX_INVENTORY_ITEM;
        } else {
            // Remplit l'inventaire avec le nouvelle item
            this->inventory[firstEmpty] = *newItem;
            newItem->quantity = 0;
        }
        return OK;
    }

    // Inventaire plein
    return INVENTORY_LENGTH_MAX_REACH;
}

int Inventory::hasEnoughOf(Item item, const int needed) {
    // Regarde dans tout l'inventaire si il y a assez d'un certain item.
    int quantity = 0;
    for (int i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        if (this->inventory[i].type == item.type) {
            quantity += this->inventory[i].quantity;
        }
    }
    return quantity >= needed;
}

void Inventory::display() {
    cout << "Inventaire du joueur :" << endl;
    int rowSize = MAX_INVENTORY_LENGTH / 4;

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < rowSize; j++) {
            int idx = i * rowSize + j;
            switch (this->inventory[idx].type) {
                case ITEM_NONE:
                    cout << "[         ] ";
                    break;
                case ITEM_X:
                    cout << "[ X (x" << setw(2) << this->inventory[idx].quantity << ") ] ";
                    break;
                case ITEM_Y:
                    cout << "[ Y (x" << setw(2) << this->inventory[idx].quantity << ") ] ";
                    break;
                default:
                    cout << "[ ? (x" << setw(2) << this->inventory[idx].quantity << ") ] ";
                    break;
            }
        }
        cout << endl;
    }
}

// ---- Player ----
Player::Player(const char *name, Hitbox hb) : Entity(hb) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strncpy(this->name, name, MAX_NAME_LENGTH);
    this->name[MAX_NAME_LENGTH - 1] = '\0';
}

/*
    /!\ Fonction vulnérable avec strcpy /!\
*/
void Player::changeName(const char *newName) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strcpy(this->name, newName);
}

void Player::displayInfos() {
    cout << endl;
    cout << "Nom du joueur : " << this->name << endl;
    cout << "Position du joueur : X=" << this->getHitbox().pos.x << " ; Y=" << this->getHitbox().pos.y << endl;
    this->inv.display();
}
