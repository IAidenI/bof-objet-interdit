#ifndef PNJ_H
#define PNJ_H

#include "entity.hpp"

// ---- Textures ----
inline constexpr float       FARMER_FRAME_W  = 32.0f;
inline constexpr float       FARMER_FRAME_H  = 32.0f;
inline constexpr float       FARMER_COLS     = 4.0f;
inline constexpr float       FARMER_ROW_IDLE = 0.0f;

inline constexpr float       GUARD_FRAME_W  = 32.0f;
inline constexpr float       GUARD_FRAME_H  = 32.0f;
inline constexpr float       GUARD_COLS     = 5.0f;
inline constexpr float       GUARD_ROW_IDLE = 0.0f;

inline constexpr float       SORCERER_FRAME_W  = 32.0f;
inline constexpr float       SORCERER_FRAME_H  = 32.0f;
inline constexpr float       SORCERER_COLS     = 5.0f;
inline constexpr float       SORCERER_ROW_IDLE = 0.0f;

// ---- Classe PNJ ----
class PNJ : public Entity {    
    public:
        PNJ(const char *name, Hitbox hb);
};

#endif // PNJ_H