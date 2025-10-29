#ifndef GDB_H
#define GDB_H

#include <sstream>
#include <iomanip> 
#include <iostream>
#include <cstdint>
#include "item.hpp"
using namespace std;

#define POS_X_OFFSET       0x00
#define POS_Y_OFFSET       0x04
#define RADIUS_OFFSET      0x08
#define NAME_OFFSET        0x0c
#define ID_OFFSET          0x1c
#define MAX_AMOUNT_OFFSET  0x20
#define SLOT_AMOUNT_OFFSET 0x24

class GDB {
    private:
        uintptr_t rbp_address;
        
        // Hitbox
        uintptr_t positionX;
        uintptr_t positionY;
        uintptr_t radius;

        uintptr_t name;
        uintptr_t inventory;

        const std::pair<Item,int>* slots() const { return reinterpret_cast<const std::pair<Item,int>*>(inventory); }

        string formatAddr(uintptr_t p) const;
    public:
        GDB(uintptr_t rbp_main);    

        uint8_t getUint8FromAddress(uintptr_t p) const;
        int getIntFromAddress(uintptr_t p) const;
        float getFloatFromAddress(uintptr_t p) const;
        string getStringFromAddress(uintptr_t p) const;
        void displayStack() const;

        float getItemPosX(int i)     const { return slots()[i].first.getHitbox().pos.x; }
        float getItemPosY(int i)     const { return slots()[i].first.getHitbox().pos.y; }
        float getItemRadius(int i)   const { return slots()[i].first.getHitbox().radius; }
        string getItemName(int i)    const { return string(slots()[i].first.getName()); }
        int getItemID(int i)         const { return slots()[i].first.getId(); }
        int getItemMaxAmount(int i)  const { return slots()[i].first.getMaxAmount(); }
        int getItemSlotAmount(int i) const { return slots()[i].second; }

        string getFormattedRBP()    const { return this->formatAddr(this->rbp_address); }
        string getFormattedX()      const { return this->formatAddr(this->positionX); }
        string getFormattedY()      const { return this->formatAddr(this->positionY); }
        string getFormattedRadius() const { return this->formatAddr(this->radius); }
        string getFormattedName()   const { return this->formatAddr(this->name); }
        string getFormattedInv()    const { return this->formatAddr(this->inventory); }

        int getPosXValue()    const { return (int)this->getFloatFromAddress(this->positionX); };
        int getPosYValue()    const { return (int)this->getFloatFromAddress(this->positionY); };
        int getRadiusValue()  const { return (int)this->getFloatFromAddress(this->radius); };
        string getNameValue() const { return this->getStringFromAddress(this->name); };
        int getInvValue()     const { return this->getIntFromAddress(this->inventory); };
};

#endif // GDB_H