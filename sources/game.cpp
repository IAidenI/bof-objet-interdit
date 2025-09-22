#include "game.hpp"

void GameInit(Font *info_font) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOF : L'objet interdit");
    SetTargetFPS(60);
    
    *info_font = LoadFont("assets/fonts/ByteBounce.ttf");
}

void DrawInfoLabel(Hitbox entity, Font font, const char *label, float font_size, float spacing) {
    Vector2 label_size = MeasureTextEx(font, label, font_size, spacing);
    float padding = 5.0f;

    Vector2 position;
    position.x = entity.pos.x - (label_size.x / 2);
    position.y = entity.pos.y - entity.radius - label_size.y - padding;
    
    DrawTextEx(font, label, position, font_size, spacing, BLACK);
}

void GameClean(Font *info_font) {
    UnloadFont(*info_font);
    CloseWindow();
}
