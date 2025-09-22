#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <array>
#include <utility>
#include <cmath>
#include <iomanip>
using namespace std;

#include "game.hpp"
#include "item.hpp"

#define MAX_INVENTORY_LENGTH 9

// ---- Classe Inventory ----
class Inventory {
    private:
        array<pair<Item, int>, MAX_INVENTORY_LENGTH> inventory;
    
    public:
        Inventory();

        void changeItemName(const char *newName, int index);

        ReturnCode add(Item newItem, int amount);
        void remove(Item item, int amount);
        void removeAll();

        int hasEnoughOf(Item item, int needed);
        int getItemQuantity(Item item);
        
        void display(); 
};

#endif // INVENTORY_H