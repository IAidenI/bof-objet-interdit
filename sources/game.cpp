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

    if (IsKeyPressed(KEY_P)) this->stackRequest = !this->stackRequest;
}

void Game::displayBackground() {
    Texture2D bg = this->tmgr[TEX_BACKGROUND];
    Rectangle src = {0, 0, (float)bg.width, (float)bg.height};
    Rectangle dst = {0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
    Vector2 origin = {0, 0};

    DrawTexturePro(bg, src, dst, origin, 0.0f, WHITE);
}

void Game::displayCommands() {
    int fontSize = 20;
    int spacing = 2;
    int margin = 10;
    int posX = 10;
    int posY = 10;

    // Texte des commandes
    const char *lines[] = {
        "COMMANDES",
        "ZQSD / Flêches : Déplacement",
        "E : Interagir",
        "I : Inventaire",
        "H : Hitbox ON/OFF",
        "P : Affichage avancé"
    };
    int lineCount = sizeof(lines) / sizeof(lines[0]);

    // Calcul de la largeur max du texte
    int maxWidth = 0;
    for (int i = 0; i < lineCount; i++) {
        Vector2 size = MeasureTextEx(this->fmgr[INFO_LABEL], lines[i], fontSize, spacing);
        if (size.x > maxWidth) maxWidth = size.x;
    }

    int boxWidth  = maxWidth + 2 * margin;
    int boxHeight = (lineCount * (fontSize + spacing)) + 2 * margin;

    DrawInfoFrame(posX, posY, boxWidth, boxHeight);

    // Texte
    for (int i = 0; i < lineCount; i++) {
        DrawTextEx(this->fmgr[INFO_LABEL], lines[i], {(float)(posX + margin), (float)(posY + margin + i * (fontSize + spacing))}, fontSize, spacing, BLACK);
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

            float radiusCircleInfo = 13.0f;
            Vector2 bottomRight = {
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
                    Vector2 textSize = MeasureTextEx(this->fmgr[DIALOGUE_LABEL], txt.c_str(), fontSize, spacing);

                    // Calculer la position pour centrer
                    Vector2 textPos = {
                        bottomRight.x - textSize.x / 2,
                        bottomRight.y - textSize.y / 2
                    };

                    // Dessiner le texte centré
                    DrawTextEx(this->fmgr[DIALOGUE_LABEL], txt.c_str(), textPos, fontSize, spacing, BLACK);
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
                    Vector2 textSize = MeasureTextEx(this->fmgr[DIALOGUE_LABEL], txt.c_str(), fontSize, spacing);

                    // Calculer la position pour centrer
                    Vector2 textPos = {
                        bottomRight.x - textSize.x / 2,
                        bottomRight.y - textSize.y / 2
                    };

                    // Dessiner le texte centré
                    DrawTextEx(this->fmgr[DIALOGUE_LABEL], txt.c_str(), textPos, fontSize, spacing, BLACK);
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
                    Vector2 textSize = MeasureTextEx(this->fmgr[DIALOGUE_LABEL], txt.c_str(), fontSize, spacing);

                    // Calculer la position pour centrer
                    Vector2 textPos = {
                        bottomRight.x - textSize.x / 2,
                        bottomRight.y - textSize.y / 2
                    };

                    // Dessiner le texte centré
                    DrawTextEx(this->fmgr[DIALOGUE_LABEL], txt.c_str(), textPos, fontSize, spacing, BLACK);
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

void Game::renderStack() {
    if (!this->stackRequest) return;

    // ---- Dessine les informations sur la stack ----
    // Paramètres texte
    Font font     = this->fmgr[INFO_LABEL]; // adapte selon ton gestionnaire
    float fontSz  = 20.0f;
    float spacing = 2.0f;
    float lineGap = 4.0f;

    vector<Line> infoStackLines = {
        { { "-- Stack informations --", BLACK } },

        { { "Adresse de la stack : ", BLACK },
        { this->gdb.getFormattedRBP(), DARKBLUE } },

        {},

        { { "Informations du joueur :", BLACK } },

        { { "  Position : ", BLACK },
        { "X=" + to_string(this->gdb.getPosXValue()), DARKGREEN },
        { ";Y=" + to_string(this->gdb.getPosYValue()), DARKPURPLE } },

        { { "  Rayon hitbox : ", BLACK },
        { to_string(this->gdb.getRadiusValue()), ORANGE } },

        { { "  Nom : ", BLACK },
        { this->gdb.getNameValue(), MAROON } },
    };

    // Padding interne
    int pad = 10;

    // Calcule largeur max
    float maxLineW = 0.0f;
    for (const auto& line : infoStackLines) {
        float w = MeasureLineWidth(line, font, fontSz, spacing);
        if (w > maxLineW) maxLineW = w;
    }

    // Ajoute le bouton [Voir plus]/[Voir moins] aux mesures
    Line invLine = {
        { "  Inventaire : ", BLACK },
        { this->stackShowMore ? "[Voir moins]" : "[Voir plus]", DARKGRAY }
    };
    float invW = MeasureLineWidth(invLine, font, fontSz, spacing);
    if (invW > maxLineW) maxLineW = invW;

    // Hauteur: +1 ligne pour invLine
    float contentH = MeasureTotalHeight(infoStackLines.size(), fontSz, lineGap);
    contentH += (fontSz + lineGap);

    // Dimensions finales du cadre
    int rectW = (int)(pad*2 + maxLineW);
    int rectH = (int)(pad*2 + contentH);

    // Position + cadre
    int posX  = SCREEN_WIDTH - rectW - 10;
    int posY  = 10;
    DrawInfoFrame(posX, posY, rectW, rectH);

    // Dessin du texte
    float x0 = posX + pad;
    float y  = posY + pad;
    for (const auto& line : infoStackLines) {
        DrawLineSegments(line, font, {x0, y}, fontSz, spacing);
        y += (fontSz + lineGap);
    }

    // Bouton interactif (dessiné après, mais compté dans le cadre)
    this->stackShowMoreHover = false;
    DrawLineSegmentsInteractive(invLine, font, {x0, y}, fontSz, spacing, this->stackShowMoreHover, this->stackShowMore);

    if (this->stackShowMore) {
        int extraPad = 10; // espace entre les 2 cadres

        posX = posX;
        posY += rectH + extraPad;

        int rectW2 = rectW;
        int rectH2 = 150;

        DrawInfoFrame(posX, posY, rectW2, rectH2);
    } else {
        int extraPad = 10; // espace entre les 2 cadres

        posX = posX;
        posY += rectH + extraPad;

        int rectW2 = rectW;
        int rectH2 = 150;

        DrawInfoFrame(posX, posY, rectW2, rectH2);
    }
}

/*
void Game::renderStack2() {
    if (!this->stackRequest) return;
    
    // ---- Affiche les informations du joueur ----
    int fontSize = 20;
    int spacing  = 2;
    int margin   = 10;
    int boxY     = 10;

    // Lignes segmentées (texte, couleur par défaut)
    vector<vector<pair<string, Color>>> lines = {
        { { "-- Stack informations --", BLACK } },

        { { "Adresse de la stack : ", BLACK },
        { this->gdb.getFormattedRBP(), DARKBLUE } },

        {},

        { { "Informations du joueur :", BLACK } },

        { { "  Position : ", BLACK },
        { "X=" + to_string(this->gdb.getPosXValue()), DARKGREEN },
        { ";Y=" + to_string(this->gdb.getPosYValue()), DARKPURPLE } },

        { { "  Rayon hitbox : ", BLACK },
        { to_string(this->gdb.getRadiusValue()), ORANGE } },

        { { "  Nom : ", BLACK },
        { this->gdb.getNameValue(), MAROON } },

        { { "  Inventaire : ", BLACK },
        { this->stackShowMoreHover ? "[Voir moins]" : "[Voir plus]", DARKGRAY } }
    };

    // Calcul largeur max
    int maxWidth = 0;
    for (auto& line : lines) {
        int lineW = 0;
        for (auto& seg : line) {
            Vector2 sz = MeasureTextEx(this->fmgr[INFO_LABEL], seg.first.c_str(), fontSize, spacing);
            lineW += (int)sz.x;
        }
        if (lineW > maxWidth) maxWidth = lineW;
    }
    int boxWidth  = maxWidth + 2 * margin;
    int boxHeight = (int)(lines.size()) * (fontSize + spacing) + 2 * margin;

    // Position en haut-droite
    int screenW = GetScreenWidth();
    int boxX = screenW - boxWidth - 10;

    // Détection hover/clic : on calcule la bounds exacte de "[Voir plus]"
    int voirPlusLineIdx = (int)lines.size() - 1;
    int voirPlusSegIdx  = 1;

    // Recalcule un "cursorX" jusqu’au segment cible pour faire la bounding box
    float tmpCursorX = (float)(boxX + margin);
    float tmpCursorY = (float)(boxY + margin + voirPlusLineIdx * (fontSize + spacing));
    for (int si = 0; si < (int)lines[voirPlusLineIdx].size(); ++si) {
        const string& t = lines[voirPlusLineIdx][si].first;
        Vector2 sz = MeasureTextEx(this->fmgr[INFO_LABEL], t.c_str(), fontSize, spacing);

        if (si == voirPlusSegIdx) {
            // on mémorise la zone cliquable pour le rendu ET l’input
            this->stackShowMoreBounds = { tmpCursorX, tmpCursorY - 2.0f, sz.x, (float)fontSize + 4.0f };
            break;
        }
        tmpCursorX += sz.x;
    }

    // Gestion hover + clic
    bool isHover = CheckCollisionPointRec(GetMousePosition(), this->stackShowMoreBounds);
    if (isHover) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    else         SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        this->stackShowMoreHover = !this->stackShowMoreHover;
    }

    // Fond + contour
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(LIGHTGRAY, 0.7f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, DARKGRAY);

    // Rendu du texte segmenté (avec hover style sur "[Voir plus]")
    for (size_t i = 0; i < lines.size(); i++) {
        float cursorX = (float)(boxX + margin);
        float cursorY = (float)(boxY + margin + i * (fontSize + spacing));

        for (size_t s = 0; s < lines[i].size(); ++s) {
            auto [txt, col] = lines[i][s];

            // Si c’est le segment "[Voir plus]" → surbrillance au survol
            if ((int)i == voirPlusLineIdx && (int)s == voirPlusSegIdx) {
                Color linkColor = isHover ? DARKBLUE : col;
                DrawTextEx(this->fmgr[INFO_LABEL], txt.c_str(), {cursorX, cursorY}, fontSize, spacing, linkColor);

                // Underline pour souligner le text
                if (isHover) {
                    Vector2 sz = MeasureTextEx(this->fmgr[INFO_LABEL], txt.c_str(), fontSize, spacing);
                    DrawLineV({cursorX, cursorY + fontSize + 1.0f}, {cursorX + sz.x, cursorY + fontSize + 1.0f}, linkColor);
                }
            } else {
                DrawTextEx(this->fmgr[INFO_LABEL], txt.c_str(), {cursorX, cursorY}, fontSize, spacing, col);
            }

            cursorX += MeasureTextEx(this->fmgr[INFO_LABEL], txt.c_str(), fontSize, spacing).x;
        }
    }

    // ---- Affiche les informations de l'inventaire ----
    // Si ouvert, affiche le menu détaillé
    if (this->stackShowMoreHover) {
        // Slot courant (on s'en sert comme index)
        int slot = invPage;
        if (slot < 0) slot = 0;
        if (slot >= MAX_INVENTORY_LENGTH) slot = MAX_INVENTORY_LENGTH - 1;

        // --- Lignes segmentées : (texte, couleur) ---
        vector<vector<pair<string, Color>>> showMore = {
            { { "  Informations sur le slot :", BLACK } },

            { { "  Position : ", BLACK },
            { "X=" + to_string((int)this->gdb.getItemPosX(slot)), BLUE },
            { ";Y=" + to_string((int)this->gdb.getItemPosY(slot)), MAGENTA } },

            { { "  Rayon hitbox : ", BLACK },
            { to_string((int)this->gdb.getItemRadius(slot)), YELLOW } },

            { { "  Nom : ", BLACK },
            { this->gdb.getItemName(slot), GOLD } },

            { { "  ID : ", BLACK },
            { to_string(this->gdb.getItemID(slot)), WHITE } },

            { { "  Valeur maximale : ", BLACK },
            { to_string(this->gdb.getItemMaxAmount(slot)), RED } },

            { { "  Valeur actuelle : ", BLACK },
            { to_string(this->gdb.getItemSlotAmount(slot)), SKYBLUE } }
        };

        // Géométrie panneau
        int pad   = 10;
        int rowH  = fontSize + 2;
        int btnSz = 26;

        // Calcule la largeur max en fonction du contenu
        int maxWidth = 0;
        for (const auto& line : showMore) {
            int lineWidth = 0;
            for (const auto& seg : line) {
                Vector2 size = MeasureTextEx(this->fmgr[INFO_LABEL], seg.first.c_str(), fontSize, spacing);
                lineWidth += (int)size.x;
            }
            if (lineWidth > maxWidth) maxWidth = lineWidth;
        }

        int w = maxWidth + pad * 2;
        int h = pad*2 + (int)showMore.size() * rowH + rowH + btnSz + pad/2;

        int px = boxX + boxWidth - w; // Aligné à droite du cadre
        int py = boxY + boxHeight + 6;

        // Fond + contour
        DrawRectangle(px, py, w, h, Fade(LIGHTGRAY, 0.7f));
        DrawRectangleLines(px, py, w, h, DARKGRAY);

        // Titre
        string header = "Inventaire (slot " + to_string(slot) + ")";
        DrawTextEx(this->fmgr[INFO_LABEL], header.c_str(), {(float)(px+pad), (float)(py+pad)}, fontSize, spacing, BLACK);

        // Rendu des lignes colorées (segments)
        for (int i = 0; i < (int)showMore.size(); ++i) {
            float cursorX = (float)(px + pad);
            float cursorY = (float)(py + pad + (i+1) * rowH);

            for (const auto& seg : showMore[i]) {
                const string& txt = seg.first;
                Color col = seg.second;

                DrawTextEx(this->fmgr[INFO_LABEL], txt.c_str(), {cursorX, cursorY}, fontSize, spacing, col);
                cursorX += MeasureTextEx(this->fmgr[INFO_LABEL], txt.c_str(), fontSize, spacing).x;
            }
        }

        // --- Boutons ◀ ▶ (pagination de slot) ---
        int btnY     = py + h - pad - btnSz;
        int btnXPrev = px + pad;
        int btnXNext = btnXPrev + btnSz + 6;

        this->invBtnPrev = { (float)btnXPrev, (float)btnY, (float)btnSz, (float)btnSz };
        this->invBtnNext = { (float)btnXNext, (float)btnY, (float)btnSz, (float)btnSz };

        Vector2 mouse = GetMousePosition();
        bool hoverPrev = CheckCollisionPointRec(mouse, invBtnPrev);
        bool hoverNext = CheckCollisionPointRec(mouse, invBtnNext);
        if (hoverPrev || hoverNext) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        auto drawButton = [&](Rectangle r, bool hover, bool right) {
            DrawRectangleRec(r, hover ? Fade(DARKGRAY,0.6f) : Fade(DARKGRAY,0.4f));
            DrawRectangleLines((int)r.x,(int)r.y,(int)r.width,(int)r.height,DARKGRAY);

            float x = r.x, y = r.y, s = r.width;
            Vector2 A, B, C;
            if (right) { // ▶
                A = { x + s - 6.0f, y + s*0.5f };
                B = { x + 6.0f,     y + 6.0f   };
                C = { x + 6.0f,     y + s - 6.0f };
            } else {     // ◀
                A = { x + 6.0f,     y + s / 2.0f };
                B = { x + s - 6.0f, y + s - 6.0f };
                C = { x + s - 6.0f, y + 6.0f };
            }
            DrawTriangle(A, B, C, BLACK);
        };

        drawButton(invBtnPrev, hoverPrev, false);
        drawButton(invBtnNext, hoverNext, true);

        // Clicks : on fait défiler le slot affiché
        if (hoverPrev && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) invPage = max(0, invPage - 1);
        if (hoverNext && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) invPage = min(MAX_INVENTORY_LENGTH - 1, invPage + 1);
    } else {
        // ---- Affiche la stack sous forme de diagramme ----
        // Géométrie panneau
        int stackContentFontSize = 13;
        float spacing = 0.0f;

        // marges internes
        int innerPad   = 8;
        int labelGap   = 6;
        int textPadX   = 10;                         // marge horizontale autour du texte dans la colonne
        int labelSpace = stackContentFontSize + labelGap;

        // Tes données (mets-les AVANT le calcul de taille)
        vector<vector<pair<string, Color>>> datas = {
            { { this->gdb.getFormattedX(), BLACK },
            { to_string(this->gdb.getPosXValue()), DARKGREEN} },
            
            { {this->gdb.getFormattedY(), BLACK},
            {to_string(this->gdb.getPosYValue()), DARKPURPLE}},

            { {this->gdb.getFormattedRadius(), BLACK},
            {to_string(this->gdb.getRadiusValue()), ORANGE}},

            { {this->gdb.getFormattedName(), BLACK},
            {this->gdb.getNameValue(), MAROON}},
            
            { {this->gdb.getFormattedInv(), BLACK},
            {"Inventaire", BLACK} }
        };
        int nDatas = (int)datas.size();

        // Mesure des largeurs
        auto textWidth = [&](const string& s){
            return MeasureTextEx(this->fmgr[INFO_LABEL], s.c_str(), stackContentFontSize, spacing).x;
        };

        // largeur max parmi toutes les clés/valeurs + le label "Stack"
        float maxTextW = textWidth("Stack");
        for (const auto& row : datas) {
            for (const auto& cell : row) {
                maxTextW = max(maxTextW, textWidth(cell.first));
            }
        }

        // largeur de la colonne = texte + marges
        int colW = (int)ceil(maxTextW + 2*textPadX);

        // largeur du panneau = colonne + marges internes gauche/droite
        int w = colW + innerPad*2;

        // hauteur inchangée (mais on garde l’espace réservé en bas pour le label)
        int h = 300;

        // position du panneau
        int px = boxX + boxWidth - w;   // aligné à droite
        int py = boxY + boxHeight + 6;

        // Fond + contour
        DrawRectangle(px, py, w, h, Fade(LIGHTGRAY, 0.7f));
        DrawRectangleLines(px, py, w, h, DARKGRAY);

        // --- Colonne "stack" colorée avec séparations ---
        // zone interne (petites marges)
        int colX = px + innerPad;
        int colY = py + innerPad;
        int colH = h  - innerPad*2 - labelSpace;

        // paramètres des bandes
        const int bands = 5;                // nombre de cases
        float bandH = (float)colH / bands;  // hauteur d’une case

        // palette approximative (haut vert -> bas orange)
        Color bandColors[bands] = {
            (Color){120, 195,  60, 255},
            (Color){250, 185,  80, 255},
            (Color){245, 155,  70, 255},
            (Color){235, 120,  60, 255},
            (Color){225,  95,  55, 255}
        };

        for (int i = 0; i < bands; ++i) {
            float y0 = colY + i * bandH;
            DrawRectangle(colX, (int)roundf(y0), colW, (int)ceilf(bandH), bandColors[i]);

            // index data : remplir de BAS en HAUT
            int dataIdx = nDatas - 1 - i;   // i=0 -> dernier élément (bas), i=1 -> avant-dernier, etc.

            if (dataIdx >= 0) {
                const auto& row = datas[dataIdx];

                // k/v + couleurs associées
                const string& k = row[0].first;  Color ck = row[0].second;
                const string& v = row[1].first;  Color cv = row[1].second;

                // mesurer
                Vector2 sz1 = MeasureTextEx(this->fmgr[INFO_LABEL], k.c_str(), stackContentFontSize, spacing);
                Vector2 sz2 = MeasureTextEx(this->fmgr[INFO_LABEL], v.c_str(), stackContentFontSize, spacing);

                // centrer verticalement le duo (k au-dessus, v en dessous)
                float gap = 2.0f; // espace entre les deux lignes
                float totalH = sz1.y + gap + sz2.y;
                float baseY  = y0 + (bandH - totalH) * 0.5f;

                // positions centrées horizontalement
                float tx1 = colX + (colW - sz1.x) * 0.5f;
                float ty1 = baseY;

                float tx2 = colX + (colW - sz2.x) * 0.5f;
                float ty2 = baseY + sz1.y + gap;

                // dessin
                DrawTextEx(this->fmgr[INFO_LABEL], k.c_str(), {tx1, ty1}, stackContentFontSize, spacing, ck);
                DrawTextEx(this->fmgr[INFO_LABEL], v.c_str(), {tx2, ty2}, stackContentFontSize, spacing, cv);
            }

            // séparation fine (sauf la dernière)
            if (i < bands - 1) {
                DrawLine(colX, (int)roundf(y0 + bandH),
                        colX + colW, (int)roundf(y0 + bandH),
                        (Color){90,90,90,180});
            }
        }

        // grosses séparations noires à ~1/3 et ~2/3 (comme sur l’image)
        auto thickLine = [&](float y){
            // épaissir en dessinant 3 lignes superposées
            int yy = (int)roundf(y);
            DrawLine(colX, yy-1, colX + colW, yy-1, BLACK);
            DrawLine(colX, yy,   colX + colW, yy,   BLACK);
            DrawLine(colX, yy+1, colX + colW, yy+1, BLACK);
        };

        // positions (ajuste selon ton image)
        thickLine(colY + bandH*1.0f);  // 1ère grosse séparation
        thickLine(colY + bandH*5.0f);  // 2ème grosse séparation

        // --- Label "Stack" centré EN BAS DU PANNEAU ---
        {
            const char* label = "Stack";
            Vector2 sz = MeasureTextEx(this->fmgr[INFO_LABEL], label, fontSize, spacing);

            float lx = colX + (colW - sz.x) * 0.5f;
            float ly = colY + colH + labelGap;  // pile dans l’espace réservé

            DrawTextEx(this->fmgr[INFO_LABEL], label, {lx, ly}, fontSize, spacing, BLACK);
        }
    }
}
*/

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
    
    DrawTextEx(text.font, text.text.c_str(), position, text.font_size, text.spacing, WHITE);
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

void DrawInfoFrame(int posX, int posY, int rectW, int rectH) {
    // Fond + contour
    DrawRectangle(posX, posY, rectW, rectH, Fade(LIGHTGRAY, 0.7f));
    DrawRectangleLines(posX, posY, rectW, rectH, DARKGRAY);
}

// Mesure la largeur d'une ligne (somme des segments)
float MeasureLineWidth(const Line& line, Font font, float fontSize, float spacing) {
    float w = 0.0f;
    for (const auto& seg : line) {
        w += MeasureTextEx(font, seg.text.c_str(), fontSize, spacing).x;
    }
    return w;
}

// Mesure hauteur totale (simple: une ligne = fontSize + lineGap)
float MeasureTotalHeight(size_t lineCount, float fontSize, float lineGap) {
    return lineCount * (fontSize + lineGap);
}

// Dessine une ligne à la position donnée, enchaînant les segments
void DrawLineSegments(const Line& line, Font font, Vector2 pos, float fontSize, float spacing) {
    float x = pos.x;
    for (const auto& seg : line) {
        DrawTextEx(font, seg.text.c_str(), {x, pos.y}, fontSize, spacing, seg.color);
        x += MeasureTextEx(font, seg.text.c_str(), fontSize, spacing).x;
    }
}

void DrawLineSegmentsInteractive(Line& line, Font font, Vector2 pos, float fontSize, float spacing, bool& hoverFlag, bool& toggleFlag) {
    float x = pos.x;
    Vector2 mouse = GetMousePosition();

    for (size_t i = 0; i < line.size(); i++) {
        auto& seg = line[i];

        Vector2 sz = MeasureTextEx(font, seg.text.c_str(), fontSize, spacing);
        Rectangle hitbox = { x, pos.y, sz.x, sz.y };

        Color color = seg.color;

        // Exemple : on considère que le dernier segment est interactif
        if (i == line.size() - 1 && (seg.text == "[Voir plus]" || seg.text == "[Voir moins]")) {
            if (CheckCollisionPointRec(mouse, hitbox)) {
                hoverFlag = true;
                color = DARKBLUE; // hover
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    toggleFlag = !toggleFlag;
                }
            }
        }

        DrawTextEx(font, seg.text.c_str(), {x, pos.y}, fontSize, spacing, color);
        x += sz.x;
    }
}
