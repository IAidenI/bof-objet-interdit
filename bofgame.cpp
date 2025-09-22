#include "entity.hpp"
#include "PNJ.hpp"
#include "player.hpp"
#include "game.hpp"

int main() {
    Font info_font;
    GameInit(&info_font);

    Item potato[POTATO_AVAILABLE];
    Item carrot = getItem(CARROT);
    Item apple  = getItem(APPLE);

    // Initialisation des patates
    Vector2 initial_position = {270, 640};
    int spacing_x = 200;  // distance horizontale entre les patates
    int spacing_y = 200;  // distance verticale entre les lignes
    int cols = 4;        // 4 par ligne

    for (int i = 0; i < POTATO_AVAILABLE; i++) {
        int row = i / cols;
        int col = i % cols;

        float x = initial_position.x + col * spacing_x;
        float y = initial_position.y + row * spacing_y;

        potato[i] = getItem(POTATO);
        potato[i].setPosition({x, y});
    }

    Vector2 pos = {950, 460};
    Hitbox hb = {pos, 30};
    PNJ farmer("Fermier", hb, RED);

    pos = {620, 70};
    hb = {pos, 30};
    PNJ guard("Garde", hb, BLUE);

    pos = {280, 320};
    hb = {pos, 30};
    PNJ sorcerer("Sorcier", hb, BLACK);

    pos = {575, 460};
    hb = {pos, 30};
    Player player("Vous", hb, PURPLE);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.setPosX(player.getHitbox().pos.x + PLAYER_SPEED);
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.setPosX(player.getHitbox().pos.x - PLAYER_SPEED);
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player.setPosY(player.getHitbox().pos.y - PLAYER_SPEED);
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.setPosY(player.getHitbox().pos.y + PLAYER_SPEED);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("[ DEBUG ] Position : %.2fx%.2f\n", GetMousePosition().x, GetMousePosition().y);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            // Affiche les joueurs/PNJ
            DrawCircleV(farmer.getHitbox().pos, farmer.getHitbox().radius, farmer.getColor());
            DrawCircleV(guard.getHitbox().pos, guard.getHitbox().radius, guard.getColor());
            DrawCircleV(sorcerer.getHitbox().pos, sorcerer.getHitbox().radius, sorcerer.getColor());
            DrawCircleV(player.getHitbox().pos, player.getHitbox().radius, player.getColor());
            DrawInfoLabel(player.getHitbox(), info_font, player.getName(), 15.0f, 2.0f);
            // Affiche les items
            for (int i = 0; i < POTATO_AVAILABLE; i++) {
                DrawCircleV(potato[i].getHitbox().pos, potato[i].getHitbox().radius, potato[i].getColor());
            }

            // --- Collisions ---            
            if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, farmer.getHitbox().pos, farmer.getHitbox().radius)) {
                DrawInfoLabel(farmer.getHitbox(), info_font, farmer.getName(), 15.0f, 2.0f);
            }
            
            if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, guard.getHitbox().pos, guard.getHitbox().radius)) {
                DrawInfoLabel(guard.getHitbox(), info_font, guard.getName(), 15.0f, 2.0f);
            }
            
            if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, sorcerer.getHitbox().pos, sorcerer.getHitbox().radius)) {
                DrawInfoLabel(sorcerer.getHitbox(), info_font, sorcerer.getName(), 15.0f, 2.0f);
            }

            for (int i = 0; i < POTATO_AVAILABLE; ++i) {
                if (CheckCollisionCircles(player.getHitbox().pos, player.getHitbox().radius, potato[i].getHitbox().pos, potato[i].getHitbox().radius)) {
                    DrawInfoLabel(potato[i].getHitbox(), info_font, potato[i].getName(), 15.0f, 2.0f);
                }
            }
        EndDrawing();
    }

    GameClean(&info_font);

    return 0;
}
