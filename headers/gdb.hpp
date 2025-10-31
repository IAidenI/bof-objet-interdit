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

class GDB {
    private:
        uintptr_t rbp_address;

    public:
        GDB(uintptr_t rbp_address);

        string getAddress(int offset);
        string getValue(int offset);
        string getValueASCII(int offset);

        vector<vector<InfoSegment>> getFormatedStack(const TextStyle& style);
        void displayInfoSegment(const std::vector<std::vector<InfoSegment>>& content);
        void displayStack();
};

#endif // GDB_H