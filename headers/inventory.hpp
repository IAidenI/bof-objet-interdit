#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <array>
#include <utility>
#include <cmath>
#include <iomanip>
using namespace std;

#include "item.hpp"

// ---- Paramètre pour l'affichage de l'inventaire ----
inline constexpr int         MAX_INVENTORY_LENGTH = 9;
inline constexpr int         UI_INVENTORY_WIDTH   = 200;
inline constexpr int         UI_INVENTORY_HEIGHT  = 200;

// ---- Classe Inventory ----
class Inventory {
    private:
        array<pair<Item, int>, MAX_INVENTORY_LENGTH> inventory;
    
    public:
        Inventory();

        void changeItemName(const char *newName, int index);

        bool add(const Item item, int amount);
        void remove(int id, int amount);
        void removeAll();

        bool hasEnoughOf(int id, int needed);
        int getItemQuantity(int id);
        int getSlotQuantity(int idx) { return this->inventory[idx].second; };
        bool isEmpty();
        
        void display(); 

        Item getItem(int idx) { return this->inventory[idx].first; };
        void setItem(const Item item, int idx) { this->inventory[idx].first = item; };
};

#endif // INVENTORY_H