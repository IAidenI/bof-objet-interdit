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
void DrawCard(const Card& card, Frame parentFrame, float roundness, int segments) {
    // ---- Calcul des frames ----
    CardLayout frames = GetCardFrame(card, parentFrame);

    // ---- Dessins ----
    // Dessin du fond + bord
    DrawRectangleRounded(frames.content, roundness, segments, card.config.color);
    DrawRectangleRoundedLinesEx(frames.content, roundness, segments, card.config.stroke, COLOR_STACK_BORDER);

    // Dessin du masque pour le titre
    float titleRoundness = 0.5f;
    DrawRectangleRounded(frames.title, titleRoundness, segments, card.config.color);
    DrawRectangleRoundedLinesEx(frames.title, titleRoundness, segments, card.config.stroke, COLOR_STACK_BORDER);

    // Dessin le titre par-dessus le masque
    Position titlePos = { frames.title.x + card.config.titlePad.x, frames.title.y + card.config.titlePad.y };
    DrawTextStyled(card.title, titlePos);

    // ---- Le contenu ----
    Position contentPosition = { frames.content.x + card.config.padIn.x, frames.title.y + frames.title.height + card.config.padIn.y };
    for (const auto& line : card.content) {
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
        contentPosition.x = frames.content.x + card.config.padIn.x;
        contentPosition.y += maxContentHeight + card.config.interline;
    }
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
void DrawInputBox(TextStyle name, int maxInputChars, Size SCREEN_SIZE, Padding padIn, float stroke) {
    // ---- Calcul la taille et position de l'input box ----

    // === TODO ===
    // Pas propre ici
    // ============
    TextStyle tmp = name;
    tmp.text = string(maxInputChars, 'W');
    Size size = MeasureTextStyled(tmp);
    Frame frame = {
        SCREEN_SIZE.x / 2.0f - ((size.x + 2.0f * padIn.x) / 2.0f),
        SCREEN_SIZE.y / 2.0f - ((size.y + 2.0f * padIn.y) / 2.0f),
        size.x + 2.0f * padIn.x,
        size.y + 2.0f * padIn.y
    };
    frame = AlignToPixels(frame);

    // ---- Dessine le focus ----
    DrawRectangleRec({ 0.0f, 0.0f, SCREEN_SIZE.x, SCREEN_SIZE.y }, COLOR_INPUT_BOX_FOCUS);

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
    DrawTextStyled(information, { SCREEN_SIZE.x / 2.0f - informationSize.x / 2.0f, frame.y + frame.height + 20.0f });
}
