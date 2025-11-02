#ifndef GDB_H
#define GDB_H

#include <cstdint>
#include <format>
#include <string>
#include "player.hpp"
#include "colors.hpp"
#include "raylib-utils.hpp"
using namespace std;

inline constexpr size_t STACK_SIZE = sizeof(Player);

// ---- Offset de la stack pour le joueur ----
inline constexpr size_t PLAYER_POS_X_OFFSET  = 0x00;
inline constexpr size_t PLAYER_POS_Y_OFFSET  = PLAYER_POS_X_OFFSET  + sizeof(float);
inline constexpr size_t PLAYER_RADIUS_OFFSET = PLAYER_POS_Y_OFFSET  + sizeof(float);
inline constexpr size_t PLAYER_NAME_OFFSET   = PLAYER_RADIUS_OFFSET + sizeof(float);

// ---- Offsets de référence pour l'inventaire ----
inline constexpr size_t SLOT_1_ITEM_POS_X_OFFSET          = PLAYER_NAME_OFFSET             + sizeof(char) * MAX_NAME_LENGTH;
inline constexpr size_t SLOT_1_ITEM_POS_Y_OFFSET          = SLOT_1_ITEM_POS_X_OFFSET       + sizeof(float);
inline constexpr size_t SLOT_1_ITEM_RADIUS_OFFSET         = SLOT_1_ITEM_POS_Y_OFFSET       + sizeof(float);
inline constexpr size_t SLOT_1_ITEM_NAME_OFFSET           = SLOT_1_ITEM_RADIUS_OFFSET      + sizeof(float);
inline constexpr size_t SLOT_1_ITEM_ID_OFFSET             = SLOT_1_ITEM_NAME_OFFSET        + sizeof(char) * MAX_NAME_LENGTH;
inline constexpr size_t SLOT_1_ITEM_MAX_AMOUNT_OFFSET     = SLOT_1_ITEM_ID_OFFSET          + sizeof(int);
inline constexpr size_t SLOT_1_ITEM_CURRENT_AMOUNT_OFFSET = SLOT_1_ITEM_MAX_AMOUNT_OFFSET  + sizeof(int);

// ---- Deltas intra-slot (par rapport à la base du slot) ----
inline constexpr size_t ITEM_POS_X_D          = 0;
inline constexpr size_t ITEM_POS_Y_D          = ITEM_POS_X_D          + sizeof(float);
inline constexpr size_t ITEM_RADIUS_D         = ITEM_POS_Y_D          + sizeof(float);
inline constexpr size_t ITEM_NAME_D           = ITEM_RADIUS_D         + sizeof(float);
inline constexpr size_t ITEM_ID_D             = ITEM_NAME_D           + sizeof(char) * MAX_NAME_LENGTH;
inline constexpr size_t ITEM_MAX_AMOUNT_D     = ITEM_ID_D             + sizeof(int);
inline constexpr size_t ITEM_CURRENT_AMOUNT_D = ITEM_MAX_AMOUNT_D     + sizeof(int);

// ---- Stride d’un slot (taille totale) ----
inline constexpr size_t SLOT_STRIDE = ITEM_CURRENT_AMOUNT_D + sizeof(int);

// ---- Accesseurs génériques pour n’importe quel slot i (0..8) ----
constexpr size_t slot_base(int i) { return SLOT_1_ITEM_POS_X_OFFSET + static_cast<size_t>(i) * SLOT_STRIDE; }

constexpr size_t SLOT_ITEM_POS_X_OFFSET(int i)          { return slot_base(i) + ITEM_POS_X_D; }
constexpr size_t SLOT_ITEM_POS_Y_OFFSET(int i)          { return slot_base(i) + ITEM_POS_Y_D; }
constexpr size_t SLOT_ITEM_RADIUS_OFFSET(int i)         { return slot_base(i) + ITEM_RADIUS_D; }
constexpr size_t SLOT_ITEM_NAME_OFFSET(int i)           { return slot_base(i) + ITEM_NAME_D; }
constexpr size_t SLOT_ITEM_ID_OFFSET(int i)             { return slot_base(i) + ITEM_ID_D; }
constexpr size_t SLOT_ITEM_MAX_AMOUNT_OFFSET(int i)     { return slot_base(i) + ITEM_MAX_AMOUNT_D; }
constexpr size_t SLOT_ITEM_CURRENT_AMOUNT_OFFSET(int i) { return slot_base(i) + ITEM_CURRENT_AMOUNT_D; }

enum FieldType {
    Float,
    String,
    Int
};

struct FieldDesc {
    size_t offset;
    string title;
    FieldType type;
};

static vector<FieldDesc> makeFields() {
    vector<FieldDesc> out;

    // Champs player
    out.push_back({ PLAYER_POS_X_OFFSET,  "Player Position X",  FieldType::Float });
    out.push_back({ PLAYER_POS_Y_OFFSET,  "Player Position Y",  FieldType::Float });
    out.push_back({ PLAYER_RADIUS_OFFSET, "Player Radius",      FieldType::Float });
    out.push_back({ PLAYER_NAME_OFFSET,   "Player Name",        FieldType::String });

    // Inventaire : 9 slots
    out.reserve(out.size() + MAX_INVENTORY_LENGTH * 7);
    for (int i = 0; i < MAX_INVENTORY_LENGTH; ++i) {
        const int n = i + 1;

        out.push_back({ SLOT_ITEM_POS_X_OFFSET(i),          "Slot " + to_string(n) + " Item Pos X",      FieldType::Float  });
        out.push_back({ SLOT_ITEM_POS_Y_OFFSET(i),          "Slot " + to_string(n) + " Item Pos Y",      FieldType::Float  });
        out.push_back({ SLOT_ITEM_RADIUS_OFFSET(i),         "Slot " + to_string(n) + " Item Radius",     FieldType::Float  });
        out.push_back({ SLOT_ITEM_NAME_OFFSET(i),           "Slot " + to_string(n) + " Item Name",       FieldType::String });
        out.push_back({ SLOT_ITEM_ID_OFFSET(i),             "Slot " + to_string(n) + " Item ID",         FieldType::Int    });
        out.push_back({ SLOT_ITEM_MAX_AMOUNT_OFFSET(i),     "Slot " + to_string(n) + " Max Amount",      FieldType::Int    });
        out.push_back({ SLOT_ITEM_CURRENT_AMOUNT_OFFSET(i), "Slot " + to_string(n) + " Current Amount",  FieldType::Int    });
    }
    return out;
}

static const vector<FieldDesc> FIELDS = makeFields();

static inline const FieldDesc* findField(size_t offset) {
    for (const auto& field : FIELDS) if (field.offset == offset) return &field;
    return nullptr;
}

class GDB {
    private:
        uintptr_t rbp_address;

        template <typename T>
        T readAt(uintptr_t base, int offset);
        string getValueInt(int offset);
        string getValueFloat(int offset, int precision = 2);
        string getAddress(int offset);
        string getValueHex(int offset);
        string getValueString(int offset, size_t maxLen = 32);

    public:
        GDB(uintptr_t rbp_address);

        vector<vector<InfoSegment>> getFormatedStack(const TextStyle& style);
        vector<vector<InfoSegment>> getMoreInfo(size_t offset, const TextStyle& titleStyle, const TextStyle& contentStyle);
        void displayInfoSegment(const std::vector<std::vector<InfoSegment>>& content);
        void displayStack();
};

#endif // GDB_H