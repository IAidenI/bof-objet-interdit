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
        this->fmgr[i] = LoadFontEx(fontPath[i], 32, 0, 250); 
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
        if (this->isPause) return;

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
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        printf("[ DEBUG ] Position : %.2fx%.2f\n", GetMousePosition().x, GetMousePosition().y);
    }

    if (this->inventoryRequest && this->invSelectorVisible) {
        int rowSize = static_cast<int>(sqrt(static_cast<float>(MAX_INVENTORY_LENGTH)));
        if (rowSize * rowSize < MAX_INVENTORY_LENGTH) rowSize += 1;
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) this->invSelectorIndex = (this->invSelectorIndex + 1) % MAX_INVENTORY_LENGTH;
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) this->invSelectorIndex = (this->invSelectorIndex - 1 + MAX_INVENTORY_LENGTH) % MAX_INVENTORY_LENGTH;
        if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) this->invSelectorIndex = (this->invSelectorIndex + rowSize) % MAX_INVENTORY_LENGTH;
        if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) this->invSelectorIndex = (this->invSelectorIndex - rowSize + MAX_INVENTORY_LENGTH) % MAX_INVENTORY_LENGTH;
    }

    if (IsKeyPressed(KEY_I)) {
        this->inventoryRequest = !this->inventoryRequest;
        this->isPause = !this->isPause;
    }
    if (IsKeyPressed(KEY_F)) this->player.inventory().display();

    if (IsKeyPressed(KEY_E)) this->dialogueRequest = true;
    if (IsKeyPressed(KEY_H)) this->hitboxRequest = !this->hitboxRequest;
}

void Game::displayCommands() {
    int fontSize = 20;
    int spacing = 2;
    int margin = 10;
    int boxX = 10;
    int boxY = 10;

    // Texte des commandes
    const char *lines[] = {
        "COMMANDES",
        "ZQSD / Flêches : Déplacement",
        "E : Interagir",
        "I : Inventaire",
        "H : Hitbox ON/OFF"
    };
    int lineCount = sizeof(lines) / sizeof(lines[0]);

    // Calcul de la largeur max du texte
    int maxWidth = 0;
    for (int i = 0; i < lineCount; i++) {
        int w = MeasureText(lines[i], fontSize);
        if (w > maxWidth) maxWidth = w;
    }

    int boxWidth  = maxWidth + 2 * margin;
    int boxHeight = (lineCount * (fontSize + spacing)) + 2 * margin;

    // Fond semi-transparent
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.7f));

    // Contour
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, DARKGRAY);

    // Texte
    for (int i = 0; i < lineCount; i++) {
        DrawText(lines[i], boxX + margin, boxY + margin + i * (fontSize + spacing),
                 fontSize, BLACK);
    }
}

void Game::displayPNJ() {
    if (this->hitboxRequest) DrawCircleV(this->farmer.getHitbox().pos, this->farmer.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->tmgr[TEX_FARMER], this->farmerAnim, this->farmer.getHitbox().pos, false, this->farmerSprite, WHITE);
    if (this->hitboxRequest) DrawCircleV(this->guard.getHitbox().pos, this->guard.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->tmgr[TEX_GUARD], this->guardAnim, this->guard.getHitbox().pos, false, this->guardSprite, RED);
    if (this->hitboxRequest) DrawCircleV(this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->tmgr[TEX_SORCERER], this->sorcererAnim, this->sorcerer.getHitbox().pos, false, this->sorcererSprite, BLUE);
}

void Game::displayItems() {
        for (int i = 0; i < POTATO_AVAILABLE; i++) {
            if (this->hitboxRequest) DrawCircleV(this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius, HITBOX_COLOR);
            DrawAnimatedEntity(tmgr[TEX_POTATO], this->potatoAnim, this->potato[i].getHitbox().pos, false, this->potatoSprite, WHITE);
        }
}

void Game::displayPlayer() {
    if (this->hitboxRequest) DrawCircleV(this->player.getHitbox().pos, this->player.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(tmgr[TEX_PLAYER], this->playerAnim, this->player.getHitbox().pos, this->isMoving, this->playerSprite, WHITE);
    DrawInfoLabel(this->player.getHitbox(), PLAYER_FRAME_H, {fmgr[ENTITY_LABEL], this->player.getName(), 15.0f, 2.0f});
}

void Game::playerInteractions() {
    // Avec le fermier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->farmer.getHitbox().pos, this->farmer.getHitbox().radius)) {
        DrawInfoLabel(this->farmer.getHitbox(), FARMER_FRAME_H, {fmgr[ENTITY_LABEL], farmer.getName(), 15.0f, 2.0f});
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = FARMER;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le garde
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->guard.getHitbox().pos, this->guard.getHitbox().radius)) {
        DrawInfoLabel(this->guard.getHitbox(), GUARD_FRAME_H, {fmgr[ENTITY_LABEL], guard.getName(), 15.0f, 2.0f});
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = GUARD;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le sorcier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius)) {
        DrawInfoLabel(this->sorcerer.getHitbox(), SORCERER_FRAME_H, {fmgr[ENTITY_LABEL], sorcerer.getName(), 15.0f, 2.0f});
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = SORCERER;
            this->dialogueRequest = false;
        }
    }

    // Avec les patates
    for (int i = 0; i < POTATO_AVAILABLE; ++i) {
        if (CheckCollisionCircles(this->player.getHitbox().pos, player.getHitbox().radius, this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius)) {
            DrawInfoLabel(potato[i].getHitbox(), POTATO_FRAME_H, {fmgr[ENTITY_LABEL], potato[i].getName(), 15.0f, 2.0f});
            if (this->dialogueRequest) {
                this->displayDialogue.request = !this->displayDialogue.request;
                this->displayDialogue.entity = ITEM;
                this->dialogueRequest = false;
            }
        }
    }
}

void Game::renderInventory() {
    if (!this->inventoryRequest) return;

    // --- Texture et centrage de la fenêtre d'inventaire ---
    Texture2D inventory = this->tmgr[TEX_SLOT];

    float inventoryW = inventory.width  * INVENTORY_SCALE;
    float inventoryH = inventory.height * INVENTORY_SCALE;

    Vector2 pos = {
        SCREEN_WIDTH  / 2.0f - inventoryW / 2.0f,
        SCREEN_HEIGHT / 2.0f - inventoryH / 2.0f
    };

    DrawTextureEx(inventory, pos, 0.0f, INVENTORY_SCALE, WHITE);

    // --- Paramètres des items ---
    Vector2 itemPos = { pos.x + 5.0f * INVENTORY_SCALE, pos.y + 5.0f * INVENTORY_SCALE }; // premier slot
    float itemScale = INVENTORY_SCALE * 0.88f;
    float gap = 3.0f * INVENTORY_SCALE;

    // On prend la patate comme référence de taille de cellule
    const Texture2D potatoTex = this->tmgr[TEX_POTATO_STATIC];
    const Texture2D carrotTex = this->tmgr[TEX_CARROT_STATIC];
    const Texture2D appleTex  = this->tmgr[TEX_APPLE_STATIC];
    const float cellW = potatoTex.width  * itemScale;
    const float cellH = potatoTex.height * itemScale;

    // --- Grille ---
    const int totalSlots = MAX_INVENTORY_LENGTH;
    int rowSize = static_cast<int>(sqrt(static_cast<float>(totalSlots)));
    if (rowSize * rowSize < totalSlots) rowSize += 1; // dernière ligne partielle si besoin

    for (int i = 0; i < rowSize; ++i) {
        for (int j = 0; j < rowSize; ++j) {
            int idx = i * rowSize + j;
            if (idx >= totalSlots) break;

            // Position du slot (coin haut-gauche)
            Vector2 cellPos = {
                itemPos.x + j * (cellW + gap),
                itemPos.y + i * (cellH + gap)
            };

            // Récup item & affichage selon l'ID
            const Item& it = this->player.inventory().getItem(idx);
            switch (it.getId()) {
                case ID_POTATO:
                    DrawTextureEx(potatoTex, cellPos, 0.0f, itemScale, WHITE);
                    break;
                case ID_CARROT:
                    DrawTextureEx(carrotTex, cellPos, 0.0f, itemScale, WHITE);
                    break;
                case ID_APPLE:
                    DrawTextureEx(appleTex, cellPos, 0.0f, itemScale, WHITE);
                    break;
                default:
                    break;
            }
        }
    }

    if (this->invSelectorVisible) {
        int i = this->invSelectorIndex / rowSize;
        int j = this->invSelectorIndex % rowSize;

        Vector2 selPos = {
            itemPos.x + j * (cellW + gap),
            itemPos.y + i * (cellH + gap)
        };

        Rectangle r = { selPos.x, selPos.y, cellW, cellH };
        float cornerLen   = 6.0f * INVENTORY_SCALE;
        float cornerThick = 2.0f * INVENTORY_SCALE;
        DrawCornerMarkers(r, cornerLen, cornerThick, GOLD);
    }
}

void Game::dialogue() {
    if (!this->displayDialogue.request) return;
    this->isPause = true;

    TextStyle basic = {fmgr[DIALOGUE_LABEL], "", 25.0f, 2.0f, WHITE};

    switch (this->displayDialogue.entity) {
        case ITEM: {
            DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_POTATO], this->potatoAnim, this->potatoSprite, WHITE);
            basic.text = "Vous récoltez " + to_string(POTATO_AVAILABLE) + " patate.";
            DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);
            
            basic.text = "Terminer [Press space]";
            basic.font_size = 18.0f;
            if (dialogueChoice(basic)) {
                this->isPause = false;
                this->displayDialogue.request = false;
                this->player.inventory().add(this->potato[0], POTATO_AVAILABLE);
            }
            break;
        }
        case FARMER: {
            DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_FARMER], this->farmerAnim, this->farmerSprite, WHITE);
            basic.text = "Je suis un fermier.";
            DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);
            
            basic.text = "Terminer [Press space]";
            if (dialogueChoice(basic)) {
                this->isPause = false;
                this->displayDialogue.request = false;
            }
            break;
        }
        case GUARD: {
            bool hasEnough = this->player.inventory().hasEnoughOf(ID_APPLE, AMOUNT_TO_FINISH_GAME);
            int quantity = this->player.inventory().getItemQuantity(ID_APPLE);

            DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_GUARD], this->guardAnim, this->guardSprite, RED);
            basic.text = hasEnough ? string("Bravo, vous avez assez de ") + this->apple.getName() + ". Vous avez terminé le jeu." : string("Vous n'avez pas assez de ") + this->apple.getName() + " (Vous avez " + to_string(quantity) + "/" + to_string(AMOUNT_TO_FINISH_GAME) + ")";
            DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);
            
            basic.text = "Terminer [Press space]";
            if (dialogueChoice(basic)) {
                this->isPause = false;
                this->displayDialogue.request = false;
                if (hasEnough) this->gameEnded = true;
            }
            break;
        }
        case SORCERER: {
            DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_SORCERER], this->sorcererAnim, this->sorcererSprite, BLUE);
            if (this->player.inventory().isEmpty()) this->sorcererStep = 4;
            switch (sorcererStep) {
                case 0: {
                    basic.text = "Vous souhaitez renommer un objet de votre inventaire ?";
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);
                    
                    basic.text = "Très bien.";
                    Vector2 labelSize = MeasureTextEx(basic.font, basic.text.c_str(), basic.font_size, basic.spacing);
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y + labelSize.y}, basic.font_size, basic.spacing, basic.color);            
                    
                    basic.text = "Continue [Press space]";
                    if (dialogueChoice(basic)) sorcererStep = 1;
                    break;
                }
                case 1: {
                    this->inventoryRequest = true;
                    this->invSelectorVisible = true;

                    basic.text = "Séléctionnez le avec ZQSD ou les flêches.";
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);

                    basic.text = "Valider [Press space]";
                    if (dialogueChoice(basic)) {
                        this->inventoryRequest = false;
                        this->invSelectorVisible = false;
                        sorcererStep = 2;
                    }
                    break;
                }
                case 2: {
                    getNewName();

                    basic.text = "Saisissez le nouveau nom.";
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);

                    basic.text = "Valider [Press enter]";
                    dialogueChoice(basic);
                    if (this->newName != nullptr) {
                        Item item = this->player.inventory().getItem(this->invSelectorIndex);
                        item.changeName(this->newName);
                        this->player.inventory().settem(item, this->invSelectorIndex);
                        this->invSelectorIndex = 0;
                        sorcererStep = 3;
                    }
                    break;
                }
                case 3: {
                    basic.text = "Vous avez renomé votre objet en :";
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);
                    
                    basic.text = string(this->newName);
                    Vector2 labelSize = MeasureTextEx(basic.font, basic.text.c_str(), basic.font_size, basic.spacing);
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y + labelSize.y}, basic.font_size, basic.spacing, basic.color);            
                    
                    basic.text = "Terminer [Press space]";
                    if (dialogueChoice(basic)) {
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->newName = nullptr;
                        sorcererStep = 0; // reset
                    }
                    break;
                }
                case 4: {
                    basic.text = "Vous n'avez aucun objet à renommer.";
                    DrawTextEx(basic.font, basic.text.c_str(), {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y}, basic.font_size, basic.spacing, basic.color);

                    basic.text = "Continue [Press space]";
                    if (dialogueChoice(basic)) {
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        sorcererStep = 0; // reset
                    }
                    break;
                }
            }
            break;
        }
        default: {
            // TODO un truc en mode c'est pas sensé arrivé
            break;
        }
    }
}

void Game::getNewName() {
    static char buffer[BUFFER_SIZE] = {0}; // texte en cours
    static int length = 0;                 // longueur actuelle
    static bool active = true;

    // Dimensions
    float boxW = 400;
    float boxH = 60;
    float posX = SCREEN_WIDTH  / 2.0f - boxW / 2.0f;
    float posY = SCREEN_HEIGHT / 2.0f - boxH / 2.0f;

    // Fond
    DrawRectangleRounded({posX, posY, boxW, boxH}, 0.2f, 10, Fade(LIGHTGRAY, 0.8f));
    DrawRectangleRoundedLines({posX, posY, boxW, boxH}, 0.2f, 10, DARKGRAY);

    // Input clavier
    if (active) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && length < BUFFER_SIZE - 1) {
                buffer[length++] = (char)key;
                buffer[length] = '\0'; // termine la chaîne
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && length > 0) {
            buffer[--length] = '\0';
        }
    }

    // Affiche le texte
    Vector2 textSize = MeasureTextEx(this->fmgr[0], buffer, 30, 2);
    DrawTextEx(this->fmgr[0], buffer, {posX + 10, posY + boxH/2 - textSize.y/2}, 30, 2, BLACK);

    // Curseur clignotant
    if (((int)(GetTime()*2)) % 2 == 0) {
        DrawTextEx(this->fmgr[0], "|", {posX + 15 + textSize.x, posY + boxH/2 - textSize.y/2}, 30, 2, DARKGRAY);
    }

    // Validation avec Entrée
    if (IsKeyPressed(KEY_ENTER)) {
        active = false;
        this->newName = buffer;
    }
}

bool Game::dialogueChoice(TextStyle label) {
    // Taille du texte
    Vector2 textSize = MeasureTextEx(label.font, label.text.c_str(), label.font_size, label.spacing);

    // Position en bas à droite
    Vector2 textPos = {
        DIALOGUE_CONTENT_END_X - textSize.x,
        DIALOGUE_CONTENT_END_Y - textSize.y
    };

    DrawTextEx(label.font, label.text.c_str(), textPos, label.font_size, label.spacing, label.color);

    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
}

void Game::resetRequests() {
    this->dialogueRequest = false;
}

Game::~Game() {
    // Déchargement des textures
    for (int i = 0; i < TEX_MAX; i++) {
        UnloadTexture(this->tmgr[i]);
    }

    // Déchargement des fonts
    for (int i = 0; i < FONT_MAX; i++) {
        UnloadFont(this->fmgr[i]);
    }

    CloseWindow();
}

// --- Helper: coins en L autour d'un rectangle ---
void DrawCornerMarkers(const Rectangle& r, float len, float thick, Color color) {
    // Top-left
    DrawLineEx({r.x, r.y}, {r.x + len, r.y}, thick, color);
    DrawLineEx({r.x, r.y}, {r.x, r.y + len}, thick, color);
    // Top-right
    DrawLineEx({r.x + r.width - len, r.y}, {r.x + r.width, r.y}, thick, color);
    DrawLineEx({r.x + r.width, r.y}, {r.x + r.width, r.y + len}, thick, color);
    // Bottom-left
    DrawLineEx({r.x, r.y + r.height - len}, {r.x, r.y + r.height}, thick, color);
    DrawLineEx({r.x, r.y + r.height}, {r.x + len, r.y + r.height}, thick, color);
    // Bottom-right
    DrawLineEx({r.x + r.width - len, r.y + r.height}, {r.x + r.width, r.y + r.height}, thick, color);
    DrawLineEx({r.x + r.width, r.y + r.height - len}, {r.x + r.width, r.y + r.height}, thick, color);
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
    Vector2 label_size = MeasureTextEx(text.font, text.text.c_str(), text.font_size, text.spacing);
    float padding = 5.0f;

    // Calcul la position
    Vector2 position;
    position.x = entity.pos.x - (label_size.x / 2);
    position.y = entity.pos.y - entitySize - label_size.y - padding;
    
    DrawTextEx(text.font, text.text.c_str(), position, text.font_size, text.spacing, BLACK);
}

void DrawDialogueFrame(Texture2D dialogue, Texture2D entity, AnimationState entityAnim, SpriteSheetInfo entitySprite, Color color) {
    // Destination (où et quelle taille à l'écran)
    Rectangle rec = {
        DIALOGUE_POS_X,    // X centré
        DIALOGUE_POS_Y,    // Y en bas
        UI_DIALOGUE_WIDTH, // largeur voulue
        UI_DIALOGUE_HEIGHT // hauteur voulue
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

