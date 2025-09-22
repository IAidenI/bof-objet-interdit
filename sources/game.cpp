#include "game.hpp"

void GameInit(Font *info_font) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOF : L'objet interdit");
    SetTargetFPS(60);
    
    *info_font = LoadFont("assets/fonts/ByteBounce.ttf");
}

void DrawAnimatedEntity(
    const Texture2D& texture,
    AnimationState& anim,
    Vector2 pos,
    bool moving,
    int rowIdle,
    int rowMove,
    int frameCols,
    int frameW,
    int frameH,
    float frameTimeIdle,
    float frameTimeMove,
    float scale,
    Color color
) {
    // Avancer l’animation
    anim.timer += GetFrameTime();
    float targetFrameTime = moving ? frameTimeMove : frameTimeIdle;
    while (anim.timer >= targetFrameTime) {
        anim.timer -= targetFrameTime;
        anim.frame = (anim.frame + 1) % frameCols;
    }

    // Ligne à utiliser de la texture
    int row = moving ? rowMove : rowIdle;

    // Rectangle source
    Rectangle src = {
        (float)(anim.frame * frameW),
        (float)(row * frameH),
        (float)frameW,
        (float)frameH
    };

    if (!anim.facingRight) {
        src.width = -src.width;
        src.x += frameW;
    }

    // Destination
    Rectangle dst = {
        pos.x,
        pos.y,
        frameW * scale,
        frameH * scale
    };
    Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };

    DrawTexturePro(texture, src, dst, origin, 0.0f, color);
}

void DrawInfoLabel(Hitbox entity, Font font, const char *label, float font_size, float spacing) {
    Vector2 label_size = MeasureTextEx(font, label, font_size, spacing);
    float padding = 5.0f;

    Vector2 position;
    position.x = entity.pos.x - (label_size.x / 2);
    position.y = entity.pos.y - entity.radius - label_size.y - padding;
    
    DrawTextEx(font, label, position, font_size, spacing, BLACK);
}

void DrawDialogueName(TextStyle text, Vector2 position, Vector2 size) {
    // Fond + contour
    DrawRectangleV(position, size, BEIGE);
    DrawRectangleLines(position.x, position.y, size.x, size.y, BLACK);

    // Taille du texte
    Vector2 textSize = MeasureTextEx(text.font, text.text, text.font_size, text.spacing);

    // Position centrée
    Vector2 textPos = {
        position.x + (size.x - textSize.x) / 2.0f,
        position.y + (size.y - textSize.y) / 2.0f
    };

    // Texte
    DrawTextEx(text.font, text.text, textPos, text.font_size, text.spacing, text.color);
}

void DrawDialogueFrame(TextStyle name) {
    // Background
    float pad_horizontal = 5.0f;
    DrawRectangle(SCREEN_WIDTH / 2 - (UI_DIALOGUE_WIDTH / 2), SCREEN_HEIGHT - UI_DIALOGUE_HEIGHT - pad_horizontal, UI_DIALOGUE_WIDTH, UI_DIALOGUE_HEIGHT, RAYWHITE);
    // Cadre
    DrawRectangleLines(SCREEN_WIDTH / 2 - (UI_DIALOGUE_WIDTH / 2), SCREEN_HEIGHT - UI_DIALOGUE_HEIGHT - pad_horizontal, UI_DIALOGUE_WIDTH, UI_DIALOGUE_HEIGHT, BLACK);

    // Contenu
    float dialogue_name_width = 100.0f;
    float dialogue_name_height = 30.0f;
    DrawDialogueName(name, {SCREEN_WIDTH / 2 - (UI_DIALOGUE_WIDTH / 2), SCREEN_HEIGHT - UI_DIALOGUE_HEIGHT - pad_horizontal - dialogue_name_height}, {dialogue_name_width, dialogue_name_height});
}

void Dialogue(Font *font) {
    DrawDialogueFrame({*font, "Patate", 15.0f, 2.0f, BLACK});
}

void GameClean(Font *info_font) {
    UnloadFont(*info_font);
    CloseWindow();
}
