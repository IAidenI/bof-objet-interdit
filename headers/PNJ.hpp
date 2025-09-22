#ifndef PNJ_H
#define PNJ_H

#include "entity.hpp"

#define PNJ_FARMER_TEXTURE   "assets/farmer.png"
#define PNJ_GUARD_TEXTURE    "assets/guard.png"
#define PNJ_SORCERER_TEXTURE "assets/player.png"

// ---- Textures ----
#define FARMER_FRAME_W  32
#define FARMER_FRAME_H  32
#define FARMER_COLS     4
#define FARMER_ROW_IDLE 0

#define GUARD_FRAME_W  32
#define GUARD_FRAME_H  32
#define GUARD_COLS     5
#define GUARD_ROW_IDLE 0

// ---- Classe PNJ ----
class PNJ : public Entity {    
    public:
        // Constructeur par défaut
        PNJ() = default;

        PNJ(const char *name, Hitbox hb, const char *texture);

        // Pas de copie
        PNJ(const PNJ&) = delete;
        PNJ& operator=(const PNJ&) = delete;

        // Move ok
        PNJ(PNJ&&) noexcept = default;
        PNJ& operator=(PNJ&&) noexcept = default;
};

#endif // PNJ_H