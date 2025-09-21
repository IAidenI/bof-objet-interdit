#include "../headers/gdb.hpp"

GDB::GDB(uintptr_t rbp_main, uint16_t offset){
    this->rbp_address = rbp_main;
    this->positionX = this->rbp_address - offset;
    this->positionY = this->positionX + 0x4;
    this->weight = this->positionY + 0x4;
    this->height = this->weight + 0x4;
    this->name = this->height + 0x4;
    this->inventory = this->name + 0x10;
}

int GDB::getIntFromAddress(uintptr_t p) {
    return *reinterpret_cast<int *>(p);
}

char* GDB::getStringFromAddress(uintptr_t p) {
    return reinterpret_cast<char*>(p);
}

void GDB::displayStack() {
    cout << " -- @RBP = 0x" << hex << this->rbp_address << " --" << endl;
    cout << "    @positionX = 0x" << hex << this->positionX << " ; value = " << dec << getIntFromAddress(this->positionX) << endl;
    cout << "    @positionY = 0x" << hex << this->positionY << " ; value = " << dec << getIntFromAddress(this->positionY) << endl;
    cout << "    @weight = 0x" << hex << this->weight << " ; value = " << dec << getIntFromAddress(this->weight) << endl;
    cout << "    @height = 0x" << hex << this->height << " ; value = " << dec << getIntFromAddress(this->height) << endl;
    cout << "    @name = 0x" << hex << this->name << " ; value = " << getStringFromAddress(this->name) << endl;
    cout << "    @inventory = 0x" << hex << this->inventory << " ; value = " << dec << getIntFromAddress(this->inventory) << endl;
}
