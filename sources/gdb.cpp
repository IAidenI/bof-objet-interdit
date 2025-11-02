#include "../headers/gdb.hpp"

GDB::GDB(uintptr_t rbp_address){
    this->rbp_address = rbp_address;
}

template <typename T>
T GDB::readAt(uintptr_t base, int offset) {
    T v{};
    memcpy(&v, reinterpret_cast<const void*>(base + offset), sizeof(T));
    return v;
}

string GDB::getAddress(int offset) {
    uintptr_t addr = static_cast<uintptr_t>(this->rbp_address) + static_cast<uintptr_t>(offset);
    return format("0x{:0{}x}", addr, 2 * sizeof(uintptr_t));
}

string GDB::getValueHex(int offset) {
    uint32_t v = readAt<uint32_t>(this->rbp_address, offset);
    return format("0x{:0{}x}", v, 2 * sizeof(uint32_t));
}

string GDB::getValueInt(int offset) {
    int value = readAt<int>(this->rbp_address, offset);
    return format("{}", value);
}

string GDB::getValueFloat(int offset, int precision) {
    float value = readAt<float>(this->rbp_address, offset);
    if (precision < 0) precision = 0;
    if (precision > 9) precision = 9;
    return format("{:.{}f}", value, precision);
}

string GDB::getValueString(int offset, size_t maxLen) {
    const char* p = reinterpret_cast<const char*>(this->rbp_address + offset);
    string out;
    out.reserve(maxLen);
    for (size_t i = 0; i < maxLen; ++i) {
        unsigned char c = static_cast<unsigned char>(p[i]);
        if (c == '\0') break;
        out.push_back(std::isprint(c) ? static_cast<char>(c) : '.');
    }
    return out;
}

/*string GDB::getAddress(int offset) {
    return format("0x{:0{}x}", this->rbp_address + offset, 2 * sizeof(uintptr_t));
}

string GDB::getValueHex(int offset) {
    return format("0x{:0{}x}", *reinterpret_cast<int*>(this->rbp_address + offset), sizeof(uintptr_t));
}*/

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

        // ---- Ajout d'une ligne par valeur ----
        for (int i = 0; i < valuesPerLine; i++) {
            size_t currentOffset = offset + i * block;
            if (currentOffset >= STACK_SIZE) break;

            size_t realOffset = currentOffset;
            if (currentOffset > PLAYER_NAME_OFFSET && currentOffset < SLOT_1_ITEM_POS_X_OFFSET) currentOffset = PLAYER_NAME_OFFSET; 

            // Ajout de la valeur
            line.push_back({
                { style.font, this->getValueHex(realOffset), style.fontSize, style.spacing, COLOR_STACK_HEXA },
                currentOffset
            });

            // Ajout de l'espace de séparation entre les valeurs
            line.push_back({
                { style.font, i < valuesPerLine - 1 ? " " : "", style.fontSize, style.spacing, COLOR_STACK_TEXT }
            });
        }
        
        // Ajout de la ligne
        stack.push_back(line);
    }

    return stack;
}

vector<vector<InfoSegment>> GDB::getMoreInfo(size_t offset, const TextStyle& titleStyle, const TextStyle& contentStyle) {
    vector<vector<InfoSegment>> moreInfo = {
        { { titleStyle } },

        { { contentStyle }, { contentStyle } },
        { { contentStyle }, { contentStyle } },
        { { contentStyle }, { contentStyle } },
    };

    moreInfo[0][0].textStyle.color = COLOR_TOOLTIP_TITLE;

    moreInfo[1][0].textStyle.text = "Type : ";
    moreInfo[1][0].textStyle.color = COLOR_TOOLTIP_LABEL;
    moreInfo[1][1].textStyle.color = COLOR_TOOLTIP_TYPE;

    moreInfo[2][0].textStyle.text = "Hexa : ";
    moreInfo[2][0].textStyle.color = COLOR_TOOLTIP_LABEL;
    moreInfo[2][1].textStyle.color = COLOR_TOOLTIP_HEXA;

    moreInfo[3][0].textStyle.text = "Valeur : ";
    moreInfo[3][0].textStyle.color = COLOR_TOOLTIP_LABEL;
    moreInfo[3][1].textStyle.color = COLOR_TOOLTIP_VALUE;

    if (const FieldDesc *field = findField(offset)) {
        moreInfo[0][0].textStyle.text = field->title;
        moreInfo[2][1].textStyle.text = this->getValueHex(offset);
        switch (field->type) {
            case Float: {
                moreInfo[1][1].textStyle.text = "float";
                moreInfo[3][1].textStyle.text = this->getValueFloat(offset);
                break;
            }
            case String: {
                moreInfo[1][1].textStyle.text = "char *";
                moreInfo[3][1].textStyle.text = this->getValueString(offset);
                break;
            }
            case Int: {
                moreInfo[1][1].textStyle.text = "int";
                moreInfo[3][1].textStyle.text = this->getValueInt(offset);
                break;
            }
            default:
                break;
        }
    } else {
        // Fallback inconnu
        moreInfo[0][0].textStyle.text  = "Inconnu";
        moreInfo[0][0].textStyle.color = COLOR_TOOLTIP_TITLE;

        moreInfo[1][1].textStyle.text  = "Inconnu";
        moreInfo[1][1].textStyle.color = COLOR_TOOLTIP_TYPE;

        moreInfo[2][1].textStyle.text  = "Inconnu";
        moreInfo[2][1].textStyle.color = COLOR_TOOLTIP_HEXA;

        moreInfo[3][1].textStyle.text  = "Inconnu";
        moreInfo[3][1].textStyle.color = COLOR_TOOLTIP_VALUE;
    }

    return moreInfo;
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
        cout << this->getValueHex(offset) << " ";
    }
}
