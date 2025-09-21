#ifndef GDB_H
#define GDB_H

#include <iostream>
#include <cstdint>
using namespace std;

class GDB {
    private:
        uintptr_t rbp_address;
        
        // Hitbox
        uintptr_t positionX;
        uintptr_t positionY;
        uintptr_t radius;
        
        // Color
        uintptr_t color;
        uintptr_t r;
        uintptr_t g;
        uintptr_t b;
        uintptr_t a;

        uintptr_t name;
        uintptr_t inventory;
    
    public:
        GDB(uintptr_t rbp_main, uint16_t rbp_address);

        uint8_t getUint8FromAddress(uintptr_t p);
        int getIntFromAddress(uintptr_t p);
        float getFloatFromAddress(uintptr_t p);
        char* getStringFromAddress(uintptr_t p);
        void displayStack();
};

#endif // GDB_H