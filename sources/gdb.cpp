#include "../headers/gdb.hpp"

GDB::GDB(uintptr_t rbp_address){
    this->rbp_address = rbp_address;
}

string GDB::getAddress(int offset) {
    return format("0x{:0{}x}", this->rbp_address + offset, 2 * sizeof(uintptr_t));
}

string GDB::getValue(int offset) {
    return format("0x{:0{}x}", *reinterpret_cast<int*>(this->rbp_address + offset), sizeof(uintptr_t));
}

string GDB::getValueASCII(int offset) {
    return format("0x{:0{}x}", *reinterpret_cast<int*>(this->rbp_address + offset), sizeof(uintptr_t));
}

vector<vector<InfoSegment>> GDB::getFormatedStack(const TextStyle& style) {
    vector<vector<InfoSegment>> stack = {};
    int block = static_cast<int>(sizeof(uintptr_t) / 2); // taille d’un mot
    int valuesPerLine = 4;

    for (size_t offset = 0; offset < STACK_SIZE; offset += block * valuesPerLine) {
        vector<InfoSegment> line = {};

        // ---- Adresse de début de ligne ----
        line.push_back({
            { style.font, this->getAddress(offset), style.fontSize, style.spacing, COLOR_STACK_ADDR }
        });

        line.push_back({
            { style.font, ": ", style.fontSize, style.spacing, COLOR_STACK_TEXT }
        });

        // ---- Concaténation des 4 valeurs ----
        string lineValues;
        string lineASCII;
        for (int i = 0; i < valuesPerLine; i++) {
            size_t currentOffset = offset + i * block;
            if (currentOffset >= STACK_SIZE) break;
            lineValues += this->getValue(currentOffset);
            lineASCII += "";
            if (i < valuesPerLine - 1) lineValues += " "; // espace entre valeurs
        }

        line.push_back({
            { style.font, lineValues, style.fontSize, style.spacing, COLOR_STACK_HEXA }
        });

        // ---- Ajout des valeurs ASCII ----

        // Ajout de la ligne
        stack.push_back(line);
    }

    return stack;
}

void GDB::displayInfoSegment(const std::vector<std::vector<InfoSegment>>& content) {
    for (const auto& line : content) {
        std::cout << "        { \n";
        for (const auto& seg : line) {
            std::cout << "            { { &this->getFont(INFO_LABEL, "
                      << seg.textStyle.fontSize << "), "
                      << "\"" << seg.textStyle.text << "\", "
                      << std::fixed << std::setprecision(1)
                      << seg.textStyle.fontSize << "f, "
                      << seg.textStyle.spacing << "f, "
                      << "COLOR_STACK_XXX"
                      << " } },\n";
        }
        std::cout << "        },\n\n";
    }
}

void GDB::displayStack() {
    int block = static_cast<int>((sizeof(uintptr_t) / 2));
    for (size_t offset = 0; offset < STACK_SIZE; offset += block) {
        if (offset % (block * 4) == 0) {
            if (offset != 0) cout << endl;
            cout << this->getAddress(offset) << ": ";
        }
        cout << this->getValue(offset) << " ";
    }
}
