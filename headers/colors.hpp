#ifndef COLOR_H
#define COLOR_H

#include "geometry.hpp"

inline constexpr Color COLOR_INVENTORY_BACKGROUND    = CLITERAL(Color){  84,  61,  89, 255 };
inline constexpr Color COLOR_VALUE_HOOVER            = CLITERAL(Color){  24,  76,  66, 255 };

// ---- Commandes ----
inline constexpr Color COLOR_COMMANDS_BACKGROUND = CLITERAL(Color){  12,  17,  20, 178 };
inline constexpr Color COLOR_COMMANDS_BORDER     = BLACK;
inline constexpr Color COLOR_COMMANDS_LABEL      = CLITERAL(Color){  92, 197, 184, 255 };
inline constexpr Color COLOR_COMMANDS_ACTION     = CLITERAL(Color){ 255, 255, 255, 178 };

// ---- Dialogue ----
inline constexpr Color COLOR_DIALOGUE_BACKGROUND         = BLACK;
inline constexpr Color COLOR_DIALOGUE_BORDER             = WHITE;
inline constexpr Color COLOR_DIALOGUE_CONTENT_BACKGROUND = CLITERAL(Color){  39,  71,  98, 255 };
inline constexpr Color COLOR_DIALOGUE_CONTENT_BORDER     = CLITERAL(Color){  16,  51,  71, 255 };
inline constexpr Color COLOR_DIALOGUE_CONTENT_TEXT       = WHITE;

// ---- Inventaire ----
inline constexpr Color COLOR_INVENTORY_HOVER_BACKGROUND          = CLITERAL(Color){  20,  28,  32, 255 };
inline constexpr Color COLOR_INVENTORY_HOVER_BORDER              = CLITERAL(Color){ 129, 179, 220, 255};
inline constexpr Color COLOR_INVENTORY_NAME                      = CLITERAL(Color){ 165, 186, 255, 178 };
inline constexpr Color COLOR_INVENTORY_ID                        = CLITERAL(Color){ 158, 225, 169, 255 };
inline constexpr Color COLOR_INVENTORY_CURRENT_AMOUNT            = CLITERAL(Color){ 158, 225, 169, 255 };
inline constexpr Color COLOR_INVENTORY_MAX_AMOUNT                = CLITERAL(Color){ 225, 118, 118, 255 };
inline constexpr Color COLOR_INVENTORY_LABEL                     = CLITERAL(Color){ 197, 198, 208, 178 };
inline constexpr Color COLOR_INVENTORY_SELECTOR                  = CLITERAL(Color){  92, 197, 184, 255 };
inline constexpr Color COLOR_INVENTORY_CURRENT_AMOUNT_BACKGROUND = CLITERAL(Color){ 230, 210, 170, 255 };

// ---- Magasin du fermier ----
inline constexpr Color COLOR_STORE_BACKGROUND        = CLITERAL(Color){  16,  43,  63, 255 };
inline constexpr Color COLOR_STORE_BORDER            = CLITERAL(Color){ 199, 168, 100, 255 };
inline constexpr Color COLOR_STORE_TITLE             = CLITERAL(Color){ 245, 240, 230, 255 };
inline constexpr Color COLOR_STORE_ARROW             = CLITERAL(Color){ 255, 248, 230, 255 };
inline constexpr Color COLOR_STORE_BUTTON_BACKGROUND = CLITERAL(Color){  39,  73,  98, 255 };
inline constexpr Color COLOR_STORE_BUTTON_BORDER     = CLITERAL(Color){ 170, 161, 136, 255 };
inline constexpr Color COLOR_STORE_BUTTON_TEXT       = WHITE;

// ---- Input box ----
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
inline constexpr Color COLOR_STACK_SPLIT             = CLITERAL(Color){  53,  90,  84, 255 };
inline constexpr Color COLOR_STACK_ASCII             = CLITERAL(Color){ 207, 230, 227, 255 };

// ---- Info bulle ----
inline constexpr Color COLOR_TOOLTIP_BACKGROUND      = CLITERAL(Color){  20,  28,  32, 255 };
inline constexpr Color COLOR_TOOLTIP_BORDER          = CLITERAL(Color){ 114, 114, 114, 255 };
inline constexpr Color COLOR_TOOLTIP_TITLE           = CLITERAL(Color){ 183, 255, 219, 255 };
inline constexpr Color COLOR_TOOLTIP_LABEL           = CLITERAL(Color){ 122, 162, 247, 255 };
inline constexpr Color COLOR_TOOLTIP_BRANCH          = COLOR_TOOLTIP_BORDER;
inline constexpr Color COLOR_TOOLTIP_TYPE            = CLITERAL(Color){  77, 156, 214, 255 };
inline constexpr Color COLOR_TOOLTIP_HEXA            = CLITERAL(Color){ 181, 206, 168, 255 };
inline constexpr Color COLOR_TOOLTIP_VALUE           = CLITERAL(Color){ 206, 145, 120, 255 };


// ---- Game Colors ----
inline constexpr Color HITBOX_COLOR                  = CLITERAL(Color){ 184, 184, 184, 102 };

#endif // COLOR_H
