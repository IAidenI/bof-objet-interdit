#include "../headers/gdb.hpp"

GDB::GDB(uintptr_t rbp_main, uint16_t offset){
    // base = début de l’objet Player/Entity dans la stack
    uintptr_t base = rbp_main - offset;

    this->rbp_address = rbp_main;

    // Hitbox
    this->positionX = base + 0x00;
    this->positionY = base + 0x04;
    this->radius    = base + 0x08;

    // name[12] (aligné à 0x0c)
    this->name = base + 0x0c;

    // inventory (aligné à 0x20)
    this->inventory = base + 0x20;
}

uint8_t GDB::getUint8FromAddress(uintptr_t p) {
    return *reinterpret_cast<uint8_t *>(p);
}

int GDB::getIntFromAddress(uintptr_t p) {
    return *reinterpret_cast<int *>(p);
}

float GDB::getFloatFromAddress(uintptr_t p) {
    return *reinterpret_cast<float *>(p);
}

char* GDB::getStringFromAddress(uintptr_t p) {
    return reinterpret_cast<char*>(p);
}

void GDB::displayStack() {
    cout << " -- @RBP = 0x" << hex << this->rbp_address << " --" << endl;
    cout << "    @positionX = 0x" << hex << this->positionX << " ; value = " << dec << getFloatFromAddress(this->positionX) << endl;
    cout << "    @positionY = 0x" << hex << this->positionY << " ; value = " << dec << getFloatFromAddress(this->positionY) << endl;
    cout << "    @raduis = 0x" << hex << this->radius << " ; value = " << dec << getFloatFromAddress(this->radius) << endl;
    cout << "    @name = 0x" << hex << this->name << " ; value = " << getStringFromAddress(this->name) << endl;
    cout << "    @inventory = 0x" << hex << this->inventory << " ; value = " << dec << getIntFromAddress(this->inventory) << endl;
}
