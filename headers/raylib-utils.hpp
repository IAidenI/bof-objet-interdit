#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>
#include <vector>
#include "colors.hpp"
using namespace std;
using Frame = Rectangle;
using Size = Vector2;
using Position = Vector2;

// ---- Padding ----
struct Spacing {
    float x;
    float y;
};
using Padding = Spacing;
using Margins = Spacing;

// ---- Constantes ----
// Card
constexpr Padding DEFAULT_TITLE_PAD_IN  = { 5.0f, 3.0f };
constexpr float   DEFAULT_TITLE_OFFSET  = 15.0f;

constexpr Padding DEFAULT_PAD_IN        = { 10.0f, 10.0f };
constexpr Margins DEFAULT_PAD_OUT       = { 15.0f, 15.0f };
constexpr float   DEFAULT_INTERLINE     = 5.0f;

constexpr float   DEFAULT_ROUNDNESS     = 0.12f;
constexpr int     DEFAULT_SEGMENTS      = 32;
constexpr float   DEFAULT_STROKE        = 2.0f;

// ---- Style pour les textes ----
struct TextStyle {
    Font *font;
    string text;
    float fontSize;
    float spacing;
    Color color;
};

inline void DrawTextStyled(TextStyle data, Position position) {
    DrawTextEx(*data.font, data.text.c_str(), position, data.fontSize, data.spacing, data.color);
}

inline Size MeasureTextStyled(TextStyle data) {
    return MeasureTextEx(*data.font, data.text.c_str(), data.fontSize, data.spacing);
}

struct InfoSegment {
    TextStyle textStyle;
    size_t offset = SIZE_MAX;
};

// ---- Position dans une frame ----
enum WindowPosition {
    TOP_LEFT,
    TOP_RIGHT,
    BOT_RIGHT,
    BOT_LEFT,
    CENTER
};

// ---- Informations pour dessiner une card ----
struct CardConfig {
    // Position dans la frame
    WindowPosition position = TOP_LEFT;

    // Général
    Color color  = COLOR_STACK_CARD_BACKGROUND;
    float stroke = DEFAULT_STROKE;

    // Titre
    float titleOffset = DEFAULT_TITLE_OFFSET;
    Padding titlePad  = DEFAULT_TITLE_PAD_IN;

    // Contenu
    Padding padIn   = DEFAULT_PAD_IN;
    Margins padOut  = DEFAULT_PAD_OUT;
    float interline = DEFAULT_INTERLINE;
};

struct Card {
    TextStyle title;
    vector<vector<InfoSegment>> content;
    CardConfig config;
};

struct CardLayout {
    Frame title;
    Frame content;
};

// Pour le hover dans la card
struct HoverValues {
    size_t offset = SIZE_MAX;
    Frame frame = { 0.0f };
};

// ---- Informations pour la section de texte ----
struct DataConfig {
    // Position dans la frame
    WindowPosition position = TOP_LEFT;

    // Général
    Color color    = COLOR_STACK_CARD_BACKGROUND;
    float stroke   = DEFAULT_STROKE;


    // Contenu
    Padding padIn   = DEFAULT_PAD_IN;
    Margins padOut  = DEFAULT_PAD_OUT;
    float interline = DEFAULT_INTERLINE;
};

struct DataSection {
    vector<vector<InfoSegment>> data;
    DataConfig config;
};

// ---- Pour arrondir les frames et ne pas avoir des demi pixels ----
inline Frame AlignToPixels(Frame f) {
    f.x = roundf(f.x);
    f.y = roundf(f.y);
    f.width  = roundf(f.width);
    f.height = roundf(f.height);
    return f;
}

Size GetMaxLineSize(const vector<InfoSegment>& line); // Calcul les dimenssions maximal pour une ligne
Size GetFrameMaxSize(const vector<vector<InfoSegment>>& content, Padding padIn, float interline); // Calcul la taille maximale dont aura besoin une frame
Size GetFrameMaxSizeWithBranch(const vector<vector<InfoSegment>>& content, Padding padIn, float interline, float indentAfterTitle, float titleInterline); // Calcul la taille maximale dont aura besoin une frame en prenant en compte une branche

Frame GetFrame(const vector<vector<InfoSegment>>& content, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut, float interline); // Calcul la position et la taille d'une frame
Frame GetFrameTitle(const TextStyle& title, Frame parentFrame, WindowPosition parentPosition, float titleOffset, Padding padTitle, Margins padFrame, float stroke); // Calcul la position et la taille d'une frame pour un titre
CardLayout GetCardFrame(const Card& card, Frame parentFrame); // Calcul la frame d'une card (titre + contenu)
Frame GetInfoFrame(const vector<Card>& cards, const vector<DataSection>& dataSection, WindowPosition position, Frame parentFrame, Padding padIn, Margins padOut); // Calcul la frame pour la section information

HoverValues DrawCard(const Card& card, Frame parentFrame, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS); // Permet de dessiner une card
void DrawDataSection(const DataSection& data, Frame parentFrame, float roundness = DEFAULT_SEGMENTS, int segments = DEFAULT_SEGMENTS); // Dessine une section avec des cards et des zones de textes
void DrawInfoSection(const vector<Card>& cards, const vector<DataSection>& dataSection, WindowPosition position, Frame parentFrame, Padding padIn = DEFAULT_PAD_IN, Margins padOut = DEFAULT_PAD_OUT, float interline = DEFAULT_INTERLINE, float stroke = DEFAULT_STROKE); // Dessine une section avec des cards et des zones de textes

void DrawInputBox(TextStyle name, int maxInputChars, Size SCREEN_SIZE, Padding padIn = DEFAULT_PAD_IN, float stroke = DEFAULT_STROKE); // Dessine une saisie utilisateur
void DrawToolTip(const vector<vector<InfoSegment>>& data, Frame parentFrame, Padding padIn = DEFAULT_PAD_IN, float roundness = DEFAULT_ROUNDNESS, int segments = DEFAULT_SEGMENTS, float stroke = DEFAULT_STROKE, float interline = DEFAULT_INTERLINE);

#endif // UTILS_H