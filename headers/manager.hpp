#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <vector>
#include "geometry.hpp"
using namespace std;

// ---- Structure texture/font ----
inline constexpr const char *ENTITY_FONT     = "assets/fonts/ByteBounce.ttf";
inline constexpr const char *DIALOGUE_FONT   = "assets/fonts/Jersey10-Regular.ttf";
inline constexpr const char *INFO_FONT       = "assets/fonts/Inconsolata-Regular.ttf";
inline constexpr const int   SMALL_SIZE      = 20;
inline constexpr const int   BIG_SIZE        = 50;
inline constexpr const int   FONT_SIZE_COUNT = 2;
// ASCII + accents FR courants
static const char32_t FR_CHARS[] = U" !\"#$%&'()*+,-./0123456789:;<=>?@"
                                   U"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                                   U"abcdefghijklmnopqrstuvwxyz{|}~"
                                   U"àâäçéèêëîïôöùûüÿÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸ’«»œŒ";

enum TextureID {
    TEX_BACKGROUND,
    // Objets
    TEX_POTATO,
    TEX_POTATO_STATIC,
    TEX_CARROT_STATIC,
    TEX_APPLE,
    TEX_APPLE_STATIC,
    // PNJ
    TEX_FARMER,
    TEX_GUARD,
    TEX_SORCERER,
    // Joueur
    TEX_PLAYER,
    // Autre
    TEX_DIALOGUE,
    TEX_INVENTORY,
    TEX_TRASH,
    // Sentinelle pour savoir la taille
    TEX_MAX
};

enum FontID {
    ENTITY_LABEL,
    DIALOGUE_LABEL,
    INFO_LABEL,
    // Sentinelle pour savoir la taille
    FONT_MAX
};

// ---- Assets ----
// Global
inline constexpr const char *ICON             = "assets/icon.png";
inline constexpr const char *BACKGROUND       = "assets/background.png";
inline constexpr const char *DIALOGUE_TEXTURE = "assets/dialogue.png";
inline constexpr const char *TRASH_TEXTURE    = "assets/trash.png";

// Item
inline constexpr const char *POTATO_TEXTURE        = "assets/potato.png";
inline constexpr const char *POTATO_STATIC_TEXTURE = "assets/potato_static.png";

inline constexpr const char *CARROT_STATIC_TEXTURE = "assets/carrot.png";

inline constexpr const char *APPLE_TEXTURE        = "assets/apple.png";
inline constexpr const char *APPLE_STATIC_TEXTURE = "assets/apple_static.png";

// PNJ
inline constexpr const char *FARMER_TEXTURE   = "assets/farmer.png";
inline constexpr const char *GUARD_TEXTURE    = "assets/guard.png";
inline constexpr const char *SORCERER_TEXTURE = "assets/sorcerer.png";

// Player
inline constexpr const char *PLAYER_TEXTURE = "assets/player.png";

// Inventaire
inline constexpr const char *INVENTORY_TEXTURE = "assets/inventory.png";

bool CheckFileExiste(const char *path);

class Manager {
    private:
        bool valid = true;

        Texture2D tmgr[TEX_MAX];
        Font fmgr[FONT_MAX][FONT_SIZE_COUNT];

        // ---- Textures/Fonts ----
        const char *textures[TEX_MAX] = {
            BACKGROUND,
            // Objets
            POTATO_TEXTURE,
            POTATO_STATIC_TEXTURE,
            CARROT_STATIC_TEXTURE,
            APPLE_TEXTURE,
            APPLE_STATIC_TEXTURE,
            // PNJ
            FARMER_TEXTURE,
            GUARD_TEXTURE,
            SORCERER_TEXTURE,
            // Joueur
            PLAYER_TEXTURE,
            // Autre
            DIALOGUE_TEXTURE,
            INVENTORY_TEXTURE,
            TRASH_TEXTURE
        };

        const char *fonts[FONT_MAX] = {
            ENTITY_FONT,
            DIALOGUE_FONT,
            INFO_FONT
        };
    public:
        bool init();

        Texture2D getTexture(TextureID id) const { return this->tmgr[id]; }
        const Font& getFont(FontID id, int size);

        ~Manager();
};

#endif // MANAGER_H
