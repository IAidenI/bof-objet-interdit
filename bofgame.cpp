#include "entity.hpp"
#include "PNJ.hpp"
#include "player.hpp"
#include "game.hpp"

int main() {
    // ---- Check si les fichiers sont présents ----
    const char* requiredTextures[] = {
        // Joueur
        PLAYER_TEXTURE,
        // PNJ
        PNJ_FARMER_TEXTURE,
        PNJ_SORCERER_TEXTURE,
        PNJ_GUARD_TEXTURE,
        // Objets
        ITEM_POTATO_TEXTURE,
        ITEM_CARROT_TEXTURE,
        ITEM_APPLE_TEXTURE,
    };

    for (const char* texPath : requiredTextures) {
        if (!FileExists(texPath)) {
            std::cerr << texPath << " introuvable." << std::endl;
            return 1;
        }
    }

    // ---- Textures/Fonts ----
    const char *textures[TEX_MAX] = {
        // Objets
        ITEM_POTATO_TEXTURE,
        ITEM_CARROT_TEXTURE,
        ITEM_APPLE_TEXTURE,
        // PNJ
        PNJ_FARMER_TEXTURE,
        PNJ_GUARD_TEXTURE,
        PNJ_SORCERER_TEXTURE,
        // Joueur
        PLAYER_TEXTURE,
        // Autre
        DIALOGUE_TEXTURE
    };
    const char *fonts[FONT_MAX] = {
        "assets/fonts/ByteBounce.ttf",
    };

    // ---- Début du jeu ----
    Game game(textures, fonts);

    while (!WindowShouldClose()) {
        game.handlePlayerMovements();

        game.handlePlayerInput();

        // ---- Affichage de l'UI ----
        BeginDrawing();
            ClearBackground(RED);

            game.displayPNJ();
            game.displayItems();
            game.playerInteractions();
            game.displayPlayer();
            game.dialogue();
        EndDrawing();
    }

    return 0;
}
