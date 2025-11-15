#include "raylib-utils.hpp"

// Calcul les dimenssions maximal pour une ligne
Size GetMaxLineSize(const vector<InfoSegment>& line) {
    float width  = 0.0f;
    float height = 0.0f;
        
    for (auto& word : line) {
        Size sizeCompute = MeasureTextStyled(word.textStyle);
        
        width += sizeCompute.x;
        height = fmaxf(height, sizeCompute.y);
    }
    return { width, height };
}

// Calcul la taille maximale dont aura besoin une frame 
Size GetFrameMaxSize(const vector<vector<InfoSegment>>& content, Padding padIn, float interline) {
    float width = 0;
    float height = 0;

    if (content.empty()) {
        return { 2.0f * padIn.x, 2.0f * padIn.y + interline };
    }

    for (size_t i = 0; i < content.size(); i++) {
        Size localSize = GetMaxLineSize(content[i]);

        // Calcul la taille horizontal
        width = fmaxf(localSize.x, width);
        
        // Calcul la taille vertical
        if (i > 0) height += interline; 
        height += localSize.y;
    }

    return { width + 2.0f * padIn.x, height + 2.0f * padIn.y };
}

// Calcul la taille maximale dont aura besoin une frame en prenant en compte une branche
Size GetFrameMaxSizeWithBranch(const vector<vector<InfoSegment>>& content, Padding padIn, float interline, float indentAfterTitle, float titleInterline) {
    float maxLineW = 0.0f;
    float totalH   = 0.0f;

    if (content.empty()) return { 2.0f*padIn.x, 2.0f*padIn.y + interline };

    for (size_t i = 0; i < content.size(); ++i) {
        Size s = GetMaxLineSize(content[i]);
        float effectiveW = s.x + ((i == 0) ? 0.0f : indentAfterTitle);
        maxLineW = fmaxf(maxLineW, effectiveW);

        totalH += s.y;
        if (i + 1 < content.size()) totalH += interline;
    }

    if (content.size() >= 2) totalH += (titleInterline - interline);

    return { maxLineW + 2.0f*padIn.x, totalH + 2.0f*padIn.y };
}

// Calcul la position et la taille d'une frame
Frame GetFrame(const vector<vector<InfoSegment>>& content, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut, float interline) {
    // Récupère la taille de la frame
    Size size = GetFrameMaxSize(content, padIn, interline);

    // Crée la frame à partir de la taille voulu et de la frame parent
    Frame frame = {
        parentFrame.x,
        parentFrame.y,
        size.x,
        size.y
    };

    // En fonction de la position souhaité, calcul les coordonnées
    switch (position) {
        case TOP_LEFT: {
            frame.x += padOut.x;
            frame.y += padOut.y;
            break;
        }
        case TOP_RIGHT: {
            frame.x += parentFrame.width - size.x - padOut.x;
            frame.y += padOut.y;
            break;
        }
        case BOT_LEFT: {
            frame.x += padOut.x;
            frame.y += parentFrame.height - size.y - padOut.y;
            break;
        }
        case BOT_RIGHT: {
            frame.x += parentFrame.width - size.x - padOut.x;
            frame.y += parentFrame.height - size.y - padOut.y;
            break;
        }
        case CENTER: {
            frame.x += (parentFrame.width - frame.width) * 0.5f;
            frame.y += (parentFrame.height - frame.height) * 0.5f;
            break;
        }
        default:
            break;
    }
    return AlignToPixels(frame);
}

// Calcul la position et la taille d'une frame pour un titre
Frame GetFrameTitle(const TextStyle& title, Frame parentFrame, WindowPosition parentPosition, float titleOffset, Padding padTitle, Margins padFrame, float stroke) {
    Size titleSize = MeasureTextStyled(title);

    // Calcul la nouvelle frame
    Frame frame = {
        parentFrame.x + titleOffset,
        0.0f,
        titleSize.x + 2.0f * padTitle.x,
        titleSize.y + 2.0f * padTitle.y
    };

    // Mers à jour la position en Y en fonction de la position souhaité sur l'écran
    frame.y = parentFrame.y + stroke * 0.5f;
    if (parentPosition == BOT_LEFT || parentPosition == BOT_RIGHT) {
        frame.y -= (titleSize.y + padTitle.y) + padFrame.y * 0.5f;
    }

    return AlignToPixels(frame);
}

// Calcul la frame d'une card (titre + contenu)
CardLayout GetCardFrame(const Card& card, Frame parentFrame) {
    // Récupère la frame pour le contenu de la card
    Frame frameContent = GetFrame(card.content, card.config.position, parentFrame, card.config.padIn, card.config.padOut, card.config.interline);

    // Récupère la frame pour le contenu du titre
    Frame frameTitle = GetFrameTitle(card.title, frameContent, card.config.position, card.config.titleOffset, card.config.titlePad, card.config.padOut, card.config.stroke);

    // Mise à jour de la frame en fontion du titre
    frameContent.y = frameTitle.y + frameTitle.height * 0.5f;
    frameContent.height += frameTitle.height * 0.5f;

    return { AlignToPixels(frameTitle), AlignToPixels(frameContent) };
}

// Calcul la frame pour la section information
Frame GetInfoFrame(const vector<Card>& cards, const vector<DataSection>& dataSection, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut) {
    // Calcul la taille maximal de la frame
    Size size = { 0 };
    bool isTopLeft = false;
    bool isTopRight = false;
    bool isBotLeft = false;
    bool isBotRight = false;

    // ---- Calcul de la taille max pour les cards ----
    for (auto& card : cards) {
        CardLayout localFrame = GetCardFrame(card, parentFrame);

        float width  = localFrame.content.width + 2.0f * card.config.padOut.x + 2.0f * padIn.x;
        float height = localFrame.content.height + localFrame.title.height * 0.5f + 2.0f * card.config.padOut.y + 2.0f * padIn.y;

        switch (card.config.position) {
            case TOP_LEFT: {
                if (isTopLeft) break;

                if ((isTopRight || isBotRight) && !isBotLeft) size.x += width;
                else if (!isBotLeft) size.x = width;

                if ((isBotLeft || isBotRight) && !isTopRight) size.y += height;
                else if (!isTopRight) size.y = height;
                isTopLeft = true;
                break;
            }
            case TOP_RIGHT: {
                if (isTopRight) break;
                
                if ((isTopLeft || isBotLeft) && !isBotRight) size.x += width; 
                else if (!isBotRight) size.x = width;
                
                if ((isBotLeft || isBotRight) && !isTopLeft) size.y += height;
                else if (!isTopLeft) size.y = height;
                isTopRight = true;
                break;
            }
            case BOT_LEFT: {
                if (isBotLeft) break;

                if ((isTopRight || isBotRight) && !isTopLeft) size.x += width;
                else if (!isTopLeft) size.x = width;

                if ((isTopLeft || isTopRight) && !isBotRight) size.y += height;
                else if (!isBotRight) size.y = height;
                isBotLeft = true;
                break;
            }
            case BOT_RIGHT: {
                if (isBotRight) break;
                
                if ((isTopLeft || isBotLeft) && !isTopRight) size.x += width;
                else if (!isTopRight) size.x = width;

                if ((isTopLeft || isTopRight) && !isBotLeft) size.y += height;
                else if (!isBotLeft) size.y = height;
                isBotRight = true;
                break;
            }
            default:
                break;
        }
    }

    // === TODO ===
    // Mieux gérer le for (auto& data : dataSection)
    // Pour pas que la largeur dépende de la card
    //=============

    // ---- Calcul de la taille max pour les zones de textes ----
    for (auto& data : dataSection) {
        Frame localFrame = GetFrame(data.data, data.config.position, parentFrame, data.config.padIn, data.config.padIn, data.config.interline);

        float width = localFrame.width + 2.0f * data.config.padOut.x + 2.0f * padIn.x;
        float height = localFrame.height + 2.0f * data.config.padOut.y + 2.0f * padIn.y;

        switch (data.config.position) {
            case TOP_LEFT: {
                if (isTopLeft) break;

                if ((isTopRight || isBotRight) && !isBotLeft) size.x += width;
                else if (!isBotLeft) size.x = width;

                if ((isBotLeft || isBotRight) && !isTopRight) size.y += height;
                else if (!isTopRight) size.y = height;
                isTopLeft = true;
                break;
            }
            case TOP_RIGHT: {
                if (isTopRight) break;
                
                if ((isTopLeft || isBotLeft) && !isBotRight) size.x += width; 
                else if (!isBotRight) size.x = width;
                
                if ((isBotLeft || isBotRight) && !isTopLeft) size.y += height;
                else if (!isTopLeft) size.y = height;
                isTopRight = true;
                break;
            }
            case BOT_LEFT: {
                if (isBotLeft) break;

                if ((isTopRight || isBotRight) && !isTopLeft) size.x += width;
                else if (!isTopLeft) size.x = width;

                if ((isTopLeft || isTopRight) && !isBotRight) size.y += height;
                else if (!isBotRight) size.y = height;
                isBotLeft = true;
                break;
            }
            case BOT_RIGHT: {
                if (isBotRight) break;
                
                if ((isTopLeft || isBotLeft) && !isTopRight) size.x += width;
                else if (!isTopRight) size.x = width;

                if ((isTopLeft || isTopRight) && !isBotLeft) size.y += height;
                else if (!isBotLeft) size.y = height;
                isBotRight = true;
                break;
            }
            default:
                break;
        }
    }

    // ---- Calcul de la position ----

    // Crée la frame à partir de la taille voulu et de la frame parent
    Frame frame = {
        parentFrame.x,
        parentFrame.y,
        size.x,
        size.y
    };

    // En fonction de la position souhaité, calcul les coordonnées
    switch (position) {
        case TOP_LEFT: {
            frame.x += padOut.x;
            frame.y += padOut.y;
            break;
        }
        case TOP_RIGHT: {
            frame.x += parentFrame.width - size.x - padOut.x;
            frame.y += padOut.y;
            break;
        }
        case BOT_LEFT: {
            frame.x += padOut.x;
            frame.y += parentFrame.height - size.y - padOut.y;
            break;
        }
        case BOT_RIGHT: {
            frame.x += parentFrame.width - size.x - padOut.x;
            frame.y += parentFrame.height - size.y - padOut.y;
            break;
        }
        case CENTER: {
            frame.x += (parentFrame.width - frame.width) * 0.5f;
            frame.y += (parentFrame.height - frame.height) * 0.5f;
            break;
        }
        default:
            break;
    }
    return AlignToPixels(frame);
}

// Dessine une card personalisé
HoverValues DrawCard(const Card& card, Frame parentFrame, float roundness, int segments) {
    // ---- Calcul des frames ----
    CardLayout frames = GetCardFrame(card, parentFrame);

    // ---- Dessins ----
    // Dessin du fond + bord
    DrawRectangleRounded(frames.content, roundness, segments, card.config.backgroundColor);
    DrawRectangleRoundedLinesEx(frames.content, roundness, segments, card.config.stroke, card.config.borderColor);

    // Dessin du masque pour le titre
    float titleRoundness = 0.5f;
    DrawRectangleRounded(frames.title, titleRoundness, segments, card.config.backgroundColor);
    DrawRectangleRoundedLinesEx(frames.title, titleRoundness, segments, card.config.stroke, card.config.borderColor);

    // Dessin le titre par-dessus le masque
    Position titlePos = { frames.title.x + card.config.titlePad.x, frames.title.y + card.config.titlePad.y };
    DrawTextStyled(card.title, titlePos);

    // ---- Le contenu ----
    Position contentPosition = { frames.content.x + card.config.padIn.x, frames.title.y + frames.title.height + card.config.padIn.y };

    // Variable pour garder l'info-bulle
    HoverValues segHover;
    for (const auto& line : card.content) {
        float maxContentHeight = 0.0f;
        for (const auto& seg : line) {
            Size textSize = MeasureTextStyled(seg.textStyle);
            
            // Hover
            Position mousPos = GetMousePosition();
            Frame valueFrame = { contentPosition.x, contentPosition.y, textSize.x, textSize.y };
            bool hovered = CheckCollisionPointRec(mousPos, valueFrame);

            if (hovered && seg.offset != SIZE_MAX) {
                DrawRectangleRounded(valueFrame, titleRoundness, segments, COLOR_VALUE_HOOVER);
                DrawRectangleRoundedLinesEx(valueFrame, titleRoundness, segments, card.config.stroke, COLOR_STACK_BORDER);

                segHover.offset = seg.offset;
                segHover.frame = valueFrame;
            }

            // Affiche le texte
            DrawTextStyled(seg.textStyle, contentPosition);

            // Calcul la position suivante
            contentPosition.x += textSize.x;
            maxContentHeight = fmaxf(textSize.y, maxContentHeight);
        }
        // Passe à la ligne suivante
        contentPosition.x = frames.content.x + card.config.padIn.x;
        contentPosition.y += maxContentHeight + card.config.interline;
    }
    
    return segHover;
}

// Dessine une section avec des cards et des zones de textes
void DrawDataSection(const DataSection& data, Frame parentFrame, float roundness, int segments) {
    // ---- Calcul des frames ----
    Frame frame = GetFrame(data.data, data.config.position, parentFrame, data.config.padIn, data.config.padOut, data.config.interline);

    // ---- Le contenu ----
    Position contentPosition = { frame.x + data.config.padIn.x, frame.y + data.config.padIn.y };
    for (const auto& line : data.data) {
        float maxContentHeight = 0.0f;
        for (const auto& seg : line) {
            // Affiche le texte
            DrawTextStyled(seg.textStyle, contentPosition);

            // Calcul la position suivante
            Size textSize = MeasureTextStyled(seg.textStyle);
            contentPosition.x += textSize.x;
            maxContentHeight = fmaxf(textSize.y, maxContentHeight);
        }
        // Passe à la ligne suivante
        contentPosition.x = frame.x + data.config.padIn.x;
        contentPosition.y += maxContentHeight + data.config.interline;
    }
}

// Dessine une section avec des cards et des zones de textes
void DrawInfoSection(const vector<Card>& cards, const vector<DataSection>& dataSection, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut, float interline, float stroke) {
    Frame frame = GetInfoFrame(cards, dataSection, position, parentFrame, padIn, padOut);
    
    // Dessin du fond + bord
    DrawRectangleRec(frame, COLOR_STACK_GLOBAL_BACKGROUND);
    DrawRectangleLinesEx(frame, stroke, COLOR_STACK_BORDER);

    // Ajoute le pading
    frame.x += padIn.x;
    frame.y += padIn.y;
    frame.width -= 2.0f * padIn.x;
    frame.height -= 2.0f * padIn.y;

    // Pour pas réécire pour rien
    bool isTopLeft = false;
    bool isTopRight = false;
    bool isBotLeft = false;
    bool isBotRight = false;
    bool isCenter = false;

    // ---- Affichage des cards ----
    for (auto& card : cards) {
        bool skip = false;

        switch (card.config.position) {
            case TOP_LEFT:
                if (isTopLeft) {
                    cout << "[ WARNING ] TOP_LEFT is already used !" << endl;
                    skip = true;
                }
                isTopLeft = true;
                break;

            case TOP_RIGHT:
                if (isTopRight) {
                    cout << "[ WARNING ] TOP_RIGHT is already used !" << endl;
                    skip = true;
                }
                isTopRight = true;
                break;

            case BOT_LEFT:
                if (isBotLeft) {
                    cout << "[ WARNING ] BOT_LEFT is already used !" << endl;
                    skip = true;
                }
                isBotLeft = true;
                break;

            case BOT_RIGHT:
                if (isBotRight) {
                    cout << "[ WARNING ] BOT_RIGHT is already used !" << endl;
                    skip = true;
                }
                isBotRight = true;
                break;

            case CENTER:
                if (isCenter) {
                    cout << "[ WARNING ] CENTER is already used !" << endl;
                    skip = true;
                }
                isCenter = true;
                break;

            default:
                break;
        }

        if (skip) continue;
        DrawCard(card, frame);
    }
    
    // ---- Affichage des zones de textes ----
    for (auto& data : dataSection) {
        bool skip = false;

        switch (data.config.position) {
            case TOP_LEFT:
                if (isTopLeft) {
                    cout << "[ WARNING ] TOP_LEFT is already used !" << endl;
                    skip = true;
                }
                isTopLeft = true;
                break;

            case TOP_RIGHT:
                if (isTopRight) {
                    cout << "[ WARNING ] TOP_RIGHT is already used !" << endl;
                    skip = true;
                }
                isTopRight = true;
                break;

            case BOT_LEFT:
                if (isBotLeft) {
                    cout << "[ WARNING ] BOT_LEFT is already used !" << endl;
                    skip = true;
                }
                isBotLeft = true;
                break;

            case BOT_RIGHT:
                if (isBotRight) {
                    cout << "[ WARNING ] BOT_RIGHT is already used !" << endl;
                    skip = true;
                }
                isBotRight = true;
                break;

            case CENTER:
                if (isCenter) {
                    cout << "[ WARNING ] CENTER is already used !" << endl;
                    skip = true;
                }
                isCenter = true;
                break;

            default:
                break;
        }

        if (skip) continue;
        DrawDataSection(data, frame);
    }
}

// Dessine une saisie utilisateur
void DrawInputBox(TextStyle name, int maxInputChars, Padding padIn, float stroke) {
    // ---- Calcul la taille et position de l'input box ----

    // === TODO ===
    // Pas propre ici
    // ============
    TextStyle tmp = name;
    tmp.text = string(maxInputChars, 'W');
    Size size = MeasureTextStyled(tmp);
    Frame frame = {
        SCREEN_WIDTH / 2.0f - ((size.x + 2.0f * padIn.x) / 2.0f),
        SCREEN_HEIGHT / 2.0f - ((size.y + 2.0f * padIn.y) / 2.0f),
        size.x + 2.0f * padIn.x,
        size.y + 2.0f * padIn.y
    };
    frame = AlignToPixels(frame);

    // ---- Dessine le focus ----
    DrawRectangleRec({ 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT }, COLOR_INPUT_BOX_FOCUS);

    // ---- Dessine l'input box
    DrawRectangleRec(frame, COLOR_INPUT_BOX_BACKGROUND);
    DrawRectangleLinesEx(frame, stroke, COLOR_INPUT_BOX_BORDER);

    // ---- Dessine la saisie ----
    Position textPos{ frame.x + padIn.x, frame.y + padIn.y };
    DrawTextStyled(name, textPos);

    Size measured = MeasureTextStyled(name);

    // ---- Dessine un curseur clignotant ----
    const bool blinkOn = fmod(GetTime(), 1.0) < 0.5;
    if (static_cast<int>(name.text.size()) < maxInputChars && blinkOn) {
        float x = roundf(textPos.x + measured.x);
        float y = textPos.y;

        // Barre verticale
        float cursorW = 2.0f;
        float cursorH = name.fontSize * 0.85f;
        DrawRectangle(x, y + name.fontSize - cursorH, cursorW, cursorH, name.color);
    }

    // ---- Dessine le texte informatif ----
    TextStyle information = {
        name.font,
        TextFormat("Caractères: %i/%i", static_cast<int>(name.text.size()), maxInputChars),
        name.fontSize,
        name.spacing,
        name.color
    };
    Size informationSize = MeasureTextStyled(information);
    DrawTextStyled(information, { SCREEN_WIDTH / 2.0f - informationSize.x / 2.0f, frame.y + frame.height + 20.0f });
}

// Dessine un info-bulle pour un hover
void DrawToolTip(const vector<vector<InfoSegment>>& data, Frame parentFrame, Padding padIn, float roundness, int segments, float stroke, float interline) {
    // ---- Calcul de la position et de la taille nécessaire ----
    float branchPad    = 12.0f;
    float branchStroke = 4.0f;
    float indentAfterTitle = 2.0f * branchPad + branchStroke;
    float titleInterline   = 2.0f * interline;

    Size size = GetFrameMaxSizeWithBranch(data, padIn, interline, indentAfterTitle, titleInterline);
    Frame frame = {
        parentFrame.x,
        parentFrame.y + parentFrame.height,
        size.x,
        size.y
    };

    // Fait en sort qu'il soit toujours afficher même si il sort de l'écran
    if (frame.x + frame.width > SCREEN_WIDTH) {
        frame.x = SCREEN_WIDTH - frame.width - (SCREEN_WIDTH - (parentFrame.x + parentFrame.width));
    }
    frame = AlignToPixels(frame);

    // ---- Affichage du fond ----
    DrawRectangleRounded(frame, roundness, segments, COLOR_TOOLTIP_BACKGROUND);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, stroke, COLOR_TOOLTIP_BORDER);

    // ---- Affiche le titre ----
    Position contentPosition = { frame.x + padIn.x, frame.y + padIn.y };
    const TextStyle& title = data[0][0].textStyle;

    const Size titleSize = MeasureTextStyled(title);
    DrawTextStyled(title, contentPosition);

    // Décalage pour la branche
    float branchHeight = 0.0f;
    contentPosition.x += indentAfterTitle;
    contentPosition.y += titleSize.y + titleInterline;

    // ---- Affiche les valeurs ----
    vector<int> branchPoint;
    branchPoint.reserve(data.size() - 1);

    const float startX = contentPosition.x;
    for (size_t i = 1; i < data.size(); ++i) {
        float lineHeight = 0.0f;

        for (const auto& seg : data[i]) {
            DrawTextStyled(seg.textStyle, contentPosition);

            Size size = MeasureTextStyled(seg.textStyle);
            contentPosition.x += size.x;
            lineHeight = fmaxf(lineHeight, size.y);
        }

        // Ajout de la position pour la branche
        branchPoint.push_back(contentPosition.y + 0.75 * lineHeight - branchStroke);

        // Passe à la ligne suivante
        contentPosition.x = startX;
        contentPosition.y += lineHeight + interline;
        branchHeight += (i < data.size() - 1 ? lineHeight : 0.75f * lineHeight) + interline;
    }

    // ---- Affichage de la branche ----
    // Affiche la branche principale
    contentPosition.x = frame.x + padIn.x + branchPad - 0.5f * branchStroke;
    contentPosition.y = frame.y + padIn.y + titleSize.y + interline;

    Frame branchFrame = {
        contentPosition.x,
        contentPosition.y,
        branchStroke,
        branchHeight
    };
    branchFrame = AlignToPixels(branchFrame);

    // 1.0f pour avoir un demi cercle parfait
    DrawRectangleRounded(branchFrame, 1.0f, 32, COLOR_TOOLTIP_BRANCH);
 
    // Affichage les sous branches
    Frame lastFork = {
        branchFrame.x,
        0.0f,
        branchPad,
        branchStroke
    };

    for (size_t i = 0; i < branchPoint.size(); i++) {
        lastFork.y = branchPoint[i];
        DrawRectangleRounded(lastFork, 1.0f, 32, COLOR_TOOLTIP_BRANCH);
    }
}

// Dessine un info-bulle pour un item de l'inventaire
void DrawItemToolTip(const vector<vector<InfoSegment>>& data, Frame parentFrame, Padding padIn, float roundness, int segments, float stroke, float interline) {
    Size size = GetFrameMaxSize(data, padIn, interline);
    
    Frame frame = {
        parentFrame.x,
        parentFrame.y + parentFrame.height,
        size.x,
        size.y
    };
    
    // ---- Affichage du fond ----
    DrawRectangleRounded(frame, roundness, segments, COLOR_INVENTORY_HOVER_BACKGROUND);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, stroke, COLOR_INVENTORY_HOVER_BORDER);

    // ---- Affiche le triangle ----
    float offsetX = -10.0f;

    Position p0 = { parentFrame.x + parentFrame.width / 2.0f - 10.0f + offsetX, parentFrame.y + parentFrame.height };
    Position p1 = { parentFrame.x + parentFrame.width / 2.0f + 10.0f + offsetX, parentFrame.y + parentFrame.height };
    Position p2 = { parentFrame.x + parentFrame.width / 2.0f + offsetX,         parentFrame.y + parentFrame.height + 10.0f };

    DrawTriangle(p1, p0, p2, COLOR_INVENTORY_HOVER_BORDER);
    
    // ---- Affiche le contenu ----
    Position contentPosition = { frame.x + padIn.x, frame.y + padIn.y };
    for (const auto& line : data) {
        float maxContentHeight = 0.0f;
        for (const auto& seg : line) {
            Size textSize = MeasureTextStyled(seg.textStyle);
            
            // Affiche le texte
            DrawTextStyled(seg.textStyle, contentPosition);

            // Calcul la position suivante
            contentPosition.x += textSize.x;
            maxContentHeight = fmaxf(textSize.y, maxContentHeight);
        }
        // Passe à la ligne suivante
        contentPosition.x = frame.x + padIn.x;
        contentPosition.y += maxContentHeight + interline;
    }
}

// Dessine une image statique
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

// Dessine une image animé
void DrawAnimatedEntity(const Texture2D& texture, AnimationState& anim, Position pos, bool moving, const SpriteSheetInfo& entitySprite, Color color) {
    // Avancer l’animation
    anim.timer += GetFrameTime();
    float targetFrameTime = moving ? anim.frameTimeMove : anim.frameTimeIdle;
    while (anim.timer >= targetFrameTime) {
        anim.timer -= targetFrameTime;
        anim.frame = fmod(anim.frame + 1.0f, entitySprite.frameCols);
    }

    // Ligne à utiliser de la texture
    int row = moving ? entitySprite.rowMove : entitySprite.rowIdle;

    // Rectangle source
    Frame src = {
        anim.frame * entitySprite.frameW,
        row * entitySprite.frameH,
        entitySprite.frameW,
        entitySprite.frameH
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

// Dessine au dessus d'une entité une étiquette d'information
void DrawInfoLabel(Hitbox entity, int entitySize, TextStyle text) {
    Size label_size = MeasureTextStyled(text);
    float padding = 5.0f;

    // Calcul la position
    Position position;
    position.x = entity.pos.x - (label_size.x / 2);
    position.y = entity.pos.y - entitySize - label_size.y - padding;
    
    DrawTextStyled(text, position);
}

// Dessine un bandeau pour un dialogue
void DrawDialogue(const vector<vector<InfoSegment>>& data, const TextStyle& continueData, IconProfile& profile, Padding padInFrameContent, Padding padInContent, Size iconFrameSize, float roundness, int segments, float stroke, float interline) {
    // ---- Calcul de la frame ----
    Size frameSize = GetFrameMaxSize(data, padInContent, interline);

    Frame frame = {
        0.0f,
        0.0f,
        frameSize.x + iconFrameSize.x + 2.0f * padInContent.x + 2.0f * padInFrameContent.x,
        frameSize.y + 2.0f * padInContent.y + 2.0f * padInFrameContent.y
    };

    // Si les tailles calculer ne sont pas acceptable
    float defaultSize = 700.0f;
    if (frame.width < defaultSize) frame.width = defaultSize;
    if (frame.height < iconFrameSize.y) frame.height = iconFrameSize.y;
    frame.x = SCREEN_WIDTH / 2.0f - frame.width / 2.0f;
    frame.y = SCREEN_HEIGHT - frame.height - 1;
    frame = AlignToPixels(frame);

    // ---- Affichage du fond global ----
    DrawRectangleRounded(frame, roundness, segments, COLOR_DIALOGUE_BACKGROUND);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, stroke, COLOR_DIALOGUE_BORDER);

    Frame contentFrame = {
        frame.x + stroke + iconFrameSize.x,
        frame.y + stroke + padInFrameContent.y,
        frame.width - stroke - padInFrameContent.x - iconFrameSize.x,
        frame.height - 2.0f * stroke - 2.0f * padInFrameContent.y
    };

    // ---- Affichage du fond pour les textes ----
    DrawRectangleRounded(contentFrame, roundness, segments, COLOR_DIALOGUE_CONTENT_BACKGROUND);
    DrawRectangleRoundedLinesEx(contentFrame, roundness, segments, stroke, COLOR_DIALOGUE_CONTENT_BORDER);

    // ---- Affiche le dialogue ----
    Position contentPosition = { contentFrame.x + padInContent.x, contentFrame.y + padInContent.y };
    for (const auto& line : data) {
        float maxContentHeight = 0.0f;
        for (const auto& seg : line) {
            // Affiche le texte
            DrawTextStyled(seg.textStyle, contentPosition);

            // Calcul la position suivante
            Size textSize = MeasureTextStyled(seg.textStyle);
            contentPosition.x += textSize.x;
            maxContentHeight = fmaxf(textSize.y, maxContentHeight);
        }
        // Passe à la ligne suivante
        contentPosition.x = contentFrame.x + padInContent.x;
        contentPosition.y += maxContentHeight + interline;
    }

    // ---- Affiche le texte "Continue" ----
    Size continueSize = MeasureTextStyled(continueData);
    Position continuePosition = { contentFrame.x + contentFrame.width - continueSize.x - padInContent.x, contentFrame.y + contentFrame.height - continueSize.y - padInContent.y };
    DrawTextStyled(continueData, continuePosition);

    // ---- Affiche l'image de profile ----
    // Destination (où et quelle taille à l'écran)
    if (profile.entity.id > 0 && profile.entity.width > 0 && profile.entity.height > 0) {
        Frame frameDest = {
            frame.x,
            frame.y,
            iconFrameSize.x,
            iconFrameSize.y
        };

        // Calcul la position du profil
        float squareSize = frameDest.height;  // hauteur de la texture affichée
        if (squareSize > frameDest.width) squareSize = frameDest.width; // rester carré

        Position center = {
            frameDest.x + squareSize / 2.0f,
            frameDest.y + squareSize / 2.0f
        };

        DrawAnimatedEntity(profile.entity, profile.entityAnim, center, false, profile.entitySprite, profile.color);
    }
}

// Dessine des marqueurs dans les angles d'un rectangle
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

// Dessine un item avec sa quantité en bas à droite
void DrawItemWithQuantity(Position pos, TextStyle& itemQuantity, const Texture2D& texture, float itemScale, float radiusCircleInfo) {
    // Affiche l'item
    DrawTextureEx(texture, pos, 0.0f, itemScale, WHITE);

    // Affiche sa quantité
    Position bottomRight = { pos.x + texture.width * itemScale - radiusCircleInfo, pos.y + texture.height * itemScale - radiusCircleInfo };
    DrawCircleV(bottomRight, radiusCircleInfo, COLOR_INVENTORY_CURRENT_AMOUNT_BACKGROUND);
    DrawRing(bottomRight, radiusCircleInfo - 2.0f, radiusCircleInfo, 0.0f, 360.0f, 64, BLACK);

    itemQuantity.text = "x" + itemQuantity.text;
    Size textSize = MeasureTextStyled(itemQuantity);
    Position textPos = { bottomRight.x - textSize.x / 2, bottomRight.y - textSize.y / 2 };
    DrawTextStyled(itemQuantity, textPos);
}

Size GetButtonSize(TextStyle& data, Padding padIn) {
    Size textSize = MeasureTextStyled(data);
    return { textSize.x + 2.0f * padIn.x, textSize.y + 2.0f * padIn.y };
}

void DrawRoundedButton(TextStyle& data, Position position, Padding padIn, float roundness, int segments, float stroke) {
    // Calcul de la taille
    Size textSize = GetButtonSize(data, padIn);
    Frame frame = {
        position.x,
        position.y,
        textSize.x,
        textSize.y
    };

    // Dessin du fond + bord
    DrawRectangleRounded(frame, roundness, segments, COLOR_STORE_BUTTON_BACKGROUND);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, stroke, COLOR_STORE_BUTTON_BORDER);

    // Affiche le texte
    DrawTextStyled(data, { frame.x + padIn.x, frame.y + padIn.y });
}

void DrawRightArrow(Frame parentFrame, float thick, Color color) {
    float tipWidth  = 10.0f;

    float thickRect = thick / 3.0f;
    Frame frame = {
        parentFrame.x,
        parentFrame.y + parentFrame.height / 2.0f - thickRect / 2.0f,
        parentFrame.width - tipWidth,
        thickRect
    };
    DrawRectangleRec(frame, color);

    // Pointe
    float centerY = parentFrame.y + parentFrame.height / 2.0f;
    float tipBase = frame.x + frame.width;
    Position p2 = { tipBase + tipWidth, centerY };
    Position p0 = { tipBase, centerY - thick / 2.0f };  
    Position p1 = { tipBase, centerY + thick / 2.0f };

    DrawTriangle(p0, p1, p2, color);
}

// Dessine un magasin où l'utilisateur peut acheter des items
void DrawStore(const TextStyle& title, TextStyle& quantity, TextStyle& quantityRatio, TextStyle& buttonStyle, const Texture2D& potatoTex, const Texture2D& carrotTex, const Texture2D& appleTex, float roundness, int segments, float stroke) {
    Frame frame = {
        SCREEN_WIDTH / 2.0f - 150.0f,
        SCREEN_HEIGHT / 2.0f - 100.0f,
        400.0f,
        200.0f
    };

    // ---- Dessin du fond + bord ----
    DrawRectangleRounded(frame, roundness, segments, COLOR_STORE_BACKGROUND);
    DrawRectangleRoundedLinesEx(frame, roundness, segments, stroke, COLOR_STORE_BORDER);

    // ---- Dessin du triangle ----
    Size textSize = MeasureTextStyled(title);

    float titleMarginY = 10.0f;
    float triWidth  = 10.0f;
    float triHeight = 20.0f;

    // Position du texte
    Position titlePos = {
        frame.x + 10.0f + triWidth + 10.0f,
        frame.y + titleMarginY
    };

    float centerY = titlePos.y + textSize.y / 2.0f;
    float marginX = 10.0f;

    Position p2 = { frame.x + marginX + triWidth, centerY };
    Position p0 = { frame.x + marginX, centerY - triHeight / 2.0f };  
    Position p1 = { frame.x + marginX, centerY + triHeight / 2.0f };

    DrawTriangle(p0, p1, p2, COLOR_STORE_BORDER);

    // ---- Dessin du titre à côté du triangle ----
    titlePos = {
        p2.x + 10.0f,
        titlePos.y
    };

    DrawTextStyled(title, titlePos);

    // ---- Dessin des items ----
    float itemScale = 3.0f;
    float radiusCircleInfo = 10.0;
    Padding tradePaddingX = { 50.0f, 20.0f };
    Size titleSize = MeasureTextStyled(title);

    // -- Patate --
    // Item in
    Position potatoPos = { frame.x + tradePaddingX.x, titlePos.y + titleSize.y + tradePaddingX.y };
    quantity.text = "8";
    //DrawRectangleRec({ potatoPos.x, potatoPos.y, (float)potatoTex.width * itemScale, (float)potatoTex.height * itemScale }, RED);
    DrawItemWithQuantity(potatoPos, quantity, potatoTex, itemScale, radiusCircleInfo);

    // Flèche
    float arrowPadX = 20.0f;
    Size arrowSize = { 100.0f, 20.0f };
    Rectangle arrow = {
        potatoPos.x + potatoTex.width * itemScale + arrowPadX,
        potatoPos.y + (potatoTex.height * itemScale) / 2.0f - arrowSize.y / 2.0f,
        arrowSize.x,
        arrowSize.y
    };
    DrawRightArrow(arrow, 20.0f, COLOR_STORE_ARROW);

    // Quantité actuelle / Quantité max
    quantityRatio.text = "/";

    // Item out
    Position carrotPos = { arrow.x + arrow.width + arrowPadX, potatoPos.y };
    quantity.text = "1";
    //DrawRectangleRec({ carrotPos.x, carrotPos.y, (float)carrotTex.width * itemScale, (float)carrotTex.height * itemScale }, RED);
    DrawItemWithQuantity(carrotPos, quantity, carrotTex, itemScale, radiusCircleInfo);

    // Bouton achat
    buttonStyle.text = "Buy";
    float buttonPadX = 20.0f;
    Padding buttonPad = { 7.0f, 5.0f };
    Size buttonSize = GetButtonSize(buttonStyle, buttonPad);
    Position buttonPos = { carrotPos.x + carrotTex.width * itemScale + buttonPadX, arrow.y + arrow.height / 2.0f - buttonSize.y / 2.0f };
    DrawRoundedButton(buttonStyle, buttonPos, buttonPad, 0.6f);

    // -- Carotte --

    // -- Pomme --
}
