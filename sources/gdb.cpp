#include "../headers/gdb.hpp"

GDB::GDB(uintptr_t rbp_main){
    this->rbp_address = rbp_main;

    // Hitbox
    this->positionX = this->rbp_address + 0x00;
    this->positionY = this->rbp_address + 0x04;
    this->radius    = this->rbp_address + 0x08;

    // name[12] (aligné à 0x0c)
    this->name = this->rbp_address + 0x0c;

    // inventory (aligné à 0x20)
    this->inventory = this->rbp_address + 0x1c;
}

uint8_t GDB::getUint8FromAddress(uintptr_t p) const {
    return *reinterpret_cast<uint8_t *>(p);
}

int GDB::getIntFromAddress(uintptr_t p) const {
    return *reinterpret_cast<int *>(p);
}

float GDB::getFloatFromAddress(uintptr_t p) const {
    return *reinterpret_cast<float *>(p);
}

string GDB::getStringFromAddress(uintptr_t p) const {
    return reinterpret_cast<char*>(p);
}

string GDB::formatAddr(uintptr_t p) const {
    ostringstream oss;
    oss << "0x" << hex << setw(sizeof(uintptr_t) * 2) << setfill('0') << p;
    return oss.str();
}

void GDB::displayStack() const {
    using std::cout;
    using std::endl;

    cout << " -- @RBP = " << this->getFormattedRBP() << " --" << endl;
    cout << "    @positionX = " << this->getFormattedX() << " ; value = " << this->getFloatFromAddress(this->positionX) << endl;
    cout << "    @positionY = " << this->getFormattedY() << " ; value = " << this->getFloatFromAddress(this->positionY) << endl;
    cout << "    @radius    = " << this->getFormattedRadius() << " ; value = " << this->getFloatFromAddress(this->radius) << endl;
    cout << "    @name      = " << this->getFormattedName() << " ; value = " << this->getStringFromAddress(this->name) << endl;
    cout << "    @inventory = " << this->getFormattedInv() << " ; value = " << this->getIntFromAddress(this->inventory) << endl;

    cout << "\n[ INVENTORY ]" << endl;

    // Exemple: afficher les 3 premiers slots
    for (int i = 0; i < 9; i++) {
        cout << "  Slot[" << i << "]" << endl;
        cout << "    posX       = " << getItemPosX(i) << endl;
        cout << "    posY       = " << getItemPosY(i) << endl;
        cout << "    radius     = " << getItemRadius(i) << endl;
        cout << "    name       = " << getItemName(i) << endl;
        cout << "    id         = " << getItemID(i) << endl;
        cout << "    max_amount = " << getItemMaxAmount(i) << endl;
        cout << "    qty        = " << getItemSlotAmount(i) << endl;
    }
}
