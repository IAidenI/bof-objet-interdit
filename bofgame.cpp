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

    // ---- Objets ----
    Font infoFont;
    GameInit(&infoFont);

    { // ======= TOUT TON MONDE VIT ICI =======
        Item potato[POTATO_AVAILABLE];
        Item carrot = getItem(CARROT);
        Item apple  = getItem(APPLE);

        // Initialisation des patates
        Vector2 initialPosition = {270, 640};
        int spacingX = 200;  // distance horizontale entre les patates
        int spacingY = 200;  // distance verticale entre les lignes
        int cols = 4;         // 4 par ligne

        for (int i = 0; i < POTATO_AVAILABLE; i++) {
            int row = i / cols;
            int col = i % cols;

            float x = initialPosition.x + col * spacingX;
            float y = initialPosition.y + row * spacingY;

            potato[i] = getItem(POTATO);
            potato[i].setPosition({x, y});
        }

        // ---- Créatures ----
        Vector2 pos = {950, 460};
        Hitbox hb = {pos, 30};
        PNJ farmer("Fermier", hb, PNJ_FARMER_TEXTURE);

        pos = {620, 70};
        hb = {pos, 30};
        PNJ guard("Garde", hb, PNJ_GUARD_TEXTURE);

        pos = {280, 320};
        hb = {pos, 30};
        PNJ sorcerer("Sorcier", hb, PNJ_SORCERER_TEXTURE);

        pos = {575, 460};
        hb = {pos, 30};
        Player player("Vous", hb, PLAYER_TEXTURE);

        // ---- Variables animation textures ----
        // Joueur
        AnimationState playerAnim;
        float playerFrameTimeIdle = 0.12f; // vitesse anim idle
        float playerFrameTimeMove = 0.12f; // vitesse anim move
        const float playerScale   = 3.0f;

        // Fermier
        AnimationState farmerAnim;
        farmerAnim.facingRight = true;
        float farmerFrameTimeIdle = 0.12f; // vitesse anim idle
        const float farmerScale   = 4.0f;

        // Garde
        AnimationState guardAnim;
        guardAnim.facingRight = false;
        float guardFrameTimeIdle = 0.12f; // vitesse anim idle
        const float guardScale   = 4.0f;

        // ---- Variables requêtes ----
        bool displayInventory = false;
        bool displayDialogue = false;
        bool dialogueRequest = false;

        while (!WindowShouldClose()) {
            bool moving = false;

            // ---- Déplacements ----
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player.setPosX(player.getHitbox().pos.x + PLAYER_SPEED);
                moving = true;
                playerAnim.facingRight = true;
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player.setPosX(player.getHitbox().pos.x - PLAYER_SPEED);
                moving = true;
                playerAnim.facingRight = false;
            }
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                player.setPosY(player.getHitbox().pos.y - PLAYER_SPEED);
                moving = true;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                player.setPosY(player.getHitbox().pos.y + PLAYER_SPEED);
                moving = true;
            }

            // ---- Evénements clavier ----
            if (IsKeyPressed(KEY_I)) displayInventory = !displayInventory;
            if (IsKeyPressed(KEY_E)) dialogueRequest = true;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("[ DEBUG ] Position : %.2fx%.2f\n", GetMousePosition().x, GetMousePosition().y);
            }

            // ---- Affichage de l'UI ----
            BeginDrawing();
                ClearBackground(RAYWHITE);

                // Affiche les PNJ
                DrawCircleV(farmer.getHitbox().pos, farmer.getHitbox().radius, HITBOX_COLOR);
                DrawAnimatedEntity(farmer.getTexture(), farmerAnim, farmer.getHitbox().pos, moving, FARMER_ROW_IDLE, FARMER_ROW_IDLE, FARMER_COLS, FARMER_FRAME_W, FARMER_FRAME_H, farmerFrameTimeIdle, farmerFrameTimeIdle, farmerScale, RED);
                DrawCircleV(guard.getHitbox().pos, guard.getHitbox().radius, HITBOX_COLOR);
                DrawAnimatedEntity(guard.getTexture(), guardAnim, guard.getHitbox().pos, moving, GUARD_ROW_IDLE, GUARD_ROW_IDLE, GUARD_COLS, GUARD_FRAME_W, GUARD_FRAME_H, guardFrameTimeIdle, guardFrameTimeIdle, guardScale, WHITE);
                DrawCircleV(sorcerer.getHitbox().pos, sorcerer.getHitbox().radius, HITBOX_COLOR);
                // Affiche le joueur
                DrawCircleV(player.getHitbox().pos, player.getHitbox().radius, HITBOX_COLOR);
                DrawAnimatedEntity(player.getTexture(), playerAnim, player.getHitbox().pos, moving, PLAYER_ROW_IDLE, PLAYER_ROW_MOVE, PLAYER_COLS, PLAYER_FRAME_W, PLAYER_FRAME_H, playerFrameTimeIdle, playerFrameTimeMove, playerScale, WHITE);
                DrawInfoLabel(player.getHitbox(), infoFont, player.getName(), 15.0f, 2.0f);
                
                // Affiche les items
                for (int i = 0; i < POTATO_AVAILABLE; i++) {
                    DrawCircleV(potato[i].getHitbox().pos, potato[i].getHitbox().radius, HITBOX_COLOR);
                }

                // --- Collisions ---
                if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, farmer.getHitbox().pos, farmer.getHitbox().radius)) {
                    DrawInfoLabel(farmer.getHitbox(), infoFont, farmer.getName(), 15.0f, 2.0f);
                    if (dialogueRequest) displayDialogue = !displayDialogue;
                }
                
                if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, guard.getHitbox().pos, guard.getHitbox().radius)) {
                    DrawInfoLabel(guard.getHitbox(), infoFont, guard.getName(), 15.0f, 2.0f);
                    if (dialogueRequest) displayDialogue = !displayDialogue;
                }
                
                if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, sorcerer.getHitbox().pos, sorcerer.getHitbox().radius)) {
                    DrawInfoLabel(sorcerer.getHitbox(), infoFont, sorcerer.getName(), 15.0f, 2.0f);
                    if (dialogueRequest) displayDialogue = !displayDialogue;
                }

                for (int i = 0; i < POTATO_AVAILABLE; ++i) {
                    if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, potato[i].getHitbox().pos, potato[i].getHitbox().radius)) {
                        DrawInfoLabel(potato[i].getHitbox(), infoFont, potato[i].getName(), 15.0f, 2.0f);
                        if (dialogueRequest) displayDialogue = !displayDialogue;
                    }
                }

                // ---- Affichage des popup ----
                // Inventaire
                if (displayInventory) player.inventory().display();
                // Boîte de dialogue
                if (displayDialogue) Dialogue(&infoFont);

                // Reset requests
                dialogueRequest = false;
            EndDrawing();
        }
    } // ======= DESTRUCTEURS ICI (UnloadTexture OK car contexte encore vivant) =======

    GameClean(&infoFont);

    return 0;
}
