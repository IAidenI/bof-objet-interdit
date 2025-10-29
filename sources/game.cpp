#include "game.hpp"

Game::Game(const char **texturesPath, const char **fontPath) 
: player("Vous", {{675, 440}, 80}),
  farmer("Fermier", {{770, 630}, 80}),
  guard("Garde", {{580, 300}, 80}),
  sorcerer("Sorcier", {{350, 490}, 80}),
  gdb(reinterpret_cast<uintptr_t>(&this->player)) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BOF : L'objet interdit");
    SetTargetFPS(60);

    // ---- Initialisation des textures ----
    for (int i = 0; i < TEX_MAX; i++) {
        this->tmgr[i] = LoadTexture(texturesPath[i]);
    }

    // ---- Initialisation de fonts ----
    for (int i = 0; i < FONT_MAX; i++) {
        for (int j = 0; j < FONT_SIZE_COUNT; j++) {
            int size = FONT_SIZES[j];
            fmgr[i][j] = LoadFontEx(fontPath[i], size, 0, 250);
        }
    }

    // ---- Initialisation des objets ----
    this->carrot = getItem(CARROT);
    this->apple  = getItem(APPLE);

    // Initialisation des patates
    Position initialPosition = {270, 740};
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

    // ---- Initialise la saisie utilisateur ----
    this->newName.font = &this->getFont(INFO_LABEL, 40);
    this->newName.text = "";
    this->newName.fontSize = 40.0f;
    this->newName.spacing = 2.0f;
    this->newName.color = COLOR_INPUT_BOX_TEXT;
}

Font& Game::getFont(FontID id, int size) {
    for (int j = 0; j < FONT_SIZE_COUNT; j++) {
        if (FONT_SIZES[j] == size) {
            return this->fmgr[id][j];
        }
    }
    // Renvoyer la taille la plus proche
    return this->fmgr[id][0];
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

    if (IsKeyPressed(KEY_P)) this->stackRequest = !this->stackRequest;
}

void Game::displayBackground() {
    Texture2D bg = this->tmgr[TEX_BACKGROUND];
    Frame src = {0, 0, (float)bg.width, (float)bg.height};
    Frame dst = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    Position origin = {0, 0};

    DrawTexturePro(bg, src, dst, origin, 0.0f, WHITE);
}

void Game::displayCommands() {
    TextStyle cardTitle = { &this->getFont(INFO_LABEL, 25), "Commandes", 25.0f, 2.0f, COLOR_STACK_TEXT };
    vector<vector<InfoSegment>> cardContent = {        
        { 
            { { &this->getFont(INFO_LABEL, 20), "ZQSD / Flêches : ", 20.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 20), "Déplacement",       20.0f, 2.0f, COLOR_STACK_TEXT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 20), "E : ",      20.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 20), "Interagir", 20.0f, 2.0f, COLOR_STACK_TEXT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 20), "I : ",       20.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 20), "Inventaire", 20.0f, 2.0f, COLOR_STACK_TEXT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 20), "H : ",          20.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 20), "Hitbox ON/OFF", 20.0f, 2.0f, COLOR_STACK_TEXT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 20), "P : ",             20.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 20), "Affichage avancé", 20.0f, 2.0f, COLOR_STACK_TEXT } },
        },
    };

    Card card = {
        .title = cardTitle,
        .content = cardContent,
        .config = { .position = TOP_LEFT }
    };

    DrawCard(card, START_FRAME);
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
    DrawInfoLabel(this->player.getHitbox(), PLAYER_FRAME_H, { fmgr[ENTITY_LABEL], this->player.getName(), 15.0f, 2.0f, WHITE });
}

void Game::playerInteractions() {
    // Avec le fermier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->farmer.getHitbox().pos, this->farmer.getHitbox().radius)) {
        DrawInfoLabel(this->farmer.getHitbox(), FARMER_FRAME_H, { fmgr[ENTITY_LABEL], farmer.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = FARMER;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le garde
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->guard.getHitbox().pos, this->guard.getHitbox().radius)) {
        DrawInfoLabel(this->guard.getHitbox(), GUARD_FRAME_H, { fmgr[ENTITY_LABEL], guard.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = GUARD;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le sorcier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius)) {
        DrawInfoLabel(this->sorcerer.getHitbox(), SORCERER_FRAME_H, { fmgr[ENTITY_LABEL], sorcerer.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = SORCERER;
            this->dialogueRequest = false;
        }
    }

    // Avec les patates
    for (int i = 0; i < POTATO_AVAILABLE; ++i) {
        if (CheckCollisionCircles(this->player.getHitbox().pos, player.getHitbox().radius, this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius)) {
            DrawInfoLabel(potato[i].getHitbox(), POTATO_FRAME_H, { fmgr[ENTITY_LABEL], potato[i].getName(), 15.0f, 2.0f, WHITE });
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

    Position pos = {
        SCREEN_WIDTH  / 2.0f - inventoryW / 2.0f,
        SCREEN_HEIGHT / 2.0f - inventoryH / 2.0f
    };

    DrawTextureEx(inventory, pos, 0.0f, INVENTORY_SCALE, WHITE);

    // --- Paramètres des items ---
    Position itemPos = { pos.x + 5.0f * INVENTORY_SCALE, pos.y + 5.0f * INVENTORY_SCALE }; // premier slot
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
            Position cellPos = {
                itemPos.x + j * (cellW + gap),
                itemPos.y + i * (cellH + gap)
            };

            float radiusCircleInfo = 13.0f;
            Position bottomRight = {
                cellPos.x + cellW - radiusCircleInfo,
                cellPos.y + cellH - radiusCircleInfo
            };

            // Récup item & affichage selon l'ID ainsi que le nombre de l'item
            const Item& it = this->player.inventory().getItem(idx);
            switch (it.getId()) {
                case ID_POTATO: {
                    DrawTextureEx(potatoTex, cellPos, 0.0f, itemScale, WHITE);
                    DrawCircleV(bottomRight, radiusCircleInfo, BEIGE_LIGHT);
                    
                    // Ton texte
                    string txt = "x" + to_string(this->player.inventory().getSlotQuantity(idx));
                    float fontSize = 17.0f;
                    float spacing = 0.0f;

                    // Mesurer la taille du texte
                    Size textSize = MeasureTextEx(*this->fmgr[DIALOGUE_LABEL], txt.c_str(), fontSize, spacing);

                    // Calculer la position pour centrer
                    Position textPos = {
                        bottomRight.x - textSize.x / 2,
                        bottomRight.y - textSize.y / 2
                    };

                    // Dessiner le texte centré
                    DrawTextEx(*this->fmgr[DIALOGUE_LABEL], txt.c_str(), textPos, fontSize, spacing, BLACK);
                    break;
                }
                case ID_CARROT: {
                    DrawTextureEx(carrotTex, cellPos, 0.0f, itemScale, WHITE);
                    DrawCircleV(bottomRight, radiusCircleInfo, BEIGE_LIGHT);
                    
                    // Ton texte
                    string txt = "x" + to_string(this->player.inventory().getSlotQuantity(idx));
                    float fontSize = 16.0f;
                    float spacing = 0.0f;

                    // Mesurer la taille du texte
                    Size textSize = MeasureTextEx(*this->fmgr[DIALOGUE_LABEL], txt.c_str(), fontSize, spacing);

                    // Calculer la position pour centrer
                    Position textPos = {
                        bottomRight.x - textSize.x / 2,
                        bottomRight.y - textSize.y / 2
                    };

                    // Dessiner le texte centré
                    DrawTextEx(*this->fmgr[DIALOGUE_LABEL], txt.c_str(), textPos, fontSize, spacing, BLACK);
                    break;
                }
                case ID_APPLE: {
                    DrawTextureEx(appleTex, cellPos, 0.0f, itemScale, WHITE);
                    DrawCircleV(bottomRight, radiusCircleInfo, BEIGE_LIGHT);
                    
                    // Ton texte
                    string txt = "x" + to_string(this->player.inventory().getSlotQuantity(idx));
                    float fontSize = 16.0f;
                    float spacing = 0.0f;

                    // Mesurer la taille du texte
                    Size textSize = MeasureTextEx(*this->fmgr[DIALOGUE_LABEL], txt.c_str(), fontSize, spacing);

                    // Calculer la position pour centrer
                    Position textPos = {
                        bottomRight.x - textSize.x / 2,
                        bottomRight.y - textSize.y / 2
                    };

                    // Dessiner le texte centré
                    DrawTextEx(*this->fmgr[DIALOGUE_LABEL], txt.c_str(), textPos, fontSize, spacing, BLACK);
                    break;
                }
                default:
                    break;
            }
        }
    }

    // Sélécteur pour le dialogue du sorcier
    if (this->invSelectorVisible) {
        int i = this->invSelectorIndex / rowSize;
        int j = this->invSelectorIndex % rowSize;

        Position selPos = {
            itemPos.x + j * (cellW + gap),
            itemPos.y + i * (cellH + gap)
        };

        Frame r = { selPos.x, selPos.y, cellW, cellH };
        float cornerLen   = 6.0f * INVENTORY_SCALE;
        float cornerThick = 2.0f * INVENTORY_SCALE;
        DrawCornerMarkers(r, cornerLen, cornerThick, GOLD);
    }
}

void Game::renderStack() {
    if (!this->stackRequest) return;

    /*
    
    -- Stack informations --

    */

    TextStyle cardTitle = { &this->getFont(INFO_LABEL, 25), "Stack Trace", 25.0f, 2.0f, COLOR_STACK_TEXT };
    vector<vector<InfoSegment>> cardContent = {        
        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "00 00 00 00 00 00 00 00", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), ". . . . . . . .",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "00 00 00 00 00 00 00 00", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), ". . . . . . . .",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },

        { 
            { { &this->getFont(INFO_LABEL, 16), "0x00007ffe6bf7c3f0",      16.0f, 2.0f, COLOR_STACK_ADDR } },
            { { &this->getFont(INFO_LABEL, 16), " : ",                     16.0f, 2.0f, COLOR_STACK_TEXT } },
            { { &this->getFont(INFO_LABEL, 16), "41 41 41 41 41 41 41 41", 16.0f, 2.0f, COLOR_STACK_HEXA } },
            { { &this->getFont(INFO_LABEL, 16), " | ",                     16.0f, 2.0f, COLOR_STACK_SPLIT } },
            { { &this->getFont(INFO_LABEL, 16), "A A A A A A A A",         16.0f, 2.0f, COLOR_STACK_ASCII } },
            { { &this->getFont(INFO_LABEL, 16), " |",                      16.0f, 2.0f, COLOR_STACK_SPLIT } },
        },
    };

    vector<vector<InfoSegment>> data = {
        {
            { { &this->getFont(INFO_LABEL, 22), "Voici un pseudo affichage de la stack, pour plus de ", 22.0f, 1.0f, COLOR_STACK_TEXT } }
        },

        {
            { { &this->getFont(INFO_LABEL, 22), "détails ouvrir gdb et faire x/x $rsp.", 22.0f, 1.0f, COLOR_STACK_TEXT } },
        }
    };

    vector<Card> cards = {
        { cardTitle, cardContent, { .position = TOP_LEFT } },
    };

    vector<DataSection> dataSection = {
        { data, { .position = BOT_LEFT }  },
    };
    DrawInfoSection(cards, dataSection, TOP_RIGHT, START_FRAME);
}

void Game::dialogue() {
    if (!this->displayDialogue.request) return;
    this->isPause = true;

    TextStyle basic = {fmgr[DIALOGUE_LABEL], "", 25.0f, 2.0f, WHITE};

    switch (this->displayDialogue.entity) {
        case ITEM: {
            DrawDialogueFrame(tmgr[TEX_DIALOGUE], tmgr[TEX_POTATO], this->potatoAnim, this->potatoSprite, WHITE);
            basic.text = "Vous récoltez " + to_string(POTATO_AVAILABLE) + " patate.";
            DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});
            
            basic.text = "Terminer [Press space]";
            basic.fontSize = 18.0f;
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
            DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});
            
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
            DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});
            
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
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});
                    
                    basic.text = "Très bien.";
                    Size labelSize = MeasureTextStyled(basic);
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y + labelSize.y});            
                    
                    basic.text = "Continue [Press space]";
                    if (dialogueChoice(basic)) sorcererStep = 1;
                    break;
                }
                case 1: {
                    this->inventoryRequest = true;
                    this->invSelectorVisible = true;

                    basic.text = "Séléctionnez le avec ZQSD ou les flêches.";
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});

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
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});

                    basic.text = "Valider [Press enter]";
                    if (dialogueChoice(basic)) {
                        Item item = this->player.inventory().getItem(this->invSelectorIndex);
                        item.changeName(this->newName.text.c_str());
                        this->player.inventory().settem(item, this->invSelectorIndex);
                        this->invSelectorIndex = 0;
                        sorcererStep = 3;
                    }
                    break;
                }
                case 3: {
                    basic.text = "Vous avez renomé votre objet en :";
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});
                    
                    basic.text = this->newName.text;
                    Size labelSize = MeasureTextStyled(basic);
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y + labelSize.y});            
                    
                    basic.text = "Terminer [Press space]";
                    if (dialogueChoice(basic)) {
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->newName.text = "";
                        sorcererStep = 0; // reset
                    }
                    break;
                }
                case 4: {
                    basic.text = "Vous n'avez aucun objet à renommer.";
                    DrawTextStyled(basic, {DIALOGUE_CONTENT_POS_X, DIALOGUE_CONTENT_POS_Y});

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
    // Récupère le caractère pressé
    int key = GetCharPressed();

    // Vide la file et ajoute chaque caractère tant que la taille max n'est pas atteinte
    while (key > 0) {
        // On filtre les caractères imprimables (32..125)
        if ((key >= 32) && (key <= 125) && (int)this->newName.text.size() < MAX_INPUT_CHARS) {
            this->newName.text.push_back(static_cast<char>(key));
        }
        key = GetCharPressed();  // Récupère le caractère suivant dans la queue
    }

    // Supprime le dernier caractère si la variable n'est pas vide
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (!this->newName.text.empty()) this->newName.text.pop_back();
    }

    // Affiche la box
    DrawInputBox(this->newName, MAX_INPUT_CHARS, { SCREEN_WIDTH, SCREEN_HEIGHT });
}

bool Game::dialogueChoice(TextStyle label) {
    // Taille du texte
    Size textSize = MeasureTextStyled(label);

    // Position en bas à droite
    Position textPos = {
        DIALOGUE_CONTENT_END_X - textSize.x,
        DIALOGUE_CONTENT_END_Y - textSize.y
    };

    DrawTextStyled(label, textPos);

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
        for (int j = 0; j < FONT_SIZE_COUNT; j++) {
            UnloadFont(this->fmgr[i][j]);
        }
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

void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Position pos, bool moving, SpriteSheetInfo entitySprite, Color color) {
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
    Frame src = {
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
    Frame dst = {
        pos.x,
        pos.y,
        entitySprite.frameW * anim.scale,
        entitySprite.frameH * anim.scale
    };
    Position origin = { dst.width / 2.0f, dst.height / 2.0f };

    DrawTexturePro(texture, src, dst, origin, 0.0f, color);
}

void DrawStaticItem(const Texture2D& texture, Position pos, float scale) {
    // Source = texture entière
    Frame src = { 0, 0, (float)texture.width, (float)texture.height };

    // Destination = centré sur la hitbox
    Frame dst = {
        pos.x,  // centre X
        pos.y,  // centre Y
        texture.width * scale,
        texture.height * scale
    };

    // Origine = le centre de la texture
    Position origin = { dst.width / 2.0f, dst.height / 2.0f };

    DrawTexturePro(texture, src, dst, origin, 0.0f, WHITE);
}

void DrawInfoLabel(Hitbox entity, int entitySize, TextStyle text) {
    Size label_size = MeasureTextStyled(text);
    float padding = 5.0f;

    // Calcul la position
    Position position;
    position.x = entity.pos.x - (label_size.x / 2);
    position.y = entity.pos.y - entitySize - label_size.y - padding;
    
    DrawTextStyled(text, position);
}

void DrawDialogueFrame(Texture2D dialogue, Texture2D entity, AnimationState entityAnim, SpriteSheetInfo entitySprite, Color color) {
    // Destination (où et quelle taille à l'écran)
    Frame rec = {
        DIALOGUE_POS_X,    // X centré
        DIALOGUE_POS_Y,    // Y en bas
        UI_DIALOGUE_WIDTH, // largeur voulue
        UI_DIALOGUE_HEIGHT // hauteur voulue
    };

    // Source (on prend toute la texture)
    Frame src = { 0, 0, (float)dialogue.width, (float)dialogue.height };

    // Afficher texture redimensionnée dans rec
    DrawTexturePro(dialogue, src, rec, (Position){0, 0}, 0.0f, WHITE);

    // Calcul la position du profil
    float squareSize = rec.height;  // hauteur de la texture affichée
    if (squareSize > rec.width) squareSize = rec.width; // rester carré

    Position center = {
        rec.x + squareSize / 2.0f,
        rec.y + squareSize / 2.0f
    };

    DrawAnimatedEntity(entity, entityAnim, center, false, entitySprite, color);
}

void DrawInfoFrame(int posX, int posY, int rectW, int rectH) {
    // Fond + contour
    DrawRectangle(posX, posY, rectW, rectH, Fade(LIGHTGRAY, 0.7f));
    DrawRectangleLines(posX, posY, rectW, rectH, DARKGRAY);
}
