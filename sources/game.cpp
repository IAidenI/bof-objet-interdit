#include "game.hpp"

Game::Game()
: player("Vous", { { 600.0f, 500.0f }, 80.0f }),
  farmer("Fermier", { { 770.0f, 630.0f }, 50.0f }),
  guard("Garde", { { 580.0f, 300.0f }, 50.0f }),
  sorcerer("Sorcier", { { 350.0f, 490.0f }, 50.0f }),
  trash("Poubelle", { { 430.0f, 620.0f }, 50.0f }),
  gdb(reinterpret_cast<uintptr_t>(&this->player)) {

    InitWindow((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, "BOF : L'objet interdit");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    Image icon = LoadImage(ICON);
    SetWindowIcon(icon);
    UnloadImage(icon);
}

bool Game::init() {
    if (!manager.init()) {
        cerr << "[FATAL] Manager invalide : ressources manquantes." << endl;
        return false;
    }

    // ---- Initialisation des objets ----
    this->carrot = getItem(CARROT);
    this->apple  = getItem(APPLE);
    this->apple.setPosition({ 1000.0f, 330.0f });

    // Initialisation des patates
    Position initialPosition = { 270.0f, 740.0f };
    int spacingX = 200;  // distance horizontale
    int spacingY = 100;  // distance verticale
    int cols     = 4;    // 4 par ligne

    for (int i = 0; i < POTATO_AVAILABLE; i++) {
        int row = i / cols;
        int col = i % cols;

        float x = initialPosition.x + col * spacingX;
        float y = initialPosition.y + row * spacingY;

        this->potato[i] = getItem(POTATO);
        this->potato[i].setPosition({ x, y });
    }

    // ---- Initialisation des animations ----
    this->playerAnim = { .frame = 0, .timer = 0.0f, .facingRight = true, .frameTimeIdle = 0.12f, .frameTimeMove = 0.12f, .scale = 3.0f };

    this->farmerAnim   = { .frame = 0, .timer = 0.0f, .facingRight = false, .frameTimeIdle = 0.12f, .frameTimeMove = 0.0f, .scale = 4.0f };
    this->guardAnim    = { .frame = 0, .timer = 0.0f, .facingRight = true, .frameTimeIdle = 0.15f, .frameTimeMove = 0.0f, .scale = 4.0f };
    this->sorcererAnim = { .frame = 0, .timer = 0.0f, .facingRight = true, .frameTimeIdle = 0.12f, .frameTimeMove = 0.0f, .scale = 4.0f };

    this->potatoAnim = { .frame = 0, .timer = 0.0f, .facingRight = true, .frameTimeIdle = 1.0f, .frameTimeMove = 0.0f, .scale = 0.8f };
    this->appleAnim  = { .frame = 0, .timer = 0.0f, .facingRight = true, .frameTimeIdle = 0.1f, .frameTimeMove = 0.0f, .scale = 0.8f };

    // ---- Initialise la saisie utilisateur ----
    this->newName = { .font = &this->manager.getFont(INFO_LABEL, 40), .text = "", .fontSize = 40.0f, .spacing = 2.0f, .color = COLOR_INPUT_BOX_TEXT };

    this->notification.config(START_FRAME, NOTIFICATION_MAX_WIDTH, this->manager.getFont(INFO_LABEL, 18), 18.0f);
    return true;
}

void Game::handlePlayerMovements() {
    if (this->isPause) return;

    Position pos = this->player.getHitbox().pos;

    this->isMoving = false;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        pos.x += PLAYER_SPEED;
        this->isMoving = true;
        this->playerAnim.facingRight = true;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        pos.x -= PLAYER_SPEED;
        this->isMoving = true;
        this->playerAnim.facingRight = false;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        pos.y -= PLAYER_SPEED;
        this->isMoving = true;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        pos.y += PLAYER_SPEED;
        this->isMoving = true;
    }

    float radius = this->player.getHitbox().radius;

    float minX = START_FRAME.x + radius;
    float maxX = START_FRAME.x + START_FRAME.width - radius;
    float minY = START_FRAME.y + radius;
    float maxY = START_FRAME.y + START_FRAME.height - radius;

    if (pos.x < minX) pos.x = minX;
    if (pos.x > maxX) pos.x = maxX;
    if (pos.y < minY) pos.y = minY;
    if (pos.y > maxY) pos.y = maxY;

    this->player.setPosition(pos);
}

void Game::handlePlayerInput() {
    if (IsKeyPressed(KEY_ESCAPE)) this->cancel = true;

    if (this->inventoryRequest && this->invSelectorVisible) {
        int rowSize = static_cast<int>(sqrt(static_cast<float>(MAX_INVENTORY_LENGTH)));
        if (rowSize * rowSize < MAX_INVENTORY_LENGTH) rowSize += 1;
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) this->invSelectorIndex = (this->invSelectorIndex + 1) % MAX_INVENTORY_LENGTH;
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) this->invSelectorIndex = (this->invSelectorIndex - 1 + MAX_INVENTORY_LENGTH) % MAX_INVENTORY_LENGTH;
        if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) this->invSelectorIndex = (this->invSelectorIndex + rowSize) % MAX_INVENTORY_LENGTH;
        if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) this->invSelectorIndex = (this->invSelectorIndex - rowSize + MAX_INVENTORY_LENGTH) % MAX_INVENTORY_LENGTH;
        return;
    }

    if (this->isTyping || this->displayDialogue.request) return;

    if (IsKeyPressed(KEY_I)) {
        this->inventoryRequest = !this->inventoryRequest;
        this->isPause = !this->isPause;
    }

    if (IsKeyPressed(KEY_E)) this->dialogueRequest = true;
    if (IsKeyPressed(KEY_F1)) this->hitboxRequest = !this->hitboxRequest;
    if (IsKeyPressed(KEY_F2)) this->stackRequest = !this->stackRequest;
}

void Game::displayBackground() {
    Texture2D bg = this->manager.getTexture(TEX_BACKGROUND);
    Frame src = {0, 0, (float)bg.width, (float)bg.height};
    Frame dst = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    Position origin = {0, 0};

    DrawTexturePro(bg, src, dst, origin, 0.0f, WHITE);
}

void Game::displayCommands() {
    TextStyle cardTitle = { &this->manager.getFont(INFO_LABEL, 25), "Commandes", 25.0f, 2.0f, COLOR_COMMANDS_ACTION };
    vector<vector<InfoSegment>> cardContent = {        
        { 
            { { &this->manager.getFont(INFO_LABEL, 20), "ZQSD / Flêches : ", 20.0f, 2.0f, COLOR_COMMANDS_LABEL } },
            { { &this->manager.getFont(INFO_LABEL, 20), "Déplacement",       20.0f, 2.0f, COLOR_COMMANDS_ACTION } },
        },

        { 
            { { &this->manager.getFont(INFO_LABEL, 20), "E : ",      20.0f, 2.0f, COLOR_COMMANDS_LABEL } },
            { { &this->manager.getFont(INFO_LABEL, 20), "Interagir", 20.0f, 2.0f, COLOR_COMMANDS_ACTION } },
        },

        { 
            { { &this->manager.getFont(INFO_LABEL, 20), "I : ",       20.0f, 2.0f, COLOR_COMMANDS_LABEL } },
            { { &this->manager.getFont(INFO_LABEL, 20), "Inventaire", 20.0f, 2.0f, COLOR_COMMANDS_ACTION } },
        },

        { 
            { { &this->manager.getFont(INFO_LABEL, 20), "F1 : ",          20.0f, 2.0f, COLOR_COMMANDS_LABEL } },
            { { &this->manager.getFont(INFO_LABEL, 20), "Hitbox ON/OFF", 20.0f, 2.0f, COLOR_COMMANDS_ACTION } },
        },

        { 
            { { &this->manager.getFont(INFO_LABEL, 20), "F2 : ",             20.0f, 2.0f, COLOR_COMMANDS_LABEL } },
            { { &this->manager.getFont(INFO_LABEL, 20), "Affichage avancé", 20.0f, 2.0f, COLOR_COMMANDS_ACTION } },
        },

        { 
            { { &this->manager.getFont(INFO_LABEL, 20), "ESC : ",             20.0f, 2.0f, COLOR_COMMANDS_LABEL } },
            { { &this->manager.getFont(INFO_LABEL, 20), "Ferme le menu ouvert", 20.0f, 2.0f, COLOR_COMMANDS_ACTION } },
        },
    };

    Card card = {
        .title = cardTitle,
        .content = cardContent,
        .config = { .position = TOP_LEFT, .backgroundColor = COLOR_COMMANDS_BACKGROUND }
    };

    DrawCard(card, START_FRAME);
}

void Game::displayPNJ() {
    if (this->hitboxRequest) DrawCircleV(this->farmer.getHitbox().pos, this->farmer.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->manager.getTexture(TEX_FARMER), this->farmerAnim, this->farmer.getHitbox().pos, false, this->farmerSprite, WHITE);
    if (this->hitboxRequest) DrawCircleV(this->guard.getHitbox().pos, this->guard.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->manager.getTexture(TEX_GUARD), this->guardAnim, this->guard.getHitbox().pos, false, this->guardSprite, RED);
    if (this->hitboxRequest) DrawCircleV(this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->manager.getTexture(TEX_SORCERER), this->sorcererAnim, this->sorcerer.getHitbox().pos, false, this->sorcererSprite, BLUE);
    if (this->hitboxRequest) DrawCircleV(this->trash.getHitbox().pos, this->trash.getHitbox().radius, HITBOX_COLOR);
    DrawStaticItem(this->manager.getTexture(TEX_TRASH), this->trash.getHitbox().pos, 0.5f);
}

void Game::displayItems() {
    for (int i = 0; i < POTATO_AVAILABLE; i++) {
        if (this->hitboxRequest) DrawCircleV(this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius, HITBOX_COLOR);
        DrawAnimatedEntity(this->manager.getTexture(TEX_POTATO), this->potatoAnim, this->potato[i].getHitbox().pos, false, this->potatoSprite, WHITE);
    }   

    if (!this->isAppleTaken) {
        if (this->hitboxRequest) DrawCircleV(this->apple.getHitbox().pos, this->apple.getHitbox().radius, HITBOX_COLOR);
        DrawAnimatedEntity(this->manager.getTexture(TEX_APPLE), this->appleAnim, this->apple.getHitbox().pos, false, this->appleSprite, WHITE);
    }
}

void Game::displayPlayer() {
    if (this->hitboxRequest) DrawCircleV(this->player.getHitbox().pos, this->player.getHitbox().radius, HITBOX_COLOR);
    DrawAnimatedEntity(this->manager.getTexture(TEX_PLAYER), this->playerAnim, this->player.getHitbox().pos, this->isMoving, this->playerSprite, WHITE);
    DrawInfoLabel(this->player.getHitbox(), this->playerSprite.frameH * this->playerAnim.scale, { &this->manager.getFont(ENTITY_LABEL, 15.0f), this->player.getName(), 15.0f, 2.0f, WHITE });
}

void Game::playerInteractions() {
    // Avec le fermier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->farmer.getHitbox().pos, this->farmer.getHitbox().radius)) {
        DrawInfoLabel(this->farmer.getHitbox(), this->farmerSprite.frameH * this->farmerAnim.scale, { &this->manager.getFont(ENTITY_LABEL, 15), this->farmer.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = FARMER;
            this->dialogueRequest = false;
        }
    }

    // Avec la poubelle
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->trash.getHitbox().pos, this->trash.getHitbox().radius)) {
        DrawInfoLabel(this->trash.getHitbox(), this->manager.getTexture(TEX_TRASH).height * 0.5f, { &this->manager.getFont(ENTITY_LABEL, 15), this->trash.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = TRASH;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le garde
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->guard.getHitbox().pos, this->guard.getHitbox().radius)) {
        DrawInfoLabel(this->guard.getHitbox(), this->guardSprite.frameH * this->guardAnim.scale, { &this->manager.getFont(ENTITY_LABEL, 15), this->guard.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = GUARD;
            this->dialogueRequest = false;
        }
    }
    
    // Avec le sorcier
    if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->sorcerer.getHitbox().pos, this->sorcerer.getHitbox().radius)) {
        DrawInfoLabel(this->sorcerer.getHitbox(), this->sorcererSprite.frameH * this->sorcererAnim.scale, { &this->manager.getFont(ENTITY_LABEL, 15), this->sorcerer.getName(), 15.0f, 2.0f, WHITE });
        if (this->dialogueRequest) {
            this->displayDialogue.request = !this->displayDialogue.request;
            this->displayDialogue.entity = SORCERER;
            this->dialogueRequest = false;
        }
    }

    // Avec les patates
    for (int i = 0; i < POTATO_AVAILABLE; ++i) {
        if (CheckCollisionCircles(this->player.getHitbox().pos, player.getHitbox().radius, this->potato[i].getHitbox().pos, this->potato[i].getHitbox().radius)) {
            DrawInfoLabel(potato[i].getHitbox(), this->potatoSprite.frameH * this->potatoAnim.scale, { &this->manager.getFont(ENTITY_LABEL, 15), this->potato[i].getName(), 15.0f, 2.0f, WHITE });
            if (this->dialogueRequest) {
                this->displayDialogue.request = !this->displayDialogue.request;
                this->displayDialogue.entity = ITEM_POTATO;
                this->dialogueRequest = false;
            }
        }
    }

    // Avec la pomme
    if (!this->isAppleTaken) {
        if (CheckCollisionCircles(this->player.getHitbox().pos, this->player.getHitbox().radius, this->apple.getHitbox().pos, this->apple.getHitbox().radius)) {
            DrawInfoLabel(this->apple.getHitbox(), this->appleSprite.frameH * this->appleAnim.scale, { &this->manager.getFont(ENTITY_LABEL, 15), this->apple.getName(), 15.0f, 2.0f, WHITE });
            if (this->dialogueRequest) {
                this->displayDialogue.request = !this->displayDialogue.request;
                this->displayDialogue.entity = ITEM_APPLE;
                this->dialogueRequest = false;
            }
        }
    }
}

void Game::renderInventory() {
    if (!this->inventoryRequest) return;
    if (this->cancel) {
        this->inventoryRequest = false;
        this->isPause = false;
        return;
    }

    // --- Texture et centrage de la fenêtre d'inventaire ---
    Texture2D inventory = this->manager.getTexture(TEX_INVENTORY);

    float inventoryW = inventory.width  * INVENTORY_SCALE;
    float inventoryH = inventory.height * INVENTORY_SCALE;

    Position pos = {
        SCREEN_WIDTH  / 2.0f - inventoryW / 2.0f,
        SCREEN_HEIGHT / 2.0f - inventoryH / 2.0f
    };

    DrawTextureEx(inventory, pos, 0.0f, INVENTORY_SCALE, WHITE);

    // --- Paramètres des items ---
    Position itemPos = { pos.x + 5.0f * INVENTORY_SCALE, pos.y + 5.0f * INVENTORY_SCALE }; // premier slot
    float itemScale = 1.0f;
    float gap = 3.0f * INVENTORY_SCALE;

    // On prend la patate comme référence de taille de cellule
    const Texture2D potatoTex = this->manager.getTexture(TEX_POTATO_STATIC);
    const Texture2D carrotTex = this->manager.getTexture(TEX_CARROT_STATIC);
    const Texture2D appleTex  = this->manager.getTexture(TEX_APPLE_STATIC);
    const float cellW = potatoTex.width  * itemScale;
    const float cellH = potatoTex.height * itemScale;

    // --- Grille ---
    const int totalSlots = MAX_INVENTORY_LENGTH;
    int rowSize = static_cast<int>(sqrt(static_cast<float>(totalSlots)));
    if (rowSize * rowSize < totalSlots) rowSize += 1; // dernière ligne partielle si besoin

    bool isHover = false;
    Rectangle hoveredRect = { 0 };
    vector<vector<InfoSegment>> hoveredData;
    for (int i = 0; i < rowSize; ++i) {
        for (int j = 0; j < rowSize; ++j) {
            int idx = i * rowSize + j;
            if (idx >= totalSlots) break;

            Position cellPos = {
                itemPos.x + j * (cellW + gap),
                itemPos.y + i * (cellH + gap)
            };

            // Hitbox du slot
            Rectangle slotRect = { cellPos.x, cellPos.y, cellW, cellH };

            const Item& item = this->player.inventory().getItem(idx);
            if (item.getId() == ID_NONE) continue;

            // --- Dessin de l’item ---
            Texture2D texture = {};
            switch (item.getId()) {
                case ID_POTATO:
                    texture = potatoTex;
                    break;
                case ID_CARROT:
                    texture = carrotTex;
                    break;
                case ID_APPLE:
                    texture = appleTex;
                    break;
                default: break;
            }

            TextStyle itemQuantity = { &this->manager.getFont(DIALOGUE_LABEL, 15), to_string(this->player.inventory().getSlotQuantity(idx)), 15.0f, 0.0f, BLACK };
            DrawItemWithQuantity(cellPos, itemQuantity, texture, itemScale);

            // --- Hover ---
            if (CheckCollisionPointRec(GetMousePosition(), slotRect)) {
                vector<vector<InfoSegment>> data = {
                    { { { &this->manager.getFont(INFO_LABEL, 20), item.getName(), 20.0f, 2.0f, COLOR_INVENTORY_NAME } } },
                    {
                        { { &this->manager.getFont(INFO_LABEL, 16), "ID : ", 16.0f, 2.0f, COLOR_INVENTORY_LABEL } },
                        { { &this->manager.getFont(INFO_LABEL, 16), to_string(item.getId()), 16.0f, 2.0f, COLOR_INVENTORY_ID } }
                    },
                    {
                        { { &this->manager.getFont(INFO_LABEL, 16), "Quantité : ", 16.0f, 2.0f, COLOR_INVENTORY_LABEL } },
                        { { &this->manager.getFont(INFO_LABEL, 16), "x" + to_string(this->player.inventory().getSlotQuantity(idx)), 16.0f, 2.0f, COLOR_INVENTORY_CURRENT_AMOUNT } }
                    },
                    {
                        { { &this->manager.getFont(INFO_LABEL, 16), "Quantité max : ", 16.0f, 2.0f, COLOR_INVENTORY_LABEL } },
                        { { &this->manager.getFont(INFO_LABEL, 16), "x" + to_string(item.getMaxAmount()), 16.0f, 2.0f, COLOR_INVENTORY_MAX_AMOUNT } }
                    }
                };

                isHover     = true;
                hoveredRect = slotRect;
                hoveredData = move(data);
            }
        }
    }

    // Affiche le hover
    if (isHover) DrawItemToolTip(hoveredData, hoveredRect);

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
        float cornerThick = INVENTORY_SCALE;
        DrawCornerMarkers(r, cornerLen, cornerThick, COLOR_INVENTORY_SELECTOR);
    }
}

void Game::renderStack() {
    if (!this->stackRequest) return;

    // Titre
    TextStyle cardTitle = { &this->manager.getFont(INFO_LABEL, 25), "Stack Trace", 25.0f, 2.0f, COLOR_STACK_TEXT };
    
    // Contenu
    TextStyle style = { &this->manager.getFont(INFO_LABEL, 16), "", 16.0f, 2.0f, RAYWHITE };
    vector<vector<InfoSegment>> cardContent = this->gdb.getFormatedStack(style);

    // Affichage
    Frame tmp = {
        START_FRAME.x,
        START_FRAME.y,
        START_FRAME.width
    };
    HoverValues ret = DrawCard({ cardTitle, cardContent, { .position = TOP_RIGHT } }, tmp, 0.05f);
    
    // Hover
    if (ret.offset != SIZE_MAX) {
        TextStyle styleHoverTitle = { &this->manager.getFont(INFO_LABEL, 19), "", 19.0f, 2.0f, COLOR_TOOLTIP_TITLE };
        TextStyle styleHoverContent = { &this->manager.getFont(INFO_LABEL, 15), "", 15.0f, 2.0f, COLOR_TOOLTIP_LABEL };

        vector<vector<InfoSegment>> dataHover = this->gdb.getMoreInfo(ret.offset, styleHoverTitle, styleHoverContent);
        DrawToolTip(dataHover, ret.frame, { 8.0f, 8.0f });
    }
}

void Game::dialogue() {
    if (!this->displayDialogue.request) return;
    this->isPause = true;

    TextStyle basic = { &this->manager.getFont(DIALOGUE_LABEL, 25), "", 25.0f, 2.0f, WHITE };

    switch (this->displayDialogue.entity) {
        case ITEM_POTATO: {
            vector<vector<InfoSegment>> data = {
                {
                    { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous récoltez ", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } },
                    { { &this->manager.getFont(DIALOGUE_LABEL, 25), "x" + to_string(POTATO_AVAILABLE), 25.0f, 2.0f, COLOR_DIALOGUE_DIGIT } },
                    { { &this->manager.getFont(DIALOGUE_LABEL, 25), string(" ") + this->potato[0].getName() + "(s).", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                },
            };

            IconProfile profile = { this->manager.getTexture(TEX_POTATO), this->potatoAnim, this->potatoSprite, WHITE };
            DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

            if (IsKeyPressed(KEY_SPACE)) {
                this->isPause = false;
                this->displayDialogue.request = false;
                bool full = this->player.inventory().add(this->potato[0], POTATO_AVAILABLE);
                if (full) this->notification.push(this->player.getName(), "Inventaire plein.");
                else this->notification.push(this->player.getName(), "x" + to_string(POTATO_AVAILABLE) + " " + this->potato->getName() + "(s) récolté(es).");
            }
            break;
        }
        case ITEM_APPLE: {
            if (this->player.inventory().isFull()) {
                this->notification.push(this->player.getName(), "Inventaire plein.");
                this->isPause = false;
                this->displayDialogue.request = false;
                break;
            }

            switch (this->appleStep) {
                case 0: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous récoltez ", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "x" + to_string(this->apple.getMaxAmount()), 25.0f, 2.0f, COLOR_DIALOGUE_DIGIT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), string(" ") + this->apple.getName() + "(s).", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    IconProfile profile = { this->manager.getTexture(TEX_APPLE), this->appleAnim, this->appleSprite, WHITE };
                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

                    if (IsKeyPressed(KEY_SPACE)) {
                        this->appleStep = 1;
                        bool full = this->player.inventory().add(this->apple, this->apple.getMaxAmount());
                        if (full) this->notification.push(this->player.getName(), "Inventaire plein.");
                        else this->notification.push(this->player.getName(), "x" + to_string(this->apple.getMaxAmount()) + " " + this->apple.getName() + " récolté(s).");
                    }
                    break;
                }
                case 1: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), string("La ") + this->apple.getName() + " s'exite.", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };


                    float speedTemp = this->appleAnim.frameTimeIdle;
                    this->appleAnim.frameTimeIdle = 0.02;
                    IconProfile profile = { this->manager.getTexture(TEX_APPLE), this->appleAnim, this->appleSprite, WHITE };
                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

                    if (IsKeyPressed(KEY_SPACE)) {
                        this->appleStep = 2;
                        this->appleAnim.frameTimeIdle = speedTemp;
                    }
                    break;
                }
                case 2: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), string("La ") + this->apple.getName() + " s'est enfuit.", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    IconProfile profile = { };
                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);


                    if (IsKeyPressed(KEY_SPACE)) {
                        this->appleStep = 0;
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->isAppleTaken = true;
                    }
                    break;
                }
                default: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Euuuh t'es pas sensé voir ce texte chef", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    IconProfile profile = {this->manager.getTexture(TEX_PLAYER), this->playerAnim, this->playerSprite, WHITE };
                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

                    if (IsKeyPressed(KEY_SPACE)) {
                        this->isPause = false;
                        this->displayDialogue.request = false;
                    }
                    break;
                }
            }
            break;
        }
        case TRASH: {
            if (this->player.inventory().isEmpty()) {
                this->notification.push(this->player.getName(), "Inventaire déjà vide.");
                this->isPause = false;
                this->displayDialogue.request = false;
                break;
            }

            vector<vector<InfoSegment>> data = {
                {
                    { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous vous videz dans la poubelle.", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                },
            };

            // Fake animation car statique
            Texture2D trashTex = this->manager.getTexture(TEX_TRASH);
            float trashScale = 0.5f;

            SpriteSheetInfo trashSprite = {
                .frameW   = (float)trashTex.width,
                .frameH   = (float)trashTex.height,
                .frameCols = 1,
                .rowIdle   = 0,
                .rowMove   = 0
            };

            AnimationState trashAnim = {
                .frame         = 0.0f,
                .timer         = 0.0f,
                .facingRight   = true,
                .frameTimeIdle = 0.2f,
                .frameTimeMove = 0.2f,
                .scale         = trashScale
            };

            IconProfile profile = { this->manager.getTexture(TEX_TRASH), trashAnim, trashSprite, WHITE };
            DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

            if (IsKeyPressed(KEY_SPACE)) {
                this->isPause = false;
                this->displayDialogue.request = false;
                this->player.inventory().removeAll();
                this->notification.push(this->player.getName(), "Inventaire vidé.");
            }
            break;
        }
        case FARMER: {
            switch (this->farmerStep) {
                case 0: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous souhaitez échanger des objets ?", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    IconProfile profile = { this->manager.getTexture(TEX_FARMER), this->farmerAnim, this->farmerSprite, WHITE };
                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

                    if (IsKeyPressed(KEY_SPACE)) this->farmerStep = 1;
                    break;
                }
                case 1: {
                    StoreResult itemToAdd = DrawStore(this->player.inventory().getItemQuantity(ID_POTATO), this->player.inventory().getItemQuantity(ID_CARROT), this->player.inventory().getItemQuantity(ID_APPLE), this->manager);
                    this->hover = itemToAdd.hover;
                    switch (itemToAdd.pressedID) {
                        case ID_POTATO: {
                            this->player.inventory().remove(ID_APPLE, TRADE_GIVE_APPLE);
                            bool full = this->player.inventory().add(this->potato[0], TRADE_GET_POTATO);
                            if (full) {
                                this->player.inventory().add(this->apple, TRADE_GIVE_APPLE);
                                this->notification.push(this->player.getName(), "Inventaire plein.");
                            } else {
                                this->notification.push(this->farmer.getName(), "x" + to_string(TRADE_GET_POTATO) + " " + this->potato[0].getName() + "(s) échangé(es) contre " + to_string(TRADE_GIVE_APPLE) + " " + this->apple.getName() + "(s).");
                            }
                            break;
                        }
                        case ID_CARROT: {
                            this->player.inventory().remove(ID_POTATO, TRADE_GIVE_POTATO);
                            bool full = this->player.inventory().add(this->carrot, TRADE_GET_CARROT);
                            if (full) {
                                this->player.inventory().add(this->potato[0], TRADE_GIVE_POTATO);
                                this->notification.push(this->player.getName(), "Inventaire plein.");
                            } else {
                                this->notification.push(this->farmer.getName(), "x" + to_string(TRADE_GET_CARROT) + " " + this->carrot.getName() + "(s) échangé(es) contre " + to_string(TRADE_GIVE_POTATO) + " " + this->potato[0].getName() + "(s).");
                            }
                            break;
                        }
                        case ID_APPLE: {
                            this->player.inventory().remove(ID_CARROT, TRADE_GIVE_CARROT);
                            bool full = this->player.inventory().add(this->apple, TRADE_GET_APPLE);
                            if (full) {
                                this->player.inventory().add(this->carrot, TRADE_GIVE_CARROT);
                                this->notification.push(this->player.getName(), "Inventaire plein.");
                            } else {
                                this->notification.push(this->farmer.getName(), "x" + to_string(TRADE_GET_APPLE) + " " + this->apple.getName() + "(s) échangé(es) contre " + to_string(TRADE_GIVE_CARROT) + " " + this->carrot.getName() + "(s).");
                            }
                            break;
                        }
                        default:
                            break;
                    }

                    if (IsKeyPressed(KEY_ESCAPE)) {
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->farmerStep = 0;
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case GUARD: {
            bool hasEnough = this->player.inventory().hasEnoughOf(ID_APPLE, AMOUNT_TO_FINISH_GAME);
            int quantity = this->player.inventory().getItemQuantity(ID_APPLE);

            vector<vector<InfoSegment>> data;
            if (hasEnough) {
                data = {
                    {
                        { { &this->manager.getFont(DIALOGUE_LABEL, 25), string("Bravo, vous avez assez de ") + this->apple.getName() + "(s). Vous avez terminé le jeu.", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                    },
                };
            } else {
                data = {
                    {
                        { { &this->manager.getFont(DIALOGUE_LABEL, 25), string("Vous n'avez pas assez de ") + this->apple.getName() + "(s) (Vous avez ", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } },
                        { { &this->manager.getFont(DIALOGUE_LABEL, 25), to_string(quantity), 25.0f, 2.0f, COLOR_DIALOGUE_DIGIT_NOT_ENOUGH } },
                        { { &this->manager.getFont(DIALOGUE_LABEL, 25), "/" + to_string(AMOUNT_TO_FINISH_GAME) + ")", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                    },
                };
            }

            IconProfile profile = { this->manager.getTexture(TEX_GUARD), this->guardAnim, this->guardSprite, RED };
            DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

            if (IsKeyPressed(KEY_SPACE)) {
                this->isPause = false;
                this->displayDialogue.request = false;
                if (hasEnough) this->gameEnded = true;
            }
            break;
        }
        case SORCERER: {
            IconProfile profile = { this->manager.getTexture(TEX_SORCERER), this->sorcererAnim, this->sorcererSprite, BLUE };
            if (this->player.inventory().isEmpty()) this->sorcererStep = 4;
            switch (this->sorcererStep) {
                case 0: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous souhaitez renommer un objet de votre inventaire ?", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Continue [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);
                    
                    if (IsKeyPressed(KEY_SPACE)) this->sorcererStep = 1;
                    break;
                }
                case 1: {
                    this->inventoryRequest = true;
                    this->invSelectorVisible = true;

                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Séléctionnez le avec ", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "ZQSD", 25.0f, 2.0f, COLOR_DIALOGUE_HIGHLIGHT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), " ou ", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "les flêches", 25.0f, 2.0f, COLOR_DIALOGUE_HIGHLIGHT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), ".", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Valider [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);
                    
                    if (this->cancel) {
                        this->sorcererStep = 0;
                        this->inventoryRequest = false;
                        this->invSelectorVisible = false;
                        this->displayDialogue.request = false;
                        this->isPause = false;
                    }

                    if (IsKeyPressed(KEY_SPACE)) {
                        Item item = this->player.inventory().getItem(this->invSelectorIndex);
                        if (item.getId() != 0) {
                            this->inventoryRequest = false;
                            this->invSelectorVisible = false;
                            this->sorcererStep = 2;
                        }
                    }
                    break;
                }
                case 2: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Saisissez le nouveau nom.", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },  
                    };

                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Valider [Press enter]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);
                    
                    Item item = this->player.inventory().getItem(this->invSelectorIndex);
                    if (this->renameInit) {
                        this->newName.text = item.getName();
                        this->renameInit = false;
                        this->isTyping = true;
                    }
                    this->getNewName();

                    if (this->cancel) {
                        this->sorcererStep = 0;
                        this->isTyping = false;
                        this->renameInit = true;
                        this->invSelectorIndex = 0;
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->newName.text = "";
                    }

                    if (IsKeyPressed(KEY_ENTER)) {
                        item.changeName(this->newName.text.c_str());
                        this->player.inventory().setItem(item, this->invSelectorIndex);
                        this->invSelectorIndex = 0;
                        this->renameInit = true;
                        this->isTyping = false;
                        this->sorcererStep = 3;
                    }
                    break;
                }
                case 3: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous avez renomé votre objet en : ", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } },
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), this->newName.text, 25.0f, 2.0f, COLOR_DIALOGUE_HIGHLIGHT } }
                        },
                    };

                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);
                    
                    if (IsKeyPressed(KEY_SPACE)) {
                        this->notification.push(this->sorcerer.getName(), "Voici un(e) " + this->newName.text);
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->newName.text = "";
                        this->sorcererStep = 0; // reset
                    }
                    break;
                }
                case 4: {
                    vector<vector<InfoSegment>> data = {
                        {
                            { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Vous n'avez aucun objet à renommer.", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                        },
                    };

                    DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Continue [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);
                    
                    if (IsKeyPressed(KEY_SPACE)) {
                        this->isPause = false;
                        this->displayDialogue.request = false;
                        this->sorcererStep = 0; // reset
                    }
                    break;
                }
            }
            break;
        }
        default: {
            vector<vector<InfoSegment>> data = {
                {
                    { { &this->manager.getFont(DIALOGUE_LABEL, 25), "Euuuh t'es pas sensé voir ce texte chef", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT } }
                },
            };

            IconProfile profile = {this->manager.getTexture(TEX_PLAYER), this->playerAnim, this->playerSprite, WHITE };
            DrawDialogue(data, { &this->manager.getFont(DIALOGUE_LABEL, 25), "Terminer [Press space]", 25.0f, 2.0f, COLOR_DIALOGUE_CONTENT_TEXT }, profile);

            if (IsKeyPressed(KEY_SPACE)) {
                this->isPause = false;
                this->displayDialogue.request = false;
            }
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
    DrawInputBox(this->newName, MAX_INPUT_CHARS);
}

void Game::resetRequests() {
    this->dialogueRequest = false;
    this->cancel = false;
}

Game::~Game() {
    CloseWindow();
}
