#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <array>
#include <utility>
#include <cmath>
#include <iomanip>
using namespace std;

#include "game.hpp"

#define MAX_INVENTORY_LENGTH 9

// Id pour les items
#define ID_NONE 0
#define ID_POTATO 65
#define ID_CARROT 66
#define ID_APPLE  67

// Taille maximal autorisé dans l'inventaire pour les items
#define MAX_POTATO 32
#define MAX_CARROT 16
#define MAX_APPLE  1

// ---- Structure pour les objets ----
typedef enum {
    VOID,
    POTATO,
    CARROT,
    APPLE
} ItemType;

// Long long pour mettre sur 8 octets et rendre plus lisible sur gdb
typedef struct {
    long long id;
    const char *name;
    long long max_amount;
} Item;

constexpr Item ITEM[] = {
    {0,  "None",    0},
    {65, "Patate",  MAX_POTATO},
    {66, "Carotte", MAX_CARROT},
    {67, "Pomme",   MAX_APPLE},
};

void displayItemInfo(Item item);

// ---- Classe Inventory ----
class Inventory {
    private:
        array<pair<Item, int>, MAX_INVENTORY_LENGTH> inventory;
    
    public:
        Inventory();

        ReturnCode add(Item newItem, int amount);
        void remove(Item item, int amount);
        void removeAll();

        int hasEnoughOf(Item item, int needed);
        int getItemQuantity(Item item);
        
        void display(); 
};

#endif // INVENTORY_H