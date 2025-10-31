#ifndef PNJ_H
#define PNJ_H

#include "entity.hpp"

// ---- Textures ----
inline constexpr int         FARMER_FRAME_W     = 32;
inline constexpr int         FARMER_FRAME_H     = 32;
inline constexpr int         FARMER_COLS        = 4;
inline constexpr int         FARMER_ROW_IDLE    = 0;
inline constexpr const char *FARMER_TEXTURE = "assets/farmer.png";

inline constexpr int         GUARD_FRAME_W     = 32;
inline constexpr int         GUARD_FRAME_H     = 32;
inline constexpr int         GUARD_COLS        = 5;
inline constexpr int         GUARD_ROW_IDLE    = 0;
inline constexpr const char *GUARD_TEXTURE = "assets/guard.png";

inline constexpr int         SORCERER_FRAME_W     = 32;
inline constexpr int         SORCERER_FRAME_H     = 32;
inline constexpr int         SORCERER_COLS        = 5;
inline constexpr int         SORCERER_ROW_IDLE    = 0;
inline constexpr const char *SORCERER_TEXTURE = "assets/sorcerer.png";

// ---- Classe PNJ ----
class PNJ : public Entity {    
    public:
        PNJ(const char *name, Hitbox hb);
};

#endif // PNJ_H