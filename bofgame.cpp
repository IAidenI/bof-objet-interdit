#include "entity.hpp"
#include "PNJ.hpp"
#include "player.hpp"
#include "game.hpp"

int main() {
    // ---- Check si les fichiers sont présents ----

    // ---- Textures/Fonts ----
    const char *textures[TEX_MAX] = {
        BACKGROUND,
        // Objets
        POTATO_TEXTURE,
        POTATO_STATIC_TEXTURE,
        CARROT_STATIC_TEXTURE,
        APPLE_STATIC_TEXTURE,
        // PNJ
        FARMER_TEXTURE,
        GUARD_TEXTURE,
        SORCERER_TEXTURE,
        // Joueur
        PLAYER_TEXTURE,
        // Autre
        DIALOGUE_TEXTURE,
        INVENTORY_TEXTURE
    };
    const char *fonts[FONT_MAX] = {
        ENTITY_FONT,
        DIALOGUE_FONT,
        INFO_FONT
    };

    for (const char* texPath : textures) {
        if (texPath == nullptr) {
            cerr << "[ ERREUR ] Chemin de police " << texPath << " détecté." << endl;
            return 1;
        }

        if (!FileExists(texPath)) {
            cerr << texPath << " introuvable." << endl;
            return 1;
        }
    }

    for (const char* texPath : fonts) {
        if (texPath == nullptr) {
            cerr << "[ ERREUR ] Chemin de police " << texPath << " détecté." << endl;
            return 1;
        }

        if (!FileExists(texPath)) {
            cerr << texPath << " introuvable." << endl;
            return 1;
        }
    }

    // ---- Début du jeu ----
    Game game(textures, fonts);

    while (!WindowShouldClose() && !game.hasEnded()) {
        game.handlePlayerMovements();

        game.handlePlayerInput();

        // ---- Affichage de l'UI ----
        BeginDrawing();
            ClearBackground(RED);

            game.displayBackground();
            game.displayPNJ();
            game.displayItems();
            
            game.playerInteractions();
            game.displayPlayer();

            game.displayCommands();
            game.renderStack();
            game.renderInventory();
            game.dialogue();
            
            game.resetRequests();
        EndDrawing();
    }

    return 0;
}
