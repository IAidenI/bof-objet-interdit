#ifndef GDB_H
#define GDB_H

#include <iostream>
#include <cstdint>
using namespace std;

class GDB {
    private:
        uintptr_t rbp_address;
        
        uintptr_t positionX;
        uintptr_t positionY;
        uintptr_t weight;
        uintptr_t height;
        uintptr_t name;
        uintptr_t inventory;
    
    public:
        GDB(uintptr_t rbp_main, uint16_t rbp_address);

        int getIntFromAddress(uintptr_t p);
        char* getStringFromAddress(uintptr_t p);
        void displayStack();
};

#endif // GDB_H