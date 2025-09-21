#include "../headers/gdb.hpp"

GDB::GDB(uintptr_t rbp_main, uint16_t offset){
    // base = début de l’objet Player/Entity dans la stack
    uintptr_t base = rbp_main - offset;

    this->rbp_address = rbp_main;

    // Hitbox
    this->positionX = base + 0x00;
    this->positionY = base + 0x04;
    this->radius    = base + 0x08;

    // Color (RGBA sur 4 octets)
    this->color = base + 0x0C;
    this->r = color + 0x00;
    this->g = color + 0x01;
    this->b = color + 0x02;
    this->a = color + 0x03;  // si tu veux l’afficher aussi

    // name[12] (aligné à 0x10)
    this->name = base + 0x10;

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
    cout << "    @color = 0x" << hex << this->color << " ; value = (" << dec << (int)getUint8FromAddress(this->r) << ", " << (int)getUint8FromAddress(this->g) << ", " << (int)getUint8FromAddress(this->b) << ", " << (int)getUint8FromAddress(this->a) << ")\n";
    cout << "    @name = 0x" << hex << this->name << " ; value = " << getStringFromAddress(this->name) << endl;
    cout << "    @inventory = 0x" << hex << this->inventory << " ; value = " << dec << getIntFromAddress(this->inventory) << endl;
}
