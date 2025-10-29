#ifndef COLOR_H
#define COLOR_H

inline constexpr Color COLOR_INPUT_BOX_TEXT          = MAROON;
inline constexpr Color COLOR_INPUT_BOX_FOCUS         = CLITERAL(Color){ 255, 255, 255,  64 };
inline constexpr Color COLOR_INPUT_BOX_BACKGROUND    = LIGHTGRAY;
inline constexpr Color COLOR_INPUT_BOX_BORDER        = DARKGRAY;

// ---- Stack Trace ----
inline constexpr Color COLOR_STACK_TEXT              = CLITERAL(Color){ 255, 255, 255, 178 };
inline constexpr Color COLOR_STACK_BORDER            = BLACK;
inline constexpr Color COLOR_STACK_GLOBAL_BACKGROUND = CLITERAL(Color){  12,  17,  20, 178 };
inline constexpr Color COLOR_STACK_CARD_BACKGROUND   = CLITERAL(Color){  20,  28,  32, 255 };
inline constexpr Color COLOR_STACK_TITLE_BACKGROUND  = CLITERAL(Color){  29,  43,  47, 255 };
inline constexpr Color COLOR_STACK_ADDR              = CLITERAL(Color){  92, 197, 184, 255 };
inline constexpr Color COLOR_STACK_HEXA              = CLITERAL(Color){ 183, 255, 219, 255 };
inline constexpr Color COLOR_STACK_SPLIT              = CLITERAL(Color){  53,  90,  84, 255 };
inline constexpr Color COLOR_STACK_ASCII             = CLITERAL(Color){ 207, 230, 227, 255 };


// ---- Game Colors ----
inline constexpr Color HITBOX_COLOR                  = CLITERAL(Color){ 184, 184, 184, 102 };


// A check
inline constexpr Color BEIGE_LIGHT = CLITERAL(Color){ 230, 210, 170, 255 };

#endif // COLOR_H