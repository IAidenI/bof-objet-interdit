#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <raylib.h>
#include <raymath.h>
using namespace std;

using Frame = Rectangle;
using Size = Vector2;
using Position = Vector2;

struct Hitbox {
    Position pos;
    float radius;
};

// ---- Padding ----
struct Spacing {
    float x;
    float y;
};
using Padding = Spacing;
using Margins = Spacing;

// ---- Constantes ----
inline constexpr Padding DEFAULT_TITLE_PAD_IN  = { 5.0f, 3.0f };
inline constexpr float   DEFAULT_TITLE_OFFSET  = 15.0f;

inline constexpr Padding DEFAULT_PAD_IN        = { 10.0f, 10.0f };
inline constexpr Margins DEFAULT_PAD_OUT       = { 15.0f, 15.0f };
inline constexpr float   DEFAULT_INTERLINE     = 5.0f;

inline constexpr float   DEFAULT_ROUNDNESS     = 0.12f;
inline constexpr int     DEFAULT_SEGMENTS      = 32;
inline constexpr float   DEFAULT_STROKE        = 2.0f;

#endif // GEOMETRY_HPP
