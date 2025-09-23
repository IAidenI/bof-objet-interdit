#include "game.hpp"

Game::Game(const char **texturesPath, const char **fontPath) 
: player("Vous", {{575, 460}, 80}),
  farmer("Fermier", {{950, 460}, 80}),
  guard("Garde", {{620, 70}, 80}),
  sorcerer("Sorcier", {{280, 320}, 80}) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOF : L'objet interdit");
    SetTargetFPS(60);

    // ---- Initialisation des textures ----
    for (int i = 0; i < TEX_MAX; i++) {
        this->tmgr[i] = LoadTexture(texturesPath[i]);
    }

    // ---- Initialisation de fonts ----
    for (int i = 0; i < FONT_MAX; i++) {
        this->fmgr[i] = LoadFont(fontPath[i]); 
    }

    // ---- Initialisation des objets ----
    this->carrot = getItem(CARROT);
    this->apple  = getItem(APPLE);

    // Initialisation des patates
    Vector2 initialPosition = {270, 740};
    int spacingX = 200;  // distance horizontale
    int spacingY = 100;  // distance verticale
    int cols     = 4;    // 4 par ligne

    for (int i = 0; i < POTATO_AVAILABLE; i++) {
        int row = i / cols;
        int col = i % cols;

        float x = initialPosition.x + col * spacingX;
        float y = initialPosition.y + row * spacingY;

        this->potato[i] = getItem(POTATO);
        this->potato[i].setPosition({x, y});
    }

    // ---- Initialisation des animations ----
    this->playerAnim = {0, 0.0f, true, 0.12f, 0.12f, 3.0f};

    this->farmerAnim = {0, 0.0f, false, 0.12f, 0.0f, 4.0f};
    this->guardAnim = {0, 0.0f, true, 0.15f, 0.0f, 4.0f};
    this->sorcererAnim = {0, 0.0f, true, 0.12f, 0.0f, 4.0f};

    this->potatoAnim = {0, 0.0f, true, 1.0f, 0.0f, 4.0f};
}

void Game::handlePlayerMovements() {
        this->isMoving = false;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            this->player.setPosX(this->player.getHitbox().pos.x + PLAYER_SPEED);
            this->isMoving = true;
            this->playerAnim.facingRight = true;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            this->player.setPosX(this->player.getHitbox().pos.x - PLAYER_SPEED);
            this->isMoving = true;
            this->playerAnim.facingRight = false;
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            this->player.setPosY(this->player.getHitbox().pos.y - PLAYER_SPEED);
            this->isMoving = true;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            this->player.setPosY(this->player.getHitbox().pos.y + PLAYER_SPEED);
            this->isMoving = true;
        }
}

void Game::handlePlayerInput() {
    if (IsKeyPressed(KEY_I)) this->displayInventory = !this->displayInventory;
    if (IsKeyPressed(KEY_E)) this->dialogueRequest = true;
    if (IsKeyPressed(KEY_H)) this->displayHitbox = !this->displayHitbox;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        printf("[ DEBUG ] Position : %.2fx%.2f\n", GetMousePosition().x, GetMousePosition().y);
    }
}

void Game::displayPNJ() {
    if (this->displayHitbox) DrawCircleV(this->farmer.getHitbox().pos, this->farmer.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->tmgr[TEX_FARMER], this->farmerAnim, this->farmer.getHitbox().pos, false, this->farmerSprite, WHITE);
    if (this->displayHitbox) DrawCircleV(this->guard.getHitbox().pos, this->guard.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->tmgr[TEX_GUARD], this->guardAnim, this->guard.getHitbox().pos, false, this->guardSprite, RED);
    if (this->displayHitbox) DrawCircleV(this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->tmgr[TEX_SORCERER], this->sorcererAnim, this->sorcerer.getHitbox().pos, false, this->sorcererSprite, BLUE);
}

void Game::displayItems() {
        for (int i = 0; i < POTATO_AVAILABLE; i++) {
            if (this->displayHitbox) DrawCircleV(this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius, HITBOX_COLOR);
            DrawAnimatedEntity(tmgr[TEX_POTATO], this->potatoAnim, this->potato[i].getHitbox().pos, false, this->potatoSprite, WHITE);
        }
}

void Game::displayPlayer() {
    if (this->displayHitbox) DrawCircleV(this->player.getHitbox().pos, this->player.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(tmgr[TEX_PLAYER], this->playerAnim, this->player.getHitbox().pos, this->isMoving, this->playerSprite, WHITE);
    DrawInfoLabel(this->player.getHitbox(), PLAYER_FRAME_H, {fmgr[BASIC], this->player.getName(), 15.0f, 2.0f});
}

void Game::playerInteractions() {
    // Avec le fermier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->farmer.getHitbox().pos, this->farmer.getHitbox().radius)) {
        DrawInfoLabel(this->farmer.getHitbox(), FARMER_FRAME_H, {fmgr[BASIC], farmer.getName(), 15.0f, 2.0f});
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = FARMER;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le garde
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->guard.getHitbox().pos, this->guard.getHitbox().radius)) {
        DrawInfoLabel(this->guard.getHitbox(), GUARD_FRAME_H, {fmgr[BASIC], guard.getName(), 15.0f, 2.0f});
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = GUARD;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le sorcier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius)) {
        DrawInfoLabel(this->sorcerer.getHitbox(), SORCERER_FRAME_H, {fmgr[BASIC], sorcerer.getName(), 15.0f, 2.0f});
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = SORCERER;
            this->dialogueRequest = false;
        }
    }

    // Avec les patates
    for (int i = 0; i < POTATO_AVAILABLE; ++i) {
        if (CheckCollisionCircles(this->player.getHitbox().pos, player.getHitbox().radius, this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius)) {
            DrawInfoLabel(potato[i].getHitbox(), POTATO_FRAME_H, {fmgr[BASIC], potato[i].getName(), 15.0f, 2.0f});
            if (this->dialogueRequest) {
                this->displayDialogue.request = !this->displayDialogue.request;
                this->displayDialogue.entity = ITEM;
                this->dialogueRequest = false;
            }
        }
    }
}

void Game::dialogue() {
    if (!this->displayDialogue.request) return;
    switch (this->displayDialogue.entity) {
    case ITEM:
        DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_POTATO], this->potatoAnim, this->potatoSprite, WHITE);
        break;
    case FARMER:
        DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_FARMER], this->farmerAnim, this->farmerSprite, WHITE);
        break;
    case GUARD:
        DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_GUARD], this->guardAnim, this->guardSprite, RED);
        break;
    case SORCERER:
        DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_SORCERER], this->sorcererAnim, this->sorcererSprite, BLUE);
        break;
    default:
        // TODO un truc en mode c'est pas sensé arrivé
        break;
    }
}

Game::~Game() {
    // Déchargement des textures
    for (int i = 0; i < TEX_MAX; i++) {
        UnloadTexture(this->tmgr[i]);
    }

    // Déchargement des fonts
    for (int i = 0; i < TEX_MAX; i++) {
        UnloadFont(this->fmgr[i]);
    }

    CloseWindow();
}

void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Vector2 pos, bool moving, SpriteSheetInfo entitySprite, Color color) {
    // Avancer l’animation
    anim.timer += GetFrameTime();
    float targetFrameTime = moving ? anim.frameTimeMove : anim.frameTimeIdle;
    while (anim.timer >= targetFrameTime) {
        anim.timer -= targetFrameTime;
        anim.frame = (anim.frame + 1) % entitySprite.frameCols;
    }

    // Ligne à utiliser de la texture
    int row = moving ? entitySprite.rowMove : entitySprite.rowIdle;

    // Rectangle source
    Rectangle src = {
        (float)(anim.frame * entitySprite.frameW),
        (float)(row * entitySprite.frameH),
        (float)entitySprite.frameW,
        (float)entitySprite.frameH
    };

    if (!anim.facingRight) {
        src.width = -src.width;
        src.x += entitySprite.frameW;
    }

    // Destination
    Rectangle dst = {
        pos.x,
        pos.y,
        entitySprite.frameW * anim.scale,
        entitySprite.frameH * anim.scale
    };
    Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };

    DrawTexturePro(texture, src, dst, origin, 0.0f, color);
}

void DrawStaticItem(const Texture2D& texture, Vector2 pos, float scale) {
    // Source = texture entière
    Rectangle src = { 0, 0, (float)texture.width, (float)texture.height };

    // Destination = centré sur la hitbox
    Rectangle dst = {
        pos.x,  // centre X
        pos.y,  // centre Y
        texture.width * scale,
        texture.height * scale
    };

    // Origine = le centre de la texture
    Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };

    DrawTexturePro(texture, src, dst, origin, 0.0f, WHITE);
}

void DrawInfoLabel(Hitbox entity, int entitySize, TextStyle text) {
    Vector2 label_size = MeasureTextEx(text.font, text.text, text.font_size, text.spacing);
    float padding = 5.0f;

    // Calcul la position
    Vector2 position;
    position.x = entity.pos.x - (label_size.x / 2);
    position.y = entity.pos.y - entitySize - label_size.y - padding;
    
    DrawTextEx(text.font, text.text, position, text.font_size, text.spacing, BLACK);
}

void DrawDialogueFrame(Texture2D dialogue, Texture2D entity, AnimationState entityAnim, SpriteSheetInfo entitySprite, Color color) {
    float pad_horizontal = 5.0f;

    // Destination (où et quelle taille à l'écran)
    Rectangle rec = {
        SCREEN_WIDTH / 2 - (UI_DIALOGUE_WIDTH / 2),   // X centré
        SCREEN_HEIGHT - UI_DIALOGUE_HEIGHT - pad_horizontal, // Y en bas
        UI_DIALOGUE_WIDTH,    // largeur voulue
        UI_DIALOGUE_HEIGHT    // hauteur voulue
    };

    // Source (on prend toute la texture)
    Rectangle src = { 0, 0, (float)dialogue.width, (float)dialogue.height };

    // Afficher texture redimensionnée dans rec
    DrawTexturePro(dialogue, src, rec, (Vector2){0, 0}, 0.0f, WHITE);

    // Calcul la position du profil
    float squareSize = rec.height;  // hauteur de la texture affichée
    if (squareSize > rec.width) squareSize = rec.width; // rester carré

    Vector2 center = {
        rec.x + squareSize / 2.0f,
        rec.y + squareSize / 2.0f
    };

    DrawAnimatedEntity(entity, entityAnim, center, false, entitySprite, color);
}

