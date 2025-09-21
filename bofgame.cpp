#include "headers/entities.hpp"
#include "headers/game.hpp"

int main() {
    Vector2 pos = {950, 460};
    Hitbox hb = {pos, 30};
    PNJ farmer("Fermier", hb, RED);

    pos = {280, 320};
    hb = {pos, 30};
    PNJ guard("Garde", hb, BLUE);

    pos = {620, 50};
    hb = {pos, 30};
    PNJ scribe("Scribe", hb, BLACK);

    pos = {575, 460};
    hb = {pos, 30};
    Player player("Joueur", hb, PURPLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello world");

    SetTargetFPS(60);

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
            DrawCircleV(farmer.getHitbox().pos, farmer.getHitbox().radius, farmer.getColor());
            DrawCircleV(guard.getHitbox().pos, guard.getHitbox().radius, guard.getColor());
            DrawCircleV(scribe.getHitbox().pos, scribe.getHitbox().radius, scribe.getColor());
            DrawCircleV(player.getHitbox().pos, player.getHitbox().radius, player.getColor());
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
