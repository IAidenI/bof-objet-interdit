#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <array>
#include <utility>
#include <cmath>
#include <iomanip>
using namespace std;

#include "item.hpp"

#define MAX_INVENTORY_LENGTH 9
#define UI_INVENTORY_WIDTH 200
#define UI_INVENTORY_HEIGHT 200

#define INVENTORY_BACKGROUND_COLOR CLITERAL(Color){ 84, 61, 89, 255 }

#define INVENTORY_TEXTURE "assets/inventory.png"

typedef enum {
    // Globale
    OK,
    // Inventaire
    INVENTORY_LENGTH_MAX_REACH,
} ReturnCode;

// ---- Classe Inventory ----
class Inventory {
    private:
        array<pair<Item, int>, MAX_INVENTORY_LENGTH> inventory;
    
    public:
        Inventory();

        void changeItemName(const char *newName, int index);

        ReturnCode add(Item item, int amount);
        void remove(int id, int amount);
        void removeAll();

        bool hasEnoughOf(int id, int needed);
        int getItemQuantity(int id);
        
        void display(); 

        Item getItem(int idx) { return this->inventory[idx].first; };
};

#endif // INVENTORY_H