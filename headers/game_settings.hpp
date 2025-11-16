#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

inline constexpr int AMOUNT_TO_FINISH_GAME = 32;

inline constexpr int MAX_INPUT_CHARS = 17;     // Vulnérabilité

inline constexpr int SCREEN_WIDTH  = 1200;
inline constexpr int SCREEN_HEIGHT = 900;

// ---- Paramètres du jeu ----
inline constexpr int PLAYER_SPEED     = 4;
inline constexpr int POTATO_AVAILABLE = 8;

inline constexpr float INVENTORY_SCALE = 5.0f;

// ---- Paramètres de trade ----
inline constexpr int TRADE_GIVE_POTATO = 8;
inline constexpr int TRADE_GET_CARROT  = 1;

inline constexpr int TRADE_GIVE_CARROT = 8;
inline constexpr int TRADE_GET_APPLE   = 1;

inline constexpr int TRADE_GIVE_APPLE  = 8;
inline constexpr int TRADE_GET_POTATO   = 1;

#endif // GAME_SETTINGS_H
