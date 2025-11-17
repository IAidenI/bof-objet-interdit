#include "entity.hpp"
#include "PNJ.hpp"
#include "player.hpp"
#include "game.hpp"

int main() {
    // ---- Début du jeu ----
    Game game;

    if (!game.init()) {
        cerr << "Impossible de démarrer le jeu." << endl;
        return 1;
    }

    while (!WindowShouldClose() && !game.hasEnded()) {
        // Reset du curseur
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        game.notificationUpdate();

        game.handlePlayerMovements();

        game.handlePlayerInput();

        // ---- Affichage de l'UI ----
        BeginDrawing();
            ClearBackground(RAYWHITE);

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
            game.notificationDraw();
        EndDrawing();
    }

    return 0;
}
